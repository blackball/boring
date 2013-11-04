/**
 * Use max- or min- pooling tricks in image subsampling.
 *
 *
 * @blackball
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

struct mpconfig_t {
        int step; /* max-pooling in step * step box */
        int type; /* 0 -> max or 1 -> min */
};

static void
subsample_mp(const struct mpconfig_t *config, unsigned char *dst, int dws, const unsigned char *data, int w, int ws, int h) {
        int i, j, m, n, di, dj;        
        const int step = config->step;
        const int type = config->type & 0x1; // now only two method supported.
        const int mh = h - h % step;
        const int mw = w - w % step;
        unsigned char v = 0;
        
        switch(type) {
        case 0:
                for (di = 0, i = 0; i < mh; i += step, ++di) {
                        for (dj = 0, j = 0; j < mw; j += step, ++dj) {
                                v = 0;
                                for (m = i; m < i+step; ++m) {
                                        for (n = j; n < j+step; ++n) {
                                                v = MAX(v, data[m * ws + n]);
                                        }
                                }
                                dst[di * dws + dj] = v;
                        }
                }
                break;
                
        case 1:                
                for (di = 0, i = 0; i < mh; i += step, ++di) {
                        for (dj = 0, j = 0; j < mw; j += step, ++dj) {
                                v = 255;
                                for (m = i; m < i+step; ++m) {
                                        for (n = j; n < j+step; ++n) {
                                                v = MIN(v, data[m * ws + n]);
                                        }
                                }
                                dst[di * dws + dj] = v;
                        }
                }
                break;
        default:
                assert(0);
                break;
        }
        return ;
}

int
main(int argc, char *argv[]) {
        const int step = 4;
        struct mpconfig_t mp = {step, 0};
        IplImage *sub = NULL, *gray = cvLoadImage("feyman.jpg", 0);

        if (gray == NULL) {
                return -1;
        }
        
        sub = cvCreateImage(cvSize(gray->width / step, gray->height / step), 8, 1);

        subsample_mp(&mp, (unsigned char*)sub->imageData, sub->widthStep, (const unsigned char*)gray->imageData, gray->width, gray->widthStep, gray->height);
        
        cvShowImage("gray", gray);
        cvShowImage("sub", sub);
        cvWaitKey(0);
        cvDestroyAllWindows();
        cvReleaseImage(&sub);
        cvReleaseImage(&gray);
        return 0;
}
