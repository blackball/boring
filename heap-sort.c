/*
  Heap and heap sort, a classical version, and a fast version.

  Fast version using a faster heap construction, and a faster sift down routine:
  Refer to: http://users.aims.ac.za/~mackay/sorting/sorting.html
  
  Default if min heap

  @blackball
*/

#include <stdio.h>
#include <stdlib.h>

typedef int item_type;
#define ITEM_TYPE_DEFAULT (-1)

struct heap_t {
        int size, capacity;
        item_type *data;
};

struct heap_t *
heap_new(int size) {
        struct heap_t *heap = malloc(sizeof(*heap));
        if (heap) {
                heap->data = malloc(sizeof(heap->data[0]) * (size + 1));
                heap->size = 0;
                heap->capacity = size;
        }
        return heap;
}

int
heap_parent_index(const int i) {
        if (i == 1) return -1;
        return i/2;
}

// right child index = left child index + 1
int
heap_left_child_index(int i) {
        return i * 2;
}

static void
_heap_swap(struct heap_t *heap, int i, int j) {
        item_type t = heap->data[i];
        heap->data[i] = heap->data[j];
        heap->data[j] = t;
}

static void
_heap_bubble_up(struct heap_t *heap, int i) {
        const int pi = heap_parent_index(i);
        if (pi == -1) {
                return ;
        }

        if (heap->data[pi] > heap->data[i]) {
                _heap_swap(heap, pi, i);
                _heap_bubble_up(heap, pi);
        }
}

void
heap_insert(struct heap_t *heap, item_type x) {
        if (heap->size < heap->capacity) {
                ++ heap->size;
                heap->data[heap->size] = x;
                _heap_bubble_up(heap, heap->size);
        }
        // @TODO overflow, give some warning !
}

void
heap_free(struct heap_t *heap) {
        if (heap) {
                free(heap->data);
                free(heap);
        }
}

item_type
heap_top(const struct heap_t *heap) {
        if (heap->size < 1) {
                return ITEM_TYPE_DEFAULT;
        }
        return heap->data[1];
}

static void
_heap_bubble_down(struct heap_t *heap, int i) {
        int c, p, min_index, k = 0;

        c = heap_left_child_index(i);
        min_index = i;

        if (c <= heap->size) {
                if (heap->data[min_index] > heap->data[c]) {
                        min_index = c;
                }
        }

        if (c + 1 <= heap->size) {
                if (heap->data[min_index] > heap->data[c + 1]) {
                        min_index = c + 1;
                }
        }
        
        if (min_index != i) {
                _heap_swap(heap, min_index, i);
                _heap_bubble_down(heap, min_index);
        }
}

static void
print_arr(const item_type A[], int n) {
        int i = 0;
        for (; i < n; ++i) {
                printf("%d,", A[i]);
        }
        printf("\n");
}

item_type
heap_pop(struct heap_t *heap) {
        item_type top = ITEM_TYPE_DEFAULT;
        if (heap->size > 0) {
                top = heap->data[1];
                heap->data[1] = heap->data[heap->size];
                -- heap->size;
                _heap_bubble_down(heap, 1);
        }
        return top;
}

static int
_heap_swap_down(struct heap_t *heap, int p) {
        const int c = heap_left_child_index(p);
        if (c > heap->size) {
                return p;
        }
        else {
                if (c + 1 > heap->size) {
                        heap->data[p] = heap->data[c];
                        return c;
                }
                else {
                        int min_index = c;
                        if (heap->data[min_index] > heap->data[c + 1]) {
                                min_index = c + 1;
                        }
                        heap->data[p] = heap->data[min_index];
                        return _heap_swap_down(heap, min_index);
                }
        }
}

// fast heapify used in pop process
item_type
heap_pop_fast(struct heap_t *heap) {
        item_type top = ITEM_TYPE_DEFAULT;
        if (heap->size > 0) {
                int t;
                top = heap->data[1];
                //print_arr(heap->data+1, heap->size);
                t = _heap_swap_down(heap, 1);
                //printf("t:%d\n", t);
                //print_arr(heap->data+1, heap->size);
                if (t != heap->size) {
                        _heap_swap(heap, t, heap->size);
                        -- heap->size;
                }
        }
        return top;
}

struct heap_t *
make_heap(const item_type A[], int n) {
        struct heap_t *heap = heap_new(n);
        if (heap) {
                int i = 0;
                for (; i < n; ++i) {
                        heap_insert(heap, A[i]);
                }
        }
        return heap;
}

// faster heap construction
struct heap_t *
make_heap_fast(const item_type A[], int n) {
        struct heap_t *heap = heap_new(n);
        if (heap) {
                int i = 0;
                heap->size = n;
                for (; i < n; ++i) {
                        heap->data[i + 1] = A[i];
                }
                
                for (i = n; i >= 1; --i) {
                        _heap_bubble_down(heap, i);
                }
        }
        return heap;
}

static void
heap_sort(item_type A[], int n) {
        struct heap_t *heap = make_heap(A, n);
        int i = 0;
        
        for (; i < n; ++i) {
                A[i] = heap_pop(heap);
        }
        print_arr(A, n);
}

static void
heap_sort_fast(item_type A[], int n) {
        struct heap_t *heap = make_heap_fast(A, n);
        int i = 0;
        
        for (; i < n; ++i) {
                A[i] = heap_pop_fast(heap);
        }
        print_arr(A, n);
}

int main(int argc, char *argv[]) {
        item_type A[] = {6,4,2,1,4,6};
        int i = 0;
        for (; i < 7; ++i) {
                heap_sort(A, i);
                heap_sort_fast(A, i);
        }
        return ;
}
