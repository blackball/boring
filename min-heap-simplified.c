/**
 * About selelct K smallest elements problem: sometimes, the K is very small, 10 or 20 ?
 * Using a decent 'heap' way is not simple and efficient enough.
 * Here is a very naive heap-like way for those scenario.
 *
 * @blackball
 */

#include <string.h>

static inline void
heaptify_simplified(int *heap, int nh) {
        int mi = 0;
        for (int j = 1; j < nh; ++j) {
                if ( heap[j] > heap[mi] ) {
                        mi = j;
                }
        }
        
        int t = heap[0];
        heap[0] = heap[mi];
        heap[mi] = t;
}

static inline int
k_smallest(const int *v, int nv, int *heap, int hn) {
        if (nv < hn) {
                return -1;
        }

        memcpy(heap, v, sizeof(int) * hn);
        
        heaptify_simplified(heap, hn);
        
        for (int i = hn; i < nv; ++i) {
                if ( v[i] < heap[0] ) {
                        heap[0] = v[i];
                        heaptify_simplified(heap, hn);
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


