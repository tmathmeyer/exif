FLAGS := -Isrc/include
BUILD := build
EXIF_SRCS := $(shell find ./src -name '*.c')
EXIF_OBJS := $(patsubst ./src/%, $(BUILD)/%, $(EXIF_SRCS:%.c=%.o))


all: $(EXIF_OBJS)
	@mkdir -p bin
	@gcc tools/photosphere.c $(EXIF_OBJS) -o bin/photosphere $(FLAGS)
	@echo gcc tools/photosphere.c


$(BUILD)/%.o: src/%.c 
	@mkdir -p $(shell dirname $@)
	@gcc $(FLAGS) -c $< -o $@
	@echo gcc -c $<

clean:
	@rm -rf build
	@rm -rf bin
