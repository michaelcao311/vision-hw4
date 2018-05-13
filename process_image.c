#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    x = (x < 0) ? 0 : ((x >= im.w) ? im.w - 1 : x);
    y = (y < 0) ? 0 : ((y >= im.h) ? im.h - 1 : y);

    return im.data[x + y*im.w + c*im.w*im.h];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (x >= 0 && x < im.w && y >= 0 && y < im.h) {
        im.data[x + y*im.w + c*im.w*im.h] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w*im.h*im.c*sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            float r = get_pixel(im, j, i, 0);
            float g = get_pixel(im, j, i, 1);
            float b = get_pixel(im, j, i, 2);
            set_pixel(gray, j, i, 0, 0.299*r+0.587*g+0.114*b);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
          set_pixel(im, j, i, c, get_pixel(im, j, i, c)+v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            for (int k = 0; k < im.c; k++) {
                float value = get_pixel(im, j, i, k);
                value = (value < 0) ? 0 : ((value > 1) ? 1: value);
                set_pixel(im, j, i, k, value);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            float r = get_pixel(im, j, i, 0);
            float g = get_pixel(im, j, i, 1);
            float b = get_pixel(im, j, i, 2);
            float V = three_way_max(r, g, b);
            float m = three_way_min(r, g, b);
            float C = V - m;
            float S = V == 0 ? 0 : C / V;
            float h = (V == r) ? (g - b)/C : ((V == g) ? (b - r)/C+2 : (r - g)/C+4);
            float H = (C == 0) ? 0 : ((h < 0) ? h/6+1 : h/6);

            set_pixel(im, j, i, 0, H);
            set_pixel(im, j, i, 1, S);
            set_pixel(im, j, i, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
            float H = get_pixel(im, j, i, 0)*360;
            float S = get_pixel(im, j, i, 1);
            float V = get_pixel(im, j, i, 2);
            float h = H/60;
            float C = V*S;
            float X = C*(1-fabs(fmod(h, 2)-1));
            
            float r, g, b;
            if (h >= 0 && h <= 1) {
                r = C;
                g = X;
                b = 0;
            } else if (h >= 1 && h <= 2) {
                r = X;
                g = C;
                b = 0;
            } else if (h >= 2 && h <= 3) {
                r = 0;
                g = C;
                b = X;
            } else if (h >= 3 && h <= 4) {
                r = 0;
                g = X;
                b = C;
            } else if (h >= 4 && h <= 5) {
                r = X;
                g = 0;
                b = C;
            } else if (h >= 5 && h <= 6) {
                r = C;
                g = 0;
                b = X;
            } else {
                r = 0;
                g = 0;
                b = 0;
            }

            float m = V - C;
            float R = r + m;
            float G = g + m;
            float B = b + m;
            set_pixel(im, j, i, 0, R);
            set_pixel(im, j, i, 1, G);
            set_pixel(im, j, i, 2, B);
        }
    }
}

void scale_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.h; i++) {
        for (int j = 0; j < im.w; j++) {
          set_pixel(im, j, i, c, get_pixel(im, j, i, c)*v);
        }
    }
}
