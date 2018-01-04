# exif
a set of tools for parsing exif data. written in C, with libs coming in later in python and golang and maybe some others

## motivation
the 'identify' command from imagemagick is slow, and won't show XAP data (for testing if a photo is a photosphere)
this tool is fast and can parse XAP (yay!)

## building
run make. you dont need any libraries outside libc.