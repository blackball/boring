/*
  Select four points from an image, transform the selected area into a rectangle area.

  @blackball
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

using std::vector;
using cv::Mat;
using cv::Point2f;

static inline float 
ddd(const cv::Point2f &p0, const cv::Point2f &p1) {
        return (p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y);
}

static void 
qua_to_rect(const Mat &srcImg, const vector<Point2f> &corners, Mat &dst) {
        // cv::findHomography(corners, targets, 
        vector<Point2f> dstCorners(4);

        const int dw = dst.cols;
        const int dh = dst.rows;
        
#define fill_dstcorner(a,b,c,d)                         \
        do {                                            \
                dstCorners[a] = cv::Point2f(0, 0);      \
                dstCorners[b] = cv::Point2f(dw, 0);     \
                dstCorners[c] = cv::Point2f(dw, dh);    \
                dstCorners[d] = cv::Point2f(0, dh);     \
        } while (0)

        // w:edge(0, 1) < h:edge(1, 2)
        if ( ddd(corners[0], corners[1]) < ddd(corners[1], corners[2]) ) {
                if (dw < dh) {
                        fill_dstcorner(0, 1, 2, 3);
                }
                else {
                        fill_dstcorner(1, 2, 3, 0);
                }
        }
        else {
                if (dw > dh) {
                        fill_dstcorner(0, 1, 2, 3);
                }
                else {
                        fill_dstcorner(1, 2, 3, 0);
                }
        }

        cv::Mat H = cv::getPerspectiveTransform(corners, dstCorners);

        cv::warpPerspective(srcImg, dst, H, dst.size(), 1, cv::INTER_CUBIC);
}

static void 
draw_points(Mat &src, const vector<Point2f> &pts) {
        for (int i = 0; i < pts.size(); ++i) {
                cv::circle(src, cv::Point(pts[i]), 2, cv::Scalar(255, 0, 0, 0), 2);
        }
} 

int 
main(int argc, char *argv[]) {
        
        cv::Mat src = cv::imread("0.bmp", 1);

        vector<Point2f> corners(4);

        corners[0] = Point2f(277,61);
        corners[1] = Point2f(385,52);
        corners[2] = Point2f(410,195);
        corners[3] = Point2f(273,215);

        Mat dst(400, 390, src.type());

        qua_to_rect(src, corners, dst);

        draw_points(src, corners);

        cv::imshow("original", src);
        cv::imshow("result", dst);
        cv::waitKey(0);
        
        return 0;
}
