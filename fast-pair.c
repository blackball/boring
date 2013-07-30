/**
 * A C version that could be easily embded in other scenario. 
 *
 * you could find the original C++ version here:
 * http://www.ics.uci.edu/~eppstein/projects/pairs/Source/testbed/
 * and the reference paper:
 * http://www.ics.uci.edu/~eppstein/projects/pairs/Papers/Epp-SODA-98.pdf
 *
 * The original version requires you give feed data points to this structure,
 * which is very space consuming. In fact, what this algorithm need only is
 * a distance function, in my version, only the index was required, but this 
 * would make the distance fucntion implementation a little tricky. The 
 * testing codes in the end will demostrate that.
 *
 * @blackball
 */

#include <stdlib.h>
#include <float.h> /* DBL_MAX */

struct fastpair {
        int npoints;
        int *points;
        int *where;
        int *neighbors;
        double *nbr_dist;

        /* external data used by distance function, modified by your needs */
        const int *data;
};

#define MAX_DISTANCE DBL_MAX

static inline double
fastpair_distance(const fastpair *fp, int a, int b) {
        const int *data = fp->data;

        int d = data[a] - data[b];

        return (d > 0 ? d : -d);
}

/**
 * create a fastpair structure.
 * np is number of points
 * mp is the maximum of points
 * you could set np = mp as you needs.
 * return NULL is failed.
 */
struct fastpair *
fastpair_new(int np, int mp, const int *data) {
#if defined(_DEBUG)
        if (np > mp || np <= 0) {
                return NULL;
        }
#endif 
        
        struct fastpair *fp = (struct fastpair *)malloc(sizeof(*fp));

        if (!fp) {
                exit(-1);
        }
        
        fp->npoints = np;

        fp->points = (int *)malloc(sizeof(int) * mp);
        fp->where = (int *)malloc(sizeof(int) * mp);
        fp->neighbors = (int *)malloc(sizeof(int) * mp);
        fp->nbr_dist = (double *)malloc(sizeof(double) * mp);

        if (!fp->points || !fp->where || !fp->neighbors || !fp->nbr_dist) {
                /* memory use out ?? */
                exit(-1);
        }

        /* link external data */
        fp->data = data;
        
        /* if your index is not like this, you need modifed this function, and assign your index to points */
        for (int i = 0; i < np; ++i) {
                fp->points[i] = i;
        }

        int *p = fp->points;
        
        while (np > 1) {
                int nbr = 1;
                double nbd = fastpair_distance(fp, p[0], p[1]);
                for (int i = 2; i < np; ++i) {
                        double d = fastpair_distance(fp, p[0], p[i]);

                        if (d < nbd) {
                                nbr = i;
                                nbd = d;
                        }
                }
                /* add 1nn edge */
                fp->neighbors[ p[0] ] = p[ nbr ];
                fp->nbr_dist[ p[0] ] = nbd;
                p[ nbr ] = p[1];
                p[1] = fp->neighbors[ p[0] ];
                
                p ++;
                np --;
        }

        /* no more neighbors, terminate */
        fp->neighbors[ p[0] ] = p[0];
        fp->nbr_dist[ p[0] ] = MAX_DISTANCE;

        for (int i = 0; i < fp->npoints; ++i) {
                fp->where[ fp->points[i] ] = i;
        }

        return fp;
}

void
fastpair_free(struct fastpair **fp) {
        if (fp && (*fp)) {
                free( (*fp)->points );
                free( (*fp)->where );
                free( (*fp)->neighbors);
                free( (*fp)->nbr_dist);
                *fp = NULL;
        }
}

/**
 * find the NN of a given point 
 */
static inline void
fastpair_findnn(struct fastpair *fp, int p) {
        if (fp->npoints == 1) {
                fp->neighbors[p] = p;
                fp->nbr_dist[p] = MAX_DISTANCE;
                return ;
        }

        int first_nbr = 0;

        if (p == fp->points[first_nbr]) {
                first_nbr = 1;
        }

        fp->neighbors[p] = fp->points[first_nbr];
        fp->nbr_dist[p] = fastpair_distance(fp, p, fp->neighbors[p]);

        for (int i = first_nbr + 1; i < fp->npoints; ++i) {
                int q = fp->points[i];
                if (q != p) {
                        double d = fastpair_distance(fp, p, q);
                        if (d < fp->nbr_dist[p]) {
                                fp->nbr_dist[p] = d;
                                fp->neighbors[p] = q;
                        }
                }
        }
}

static inline void
fastpair_add(struct fastpair *fp, int p) {
        fastpair_findnn(fp, p);
        fp->points[ fp->where[p] = fp->npoints ++ ] = p; 
}

static inline void
fastpair_delete(struct fastpair *fp, int p) {
        fp->npoints --;

        int q = fp->where[p];

        fp->where[ fp->points[q] = fp->points[ fp->npoints ] ] = q;

        for (int i = 0; i < fp->npoints; ++i) {
                if (fp->neighbors[ fp->points[i] ] == p) {
                        fastpair_findnn(fp, fp->points[i]);
                }
        }
}

/**
 * Find the closest pair (a, b), and store the closes distance in dist.
 *
 * if error happened, return -1, else 0
 */
int 
fastpair_find(const struct fastpair *fp, int *a, int *b, double *dist) {
        if ( fp->npoints < 2 ) {
                /* !! */
                return -1;
        }

        double d = fp->nbr_dist[ fp->points[0] ];
        int r = 0;

        for (int i = 1; i < fp->npoints; ++i) {
                const double t = fp->nbr_dist[ fp->points[i] ];
                if ( t < d) {
                        d = t;
                        r = i;
                }
        }

        *a = fp->points[r];
        *b = fp->neighbors[ *a ];
        *dist = d;

        return 0;
}


#define TEST

#ifdef TEST

#include <stdio.h>

int
main(int argc, char *argv[]) {
        int pts[10] = {
                2,4,6,8,11,55,77,99,33,11,
        };

        struct fastpair * fp = fastpair_new(10, 10, pts);

        int a, b;
        double dist;

        int err = fastpair_find(fp, &a, &b, &dist);

        if (err != 0) {
                fprintf(stderr, "Exception!\n");
                exit(-1);
        }

        printf("%d, %d, %lf\n", a, b, dist);

        fastpair_delete(fp, a);

        err = fastpair_find(fp, &a, &b, &dist);

        if (err != 0) {
                fprintf(stderr, "Exception!\n");
                exit(-1);
        }

        printf("%d, %d, %lf\n", a, b, dist);

        fastpair_free( &fp );
        
        getchar();

        return 0;
}

#endif 
