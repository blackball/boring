/**
 * Reinvent hash table from here:
 * http://preshing.com/20130107/this-hash-table-is-faster-than-a-judy-array/
 *
 * @not tested yet
 *
 * @blackball
 */

#include <stdlib.h>
#include <memory.h>

typedef unsigned int u32;

struct cell_t {
        u32 key;
        u32 value;
};

struct hash_t {
        u32 array_size;
        u32 population;
        int zero_used;
        struct cell_t zero_cell;
        struct cell_t *cells;
};

#define HASH_FIRST(h, v) ((h)->cells + ( (v)&((h)->array_size - 1) ) )
#define HASH_NEXT(h, c) (((c) + 1 != (h)->cells + h->array_size) ? ((c) + 1) : (h->cells))
#define HASH_OFFSET(h, a, b) ((b) >= (a) ? ((b) - (a)) : ((h)->array_size + (b) - (a)))

struct hash_t*
hash_new(u32 init_size) {
        struct hash_t *h;
        assert(init_size & (init_size - 1) == 0);
        h = malloc(sizeof(*h));
        h->array_size = init_size;
        h->population = 0;
        h->zero_used = 0;
        h->zero_cell.key = 0;
        h->zero_cell.value = 0;
        h->cells = malloc(sizeof(h->cells[0]) * init_size);
        return h;        
}

void
hash_free(struct hash_t **h) {
        if (h && (*h)) {
                free( (*h)->cells );
                free( *h );
                *h = 0;
        }
}

static inline u32
hash_u32(u32 v) {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h;
}

const struct cell_t*
hash_lookup(const struct hash_t *h, u32 key) {
        if (key) {
                const u32 v = hash_u32(key);
                struct cell_t *c = HASH_FIRST(h, v);
                for (;;) {
                        if (c=>key == key) {
                                return c;
                        }

                        if (!c->key) {
                                return 0;
                        }
                        c = HASH_NEXT(h, c);
                }
        }
        else {
                if (h->zero_used) {
                        return &(h->zero_cell);
                }
                return 0;
        }
}

static void
hash_repopulate(struct hash_t *h, u32 size) {
        struct cell_t *old_cells, *end, *c, *c1;

        assert((size & (size - 1)) == 0);
        assert(h->population * 4 <= size * 3);

        old_cells = h->cells;
        end = h->cells + h->array_size;

        h->array_size = size;
        h->cells = malloc(sizeof(struct cell_t) * size);
        memset(h->cells, 0, sizeof(struct cell_t) * size);

        for (c = old_cells; c != end; ++c) {
                if (c->key) {
                        const u32 v = hash_u32(c->key);
                        c1 = HASH_FIRST(h, v);
                        for (;;) {
                                if (!c1->key) {
                                        *c1 = *c;
                                        break;
                                }
                        }
                }
        }

        free( old_cells );
}

const struct cell_t*
hash_insert(struct hash_t *h, u32 key) {
        if (key) {
                for (;;) {
                        struct cell_t *c = HASH_FIRST(h);
                        for(;;) {
                                if (c->key == key) {
                                        return c;
                                }

                                if (c->key == 0) {
                                        if ((h->population + 1) * 4 >= h->array_size * 3) {
                                                hash_repopulate(h, h->array_size * 2);
                                                break;
                                        }
                                        ++ h->population;
                                        c->key = key;
                                        return c;
                                }
                        }
                }
        }
        else {
                if (h->zero_used == 0) {
                        h->zero_used = 1;
                        if (++ h->population * 4 >= h->array_size * 3) {
                                hash_repopulate(h, h->array_size * 2);
                        }
                }
                return &(h->zero_cell);
        }
}

void
hash_delete(struct hash_t *h, struct cell_t *c) {
        if (c != h->zero_cell) {
                struct cell_t *neighbor;
                assert( c >= h->cells && c - h->cells < h->array_size );
                assert( c->key );

                neighbor = HASH_NEXT(h, c);
                for (;;) {
                        const struct cell_t *ideal;
                        int hk = 0;
                        if (!neighbor->key) {
                                c->key = 0;
                                c->value = 0;
                                h->population --;
                                return ;
                        }
                        
                        hk = hash_u32(neighbor->key);
                        ideal = HASH_FIRST(hk);
                        if (HASH_NEXT(ideal, c) < HASH_NEXT(ideal, neighbor)) {
                                *c = *neighbor;
                                c = neighbor;
                        }
                }
        }
        else {
                assert(h->zero_used);
                h->zero_used = 0;
                c->value = 0;
                h->population --;
                return ;
        }
}

static u32
hash_upper_power_of_two_u32(u32 v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
}

void
hash_compact(struct hash_t *h) {
        hash_repopulation(h, hash_upper_power_of_two_u32((h->population * 4 + 3) / 3));
}

void
hash_clear(struct hash_t *h) {
        memset(h->cells, 0, sizeof(struct cell_t) * h->array_size);
        h->population = 0;
        h->zero_used = 0;
        h->zero_cell.value = 0;
}

const struct cell_t*
hash_next(const struct hash_t *h, const struct cell_t *curr) {
        const struct cell_t *end = h->cells + h->array_size;
        
        /* Already finished? */
        if (!curr)
                return curr;
        
        /* Iterate past zero cell */
        if (cur == &m_h->zero_cell) {
                cur = &h->cells[-1];
        }

        /* Iterate through the regular cells */
        while (++ curr != end)
        {
                if (curr->key)
                        return curr;
        }

        /* Finished */
        return NULL;
}

const struct cell_t*
hash_head(const struct hash_t *h) {
        if (h->zero_used == 0) {
                return hash_next(h, &(h->zero_cell));
        }
        else {
                return &(h->zero_cell);
        }
}
