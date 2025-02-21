### Cuda Mandelbrot 🔀
###### how to use cuda acceleration to compute mandelbrot set

The code is written for my ___rtx 4070s___ so there is no guarantee it will work on others gpus
but it probably will work on every rtx 4000, i will take a closer look at portability in the future

https://en.wikipedia.org/wiki/Netpbm

https://developer.nvidia.com/blog/cuda-pro-tip-write-flexible-kernels-grid-stride-loops/

https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#thread-hierarchy

https://codingbyexample.com/2018/12/17/cuda-threads-memory/

https://www.youtube.com/playlist?list=PLC6u37oFvF40BAm7gwVP7uDdzmW83yHPe

https://cuda-tutorial.readthedocs.io/en/latest/

```cpp
PPM3 x{3, 2};

x(0,0) = {255, 0,   0};
x(0,1) = {0,   255, 0};
x(0,2) = {0,   0,   255};

x(1,0) = {255, 255, 0};
x(1,1) = {255, 255, 255};
x(1,2) = {0,   0,   0};

x.write_file_content("color.ppm");
```


profiling, nvprof is a sort of compatibility layer to use old nvprof syntax, 
but most of nvprof flags are simply ignored by nsys

```
nsys --help profile
nsys --help nvprof

# es.
nsys profile -o report.qdrep ./cuda 
nsys nvprof ./cuda 
nsys profile --stats=true ./cuda
nsys nvprof --print-gpu-trace ./cuda 
```

###### Copyright © 2025, [Manu-sh](https://github.com/Manu-sh), s3gmentationfault@gmail.com. Released under the [MIT license](LICENSE).
