caselib
=======

This is a tiny header which adds a Haskell/ML-likepattern matching case statement to C++.

Usage
=====

For example, implementing the [Ackermann function](http://en.wikipedia.org/wiki/Ackermann_function)

    A(m,n) = case (0,n) = n + 1
                | (m,0) = A(m-1,1)
                | (m,n) = A(m - 1, A(m, n-1)),

is as simple as

    #include <case.h>
    int A(int m, int n) {
        return CASE((m, n), (0, n) = n + 1,
                            (m, 0) = A(m - 1, 1),
                            (m, n) = A(m - 1, A(m, n-1)),
                            -1); // Default return value, if no cases match
    }

which expands into

    int A(int m, int n) {
        return (m == 0 && n == n) ? n + 1 :
               (m == m && n == 0) ? A(m - 1, 1) :
               (m == m && n == n) ? A(m - 1, A(m, n-1)) :
               -1;
