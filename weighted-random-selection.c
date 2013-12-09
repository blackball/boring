/**
 * Weighted random selection (a.k.a fitness proportionate selection )
 *
 * Random sampling is very easy, given an array, uniformly select one element.
 * Weighted one is: given an array of elements, and the same number of weights, 
 * for example: [3,4,2], [0.3, 0.5, 0.2], the probability of selecting 3 is 0.3,
 * 4 is 0.5, 2 is 0.2.
 *
 * In fact we don't need to give a array of elements, if we just return the index, the 
 * size of elements is enough. 
 *
 * @blackball
 */

/* random() uniformly generate floating number in [0.0, 1.0] */
int
wrand(const double *weights, const int elem_size) {
        double choice = random() * sum(weights, elem_size);
        int i = 0;
        for (; i < elem_size; ++i) {
                choice -= weights[i];
                if (choice < 0) {
                        return i;
                }
        }
        return -1; // someting wrong
}

int
main(int argc, char *argv[]) {
        const double w[3] = {0.2, 0.3, 0.5};
        wrand(w, 3);
        return 0;
}
