#!/bin/bash
if [[ $HOST = "rome1" ]] || [[ $HOST = "rome2" ]]; then
    export HWLOC_ROOT=${HOME}/local.rome/
    export ROCM_DIR=/opt/rocm-5.1.3/
	module unload cmake/3.19.2
	module load gcc-11.2.0-gcc-11.2.0-l5uwhvj cmake-3.22.2-gcc-11.2.0-7zhlpev boost-1.78.0-gcc-11.2.0-b7eazfv
	export LD_LIBRARY_PATH=${ROCM_DIR}/rocm_smi/lib:$LD_LIBRARY_PATH
fi
