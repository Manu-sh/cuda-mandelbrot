### Cuda Mandelbrot 🔀
###### how to use cuda acceleration to compute mandelbrot set

The code is written for my ___rtx 4070s___ so there is no guarantee it will work on others gpus
but it probably will work on every rtx 4000, ~~i will take a closer look at portability in the future~~

you can select your architecture passing the `-DCMAKE_CUDA_ARCHITECTURES=XX` flag where `XX`
is [your cuda architecture](https://developer.nvidia.com/cuda-gpus). 


| GPU Series         | Compute Capability |
|--------------------|-------------------|
| RTX 40xx (Ada)     | 89                |
| RTX 30xx (Ampere)  | 86                |
| RTX 20xx (Turing)  | 75                |
| GTX 16xx (Turing)  | 75                |
| GTX 10xx (Pascal)  | 61, 62            |
| GTX 900 (Maxwell)  | 50, 52            |

otherwise you can use `nvcc -ls-arch` to get a detailed list of 


```bash
mkdir -p build && cd build

cmake                         \ 
-DCMAKE_BUILD_TYPE=Release    \
-DCMAKE_CUDA_ARCHITECTURES=89 \
..

make -j`nproc --all`
./cuda
feh test.ppm
```

Learning resources

- https://en.wikipedia.org/wiki/Netpbm
- https://developer.nvidia.com/blog/cuda-pro-tip-write-flexible-kernels-grid-stride-loops/
- https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#thread-hierarchy
- https://codingbyexample.com/2018/12/17/cuda-threads-memory/
- https://www.youtube.com/playlist?list=PLC6u37oFvF40BAm7gwVP7uDdzmW83yHPe
- https://cuda-tutorial.readthedocs.io/en/latest/
- https://tschmidt23.github.io/cse599i/CSE%20599%20I%20Accelerated%20Computing%20-%20Programming%20GPUs%20Lecture%2017.pdf
- https://netpbm.sourceforge.net/doc/pbm.html
- https://en.wikipedia.org/wiki/Netpbm

```cpp
PBM<pnm::monochrome_t> x{1920, 1080};
bool color = pnm::monochrome_t::BLACK;

for (int h = 0; h < x.height(); ++h, color = !color)
    for (int w = 0; w < x.width(); ++w, color = !color)
        x(h, w, color);

x.write_file_content<pnm::Format::PBM1>("color.pbm1");
x.write_file_content<pnm::Format::PBM4>("color.pbm4");

PBM<pnm::monochrome_t> x{3, 2};

x(0,0, {255, 0,   0});
x(0,1, {0,   255, 0});
x(0,2, {0,   255, 0});

x(1,0, {255, 255, 0});
x(1,1, {255, 255, 255});
x(1,2, {0,   0,   0});

x.write_file_content<pnm::Format::PBM4>("color.pbm4");
x.write_file_content<pnm::Format::PBM1>("color.pbm1");


PPM<pnm::rgb<pnm::BIT_8>> x{3, 2};

x(0,0) = {255, 0,   0};
x(0,1) = {0,   255, 0};
x(0,2) = {0,   0,   255};

x(1,0) = {255, 255, 0};
x(1,1) = {255, 255, 255};
x(1,2) = {0,   0,   0};

x.write_file_content<pnm::Format::PPM3>("color.ppm3");
x.write_file_content<pnm::Format::PPM6>("color.ppm6");

PGM<pnm::grayscale<pnm::BIT_8>> pgm{3, 2};

pgm(0,0) = {255, 0,   0};
pgm(0,1) = {0,   255, 0};
pgm(0,2) = {0,   0,   255};

pgm(1,0) = {255, 255, 0};
pgm(1,1) = {255, 255, 255};
pgm(1,2) = {0,   0,   0};

pgm.write_file_content<pnm::Format::PGM2>("test.pgm2");
pgm.write_file_content<pnm::Format::PGM5>("test.pgm5");
return 0;
```

profiling, nvprof is a sort of compatibility layer to use old nvprof syntax, 
but most of nvprof flags are simply ignored by nsys

gpu profiling
```
nsys --help profile
nsys --help nvprof

# es.
nsys profile -o report.qdrep ./cuda 
nsys nvprof ./cuda 
nsys profile --stats=true ./cuda
nsys nvprof --print-gpu-trace ./cuda 
```

cpu profiling
```sh
perf stat -e task-clock,cycles,instructions,r1b1,r10e,stalled-cycles-frontend,stalled-cycles-backend,L1-dcache-load-misses,cache-misses ./cuda
perf stat -r 10 valgrind --tool=callgrind ./cuda
valgrind --tool=callgrind ./cuda
valgrind --tool=callgrind ./cuda | kcachegrind
```

dynamic analysis
```
valgrind --undef-value-errors=no --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./cuda
```

asm
```sh
objdump -S -M intel cuda | gedit - &
```

###### Copyright © 2025, [Manu-sh](https://github.com/Manu-sh), s3gmentationfault@gmail.com. Released under the [MIT license](LICENSE).
