#!/bin/sh

#
# @file      load_modules.sh
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
# @brief     PCG - Assignment 2/ Modules
#
# @version   2025
#
# @date      04 October   2023, 09:00 (created) \n
# @date      14 November  2025, 13:25 (revised) \n
#

if [ "$CLUSTERNAME" = "KAROLINA" ]; then
  ml purge 2>/dev/null
  ml CMake/4.0.3-GCCcore-14.3.0 Python/3.13.5-GCCcore-14.3.0 HDF5/1.14.6-iimpi-2025b NVHPC/25.3-CUDA-12.8.0
  export CC=nvc
  export CXX=nvc++
  export ACC_DEVICE_TYPE=nvidia
fi

if [ "$CLUSTERNAME" = "BARBORA" ]; then
  ml purge 2>/dev/null
  ml CMake/4.0.3-GCCcore-14.3.0 Python/3.13.5-GCCcore-14.3.0 HDF5/1.14.6-iimpi-2025b NVHPC/24.3-CUDA-12.3.0
  export CC=nvc
  export CXX=nvc++
  export ACC_DEVICE_TYPE=nvidia
fi
