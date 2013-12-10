/**
 * Paper: An Efficient Representation for Sparse Sets.
 * 
 * Using this structure to calculate the intersection of two unsorted arrays.
 *
 * @blackball
 */

#include <stdio.h>
#include <stdlib.h>

/** static sparse set, for performance **/
static int _ss_i, _ss_n;
static int *_ss_dense;
static int *_ss_sparse;

#if defined(SS_INLINED)

static __inline void
ss_new(const int n) {
        _ss_i = 0;
        _ss_n = n;
        _ss_dense = (int *)malloc(sizeof(int) * n);
        _ss_sparse = (int *)malloc(sizeof(int) * n);
}

static __inline void
ss_free(void) {
        _ss_i = _ss_n = 0;
        free(_ss_dense);
        free(_ss_sparse);
}

static __inline int
ss_add(const int v) {
        _ss_dense[ _ss_i ] = v;
        _ss_sparse[v] = _ss_i;
        _ss_i++;
}

static __inline int
ss_hit(const int v) {
        return _ss_sparse[v] < _ss_i && _ss_dense[ _ss_sparse[v] ];
}

static __inline void
ss_clear(void) {
        _ss_i = 0;
}

#esle // use macro

#define ss_new(n)                                               \
        do {                                                    \
                _ss_i = 0;                                      \
                _ss_n = n;                                      \
                _ss_dense = (int *)malloc(sizeof(int) * n);     \
                _ss_sparse = (int *)malloc(sizeof(int) * n);    \
        } while (0)                                             

#define ss_free()                               \
        do {                                    \
                _ss_i = _ss_n = 0;              \
                free(_ss_dense);                \
                free(_ss_sparse);               \
        } while (0)

#define ss_add(v)                               \
        do {                                    \
                _ss_dense[ _ss_i ] = v;         \
                _ss_sparse[v] = _ss_i;          \
                _ss_i++;                        \
        } while (0)

#define ss_hit(v)                                               \
        (_ss_sparse[v] < _ss_i && _ss_dense[ _ss_sparse[v] ])

#define ss_clear() (_ss_i = 0)

#endif

static __inline int
array_intersection(const int *a, const int *b, int n) {
        int i = 0, ret = 0;
        for (; i < n; ++i) {
                ss_add(a[i]);
        }
        for (i = 0; i < n; ++i) {
                ret += ss_hit(b[i]);
        }
        ss_clear();
        return ret;
}

int
main(int argc, char *argv[]) {
        
        const int a[10] = {0,2,3,24,1,4,5,6,7,12};
        const int b[10] = {2,3,4,5,54,58,111,454, 21,22};
        int i, intersect = 0;
        
        ss_new(512);

        intersect = array_intersection(a, b, 10);
        
        ss_free();
                
        return 0;
}
