#include "image.h"
#include <math.h>

float nn_interpolate(image im, float x, float y, int c)
{
    int rounded_x = (int) lroundf(x);
    int rounded_y = (int) lroundf(y);
    return get_pixel(im, rounded_x, rounded_y, c);
}

image nn_resize(image im, int w, int h)
{
    image result = make_image(w, h, im.c);
    float x_a = im.w / (float) w;
    float x_b = -0.5 + 0.5*x_a;
    float y_a = im.h / (float) h;
    float y_b = -0.5 + 0.5*y_a;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < im.c; k++) {
                float x = x_a*i + x_b;
                float y = y_a*j + y_b;
                float interpolated_value = nn_interpolate(im, x, y, k);
                set_pixel(result, i, j, k, interpolated_value);
            }
        }
    }
    return result;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int x_floor = (int) floor(x);
    int x_ceil  = (int) ceil(x);
    int y_floor = (int) floor(y);
    int y_ceil  = (int) ceil(y);
    float v1 = get_pixel(im, x_floor, y_floor, c);
    float v2 = get_pixel(im, x_ceil, y_floor, c);
    float v3 = get_pixel(im, x_floor, y_ceil, c);
    float v4 = get_pixel(im, x_ceil, y_ceil, c);
    float d1 = x - x_floor;
    float d2 = x_ceil - x;
    float d3 = y - y_floor;
    float d4 = y_ceil - y;

    return v1*d2*d4 + v2*d1*d4 + v3*d2*d3 + v4*d1*d3;
}

image bilinear_resize(image im, int w, int h)
{
    image result = make_image(w, h, im.c);
    float x_a = im.w / (float) w;
    float x_b = -0.5 + 0.5*x_a;
    float y_a = im.h / (float) h;
    float y_b = -0.5 + 0.5*y_a;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            for (int k = 0; k < im.c; k++) {
                float x = x_a*i + x_b;
                float y = y_a*j + y_b;
                float interpolated_value = bilinear_interpolate(im, x, y, k);
                set_pixel(result, i, j, k, interpolated_value);
            }
        }
    }
    return result;
}

