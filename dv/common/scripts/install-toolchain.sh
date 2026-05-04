#!/bin/bash
set -e

INSTALL_DIR="/tools/riscv-elf-toolchain"
SOURCE_DIR="/tools/src/riscv-gnu-toolchain"
BFD_BUILD_DIR="$SOURCE_DIR/build-binutils-pic"

if [ ! -d "$INSTALL_DIR" ]; then
    mkdir -p "$INSTALL_DIR"
fi

if [ -x "$INSTALL_DIR/bin/riscv64-unknown-elf-gcc" ]; then
    echo "RISC-V toolchain already installed at ${INSTALL_DIR}"
else
    echo "Installing RISC-V GNU toolchain into ${INSTALL_DIR}"

    if [ ! -d "$SOURCE_DIR" ]; then
        git clone https://github.com/aifoundry-org/riscv-gnu-toolchain "$SOURCE_DIR"
    fi

    cd "$SOURCE_DIR"
    ./configure --prefix="$INSTALL_DIR" --with-arch=rv64imfc --with-abi=lp64f \
                --with-languages=c,c++ --with-cmodel=medany
    make -j "$(nproc)"
fi

if [ -f "$BFD_BUILD_DIR/bfd/.libs/libbfd.a" ]; then
    echo "BFD already built at ${BFD_BUILD_DIR}"
    exit 0
fi

echo "Building BFD into ${BFD_BUILD_DIR}"

mkdir -p "$BFD_BUILD_DIR"
cd "$BFD_BUILD_DIR"

CFLAGS="-fPIC -g -O2 -ftls-model=global-dynamic" \
CXXFLAGS="-fPIC -g -O2 -ftls-model=global-dynamic" \
    "$SOURCE_DIR/binutils/configure" \
        --target=riscv64-unknown-elf \
        --prefix="$INSTALL_DIR" \
        --enable-plugins --disable-werror --with-expat=yes \
        --disable-gdb --disable-sim --disable-libdecnumber --disable-readline \
        --with-abi=lp64f --with-arch=rv64imfc --with-isa-spec=20191213

make -j "$(nproc)" all-bfd all-opcodes
