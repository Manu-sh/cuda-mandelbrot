cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(cuda CXX CUDA)

set(CMAKE_CUDA_STANDARD              17)
set(CMAKE_CUDA_STANDARD_REQUIRED     ON)
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON) # Abilita le compilazioni separabili per supportare più file CUDA

if(NOT CMAKE_CUDA_ARCHITECTURES)
    # -arch=sm_89 -gencode=arch=compute_89,code=sm_89 -gencode=arch=compute_89,code=compute_89 (https://arnon.dk/matching-sm-architectures-arch-and-gencode-for-various-nvidia-cards/)
    message("CMAKE_CUDA_ARCHITECTURES is not set, using 89")
    set(CMAKE_CUDA_ARCHITECTURES 89) # rtx 4000
endif()

# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
# analyze-build --cdb compile_commands.json --use-analyzer `which clang`
# or scanbuild make -j24
#set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    # set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE Debug) # keep assertion() on
endif()

# equivalent of: strip --strip-all
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
#add_link_options($<$<CONFIG:RELEASE>:-s>)

set(INC_PATH  ${PROJECT_SOURCE_DIR}/include)
set(SRC_PATH  ${PROJECT_SOURCE_DIR}/src)
#set(LIB_PATH  ${PROJECT_SOURCE_DIR}/pnm/lib)

add_executable(${PROJECT_NAME} ${SRC_PATH}/main.cu)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${INC_PATH}
)

# add & link opnm
include(FetchContent)
FetchContent_Declare(opnm GIT_REPOSITORY https://github.com/Manu-sh/opnm.git GIT_TAG d940d52dbdeac821b9b23be050cad768ef7913b6)
FetchContent_MakeAvailable(opnm)

target_link_libraries(${PROJECT_NAME} PRIVATE opnm)


target_compile_options(${PROJECT_NAME} PRIVATE
        -g
        #-D_DEBUG -G # abilita il dbg su cpu e gpu
        -Xptxas
        -O3
        --expt-relaxed-constexpr
        #-pedantic
        -Wall -Wextra -Wno-unused-function -pipe
        -fstack-protector-all -fstack-protector-strong
        -O3 -ffast-math -funroll-loops -march=native -mtune=native
        -funswitch-loops -ftree-vectorize -fivopts -fmodulo-sched

        # -msse -msse2 -msse3 -msse4.1 -msse4.2 -mmmx -m3dnow -mavx2 -mavx512f
        # -mfma -mbmi -mxsave -mxsaveopt -mrdrnd
)


