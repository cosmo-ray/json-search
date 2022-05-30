/**        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                   Version 2, December 2004
 *
 * Copyright (C) 2021 Matthias Gatto <uso.cosmo.ray@gmail.com>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

#define panic(...)				\
	do {					\
		fprintf(stderr, __VA_ARGS__);	\
		exit(1);			\
	} while (0)


#define MAX_FILES 128
#define MAX_EXPRESIONS 128
#define MAX_LOOKERS 128

struct looker {
	char *expresions[MAX_EXPRESIONS];
	int deep;
};

int nb_found;
int max_founds;

#define VERBOSE (1 << 0)
#define RAW_PRINT (1 << 2)
#define CASE_INSENSITIVE (1 << 3)
#define PRINT_PARENT (1 << 4)
#define CHECK_VALUE (1 << 5)
#define LOCATION_INFO (1 << 6)
#define PRINT_KEYS (1 << 7)

int program_flag;

struct print_info {
	struct json_object *j;
	const char *f;
	const char *k;
	struct print_info *next;
} *pinfo;

void usage(void)
{
	printf("json-search [NEEDLE] [FILE] [FILES] [OPTION]\n"
	       "Search json object or value inside others jsons\n"
	       "just reformat input json if no NEEDLE\n\n"
	       "OPTIONS:\n"
	       "\t-v: verbose mode\n"
	       "\t-i: case insensitive\n"
	       "\t-V: Check Value instead of keys\n"
	       "\t-K: Print only objects keys\n"
	       "\t-R: raw print\n"
	       "\t-M NUM: limitate the number of returned objects\n"
	       "\t-p: print parent instead of element\n"
	       "\t-s: locate sub-string instead of strict comparison\n"
	       "\t\tex: 'file' will match with 'files'\n"
	       "\t-r: recursive file search, NOT YET IMPLEMENTED\n"
	       "\t-o: multiple patern search\n"
	       "\t-t: look for object child, NOT YET IMPLEMENTED\n"
	       "\t-l: location info: print file and key\n"
	       "\t-h: are you really wondering what this is ?\n");
}

int strcasestr_look(const char *haystack, const char *needle)
{
	return !!strcasestr(haystack, needle);
}

int strcasecmp_look(const char *haystack, const char *needle)
{
	return !strcasecmp(haystack, needle);
}

int strstr_look(const char *haystack, const char *needle)
{
	return !!strstr(haystack, needle);
}

int strcmp_look(const char *haystack, const char *needle)
{
	return !strcmp(haystack, needle);
}

int (*looker)(const char *, const char *) = strcmp_look;

int should_print_array(void)
{
	return nb_found > 1 &&
		!(program_flag & RAW_PRINT) &&
		!(program_flag & LOCATION_INFO);
}

void print(const char *f, const char *k, struct json_object *v,
	   int print_colun)
{
	if (program_flag & PRINT_KEYS &&
	    json_object_get_type(v) == json_type_object) {
		int loop = 0;
		if (!(program_flag & RAW_PRINT))
			putchar('[');
		json_object_object_foreach(v, k0, unused) {
			if (loop++) {
				putchar(',');
			}
			if (!(program_flag & RAW_PRINT))
				printf("\n  ");
			printf("\"%s\"", k0);
		}
		if (!(program_flag & RAW_PRINT))
			printf("\n]");
		putchar('\n');
		return;
	}
	if (program_flag & RAW_PRINT) {
		if (json_object_get_type(v) == json_type_array ||
		    json_object_get_type(v) == json_type_object) {
			printf("%s type match\n",
			       json_object_get_type(v) == json_type_array ?
			       "array": "object");
		} else if (json_object_get_type(v) == json_type_string) {
			printf("%s\n", json_object_get_string(v));
		} else if (json_object_get_type(v) == json_type_int) {
			printf("%d\n", json_object_get_int(v));
		} else if (json_object_get_type(v) == json_type_double) {
			printf("%f\n", json_object_get_double(v));
		} else if (json_object_get_type(v) == json_type_boolean) {
			printf("%s\n", json_object_get_boolean(v) ?
			       "true" : "false");
		} else {
			printf("match on unsuported type");
		}
		return;
	}
	if ((program_flag & LOCATION_INFO))
		printf("%s - %s: ", f, k);
	printf("%s",
	       json_object_to_json_string_ext(v, JSON_C_TO_STRING_PRETTY |
					      JSON_C_TO_STRING_NOSLASHESCAPE));
	if (print_colun)
		printf(",");
	printf("\n");
}

void obj_lookup(const char *f, struct json_object *o, struct looker *lks)
{
	char *expresion = lks->expresions[lks->deep - 1];

	if (json_object_get_type(o) == json_type_object) {
		json_object_object_foreach(o, k, v) {
			char num_buf[128];
			const char *to_look = k;

			if (program_flag & CHECK_VALUE) {
				if (json_object_get_type(v) == json_type_string) {
					to_look = json_object_get_string(v);
				} else if (json_object_get_type(v) ==
					   json_type_int) {
					snprintf(num_buf,  128, "%d",
						 json_object_get_int(v));
					to_look = num_buf;
				} else if (json_object_get_type(v) ==
					   json_type_double) {
					snprintf(num_buf, 128, "%f",
						 json_object_get_double(v));
					to_look = num_buf;
				}
			}

			if (program_flag & VERBOSE) {
				printf("object inspect %s\n", to_look);
			}

			if (looker(to_look, expresion)) {
				if (max_founds && nb_found >= max_founds)
					return;
				++nb_found;
				struct print_info *tmp = malloc(sizeof *tmp);

				tmp->k = k;
				tmp->f = f;
				tmp->next = pinfo;
				pinfo = tmp;
				if (program_flag & PRINT_PARENT) {
					tmp->j = o;
				} else {
					tmp->j = v;
				}
			}
			obj_lookup(f, v, lks);
		}
	} else if (json_object_get_type(o) == json_type_array) {
		int l = json_object_array_length(o);

		if (program_flag & VERBOSE) {
			printf("array check...\n");
		}
		for (int i = 0; i < l; ++i) {
			obj_lookup(f, json_object_array_get_idx(o, i), lks);
		}
	}
}

void file_look(const char *f, struct json_object *j_file, struct looker *lks)
{
	if (program_flag & VERBOSE) {
		printf("look for '%s' in file '%s'",
		       lks->expresions[lks->deep - 1], f);
	}
	obj_lookup(f, j_file, lks);
}

int main(int argc, char **argv)
{
	char *files[MAX_FILES];
	struct json_object *j_files[MAX_FILES];
	int nb_files = 0;
	int nb_lookers = 1;
	int fd = -1;
	struct looker lks[MAX_LOOKERS] = {0};

	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			char *pc = argv[i] + 1;

			for(; *pc; ++pc) {
				if (*pc == 'v') {
					printf("verbose mode\n");
					program_flag |= VERBOSE;
				} else if (*pc == 'h') {
					usage();
					return 0;
				} else if (*pc == 'V') {
					if (program_flag & VERBOSE)
						printf("check value mode\n");
					program_flag |= CHECK_VALUE;
				} else if (*pc == 'M') {
					if (*(pc + 1))
						panic("can't have option after -M, need NUMBER");
					if (!argv[i + 1])
						panic("-M, not enough arguments, need number");
					++i;
				        max_founds = atoi(argv[i]);
					if (program_flag & VERBOSE)
						printf("max number of foundables %d\n",
							max_founds);
					goto next_arg;
				} else if (*pc == 's') {
					if (program_flag & VERBOSE)
						printf("strstr mode\n");
					looker = strstr_look;
				} else if (*pc == 'l') {
					if (program_flag & VERBOSE)
						printf("just json mode\n");
					program_flag |= LOCATION_INFO;
				} else if (*pc == 'i') {
					if (program_flag & VERBOSE)
						printf("case insensitive mode\n");
					program_flag |= CASE_INSENSITIVE;
				} else if (*pc == 'R') {
					if (program_flag & VERBOSE)
						printf("raw print mode\n");
					program_flag |= RAW_PRINT;
				} else if (*pc == 'p') {
					if (program_flag & VERBOSE)
						printf("print parent mode\n");
					program_flag |= PRINT_PARENT;
				} else if (*pc == 'K') {
					if (program_flag & VERBOSE)
						printf("print keys mode\n");
					program_flag |= PRINT_KEYS;
				} else if (*pc == 'r') {
					panic("-r not impemented reseved for recursive files search\n");
				} else if (*pc == 'o') {
					if (!lks[nb_lookers - 1].deep) {
						panic("missing argument");
					}
					++nb_lookers;
				} else if (*pc == 't') {
					panic("-t not impemented reseved for 'though'\n");
				} else {
					panic("'%c': not sure option\n", *pc);
				}
			}
		next_arg:
			continue;
		}
		if (lks[nb_lookers - 1].deep > 0) {
			if (++nb_files > MAX_FILES)
				panic("Too much files, max: %d\n", MAX_FILES);
			files[nb_files - 1] = argv[i];
		} else {
			int deep = lks[nb_lookers - 1].deep;

			lks[nb_lookers - 1].expresions[deep] = argv[i];
			lks[nb_lookers - 1].deep++;
		}
	}

	if (!nb_files) {
		fd = 0;
		nb_files = 1;
	}

	if (program_flag & CASE_INSENSITIVE) {
		if (looker == strstr_look)
			looker = strcasestr_look;
		else
			looker = strcasecmp_look;
	}

	for (int i = 0; i < nb_files; ++i) {
		struct json_object *j_file;
		const char *f = fd < 0 ? files[i] : "<stdin>";

		if (fd < 0)
			j_file = json_object_from_file(files[i]);
		else
			j_file = json_object_from_fd(fd);

		if (!j_file) {
			panic("error in %s json", f);
		}

		j_files[i] = j_file;

		if (!lks[nb_lookers - 1].deep) {
			printf("%s\n", json_object_to_json_string_ext(j_file, JSON_C_TO_STRING_PRETTY |
								    JSON_C_TO_STRING_NOSLASHESCAPE));
			return 0;
		} else {
			for (int j = 0; j < nb_lookers; ++j) {

				file_look(f, j_file, &lks[j]);
			}
		}
	}
	if (should_print_array())
		printf("[\n");

	for (struct print_info *tmp = pinfo; tmp; tmp = pinfo) {
		pinfo = pinfo->next;
		print(tmp->f, tmp->k, tmp->j,
		      pinfo && should_print_array());
		free(tmp);
	}

	if (should_print_array())
		printf("]\n");

	for (int i = 0; i < nb_files; ++i)
		json_object_put(j_files[i]);


	if (!nb_found)
		panic("nothing found");

	return 0;
}
