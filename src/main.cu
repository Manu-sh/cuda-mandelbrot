#include <cassert>
#include <cstdint>
#include <ppm3.hpp>
#include <Matrix1D.hpp>
#include <common.hpp>

#include <cuda_runtime.h>
#include <thrust/complex.h>

inline constexpr uint16_t cols = 1920, rows = 1080;
//inline constexpr uint16_t cols = 4096, rows = 3112;
static_assert(
        ((rows-1) * cols) <= 0xff'ff'ff,
        "ATfast(cols, r_idx, c_idx) -> __umul24(r_idx, cols) doesnt support more than 24 bit"
);


// https://stackoverflow.com/questions/16119923/using-constants-with-cuda
__device__ inline constexpr uint16_t ixsize = rows, gpu_rows = rows, iysize = cols, gpu_cols = cols, max_i = 1000;
__device__ inline constexpr float cxmin = -2.5f, cxmax = 2.5f, cymin = -2.5f, cymax = 2.5f;

#if 0
#include <cuda_fp16.h>

__device__ inline thrust::complex<float> fast_mul(thrust::complex<float> a, thrust::complex<float> b) {

    // c.real() = a.real() * b.real() - a.imag() * b.imag()
    // c.imag() = a.real() * b.imag() + a.imag() * b.real()

    using fp16 = __half;
    fp16 a_real = (fp16)a.real(), a_imag = (fp16)a.imag();
    fp16 b_real = (fp16)b.real(), b_imag = (fp16)b.imag();

    //fp16 c_real = a_real * b_real - a_imag * b_imag;
    //fp16 c_imag = a_real * b_imag + a_imag * b_real;

    fp16 c_real = __hmul(a_real, b_real) - __hmul(a_imag, b_imag);
    fp16 c_imag = __hmul(a_real, b_imag) + __hmul(a_imag, b_real);

    return {c_real, c_imag};
}

__device__ inline thrust::complex<float> ffast_mul(thrust::complex<float> a, thrust::complex<float> b) {

    // c.real() = a.real() * b.real() - a.imag() * b.imag()
    // c.imag() = a.real() * b.imag() + a.imag() * b.real()

    using fp16 = __half;
    using fp16_vector = __half2;

    fp16_vector a_vec = __halves2half2((fp16)a.real(), (fp16)a.imag()); // a[] = {a.real(), a.imag() }
    fp16_vector b_vec = __halves2half2((fp16)b.real(), (fp16)b.imag()); // b[] = {b.real(), b.imag() }

    // c[] = {a[0] * b[0], a[1] * b[1]}; -> a.real() * b_real(), a.imag() * b.imag()
    // c[] = {a.real() * b_real(), a.imag() * b.imag()};
    fp16_vector c_real_tmp = __hmul2(a_vec, b_vec); // computa le prime 2 moltiplicazioni necessarie a calcolare la parte real di c senza fare sottrazione

    // d[] = { a[0] * b[1], a[1] * b[0] }
     // fp16_vector c_imag_tmp = __hmul2(a_vec, __halves2half2(__high2half(b_vec), __low2half(b_vec)));
    fp16_vector c_imag_tmp = __halves2half2(
             __low2half(a_vec) * __high2half(b_vec), // d[0] = a[0] * b[1] -> a.real() * b.imag()
            __high2half(a_vec) *  __low2half(b_vec)  // d[1] = a[1] * b[0] -> a.imag() * b.real()
    );

    fp16_vector c_vec = __halves2half2(
            __low2half(c_real_tmp) - __high2half(c_real_tmp), // real part
            __low2half(c_imag_tmp) + __high2half(c_imag_tmp)  // imag part
    );

    return {__low2float(c_vec), __high2float(c_vec)};
}
#endif

__device__ rgb_t calc_mandelbrot(uint16_t ix, uint16_t iy) {

    using thrust::complex, thrust::abs, thrust::cos;

    complex<float> c{
            cxmin + ix / (ixsize-1.0f) * (cxmax - cxmin),
            cymin + iy / (iysize-1.0f) * (cymax - cymin)
    }, z = 0;

    c *= c * c; // c = pow(c, 3);

    uint16_t i;
    for (i = 0; i < max_i && abs(z) < 4.0f; ++i)
        z = cos(z * c); // z = cos(z * pow(c, 3));

    // GPU handle float truncation differently from CPU, making round-toward-zero instead of round-to-nearest.
    // that took me a lot debugging to figure why the image generated was so different than another generated
    // by the same code but running on cpu instead of gpu

    //const uint8_t col = (uint8_t)(int)(z.real() * 1);                 // -> works but is signed integer overflow which is UB
    //const uint8_t col = min(255, max(0, (unsigned)(z.real() * 255))); // -> different result
    //const uint8_t col = (uint8_t)lround(z.real() * 1);                // ok
    const uint8_t col = (uint8_t)__float2int_rn(z.real() * 1);          // same of calling std::lround(float) but cuda specific
    return (i == max_i) ? rgb_t{0,0,0} : rgb_t{col,col,col};
}


__global__ void kernel(rgb_t *const v, uint32_t len) {

          uint16_t tr = blockIdx.y * blockDim.y + threadIdx.y;
    const uint16_t tc = blockIdx.x * blockDim.x + threadIdx.x;

    for (; tr < gpu_rows; tr += blockDim.y * gridDim.y) {
        for (uint16_t c = tc; c < gpu_cols; c += blockDim.x * gridDim.x) {
            const uint32_t index = ATfast(gpu_cols, tr, c);
            //assert(index < len);
            v[index] = calc_mandelbrot(tr, c);
            //v[ __umul24(tr, gpu_cols) + c ] = calc_mandelbrot(tr, c);
        }
    }

}

int main() {

    cudaSetDevice(0);

    rgb_t *gpu_vct;
    cudaMalloc(&gpu_vct, sizeof(rgb_t) * cols * rows);

    int maxThreadsPerBlock;
    cudaDeviceGetAttribute(&maxThreadsPerBlock, cudaDevAttrMaxThreadsPerBlock, 0);
    cudaMemset(gpu_vct, 0, sizeof(rgb_t) * cols * rows);

    dim3 threadsPerBlock((int)sqrt(maxThreadsPerBlock), (int)sqrt(maxThreadsPerBlock));
    dim3 blocksDim(
            (cols + threadsPerBlock.x - 1) / threadsPerBlock.x, // forza l'allineamento a un multiplo di threadsPerBlock
            (rows + threadsPerBlock.y - 1) / threadsPerBlock.y
    );

    kernel<<<blocksDim, threadsPerBlock>>>(gpu_vct, cols * rows);
    cudaDeviceSynchronize(); // wait for gpu

    PPM3 img{cols, rows};
    cudaMemcpy(img.unwrap(), gpu_vct, sizeof(PPM3::pixel_type) * img.width() * img.height(), cudaMemcpyDeviceToHost);
    img.write_file_content("test.ppm");

    cudaFree(gpu_vct);
    cudaDeviceReset();
    return 0;
}
