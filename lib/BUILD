load("//rules/core/C/build_defs.py")

EXIF_FILES = ['exif', 'exif_tags', 'map']

for _exif in EXIF_FILES:
  cpp_object (
    name = "libexif_" + _exif,
    compiler = "gcc",
    deps = [ ":libexif_h" ],
    srcs = [
      "exif/" + _exif + ".c",
    ],
  )

cpp_library(
  name = "libexif",
  deps = [":libexif_{}".format(x) for x in EXIF_FILES],
)

c_header(
  name = "libexif_h",
  srcs = [
    "include/exif.h",
    "include/exif_tags.h",
    "include/map.h",
  ],
)