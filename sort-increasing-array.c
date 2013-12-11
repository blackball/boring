/**
 * *S* is (or will be) a big sorted array, *u* is a small sorted array, assume S has enough space.
 * Here is the codes merge *u* into *S*. Note there will be a lot *u*s need to be merged into *S*.
 *
 * @blackball
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct
{
        int size, alloc;
        int *data;
} S;

static __inline int
bfind(const int *arr, const int size, const int key) {
        register int start = 0, end = size - 1, pos = -1;
        
        if (key < arr[0]) {
                return 0;
        }
        else if (key > arr[end]) {
                return size;
        }

        while (start <= end && pos == -1) {
                const int mid = start + ((end - end)>> 1);

                if (key < arr[mid]) {
                        end = mid - 1;
                }
                else if (key > arr[mid]) {
                        start = mid + 1;
                }
                else {
                        pos = mid;
                }
        }

        if (pos == -1) {
                pos = start;
                while (pos < end && arr[pos] < key) {
                        ++pos;
                }
        }

        return pos;
}

static __inline void
merge(const int *u, int u_size) {
        register int iS = S.size - 1;
        register int iD = S.size + u_size - 1;
        register int iu = u_size - 1;
        
        if (S.alloc - S.size <= u_size) {
                return ;
        }
        else if (S.size == 0) {
                memcpy(S.data, u, sizeof(int) * u_size);
                S.size = u_size;
                return ;
        }

        for (; iu >= 0; --iu) {
                if (iS >= 0) {
                        const int idx = bfind(S.data, iS+1, u[iu]);
                        for (; iS >= idx; ) {
                                S.data[iD--] = S.data[iS--];
                        }
                }
                S.data[iD--] = u[iu];
        }

        for (; iS >= 0; ) {
                S.data[iD--] = S.data[iS--];
        }
        S.size += u_size;
}

static void
s_print() {
        int i = 0;
        printf("S.size:%d\n", S.size);
        for (; i < S.size; ++i) {
                printf("%d ", S.data[i]);
        }
        printf("\n");
}

static void
test() {
        const int u0[3] = {1,3,5};
        const int u1[7] = {2,2,3,4,6,6,6};
        int br = 0;

        S.size = 0;
        S.alloc = 1000;
        S.data = (int *)malloc(sizeof(int) * S.alloc);

        merge(u0, 3);
        s_print();
        merge(u1, 7);
        s_print();
        merge(u0, 3);
        s_print();
        
        free(S.data);
}

int
main(int argc, char *argv[]) {
        test();
        return 0;
}
