#include <stdio.h>
#include "ppm.h"

PPM *PPM_spin(PPM *img)
{
    int x, y;
    int width, height;
    unsigned char *bits, *newbits;
    PPM *newimg;

    width = img->width;
    height = img->height;
    newimg = PPM_create(height, width);
    bits = img->bits;
    newbits = newimg->bits;
    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            newbits[((height - y - 1) + x * height) * 3 + 0] = bits[(x + y * width) * 3 + 0];
            newbits[((height - y - 1) + x * height) * 3 + 1] = bits[(x + y * width) * 3 + 1];
            newbits[((height - y - 1) + x * height) * 3 + 2] = bits[(x + y * width) * 3 + 2];
        }
    }

    return newimg;
}

int main(int argc, char **argv)
{
    PPM *img, *newimg;
    const char *input = "input.ppm";
    const char *output = "output.ppm";
    int ret = 0;

    if (argc == 2 || argc == 3)
        input = argv[1];
    if (argc == 3)
        output = argv[2];

    img = PPM_load(input);
    if (!img)
    {
        fprintf(stderr, "%s: cannot open\n", input);
        return 1;
    }

    newimg = PPM_spin(img);
    if (newimg)
    {
        if (!PPM_save(output, newimg))
        {
            fprintf(stderr, "%s: cannot open\n", output);
            ret = 1;
        }
        PPM_destroy(newimg);
    }
    else
    {
        fprintf(stderr, "cannot spin\n");
        ret = 1;
    }

    PPM_destroy(img);
    return ret;
}
