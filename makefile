CC      = clang
FLAGS   = -framework QuartzCore -framework Cocoa -framework MetalKit -framework Metal
SHADER  = ./assets/shaders/triangle.glsl
HEADER  = triangle_shader.h

.PHONY: default run clean

default: build/main

$(HEADER): $(SHADER)
	./sokol-shdc -i $(SHADER) -o $(HEADER) -l metal_macos

build/main: main.c sokol_impl.m $(HEADER)
	mkdir -p build
	$(CC) main.c sokol_impl.m -o build/main $(FLAGS)

run: build/main
	./build/main

clean:
	rm -rf build $(HEADER)
