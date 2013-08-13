/**
 * Rotate a rect around it's center. Could be simply modified to polygon rotation.
 *
 * @blackball
 */

#include <math.h>

static void
rotate(float x[4], float y[4], float angle) {
        float cx = 0.0f, cy = 0.0f;

        for (int i = 0; i < 4; ++i) {
                cx += x[i];
                cy += y[i];
        }
        
        cx /= 4;
        cy /= 4;

        /* translate */

        for (int i = 0; i < 4; ++i) {
                x[i] -= cx;
                y[i] -= cy;
        }
        
        /* rotate 
           [x'] = [cos0  -sin0] [x]
           [y'] = [sin0,  cos0] [y]
         */
        float cos0 = cos(angle);
        float sin0 = sin(angle);
        
        for (int i = 0; i < 4; ++i) {
                float
                        xn = cos0 * x - sin0 * y,
                        yn = sin0 * x + cos0 * y;
                x[i] = xn;
                y[i] = yn;
        }

        for (int i = 0; i < 4; ++i) {
                x[i] += cx;
                y[i] += cy;
        }
}

#ifdef TEST

#include <stdio.h>

int
main(int argc, char *argv[]) {

        double x[4] = {100, 300, 300, 100};
        double y[4] = {100, 100, 300, 300};

        rotate(x, y, 50);
        
        return 0;
}

#endif 
