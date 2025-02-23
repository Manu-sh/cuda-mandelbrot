#include <cassert>
#include <cstdint>
#include <ppm3.hpp>
#include <common.hpp>

#include <cuda_runtime.h>
#include <thrust/complex.h>
#include <memalign/al_allocator.hpp>

inline constexpr uint16_t cols = 1920, rows = 1080;
//inline constexpr uint16_t cols = 4096, rows = 3112;
static_assert(
        ((rows-1) * cols) <= 0xff'ff'ff,
        "ATfast(cols, r_idx, c_idx) -> __umul24(r_idx, cols) does not support more than 24 bit"
);


// https://stackoverflow.com/questions/16119923/using-constants-with-cuda
__device__ inline constexpr uint16_t ixsize = rows, gpu_rows = rows, iysize = cols, gpu_cols = cols, max_i = 1000;
__device__ inline constexpr float cxmin = -2.5f, cxmax = 2.5f, cymin = -2.5f, cymax = 2.5f;

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

    #pragma unroll
    for (; tr < gpu_rows; tr += blockDim.y * gridDim.y) {
        #pragma unroll
        for (uint16_t c = tc; c < gpu_cols; c += blockDim.x * gridDim.x)
            v[AT(gpu_cols, tr, c)] = calc_mandelbrot(tr, c);
    }

}

#if 0
int main() {

    cudaSetDevice(0);

    rgb_t *gpu_vct;
    cudaMalloc(&gpu_vct, sizeof(rgb_t) * cols * rows);

    int maxThreadsPerBlock;
    cudaDeviceGetAttribute(&maxThreadsPerBlock, cudaDevAttrMaxThreadsPerBlock, 0);

    dim3 threadsPerBlock((int)sqrt(maxThreadsPerBlock), (int)sqrt(maxThreadsPerBlock));
    dim3 blocksDim(
            (cols + threadsPerBlock.x - 1) / threadsPerBlock.x, // force the ""alignment"" to a multiple of threadsPerBlock
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
#else
#include <iostream>
#include <unistd.h>

using std::cout, std::endl;
int main() {

    struct __attribute__((__packed__)) big_t {
        big_t() {
            p = malloc(113);
            cout << "constructor called, malloc pointer " << p << '\n';
        }
        ~big_t() {
            free(p);
            cout << "destructor called, free pointer " << p << '\n';
        }
        void *p = nullptr;
        char buf[11];
    };

    /*
    cout << sizeof(big_t) << endl;
    Matrix1D<big_t, 4> mtx{3, 2};
    //Matrix1D<char, 3> mtx2{3, 3}; // should raise an error at compilation time
    Matrix1D<char, 1> mtx2{3, 3};
     */

    ///std::basic_string<char, std::char_traits<char>, ::al_allocator<char, 32> > s;
    //std::basic_string<char, std::char_traits<char>, std::allocator<char> > s;
    //std::basic_string<char, std::char_traits<char>, ::fuck_allocator<char> > s;

    std::basic_string<char, std::char_traits<char>, ::fuck_allocator<char> > s;
    for (int i = 0, x = 1920 * 1080 * 2; i < x; ++i) {
        s += std::string("ciao mondo");
    }

    return 0;
}
#endif