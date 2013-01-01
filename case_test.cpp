#include <iostream>
#include "case.h"

int pow(int a, int i) {
    return CASE((i), (0) = 1,
                     (i) = a * pow(a,i-1),
                     -1);
}

int A(int m, int n) {
    return CASE((m, n), (0, n) = n + 1,
                        (m, 0) = A(m - 1, 1),
                        (m, n) = A(m - 1, A(m, n-1)),
                        -1);
}

int main() {
    std::cout << pow(2,5) << std::endl;
    std::cout << A(3,5) << std::endl;
    return 0;
}
