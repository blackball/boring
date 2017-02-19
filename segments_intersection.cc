/*
----------------------------------------
calculate intersection of two segments:
improved based on: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

@blackball 
----------------------------------------
*/

#include <iostream>

struct Point2df {
        float x, y;

        Point2df(){}
        Point2df(float _x, float _y) : x(_x), y(_y){}
        inline bool operator==(const Point2df& other) const {
                return x == other.x && y == other.y;
        }
};


bool
segment_intersection(const Point2df &p0, const Point2df &p1, const Point2df &p2, const Point2df &p3, Point2df &ip) {
        const float
                s1_x = p1.x - p0.x, s1_y = p1.y - p0.y,
                s2_x = p3.x - p2.x, s2_y = p3.y - p2.y,
                det = -s2_x * s1_y + s1_x * s2_y;
        if (det != 0.f) {
                const float inv_det = 1.0f / det;
                const float s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) * inv_det;
                const float t = ( s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) * inv_det;
                if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
                        ip.x = p0.x + t * s1_x;
                        ip.y = p0.y + t * s1_y;
                        return true;
                }
                return false;
        }
        
        const bool b0 = (p0 == p2 || p0 == p3), b1 = (p1 == p2 || p1 == p3);
        if ((b0 || b1) && ((b0^b1) || (p0 == p1))) {
                ip = b0 ? p0 : p1;
                return true;
        }
        return false;        
}


int main(int argc, char **argv) {
        bool ret;
        Point2df ip;
        
        ret = segment_intersection(Point2df(0,0), Point2df(2,2), Point2df(0,1), Point2df(1,0), ip);
        std::cout << "normal cases: intersected" << std::endl;
        std::cout << "expect: 1 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;

        ret = segment_intersection(Point2df(0, 0), Point2df(1, 4), Point2df(1, 1), Point2df(2, 2), ip);
        std::cout << "normal cases: not intersected" << std::endl;
        std::cout << "expect: 0 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;

        ret = segment_intersection(Point2df(0, 0), Point2df(1, 1), Point2df(1, 1), Point2df(2, 2), ip);
        std::cout << "special cases: share one point, positive length" << std::endl;
        std::cout << "expect: 1 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;

        ret = segment_intersection(Point2df(0, 0), Point2df(1, 1), Point2df(1, 1), Point2df(0, 0), ip);
        std::cout << "special cases: share two points, positive length" << std::endl;
        std::cout << "expect: 0 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;

        ret = segment_intersection(Point2df(1, 1), Point2df(1, 1), Point2df(1, 1), Point2df(2, 2), ip);
        std::cout << "special cases: share one point, one zero length" << std::endl;
        std::cout << "expect: 1 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;

        ret = segment_intersection(Point2df(0, 0), Point2df(1, 1), Point2df(1, 1), Point2df(1, 1), ip);
        std::cout << "special cases: share one point, one zero length" << std::endl;
        std::cout << "expect: 1 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;

        ret = segment_intersection(Point2df(0, 0), Point2df(0, 0), Point2df(0, 0), Point2df(0, 0), ip);
        std::cout << "special cases: share two points, two zero length" << std::endl;
        std::cout << "expect: 1 : " << ret << std::endl;
        std::cout << ip.x << "," << ip.y << std::endl;
        return 0;
}
