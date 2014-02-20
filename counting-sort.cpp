#include <iostream>

using namespace std;

template<size_t LBOUND, size_t RBOUND>
void counting_sort(int A[], int n) {
        int buff[RBOUND - LBOUND + 1] = {0};
        for (int i = 0; i < n; ++i) {
                ++buff[A[i] - LBOUND];
        }

        for (int i = 0; i < RBOUND - LBOUND + 1; ++i) {
                for (int j = 0; j < buff[i]; ++j) {
                        *A++ = i;
                }
        }
}

int main() {
        int A[] = {1,0,1,2,4};
        counting_sort<0, 4>(A, 5);
        for (int i = 0; i < 5; ++i) {
                cout << A[i] << ",";
        }
        cout << endl;
        return 0;
}
