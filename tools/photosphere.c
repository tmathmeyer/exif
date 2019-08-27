
#include <stdio.h>
#include <string.h>

#include "exif/lib/include/exif_tags.h"
#include "exif/lib/include/exif.h"
#include "exif/lib/include/map.h"

#define ERR_NO_FILE 1
#define NOT_PHOTOSPHERE 2

int main(int argc, char **argv) {
	if (argc < 2) {
		puts("No file");
		return ERR_NO_FILE;
	}

	tag_init();
	map_t map = jpeg_data_parse(mmap_image(argv[1]));

	tabledata *xml;
	if (hashmap_get(map, "XapXml", (void **)&xml)) {
		puts("NOT PHOTOSPHERE");
		return NOT_PHOTOSPHERE;
	}

	if (xml->type != TYPE_REFERENCE) {
		puts("NOT PHOTOSPHERE");
		return NOT_PHOTOSPHERE;
	}

	if(!strstr(xml->ref, "GPano")) {
		puts("NOT PHOTOSPHERE");
		return NOT_PHOTOSPHERE;
	}

	puts("PHOTOSPHERE");
	return 0;
}