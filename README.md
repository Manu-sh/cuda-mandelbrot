https://en.wikipedia.org/wiki/Netpbm

https://developer.nvidia.com/blog/cuda-pro-tip-write-flexible-kernels-grid-stride-loops/

https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#thread-hierarchy

https://codingbyexample.com/2018/12/17/cuda-threads-memory/

https://www.youtube.com/playlist?list=PLC6u37oFvF40BAm7gwVP7uDdzmW83yHPe

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