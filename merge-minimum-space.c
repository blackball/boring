/**
 * Merge two sorted array with minimum memory requirement.
 *
 * @blackball
 */

#include <stdlib.h>
#include <string.h>

static inline void
merge_min_space(int **pdst, int nd, int *src, int ns) {
        *pdst = (int *)realloc(*pdst, sizeof(int) * (nd + ns));

        int *dst = *pdst;
        int g = nd + ns - 1;
        int i = nd - 1;
        int j = ns - 1;
        
        while (1) {
                if ( dst[i] > src[j] ) {
                        dst[ g-- ] = dst[ i-- ];
                }
                else {
                        dst[ g-- ] = src[ j-- ];
                }

                if (j < 0) {
                        break;
                }

                if (i < 0) {
                        while ( g >= 0) {
                                dst[g] = src[g];
                                g --;
                        }
                        break;
                }
        }
}

#ifdef MAIN_TEST

int
main(int argc, char *argv[]) {
        const int nd = 3;
        const int ns = 4;
        int *dst = (int *)malloc(sizeof(int) * nd);
        int *src = (int *)malloc(sizeof(int) * ns);

        dst[0] = 0;
        dst[1] = 1;
        dst[2] = 4;

        src[0] = 2;
        src[1] = 2;
        src[2] = 3;
        src[3] = 5;

        merge_min_space(&dst, 3, src, 4);

        free(dst);
        free(src);
        
        return 0;
}

#endif 
