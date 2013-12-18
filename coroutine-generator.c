#include <stdio.h>

static int
select_next_3(const int n, int *a, int *b, int *c) {
        static int ia, ib, ic, state = 0;

        switch(state) {
        case 0: goto g_start;
        case 1: goto g_break;
        }

        g_start:
        for (ia = 0; ia < n; ++ia) {
                *a = ia;
                for (ib = ia + 1; ib < n; ++ib) {
                        *b = ib;
                        for (ic = ib + 1; ic < n; ++ic) {
                                *c = ic;
                                state = 1;
                                return 1;
                        g_break:;
                        }
                }
        }
        return 0;
}

int 
main(int argc, char *argv[]) {
        int a, b, c, n = 4;
        
        while (select_3_next(n, &a, &b, &c)) {
                printf("%d%d%d\n", a, b, c);        
        }
        
        return 0;
}
