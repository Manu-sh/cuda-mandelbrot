cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(cuda CXX CUDA)

# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
# analyze-build --cdb compile_commands.json --use-analyzer `which clang`
# or scanbuild make -j24
#set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Imposta lo standard CUDA
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)


if(NOT CMAKE_BUILD_TYPE)
    # set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE Debug) # keep assertion() on
endif()

# rtx 4000
set(CMAKE_CUDA_ARCHITECTURES 89)


# Abilita le compilazioni separabili per supportare più file CUDA
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

set(INC_PATH  ${PROJECT_SOURCE_DIR}/include)
set(SRC_PATH  ${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SRC_PATH}/main.cu)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${INC_PATH}
        ${INC_PATH}/memalign
)

#message(STATUS "INC_PATH: ${INC_PATH}")
# CC flags
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --compiler-options '-Wall -Wextra -Wno-unused-function -pipe -pedantic -fstack-protector-all -fstack-protector-strong -O3 -ffast-math -funroll-loops -march=native -mtune=native -funswitch-loops -ftree-vectorize -fivopts -fmodulo-sched -msse -msse2 -msse3 -mmmx -m3dnow -mavx2'")

#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -S -masm=intel")  # Intel syntax
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --keep --ptx --source-in-ptx")

target_compile_options(${PROJECT_NAME} PRIVATE
        #-g
        #-D_DEBUG -G # abilita il dbg su cpu e gpu
        -Xptxas
        -O3
        --expt-relaxed-constexpr
        -pedantic
        -Wall -Wextra -Wno-unused-function -pipe
        -fstack-protector-all -fstack-protector-strong
        -O3 -ffast-math -funroll-loops -march=native -mtune=native
        -funswitch-loops -ftree-vectorize -fivopts -fmodulo-sched
        # -msse -msse2 -msse3 -msse4.1 -msse4.2 -mmmx -m3dnow -mavx2 -mavx512f
        # -mfma -mbmi -mxsave -mxsaveopt -mrdrnd
)


