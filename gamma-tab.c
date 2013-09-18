/**
 * generate a gamma correction table.
 *
 * @blackball
 */

/* This piece of codes could be useless if you could 
 * precomputed the table using other more convinient 
 * language, say Matlab or Python.
 */

static void
_gen_gamma_tab(float tab[256], float gamma) {
        int i = 0;

        const float t = 255.f / pow(255.f, gamma);
        
        for (; i < 256; ++i) {
                tab[i] = pow(i, gamma) * t;
        }
}

int
main(int argc, char *argv[]) {
        float gamma_tab[256];
        _gen_gamma_tab(gamma_tab, 0.2f);
        return 0;
}
