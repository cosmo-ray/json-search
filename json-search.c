#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

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

#define VERBOSE 1
int program_flag;

void obj_lookup(const char *f, struct json_object *o, struct looker *lks)
{
	char *expresion = lks->expresions[lks->deep - 1];

	if (json_object_get_type(o) == json_type_object) {
		json_object_object_foreach(o, k, v) {
			if (program_flag & VERBOSE) {
				printf("object inspect %s\n", k);
			}

			if (!strcmp(expresion, k)) {
				++nb_found;
				printf("%s: %s\n", f,
				       json_object_to_json_string_ext(v,
								      JSON_C_TO_STRING_PRETTY));
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

void file_look(const char *f, struct looker *lks)
{
	struct json_object *j_file = json_object_from_file(f);

	if (!j_file) {
		panic("error in %s json", f);
	}

	if (program_flag & VERBOSE) {
		printf("look for '%s' in file '%s'",
		       lks->expresions[lks->deep - 1], f);
	}
	obj_lookup(f, j_file, lks);
}

int main(int argc, char **argv)
{
	char *files[MAX_FILES];
	int nb_files = 0;
	int nb_lookers = 1;
	struct json_object *j_file;
	static struct looker lks[MAX_LOOKERS];

	if (argc < 3)
		panic("not enough args");

	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			char *pc = argv[i] + 1;

			for(; *pc; ++pc) {
				if (*pc == 'v') {
					printf("verbose mode\n");
					program_flag |= VERBOSE;
				} else {
					panic("'%c': not sure option", *pc);
				}
			}
			continue;
		}
		if (lks[nb_lookers - 1].deep > 0) {
			if (++nb_files > MAX_FILES)
				panic("Too much files, max: %d", MAX_FILES);
			files[nb_files - 1] = argv[i];
		} else {
			int deep = lks[nb_lookers - 1].deep;

			lks[nb_lookers - 1].expresions[deep] = argv[i];
			lks[nb_lookers - 1].deep++;
		}
	}
	for (int i = 0; i < nb_files; ++i)
		for (int j = 0; j < nb_lookers; ++j)
			file_look(files[i], &lks[j]);
	return !nb_found;
}
