#ifndef __exif_h__
#define __exif_h__

#include <stdint.h>
#include <arpa/inet.h>

#include "map.h"


#define PROJECT_URL "github.com/tmathmeyer/***"

#define ZERO_2B (0x0000)
#define JPEG_SOI (0xFFD8)
#define JPEG_EOI (0xFFD9)

#define APP1_MARKER (0xFFE1)
#define EXIF_MARKER (0x45786966)
#define XAP_MARKER (0x68747470)

#define TIFF_BYTE_ALIGN_INTEL (0x4949) // JPEG does _not_ allow this nonsense.
#define TIFF_BYTE_ALIGN_MOTOR (0x4D4D)
#define TIFF_STATIC_2A (0x002A)


#define test_struct(st, f) ((f) == (st)._fixed_ ## f)
#define check_struct(st, f) do { \
  check(((f) != (st)._fixed_ ## f), \
    "line: %04u - (%s).%s != %x (was %x)", \
    __LINE__, #st, #f, f, (st)._fixed_ ## f); \
} while(0)



typedef struct {
  uint16_t _fixed_APP1_MARKER;
  uint16_t size;
  uint32_t marker;
} __attribute__((packed, scalar_storage_order("big-endian"))) AppHeader;


/* EXIF FORMAT (JPG)
+--------+--------+--------+--------+--------+--------+--------+--------+
|        |        |        |        |        |        |   FF   |   E1   |
+--------+--------+--------+--------+--------+--------+--------+--------+
|   APP1 Size     |   45   |   78   |   69   |   66   |   00   |   00   |
+--------+--------+--------+--------+--------+--------+--------+--------+
| motorola align  |   00   |   2a   | the byte offset of IFD past *TIFF |
+--------+--------+--------+--------+--------+--------+--------+--------+
*/
typedef struct {
  AppHeader header;
  uint16_t _fixed_ZERO_2B;
  uint8_t OFFSET_POINT[0]; // for using as an address
  uint16_t _fixed_TIFF_BYTE_ALIGN_MOTOR;
  uint16_t _fixed_TIFF_STATIC_2A;
  uint32_t IDF0_offset;
} __attribute__((packed, scalar_storage_order("big-endian"))) ExifHeader;

typedef struct {
  uint16_t _fixed_JPEG_SOI;
  AppHeader header;
} __attribute__((packed, scalar_storage_order("big-endian"))) JpegHeader;




/* IDF FORMAT
+--------+--------+
|   Entry Count   |
+--------+--------+
         |
         v
    instances of:
         |
         v
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Tag Number    |  "data format"  |    number of "components"         |
+--------+--------+--------+--------+--------+--------+--------+--------+
|      data value _or_ offset       |
+--------+--------+--------+--------+
         |
         v
    followed by:
         |
         v
+--------+--------+--------+--------+
|   offset to next  (stop at 0)     |
+--------+--------+--------+--------+
*/
typedef struct {
	uint16_t tag_type;
	uint16_t tag_format;
	uint32_t tag_components;
  union {
    struct {
      uint32_t offset;
    }__attribute__((scalar_storage_order("big-endian")));
    struct {
      uint32_t value;
    }__attribute__((scalar_storage_order("big-endian")));
  };
} __attribute__((packed, scalar_storage_order("big-endian"))) IdfTag;

typedef struct {
	uint16_t entry_count;
	IdfTag entries[0];
} __attribute__((packed, scalar_storage_order("big-endian"))) IdfTable;

#define next_table_offset_address(t)  (((uint8_t *)(t)) + (sizeof(IdfTable)) + (((t)->entry_count) * sizeof(IdfTag)))
#define rev_next_table_offset(t) (*((uint32_t *)(next_table_offset_address(t))))
#define next_table_offset(t) htonl(rev_next_table_offset(t))
#define next_table(t, o) \
  (next_table_offset(t)==0 ? NULL : \
    (IdfTable *) (o + next_table_offset(t)))

// Type -> sizeof conversion array
#define IdfDataFormatMax (13)





/*XAP format (literally just a stupid xml string)*/
typedef struct {
  AppHeader header;
  char broken_link[25]; // Thanks Adobe...
  char xml[0]; // why tho
} __attribute__((packed, scalar_storage_order("big-endian"))) XapHeader;








// exported functions and structs
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

map_t jpeg_data_parse(void *mm);
void *mmap_image(char *filename);
#endif