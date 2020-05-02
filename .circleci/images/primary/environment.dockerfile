FROM ubuntu:bionic

## INSTALL PREREQUISITES
RUN apt-get update \
 && apt-get install -y \
   ssh \
   wget \
   zlib1g \
   xz-utils \
   libssl-dev \
   build-essential \
   ca-certificates \
   software-properties-common \
 && rm -rf /var/lib/apt/lists/*

## INSTALL CMAKE
WORKDIR /home/src/cmake
RUN wget -q - https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2.tar.gz \
 && tar xf cmake-3.17.2.tar.gz \
 && cd cmake-3.17.2 \
 && ./bootstrap -- -DCMAKE_BUILD_TYPE:STRING=Release \
 && make \
 && make install \
 && cd ../ && rm -rf /home/src/cmake

## INSTALL CLANG 10
RUN wget -qO - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
 && add-apt-repository 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-10 main' \
 && apt-get update \
 && apt-get install -y clang-10 lld-10 libc++-10-dev libc++abi-10-dev zlib1g-dev \
 && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-10 100 \
 && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-10 100 \
 && rm -rf /var/lib/apt/lists/*

## BUILD & INSTALL LLVM 10
WORKDIR /home/src/llvm
RUN wget -q - https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/llvm-10.0.0.src.tar.xz \
 && tar xf llvm-10.0.0.src.tar.xz \
 && cd llvm-10.0.0.src \
 && mkdir build && cd build \
 && cmake ../ \
   -DCMAKE_C_COMPILER=clang \
   -DCMAKE_CXX_COMPILER=clang++ \
   -DCMAKE_INSTALL_PREFIX=/opt/llvm-10 \
   -DCMAKE_BUILD_TYPE:STRING=Release \
 && cmake --build . -j 8 \
 && cmake --build . -j 8 --target install \
 && cd ../ && rm -rf /home/src/llvm

## CONFIGURE
ENV CC clang
ENV CXX clang++
ENV CMAKE_PREFIX_PATH /opt/llvm-10
