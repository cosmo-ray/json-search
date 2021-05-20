
json-search: json-search.o
	$(CC) -ljson-c -std=c99 -Wall -Wextra json-search.o -o json-search

appimagetool-x86_64.AppImage:
	wget https://github.com/AppImage/AppImageKit/releases/download/12/appimagetool-x86_64.AppImage
	chmod +x appimagetool-x86_64.AppImage

json-search-x86_64.AppImage: json-search appimagetool-x86_64.AppImage
	mkdir -p json-search.AppDir/usr/
	mkdir -p json-search.AppDir/usr/bin/
	mkdir -p json-search.AppDir/usr/lib/
	cp json-search json-search.AppDir/usr/bin/
	cp $(shell ldd json-search | grep json | cut -d ' ' -f  3) json-search.AppDir/usr/lib/
	./appimagetool-x86_64.AppImage json-search.AppDir

clean:
	rm json-search.o

.PHONY: clean
