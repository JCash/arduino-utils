/* image2header by Mathias Westerdahl

VERSION:
    0.1 2017-06-06  Initial version

LICENSE:
    This tool is public domain.
    No warranty implied or given.
    Use at own risk.

*/

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static void Usage()
{
    fprintf(stderr, "image2header <input image file>\n");
    fprintf(stderr, "\tOutputs a C header representation of the input image.\n");
    fprintf(stderr, "\tThe output format is in LCD compatible format where\n");
    fprintf(stderr, "\teach byte represents 8 pixels in a column in a page (each page is 8 rows).\n");
    fprintf(stderr, "\tThe input colors are converted to 0 or 1, depending on the average luminance of each pixel:\n");
    fprintf(stderr, "\taverage(r,g,b) > 127 ? 1 : 0\n");
    fprintf(stderr, "\tIf alpha exists: a > 127 ? (average(r,g,b) > 127 ? 1 : 0) : 0\n");
}

static uint8_t convert(uint8_t luminance)
{
    return luminance > 127 ? 1 : 0;
}

static uint8_t average(uint8_t r, uint8_t g, uint8_t b)
{
    return (r + g + b) / 3;
}

static uint8_t average(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return a > 127 ? average(r, g, b) : 0;
}

int main(int argc, const char** argv)
{
    if( argc < 2 )
    {
        Usage();
        return 1;
    }

    const char* filename = argv[1];

    int width,height,numchannels;
    uint8_t* data = stbi_load(filename, &width, &height, &numchannels, 0);

    FILE* output = stdout;

    fprintf(output, "const uint16_t image_dimensions[2] = {%u, %u};\n", (uint32_t)width, height);
    fprintf(output, "const uint16_t image_size = %u;\n", (height+7)/8 * width );
    fprintf(output, "const uint8_t image[%u] = {\n\t", (height+7)/8 * width);

    for( int y = 0; y < height; y += 8)
    {
        for( int x = 0; x < width; ++x )
        {
            uint8_t byte = 0;
            for( int bit = 0; bit < 8; ++bit )
            {
                if( y + bit >= height )
                    break;

                uint8_t color = 0;
                if( numchannels == 1 )
                    color = data[(y+bit) * width * numchannels + x * numchannels];
                else if( numchannels == 3 )
                    color = average(data[(y+bit) * width * numchannels + x * numchannels + 0],
                                    data[(y+bit) * width * numchannels + x * numchannels + 1],
                                    data[(y+bit) * width * numchannels + x * numchannels + 2]);
                else if( numchannels == 4 )
                    color = average(data[(y+bit) * width * numchannels + x * numchannels + 0],
                                    data[(y+bit) * width * numchannels + x * numchannels + 1],
                                    data[(y+bit) * width * numchannels + x * numchannels + 2],
                                    data[(y+bit) * width * numchannels + x * numchannels + 3]);

                byte |= convert(color) << bit;
            }

            fprintf(output, "0x%02x,", byte);
        }
        fprintf(output, "\n\t");
    }

    fprintf(output, "};\n");
    stbi_image_free(data);

    return 0;
}