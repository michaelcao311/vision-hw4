#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum = 0.0;
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.c; k++) {
                sum += get_pixel(im, i, j, k);
            }
        }
    }

    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.c; k++) {
                float v = get_pixel(im, i, j, k);
                set_pixel(im, i, j, k, v*(1/sum));
            }
        }
    }
}

image make_box_filter(int w)
{
    image result = make_image(w, w, 1);
    float inv_nm = 1.0/(w*w);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            set_pixel(result, i, j, 0, inv_nm);
        }
    }
    return result;
}

image convolve_image(image im, image filter, int preserve)
{
    // make sure filter has 1 channel or same number as im
    assert(filter.c == 1 || filter.c == im.c);
    int shift = (filter.w-1)/2;
    int filter_single_channel = filter.c == 1;
    if (preserve == 1) {
        image result = make_image(im.w, im.h, im.c);
        for (int i = 0; i < im.w; i++) {
            for (int j = 0; j < im.h; j++) {
                for (int k = 0; k < im.c; k++) {
                    float sum = 0;
                    for (int l = 0; l < filter.w; l++) {
                        for (int m = 0; m < filter.h; m++) {
                            float v_image = get_pixel(im, i-shift+l, j-shift+m, k);
                            float v_filter = get_pixel(filter, l, m, filter_single_channel ? 0 : k);
                            sum += v_image*v_filter;
                        }
                    }
                    set_pixel(result, i, j, k, sum);
                }
            }
        }
        return result;
    } else {
        image result = make_image(im.w, im.h, 1);
        for (int i = 0; i < im.w; i++) {
            for (int j = 0; j < im.h; j++) {
                float sum = 0;
                for (int k = 0; k < im.c; k++) {
                    for (int l = 0; l < filter.w; l++) {
                        for (int m = 0; m < filter.h; m++) {
                            float v_image = get_pixel(im, i-shift+l, j-shift+m, k);
                            float v_filter = get_pixel(filter, l, m, filter_single_channel ? 0 : k);
                            sum += v_image*v_filter;
                        }
                    }
                }
                set_pixel(result, i, j, 0, sum);
            }
        }
        return result;
    }
}

image make_highpass_filter()
{
    image result = make_image(3, 3, 1);
    set_pixel(result, 0, 0, 0, 0);
    set_pixel(result, 1, 0, 0, -1);
    set_pixel(result, 2, 0, 0, 0);
    set_pixel(result, 0, 1, 0, -1);
    set_pixel(result, 1, 1, 0, 4);
    set_pixel(result, 2, 1, 0, -1);
    set_pixel(result, 0, 2, 0, 0);
    set_pixel(result, 1, 2, 0, -1);
    set_pixel(result, 2, 2, 0, 0);
    return result;
}

image make_sharpen_filter()
{
    image result = make_image(3, 3, 1);
    set_pixel(result, 0, 0, 0, 0);
    set_pixel(result, 1, 0, 0, -1);
    set_pixel(result, 2, 0, 0, 0);
    set_pixel(result, 0, 1, 0, -1);
    set_pixel(result, 1, 1, 0, 5);
    set_pixel(result, 2, 1, 0, -1);
    set_pixel(result, 0, 2, 0, 0);
    set_pixel(result, 1, 2, 0, -1);
    set_pixel(result, 2, 2, 0, 0);
    return result;
}

image make_emboss_filter()
{
    image result = make_image(3, 3, 1);
    set_pixel(result, 0, 0, 0, -2);
    set_pixel(result, 1, 0, 0, -1);
    set_pixel(result, 2, 0, 0, 0);
    set_pixel(result, 0, 1, 0, -1);
    set_pixel(result, 1, 1, 0, 1);
    set_pixel(result, 2, 1, 0, 1);
    set_pixel(result, 0, 2, 0, 0);
    set_pixel(result, 1, 2, 0, 1);
    set_pixel(result, 2, 2, 0, 2);
    return result;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We use preserve on sharpen and emboss. This is because they preserve color, while highpass is greyscale.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Yes, we must clamp for all of them because they can cause overflow.

image make_gaussian_filter(float sigma)
{
    int size = (int) ceil(sigma*6);
    size = size % 2 == 0 ? size+1 : size;
    int shift = size/2;
    image result = make_image(size, size, 1);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int x = i - shift;
            int y = j - shift;
            float val = 1/(TWOPI*sigma*sigma) * exp(-((x*x)+(y*y))/(2*sigma*sigma));
            set_pixel(result, i, j, 0, val);
        }
    }
    l1_normalize(result);
    return result;
}

image add_image(image a, image b)
{
    assert(a.w==b.w && a.h==b.h && a.c==b.c);
    image result = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w; i++) {
        for (int j = 0; j < a.h; j++) {
            for (int k = 0; k < a.c; k++) {
                float v_a = get_pixel(a, i, j, k);
                float v_b = get_pixel(b, i, j, k);
                set_pixel(result, i, j, k, v_a+v_b);
            }
        }
    }
    return result;
}

image sub_image(image a, image b)
{
    assert(a.w==b.w && a.h==b.h && a.c==b.c);
    image result = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w; i++) {
        for (int j = 0; j < a.h; j++) {
            for (int k = 0; k < a.c; k++) {
                float v_a = get_pixel(a, i, j, k);
                float v_b = get_pixel(b, i, j, k);
                set_pixel(result, i, j, k, v_a-v_b);
            }
        }
    }
    return result;
}

image make_gx_filter()
{
    image result = make_image(3, 3, 1);
    set_pixel(result, 0, 0, 0, -1);
    set_pixel(result, 1, 0, 0, 0);
    set_pixel(result, 2, 0, 0, 1);
    set_pixel(result, 0, 1, 0, -2);
    set_pixel(result, 1, 1, 0, 0);
    set_pixel(result, 2, 1, 0, 2);
    set_pixel(result, 0, 2, 0, -1);
    set_pixel(result, 1, 2, 0, 0);
    set_pixel(result, 2, 2, 0, 1);
    return result;
}

image make_gy_filter()
{
    image result = make_image(3, 3, 1);
    set_pixel(result, 0, 0, 0, -1);
    set_pixel(result, 1, 0, 0, -2);
    set_pixel(result, 2, 0, 0, -1);
    set_pixel(result, 0, 1, 0, 0);
    set_pixel(result, 1, 1, 0, 0);
    set_pixel(result, 2, 1, 0, 0);
    set_pixel(result, 0, 2, 0, 1);
    set_pixel(result, 1, 2, 0, 2);
    set_pixel(result, 2, 2, 0, 1);
    return result;
}

void feature_normalize(image im)
{
    assert(im.w>0 && im.h>0 && im.c>0);
    float min = get_pixel(im, 0, 0, 0);
    float max = min;
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.c; k++) {
                float val = get_pixel(im, i, j, k);
                min = val < min ? val : min;
                max = val > max ? val : max;
            }
        }
    }
    float range = max-min;
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.c; k++) {
                float val = get_pixel(im, i, j, k);
                set_pixel(im, i, j, k, range == 0 ? 0 : (val-min)/range);
            }
        }
    }
}

image *sobel_image(image im)
{
    image *result = calloc(2, sizeof(image));
    image gx = make_gx_filter();
    image gy = make_gy_filter();
    image convolve_x = convolve_image(im, gx, 0);
    image convolve_y = convolve_image(im, gy, 0);

    image G = make_image(im.w, im.h, 1);
    image theta = make_image(im.w, im.h, 1);

    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            float val_gx = get_pixel(convolve_x, i, j, 0);
            float val_gy = get_pixel(convolve_y, i, j, 0);
            float val_G = sqrtf(val_gx*val_gx + val_gy*val_gy);
            float val_theta = atanf(val_gy/val_gx);
            set_pixel(G, i, j, 0, val_G);
            set_pixel(theta, i, j, 0, val_theta);
        }
    }
    result[0] = G;
    result[1] = theta;
    return result;
}

image colorize_sobel(image im)
{
    image *sobel = sobel_image(im);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);
    image result = make_image(im.w, im.h, 3);
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            float val_G = get_pixel(sobel[0], i, j, 0);
            float val_theta = get_pixel(sobel[1], i, j, 0);
            set_pixel(result, i, j, 0, val_theta);
            set_pixel(result, i, j, 1, val_G);
            set_pixel(result, i, j, 2, val_G);
        }
    }
    hsv_to_rgb(result);
    free_image(sobel[0]);
    free_image(sobel[1]);
    free(sobel);
    return result;
}
