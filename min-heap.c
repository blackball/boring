/**
 * Select k smallest / highest elem from an unsorted vector using a min heap.
 *
 * This is very confusing, when we need to select smallest elements, we use max heap,
 * but min heap for biggest elements. The explain is: min heap always store the minium
 * item in the root, the max etc..., sounds really make sense, but how about min heap is
 * used to store minimum items ? This is more easy to remember, IMOP. 
 *
 * @blackball 
 */
#include <string.h>

/* left and right child */
#define LC(i) (i*2 + 1) 
#define RC(i) (i*2 + 2)

/**
 * ri is a parent in heap. when there's new root settled in heap[ri],
 * need to re-arrange heap[ri] to make the sub-tree a heap.
 */
static inline void
heaptify(int *heap, int n, int ri) {
        int root = heap[ri];
        int pos  = ri;
        
        do {    /* iterative process */
                int pp = pos, lp = LC(pos), rp = RC(pos);

                int t = root;
                
                if (lp < n) {
                        if (t < heap[lp]) {
                                pp = lp;
                                t = heap[lp];
                        }
                }

                if (rp < n) {
                        if (t < heap[rp]) {
                                pp = rp;
                                t = heap[rp];
                        }
                }

                /* no bigger child exists */
                if (t == root) {
                        break;
                }

                /* swap root */
                heap[pos] = t;
                heap[pp] = root;
                
                pos = pp;
                root = heap[pp];
        } while (1);        
}

/**
 * Build a heap, you need asure the size of arr is bigger than n
 */
static inline void  
heap_build(int *heap, int n) {
        for (int i = n/2 - 1; i >= 0; --i) {
                heaptify(heap, n, i);
        }
}

static inline int
k_smallest(const int *v, int vn, int *heap, int hn) {
        if (vn < hn) {
                return -1;
        }

        /* fill heap */
        memcpy(heap, v, sizeof(int) * hn);

        /* build heap shape */
        heap_build(heap, hn);

        for (int i = hn; i < vn; ++i) {
                if (v[i] < heap[0]) {
                        heap[0] = v[i];
                        heaptify(heap, hn, 0);
                }
        }

        return 0;
}

#ifdef TEST

#include <stdio.h>

static void 
printv(const int *arr, int n) {
	for (int i = 0; i < n; ++i) {
		printf("%d, ", arr[i]);
	}
	printf("\n");
}

int 
main(int argc, char *argv[]) {
	int arr[9] = {5,17,3,10,84,19,6,22,9};
	int heap[5];

	printv(arr, 9);

	k_smallest(arr, 9, heap, 5);

	printv(heap, 5);
	
	getchar();
	return 0;
}

#endif 
