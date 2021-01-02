
json-search: json-search.o
	$(CC) -ljson-c -std=c99 -Wall -Wextra json-search.o -o json-search

clean:
	rm json-search.o

.PHONY: clean
