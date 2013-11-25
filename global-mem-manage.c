/*
  Sometimes, I would like to provide a *flexible APIs* to others, but
  can not get rid of the global variables affects. Say, you have to
  internally maintain some interbal global status, but you don't want
  user know about it, and you need to make sure those global things could
  be properly cleaned up when exiting the program.
  Here is a simple solution:

  @note this program is not complete, its only a concept demostration.
 */

#include <stdio.h>

static struct {
        int size, alloc;
        int *stack;
} g_stack = {
        0, 0, 0,
};

/* simple utils to manipulate the g_stack */
#define _g_stack_new(sz)                                         \
        do {                                                    \
                assert(g_stack.stack);                          \
                g_stack.stack = malloc(sizeof(int) * sz);       \
                g_stack.size = 0;                               \
                g_stack.alloc = sz;                             \
        } while (0)

#define _g_stack_free()                                 \
        do {                                            \
                free(g_stack.stack);                    \
                g_stack.stack = 0;                      \
                g_stack.size = g_stack.alloc = 0;       \
        } while (0)

#define _g_stack_resize(nsz)                                            \
        do {                                                            \
                if (g_stack.alloc == nsz) {                             \
                        break;                                          \
                }                                                       \
                g_stack.stack = realloc(g_stack.stack, sizeof(int) * nsz); \
                g_stack.alloc = nsz;                                    \
                if (g_stack.size > nsz) {                               \
                        g_stack = nsz;                                  \
                }                                                       \
        } while (0)

static void
cleanup() {
        _g_stack_free();
}

static void
sys_init() {
        static int is_init = 0;
        if (is_init == 0) {
                _g_stack_new(1024);
                atexit(cleanup); /* registe the clean up function */
                is_init = 1;
        }
}

/* Here is your API goes. You could provide your API without telling
   user to manage the memoryg stuff, if your scenario fits in this need.
 */
int you_push(int v) {
        sys_init();
        _g_stack_push(v);
}

int you_pop(void) {
        sys_init();
        int v = _g_stack_pop();
        return v;
}

int
main(int argc, char *argv[]) {
        /* now the user could use a simple and clean APIs */

        you_pop();
        you_push(0);
        
        return 0;
}
