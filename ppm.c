// ppm.c --- PPM image handling
#include <stdlib.h>
#include <stdio.h>
#include "ppm.h"

#define PPM_BUFSIZE 0x8000

PPM *PPM_create(int width, int height)
{
    PPM *img = (PPM *)malloc(sizeof(PPM));
    if (!img)
        return NULL;

    img->width = width;
    img->height = height;
    img->bits = (unsigned char *)calloc(width * height, 3);
    if (!img->bits)
    {
        free(img);
        return NULL;
    }
    return img;
}

void PPM_destroy(PPM *img)
{
    free(img->bits);
    free(img);
}

PPM *PPM_load(const char *fname)
{
    FILE *fp;
    int type, width, height, max;
    int x, y, tmp, r, g, b;
    char buf[PPM_BUFSIZE];
    PPM *img;
    unsigned char *bits;

    /* Open file */
    fp = fopen(fname, "rb");
    if (fp == NULL)
        return NULL;

    /* Type */
    fgets(buf, PPM_BUFSIZE, fp);
    if (sscanf(buf, "P%d", &type) < 0)
    {
        fprintf(stderr, "%s: invalid format\n", fname);
        fclose(fp);
        return NULL;
    }

    /* Skip comment lines */
    do
    {
        fgets(buf, PPM_BUFSIZE, fp);
    } while(buf[0] == '#');

    /* Width and height */
    if (sscanf(buf, "%d%d", &width, &height) < 0 ||
        width <= 0 || height <= 0)
    {
        fprintf(stderr, "%s: invalid format\n", fname);
        fclose(fp);
        return NULL;
    }

    /* Skip comment lines */
    do
    {
        fgets(buf, PPM_BUFSIZE, fp);
    } while(buf[0] == '#');

    /* Max brightness */
    if (sscanf(buf, "%d", &max) < 0 || !(0 < max && max <= 255))
    {
        fprintf(stderr, "%s: invalid format\n", fname);
        fclose(fp);
        return NULL;
    }

    img = PPM_create(width, height);
    if (img == NULL)
    {
        fclose(fp);
        return NULL;
    }

    bits = img->bits;
    switch(type)
    {
    case 3:     /* Full color ASCII format */
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                if (fscanf(fp, buf, "%d", &r) < 0 ||
                    fscanf(fp, buf, "%d", &g) < 0 ||
                    fscanf(fp, buf, "%d", &b) < 0)
                {
                    PPM_destroy(img);
                    fclose(fp);
                    return NULL;
                }
                *bits++ = r * 255 / max;
                *bits++ = g * 255 / max;
                *bits++ = b * 255 / max;
            }
        }
        break;

    case 6:     /* Full color raw format */
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                if (fread(buf, 3, 1, fp) <= 0)
                {
                    PPM_destroy(img);
                    fclose(fp);
                    return NULL;
                }
                *bits++ = buf[0] * 255 / max;
                *bits++ = buf[1] * 255 / max;
                *bits++ = buf[2] * 255 / max;
            }
        }
        break;

    default:
        fprintf(stderr, "%s: invalid format\n", fname);
        PPM_destroy(img);
        img = NULL;
        break;
    }
    fclose(fp);

    return img;
}

int PPM_save(const char *fname, PPM *img)
{
    FILE *fp = fopen(fname, "wb");
    if (fp == NULL)
        return 0;

    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "255\n");
    fwrite(img->bits, img->width * img->height * 3, 1, fp);
    fclose(fp);
    return 1;
}
