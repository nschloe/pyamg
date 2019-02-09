#include <complex>
#include <iostream>
#include <stdio.h>
#include <omp.h>
//
// Threaded SpMV
//
// y <- A * x
//
// Parameters
// ----------
// n_row, n_col : int
//    dimensions of the n_row x n_col matrix A
// Ap, Aj, Ax : array
//    CSR pointer, index, and data vectors for matrix A
// Xx : array
//    input vector
// Yy : array
//    output vector (modified in-place)
//
// See Also
// --------
// csr_matvec
//
// Notes
// -----
// Requires GCC 4.9 for ivdep
// Requires a compiler with OMP
//
template <class I, class T>
void csr_matvec(const I n_row,
                const I n_col,
                const I Ap[], const int Ap_size,
                const I Aj[], const int Aj_size,
                const T Ax[], const int Ax_size,
                const T Xx[], const int Xx_size,
                      T Yx[], const int Yx_size)
{
    I i, jj;
    T sum;
    //int nthreads, tid;
    //nthreads = omp_get_num_threads();

    #pragma omp parallel for default(shared) schedule(static) private(i, sum, jj)
    for(i = 0; i < n_row; i++){
        //tid = omp_get_thread_num();
        //printf("row %d, thread %d\n", i, tid+1);
        //std::cout << "thread " << tid+1 << " of " << nthreads << "     (row " << i << ")" << std::endl;
        sum = Yx[i];
        #pragma GCC ivdep
        for(jj = Ap[i]; jj < Ap[i+1]; jj++){
            sum += Ax[jj] * Xx[Aj[jj]];
        }
        Yx[i] = sum;
    }
}


//
// OMP analytics
//
void omp_info()
{
    int nthreads, tid;

    #pragma ommp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();
        nthreads omp_get_num_threads();
        printff("Thread %d of %d total threads", tid, nthreads);
    }
}
