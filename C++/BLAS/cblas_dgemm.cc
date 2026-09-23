#include <vector>
#include <iostream>
#include <chrono>
#include <cblas.h>

#define M 800
#define N 400
#define P 600

#define PRINT_MATRICES false

using namespace std;


int main() {
    /* ---------------
     * Print some info
     * --------------- */
    const auto openblas_parallel_mode = openblas_get_parallel();

    if (openblas_parallel_mode == 0) {
        cout << "OpenBLAS parallel mode: single-threaded" << endl;
    } else if (openblas_parallel_mode == 1) {
        cout << "OpenBLAS parallel mode: pthreads or native OS threads" << endl;
    } else if (openblas_parallel_mode == 2) {
        cout << "OpenBLAS parallel mode: OpenMP threads" << endl;
    } else {
        cout << "ERROR: unexpected OpenBLAS parallel mode ("
             << openblas_parallel_mode << ")" << endl;
    }

    cout << "OpenBLAS configuration: " << openblas_get_config()      << endl;
    cout << "Core architecture:      " << openblas_get_corename()    << endl;
    cout << "Number of threads:      " << openblas_get_num_threads() << endl;


    /* -------------------
     * Set up the matrices
     * ------------------- */
    constexpr size_t m = M;
    constexpr size_t n = N;
    constexpr size_t p = P;

    cout << endl
         << "Matrix A    dimensions: " << m << " * " << p << endl
         << "Matrix B    dimensions: " << p << " * " << n << endl
         << "Matrix C=AB dimensions: " << m << " * " << n << endl
         << endl;

    constexpr size_t size_a = m*p;
    constexpr size_t size_b = p*n;
    constexpr size_t size_c = m*n;

    vector<double> a(size_a), b(size_b), c(size_c);


    #if PRINT_MATRICES
    cout << "=====" << endl
         << "  A"   << endl
         << "=====" << endl;
    #endif

    for (auto i = decltype(m){0}; i < m; ++i) {
        const auto idx_i = i*p;
        for (auto k = decltype(p){0}; k < p; ++k) {
            const auto a_ik = 2.0*i + 3.0*k;
            a[idx_i + k] = a_ik;
            #if PRINT_MATRICES
            cout << a_ik << "\t";
            #endif
        }
        #if PRINT_MATRICES
        cout << endl;
        #endif
    }


    #if PRINT_MATRICES
    cout << endl
         << "=====" << endl
         << "  B"   << endl
         << "=====" << endl;
    #endif

    for (auto k = decltype(p){0}; k < p; ++k) {
        const auto idx_k = k*n;
        for (auto j = decltype(n){0}; j < n; ++j) {
            const auto b_kj = -3.0*k + 2.0*j;
            b[idx_k + j] = b_kj;
            #if PRINT_MATRICES
            cout << b_kj << "\t";
            #endif
        }
        #if PRINT_MATRICES
        cout << endl;
        #endif
    }


    /* ---------------------------------------------------------------
     * Compute  C = alpha*A*B + beta*C  with alpha=1 and beta=0, i.e.,
     *   C = alpha*A*B
     * --------------------------------------------------------------- */
    constexpr double alpha = 1.0;
    constexpr double beta  = 0.0;

    /* "Leading dimensions" = Number of columns
     *  CBLAS defaults to row-major layout      */
    constexpr size_t lda = p;
    constexpr size_t ldb = n;
    constexpr size_t ldc = n;

    const auto dgemm_begin = chrono::high_resolution_clock::now();

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, p, alpha, a.data(), lda, b.data(), ldb,
                beta, c.data(), ldc);

    const auto dgemm_end = chrono::high_resolution_clock::now();
    const chrono::duration<double, std::milli> dgemm_duration = dgemm_end - dgemm_begin;

    #if PRINT_MATRICES
    cout << endl
         << "=====" << endl
         << "  C"   << endl
         << "=====" << endl;

    for (auto i = decltype(m){0}; i < m; ++i) {
        const auto idx_i = i*n;
        for (auto j = decltype(n){0}; j < n; ++j) {
            cout << c[idx_i + j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
    #endif

    cout << "CBLAS dgemm product took " << dgemm_duration.count() << " ms" << endl;

    return 0;
}
