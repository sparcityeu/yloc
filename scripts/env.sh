#!/bin/bash
HOST=$(hostname)

if [[ $HOST = "rome1" ]] || [[ $HOST = "rome2" ]]; then
    export HWLOC_ROOT=${HOME}/local.rome/
    export ROCM_DIR=/opt/rocm-5.1.3/
	module unload cmake/3.19.2
	module load gcc-11.2.0-gcc-11.2.0-l5uwhvj cmake-3.22.2-gcc-11.2.0-7zhlpev boost-1.78.0-gcc-11.2.0-b7eazfv
	export LD_LIBRARY_PATH=${ROCM_DIR}/rocm_smi/lib:$LD_LIBRARY_PATH
fi

if [[ $HOST = "thx1.srve" ]] || [[ $HOST = "thx2.srve" ]]; then
	export HWLOC_ROOT=${HOME}/local.thx/
	# export NVIDIA_DIR=/home/sw/aarch64/cuda/11.1.1/targets/sbsa-linux/
	module load cmake/3.18.3
	module load cuda/11.1.1 llvm/11.0.0_nvptx_offloading
	module load gcc_nvptx_offloading/10.2.0
	module load llvm/11.0.0_nvptx_offloading
	# module load boost # not available on thx ...
	export BOOST_ROOT=/home/ru37geh/local.thx/
fi

if [[ $HOST = "ice1" ]] || [[ $HOST = "ice2" ]]; then
	export HWLOC_ROOT=${HOME}/local.ice/
	# nvidia-smi --help-query-gpu
	# nvidia-smi topo -m
	#export NVIDIA_DIR=/usr/local/cuda-11.7/targets/x86_64-linux/
	export NVIDIA_DIR=/home/di75gix/spack_ice/spack/opt/spack/linux-sles15-icelake/gcc-11.2.0/cuda-11.5.1-357smjb43fq4ycrh23acrz755rz645dw/
	module load cmake/3.22.2/intel-2021.4.0-module-ly2hcqa
	module load cuda-11.5.1-gcc-11.2.0-357smjb
	# module load cuda/11.6.1/intel-2021.4.0-module-ntdpcyp
	module load boost
	module load gcc-11.2.0-gcc-7.5.0-xmg4ukh
	export LD_LIBRARY_PATH=${NVIDIA_DIR}/lib:$LD_LIBRARY_PATH
	export CMAKE_BIN=$(which cmake)/bin/cmake
fi
