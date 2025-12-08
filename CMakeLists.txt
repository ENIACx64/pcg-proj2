#
# @file      CMakeLists.txt
#
# @author    Oliver Kunik \n
#            Faculty of Information Technology \n
#            Brno University of Technology \n
#            ikunik@fit.vutbr.cz
#
# @author    David Bayer \n
#            Faculty of Information Technology \n
#            Brno University of Technology \n
#            ibayer@fit.vutbr.cz
#
# @brief     PCG - Assignment 2 / CMake
#
# @version   2025
#
# @date      04 October   2023, 09:00 (created) \n
# @date      14 November  2025, 12:49 (revised) \n
#


cmake_minimum_required(VERSION 3.24)

project(Assignment2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD               17)
set(CMAKE_CXX_STANDARD_REQUIRED      ON)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "NVHPC")
  message(FATAL_ERROR "Unsupported compiler. Use NVHPC compiler.")
endif()

# Mem flag
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 24.5)
  set(GPU_MEM_FLAG "pinned")
else()
  set(GPU_MEM_FLAG "mem:separate:pinnedalloc")
endif()


# Set the CUDA architecture
if("$ENV{CLUSTERNAME}" STREQUAL "KAROLINA")
  set(CUDA_ARCHITECTURE 80)
elseif("$ENV{CLUSTERNAME}" STREQUAL "BARBORA")
  set(CUDA_ARCHITECTURE 70)
else()
  set(CUDA_ARCHITECTURE native)
endif()

# Find OpenACC
find_package(OpenACC REQUIRED)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Find HDF5
find_package(HDF5 REQUIRED COMPONENTS C)

# Gen
add_executable(gen Commons/gen.cpp)
target_include_directories(gen PRIVATE Commons)
target_link_libraries(gen PRIVATE OpenMP::OpenMP_CXX HDF5::HDF5)

# Cpu
add_executable(nbodyCpu Cpu/main.cpp Cpu/nbody.cpp Commons/h5Helper.cpp)
target_include_directories(nbodyCpu PRIVATE Commons)
target_link_libraries(nbodyCpu PRIVATE OpenMP::OpenMP_CXX HDF5::HDF5)
target_compile_options(nbodyCpu PRIVATE -O4 -fast -march=native)

# GPU steps
function(add_step target_name step_dir)
  add_executable(${target_name} ${step_dir}/main.cpp ${step_dir}/nbody.cpp Commons/h5Helper.cpp)
  target_include_directories(${target_name} PRIVATE Commons)
  target_compile_options(${target_name} PRIVATE
          -acc=gpu -gpu=cc${CUDA_ARCHITECTURE},${GPU_MEM_FLAG}
          -acc=multicore -mp=multicore
          -Minfo=accel
  )
  target_compile_options(${target_name} PRIVATE
          $<$<CONFIG:Debug>:-gpu=debug -g -O0>
          $<$<CONFIG:Release>:-gpu=fastmath -O4 -fast -march=native>
  )
  target_link_libraries(${target_name} PRIVATE OpenACC::OpenACC_CXX HDF5::HDF5)
endfunction()

add_step(nbody0 Step0)
add_step(nbody1 Step1)
add_step(nbody2 Step2)
add_step(nbody3 Step3)
