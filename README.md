# arduino-utils
A bunch of headers and tools for Arduino development

## Build Tools

Build the tools with

    $ ./build.sh


## image2header

Converts an image into a C++ header with a byte array suitable for rendering on an OLED device (e.g. SSD1306)
The input formats are: .png, .tga, .jpg, .bmp etc. See [src/stb_image.h](stb_image.h) for more info.

Usage:

    $ ./image2header img.png > img.h
