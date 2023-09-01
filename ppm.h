#pragma once

typedef struct tagPPM
{
    int width, height;
    unsigned char *bits;
} PPM;

PPM *PPM_create(int width, int height);
PPM *PPM_load(const char *fname);
void PPM_destroy(PPM *img);
int PPM_save(const char *fname, PPM *img);
