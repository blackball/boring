/**
 * In fact, here is a simple trianguar matrix (the column size incresing one, whatever low or high)
 * I used it to cache the distance when performing some clustering algorithm. Simple, and small.
 * 
 * @blackball
 */

#include <stdio.h>
#include <stdlib.h>

struct tcache {
        union {int size, rows, cols;};
        double *data;
};

#define tcache_at(tc, r, c) ( (tc)->data[ ((c) <= (r)) ? ((r) * ((r) + 1) / 2 + (c)) : ((c) * ((c) + 1) / 2 + (r))] )

/* I could assume that the distance will always >= 0,
   so a negtive value could used to tell "unset" */
#define TCACHE_UNSET (-1.0)

struct tcache *
tcache_new(int size) {
        struct tcache *tc = (struct tcache *)malloc(sizeof(*tc));
        
        const int sz = size * (size + 1) / 2;

        tc->data = (double *)malloc(sizeof(double) * sz);
        tc->size = size;

        /* set to default */
        for (int i = 0; i < sz; ++i) {
                tc->data[i] = TCACHE_UNSET;
        }
        
        return tc;
}

void
tcache_free(struct tcache **tc) {
        if (tc) {
                free( (*tc)->data );
                free( (*tc) );
                *tc = NULL;
        }
}

/**
 * search if the value of (a,b) was calculated before.
 * if hit success, will store the value in d.
 * return 1 if success, 0 for miss, -1 for error
 */
int
tcache_hit(const struct tcache *tc, int a, int b, double *d) {
#if defined(_DEBUG)
        if ( a >= tc->size || b >= tc->size) {
                return -1;
        }
#endif 
        const double t = tcache_at(tc, a, b);
        
        if ( t == TCACHE_UNSET) {
                return 0;
        }

        *d = t;
        
        return 1;
}

int
tcache_insert(struct tcache *tc, int a, int b, double d) {
#if defined(_DEBUG)
        if ( a >= tc->size || b >= tc->size) {
                return -1;
        }
#endif
        tcache_at(tc, a, b) = d;
        
        return 0;
}

#ifdef TEST
        
#include <stdio.h>

int
main(int argc, char *argv[]) {
        struct tcache *tc = tcache_new(10);

        for (int i = 0; i < 100; ++i) {
                tcache_insert(tc, i, i, 10);
        }


        int hit = 0;
        double d;
        
        hit = tcache_hit(tc, 3, 2, &d);
        hit = tcache_hit(tc, 3, 3, &d);
        hit = tcache_hit(tc, 3, 99, &d);
        

        tcache_free(&tc);
        
        return 0;
}

#endif 
