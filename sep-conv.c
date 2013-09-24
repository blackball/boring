/**
 * It's very strange that it's so hard to find a *right* and *straightforeward* 
 * implementation of seperable 2D convolution algorithm.
 *
 * Then, here comes one.
 *
 * I think this one would be relatively fast for your application, but you still
 * could use OpenMP to gain a little more speed.
 *
 * You could easily to wrap a 2D seperable convolution function ;)
 *
 * @blackball
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static void
sep_conv_h(const float *m, int w, int h, const float *kernel, int kernel_size, float *dst) {
        const int border = kernel_size / 2;
        const float *mdata = m;
        
        int i, j, k;

        if (kernel_size > w) {
                return ;
        }

        for (i = 0; i < h; ++i) {
                for (j = 0; j < border; ++j) {
                        float sum = 0.f;
                        int d = j - border;

                        for (k = 0; k < kernel_size; ++k, ++d) {
                                if (d < 0) {
                                        sum += kernel[k] * mdata[i * w];
                                }
                                else {
                                        sum += kernel[k] * mdata[i * w + d];
                                }
                        }
                        
                        dst[i * w + j] = sum;
                }
                
                for (j = border; j < w - border; ++j) {
                        float sum = 0.f;
                        int d = j - border;

                        for (k = 0; k < kernel_size; ++k, ++d) {
                                sum += kernel[k] * mdata[i * w + d];
                        }
                        
                        dst[i * w + j] = sum;
                }

                for (j = w - border; j < w; ++j) {
                        float sum = 0.f;
                        int d = j - border;

                        for (k = 0; k < kernel_size; ++k, ++d) {
                                if (d >= w) {
                                        sum += kernel[k] * mdata[i * w + w - 1];
                                }
                                else {
                                        sum += kernel[k] * mdata[i * w + d];
                                }
                        }

                        dst[i * w + j] = sum;
                }
        }
}

static void
sep_conv_v(const float *m, int w, int h, const float *kernel, int kernel_size, float *dst) {
        const int border = kernel_size / 2;
        const float *mdata = m;
        
        int i, j, k;

        if (kernel_size > h) {
                return ;
        }

        for (i = 0; i < border; ++i) {
                for (j = 0; j < w; ++j) {
                        float sum = 0.f;
                        int d = i - border;

                        for (k = 0; k < kernel_size; ++k, ++d) {
                                if (d < 0) {
                                        sum += kernel[k] * mdata[j];
                                }
                                else {
                                        sum += kernel[k] * mdata[d * w + j];
                                }
                        }

                        dst[i * w + j] = sum;
                }
        }

        for (i = border; i < h - border; ++i) {
                for (j = 0; j < w; ++j) {
                        float sum = 0.f;
                        int d = i - border;

                        for (k = 0; k < kernel_size; ++k, ++d) {
                                sum += kernel[k] * mdata[d * w + j];
                        }

                        dst[i * w + j] = sum;
                }
        }

        for (i = h - border; i < h; ++i) {
                for (j = 0; j < w; ++j) {
                        float sum = 0.f;
                        int d = i - border;

                        for (k = 0; k < kernel_size; ++k, ++d) {
                                if (d >= h) {
                                        sum += kernel[k] * mdata[w*h - w + j];
                                }
                                else {
                                        sum += kernel[k] * mdata[d * w + j];
                                }
                        }

                        dst[i * w + j] = sum;
                }
        }
}

static void 
printm(const float *m, int w, int h) { 
        int i, j;
        printf("\n");
        for (i = 0; i < h; ++i) {
                for (j = 0 ; j < w; ++j) {
                        printf("%f, ", m[i * w + j]);
                }
                printf("\n");
        }
}

static void
use_case(void) {
        const float kernel[3] = {1.f, 2.f, 1.f};

        float m[3*3] = {
                1.f, 2.f, 3.f,
                4.f, 5.f, 6.f,
                7.f, 8.f, 9.f,
        };

        float dst[3*3] = {0};

        /* horizontal convolution */
        sep_conv_h(m, 3, 3, kernel, 3, dst);
                
        printm(dst, 3, 3);

        /* vertical convolution */
        sep_conv_v(m, 3, 3, kernel, 3, dst);

        printm(dst, 3, 3);
}

int
main(int argc, char *argv) {
        use_case();
        return 0;;
}
