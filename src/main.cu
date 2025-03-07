#include <cassert>
#include <cstdint>

#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <thrust/complex.h>

#include <pnm/ppm/PPM.hpp>
#include <pnm/pgm/PGM.hpp>
#include <pnm/pbm/PBM.hpp>

#include <pnm/pnm.hpp>
#include <pnm/common.hpp>

// https://stackoverflow.com/questions/2151084/map-a-2d-array-onto-a-1d-array
constexpr FORCED(inline) uint32_t AT(uint16_t cols, uint16_t r, uint16_t c) { // cols = width = x_size
    return r * cols + c;
}


inline constexpr uint16_t cols = 1920, rows = 1080;
static_assert(((rows-1) * cols) <= 0xff'ff'ff, "pls stay under 24 bit");

// https://stackoverflow.com/questions/16119923/using-constants-with-cuda
__device__ inline constexpr uint16_t ixsize = rows, gpu_rows = rows, iysize = cols, gpu_cols = cols, max_i = 1000;
__device__ inline constexpr float cxmin = -2.5f, cxmax = 2.5f, cymin = -2.5f, cymax = 2.5f;


template <typename Pixel>
__device__ static Pixel random_primary(int seed) {

    static const Pixel rgb_primary[] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 255},
        {0, 0, 0},
    };

    curandState state;
    curand_init(seed, 0, 0, &state);
    return rgb_primary[curand(&state) % std::size(rgb_primary)];
}


template <typename Pixel>
__device__ Pixel calc_mandelbrot(uint16_t ix, uint16_t iy) {

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
    //return (i == max_i) ? Pixel{0,0,0} : Pixel{col,col,col};
    return (i == max_i) ? Pixel{0,0,0} : random_primary<Pixel>(col);
}


template<typename Pixel>
__global__ void kernel(Pixel *const v, uint32_t len) {

    uint16_t tr = blockIdx.y * blockDim.y + threadIdx.y;
    const uint16_t tc = blockIdx.x * blockDim.x + threadIdx.x;

    #pragma unroll
    for (; tr < gpu_rows; tr += blockDim.y * gridDim.y) {
        #pragma unroll
        for (uint16_t c = tc; c < gpu_cols; c += blockDim.x * gridDim.x)
            v[AT(gpu_cols, tr, c)] = calc_mandelbrot<Pixel>(tr, c);
    }

}

using namespace std;
int main() {

    //using pixel_t = pnm::grayscale<pnm::BIT_8>;
    using pixel_t = pnm::rgb<pnm::BIT_8>;
    //using pixel_t = pnm::monochrome_t;
    cudaSetDevice(0);

    pixel_t *gpu_vct;
    cudaMalloc(&gpu_vct, sizeof(pixel_t) * cols * rows);

    int maxThreadsPerBlock;
    cudaDeviceGetAttribute(&maxThreadsPerBlock, cudaDevAttrMaxThreadsPerBlock, 0);

    dim3 threadsPerBlock((int)sqrt(maxThreadsPerBlock), (int)sqrt(maxThreadsPerBlock));
    dim3 blocksDim(
            (cols + threadsPerBlock.x - 1) / threadsPerBlock.x, // force the ""alignment"" to a multiple of threadsPerBlock
            (rows + threadsPerBlock.y - 1) / threadsPerBlock.y
    );

    kernel<<<blocksDim, threadsPerBlock>>>(gpu_vct, cols * rows);
    cudaDeviceSynchronize(); // wait for gpu

    PNM<pixel_t> img{cols, rows};
    cudaMemcpy(img.unwrap(), gpu_vct, sizeof(pixel_t) * img.width() * img.height(), cudaMemcpyDeviceToHost);
    img.write_ascii("test-ascii.pnm");
    img.write_binary("test-bin.pnm");

    PNM<pnm::monochrome_t> x{1920, 1080};
    bool color = pnm::monochrome_t::BLACK;

    for (int h = 0; h < x.height(); ++h, color = !color)
        for (int w = 0; w < x.width(); ++w, color = !color)
            x(h, w, color);

    x.write_ascii("color.pbm1");
    x.write_binary("color.pbm4");

    cudaFree(gpu_vct);
    cudaDeviceReset();
    return 0;
}