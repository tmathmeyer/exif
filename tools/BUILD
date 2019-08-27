load("//rules/core/C/build_defs.py")

cpp_binary(
  name = "photosphere",
  srcs = ["photosphere.c"],
  compiler = "gcc",
  deps = [
    "//exif/lib:libexif_h",
    "//exif/lib:libexif",
  ],
)