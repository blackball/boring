/**
 * Mosaic effects, based on OpenCV.
 *
 * @blackball
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

static void
mosaic(cv::Mat &img, cv::Rect area, cv::Size size = cv::Size(10, 10)) {

        for (int i = 0; i < img.rows; i += size.height) {
                for (int j = 0; j < img.cols; j += size.width) {
                        cv::Rect rect = cv::Rect(j, i, size.width, size.height) & cv::Rect(0, 0, img.cols, img.rows);

                        cv::Mat sub_img(img, rect);

                        sub_img.setTo(cv::mean(img(rect)));
                }
        }
}


int
main(int argc, char *argv[]) {
        const char *iname = "test.jpg";

        cv::Mat img = cv::imread(iname);
        
        cv::Rect area = cv::Rect(0, 0, img.cols, img.rows);
        
        mosaic(img, area);

        cv::imshow("mosaic", img);
        cv::waitKey(0);
        
        return 0;
}
