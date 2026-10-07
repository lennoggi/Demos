#include <vector>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <hip/hip_runtime.h>
#include <rocblas/rocblas.h>

#define M 49011
#define N 220
#define P 150000

#define PRINT_MATRICES false

using namespace std;


/* ===============================================
 * Macros checking sanity of HIP and rocBLAS calls
 * =============================================== */
#define HIP_CHECK(routine)                                      \
do {                                                            \
    const hipError_t err = routine;                             \
    if (err != hipSuccess) {                                    \
        ostringstream err_ss;                                   \
        err_ss << "[" << __FILE__ << ", " << __LINE__           \
               << "] Routine '" << #routine << "' failed: \""   \
               << hipGetErrorString(err) << "\"." << std::endl; \
        throw runtime_error(err_ss.str());                      \
    }                                                           \
} while (0)


#define ROCBLAS_CHECK(routine)                                         \
do {                                                                   \
    const rocblas_status err = routine;                                \
    if (err != rocblas_status_success) {                               \
        ostringstream err_ss;                                          \
        err_ss << "[" << __FILE__ << ", " << __LINE__                  \
               << "] Routine '" << #routine << "' failed: \""          \
               << rocblas_status_to_string(err) << "\"." << std::endl; \
        throw runtime_error(err_ss.str());                             \
    }                                                                  \
} while (0)



/* =====
 * Begin
 * ===== */
int main() {
    /* -------------
     * Sanity checks
     * ------------- */
    static_assert(M > 0);
    static_assert(N > 0);
    static_assert(P > 0);
    static_assert(PRINT_MATRICES or not PRINT_MATRICES);

    /* -------------------------------
     * Set up the matrices on the host
     * ------------------------------- */
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


    /* ---------------------------------
     * Set up the matrices on the device
     * --------------------------------- */
    const auto device_setup_begin = chrono::high_resolution_clock::now();

    double *da = nullptr;
    double *db = nullptr;
    double *dc = nullptr;

    HIP_CHECK(hipMalloc(reinterpret_cast<void**>(&da), size_a*sizeof(double)));
    HIP_CHECK(hipMalloc(reinterpret_cast<void**>(&db), size_b*sizeof(double)));
    HIP_CHECK(hipMalloc(reinterpret_cast<void**>(&dc), size_c*sizeof(double)));

    HIP_CHECK(hipMemcpy(reinterpret_cast<void*>(da), reinterpret_cast<void*>(a.data()), size_a*sizeof(double), hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(reinterpret_cast<void*>(db), reinterpret_cast<void*>(b.data()), size_b*sizeof(double), hipMemcpyHostToDevice));

    rocblas_handle handle;
    ROCBLAS_CHECK(rocblas_create_handle(&handle));

    const auto device_setup_end = chrono::high_resolution_clock::now();
    const chrono::duration<double, std::milli> device_setup_duration = device_setup_end - device_setup_begin;
    cout << "Device setup took " << device_setup_duration.count() << " ms" << endl;


    /* ---------------------------------------------------------------
     * Compute  C = alpha*A*B + beta*C  with alpha=1 and beta=0, i.e.,
     *   C = alpha*A*B
     * --------------------------------------------------------------- */
    constexpr double alpha = 1.0;
    constexpr double beta  = 0.0;

     /* NOTE: rocBLAS uses a column-major layout, so the leading dimension would
      *   naively be the number of rows, but A[CBLAS] = A^T[rocBLAS] (see
      *   below), so we still effectively get the number of columns.            */
    constexpr size_t lda = p;
    constexpr size_t ldb = n;
    constexpr size_t ldc = n;

    const auto dgemm_begin = chrono::high_resolution_clock::now();

    /* NOTE: because rocblas_dgemm uses a column-major layout, we can map
     *         A[CBLAS] = A^T[rocBLAS]
     *       and calculate
     *         B*A[rocBLAS] = A*B[CBLAS]
     *
     *   EXAMPLE:
     *   - Flattened arrays
     *       A: [1  2  3  4  5  6]
     *       B: [7  8  9  10 11 12]
     *
     *   - CBLAS (row-major)
     *             |1 2 3|   |7  8 |   |58  64 |
     *       A*B = |4 5 6| * |9  10| = |139 154|
     *                       |11 12|       ||
     *                                 [**SAME**]
     *   - rocBLAS (column-major):         ||
     *             |7  9  11|   |1 4|   |58  64 |
     *       B*A = |8  10 12| * |2 5| = |139 154|
     *                          |3 6|                                           */
    ROCBLAS_CHECK(rocblas_dgemm(handle, rocblas_operation_none, rocblas_operation_none,  // No transposition
                  n, m, p, &alpha, db, ldb, da, lda, &beta, dc, ldc));

    const auto dgemm_end = chrono::high_resolution_clock::now();
    const chrono::duration<double, std::milli> dgemm_duration = dgemm_end - dgemm_begin;

    HIP_CHECK(hipDeviceSynchronize());

    /* --------------------------------
     * Copy the result back to the host
     * -------------------------------- */
    HIP_CHECK(hipFree(da));
    HIP_CHECK(hipFree(db));
    HIP_CHECK(hipMemcpy(reinterpret_cast<void*>(c.data()), reinterpret_cast<void*>(dc), size_c*sizeof(double), hipMemcpyDeviceToHost));
    HIP_CHECK(hipFree(dc));

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
