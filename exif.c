#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "map.h"
#include "exif.h"
#include "exif_tags.h"

#define errstr (strerror(errno))

static void check (int test, const char * message, ...) {
    if (test) {
        va_list args;
        va_start (args, message);
        vfprintf (stderr, message, args);
        va_end (args);
        fprintf (stderr, "\n");
        exit (EXIT_FAILURE);
    }
}

#define TYPE_VALUE 0
#define TYPE_REFERENCE 1
typedef struct {
	uint8_t e_size;
	uint8_t e_count;
	uint8_t type;
	union {
		void *ref;
		uint32_t value;
	};
} tabledata;


void parse_table(map_t result, uint32_t table_offset, void *offset_base) {
	IdfTable *idftable = (IdfTable *)(table_offset + offset_base);
	while(idftable) {
		for(int i=0; i<idftable->entry_count; i++) {
			IdfTag t = idftable->entries[i];
			check(t.tag_format >= IdfDataFormatMax, "tag format [%x] on tag [%i] is out of bounds", t.tag_format, i);
			char *tagtype = type(t.tag_type);
			if (tagtype) {
				tabledata *td = malloc(sizeof(tabledata));
				td->e_size = IdfDataFormat[t.tag_format];
				td->e_count = t.tag_components;

				uint32_t entry_size = IdfDataFormat[t.tag_format] * t.tag_components;
				if (entry_size <= 4) {
					td->type = TYPE_VALUE;
					td->value = t.value >> (32 - entry_size * 8);
				} else {
					td->type = TYPE_REFERENCE;
					td->ref = (void *)(offset_base + t.offset);
				}
				hashmap_put(result, tagtype, td);
			} else if (t.tag_type == 0x8769) { // nested table
				parse_table(result, t.offset, offset_base);
			} else if (t.tag_type == 0x8825) {
				puts("Can't handle a GPS table yet!");
			} else {
				printf("Unhandled type: 0x%x\n", t.tag_type);
			}
		}
		idftable = next_table(idftable, offset_base);
	}
}

void parse_exif(ExifHeader *exif, map_t map) {
	check_struct(*exif, ZERO_2B);
	check_struct(*exif, TIFF_BYTE_ALIGN_MOTOR);
	check_struct(*exif, TIFF_STATIC_2A);
	parse_table(map, exif->IDF0_offset, exif->OFFSET_POINT);
}

void parse_xap(XapHeader *xap, map_t map) {
	//TODO parse XML instead of spewing.
	size_t xmllen = xap->header.size - sizeof(XapHeader);
	char xml[xmllen];
	strncpy(xml, (char *)&xap->xml, xmllen);
	printf("%s\n", xml);
}

size_t app_parse(AppHeader *header, map_t recovered) {
	//check_struct(*header, APP1_MARKER); // Better way to do this
	switch(header->marker) {
		case EXIF_MARKER:
			parse_exif((ExifHeader *)header, recovered);
			break;
		case XAP_MARKER:
			parse_xap((XapHeader *)header, recovered);
			break;
		default:
			return 0;
	}

	return (((size_t)header) + header->size + 2);
}

map_t jpeg_data_parse(void *mm) {
	JpegHeader *jpeg = (JpegHeader *)mm;
	check_struct(*jpeg, JPEG_SOI);

	map_t result = hashmap_new();
	if (!result) {
		return NULL;
	}

	AppHeader *header = &jpeg->header;
	do {
		size_t app_offset = app_parse(header, result);
		header = (AppHeader *)app_offset;
	} while(header);

	return result;
}

void *mmap_file(char *filename) {
	int fd = open(filename, O_RDONLY);
	check(fd < 0, "open %s failed: %s", filename, errstr);
	struct stat s;
	check(fstat(fd, &s) < 0, "stat %s failed: %s", filename, errstr);
	void *mapped = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	check(mapped == MAP_FAILED, "mmap %s failed: %s", filename, errstr);
	return mapped;
}

void do_image(char *image) {
	map_t map = jpeg_data_parse(mmap_file(image));

	char *key;
	tabledata *val;
	hashmap_iterate(map, key, val) {
		switch(val->type) {
			case TYPE_VALUE:
				printf("%s: %i\n", key, val->value);
				break;
			case TYPE_REFERENCE:
				printf("%s: %s\n", key, (char *)(val->ref));
				break;
		}
	}
}

int main() {
	tag_init();
	do_image("/home/ted/Downloads/pano.jpg");
	printf("\n\n==============================\n\n\n");
	do_image("/home/ted/gallery/.images/album/GOPR0617.JPG");
}