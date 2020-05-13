FROM rushlang/clang-x86-64-linux-gnu

ARG llvm_install_dir=/opt/llvm
ARG cmake_install_dir=/opt/cmake

# Install CMAKE & LLVM.
COPY --from=rushlang/llvm-x86-64-linux-gnu ${llvm_install_dir} ${llvm_install_dir}
COPY --from=rushlang/cmake-x86-64-linux-gnu ${cmake_install_dir} ${cmake_install_dir}
RUN update-alternatives --install /usr/bin/cmake cmake ${cmake_install_dir}/bin/cmake 100 \
 && update-alternatives --install /usr/bin/ctest ctest ${cmake_install_dir}/bin/ctest 100

# Install additional dependencies.
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    ca-certificates \
    libssl-dev \
    zlib1g-dev \
    git \
 && rm -rf /var/lib/apt/lists/*

# Add user rush to run tests as.
RUN useradd -ms /bin/bash -d /home/rush -g root rush
USER rush
WORKDIR /home/rush
