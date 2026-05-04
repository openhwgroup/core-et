#!/usr/bin/bash

#USAGE: ./compile <snip_dir> <elf_name>

source $1/sourceme.sh

# Select linker arguments based on target (default: erbium)
MTG_TARGET=${MTG_TARGET:-erbium}

if [ "$MTG_TARGET" = "erbium" ]; then
    MIN_DIAGS_LD_ARGS="-T $MINION_DIAGS/common/erbium.ld"
    EXTRA_CFLAGS+=" -DTARGET_ERBIUM"
else
    MIN_DIAGS_LD_ARGS="-Wl,--section-start=bootrom=0x8000001000,--section-start=stack=0x80A1FFC000"
fi

riscv64-unknown-elf-gcc -Wall -Werror -Wextra -Wpedantic $EXTRA_CFLAGS -I$MINION_DIAGS/include -I$MINION_DIAGS/vpu/compare -I$MINION_DIAGS/vpu/computational -I$MINION_DIAGS/micro_kernels -I$MINION_DIAGS/vpu/trans -I$MINION_DIAGS/vm_boundary/common -I$MINION_DIAGS/ultrasoc -I$REPOROOT/rtl/shire/esr/scripts/ -fno-delete-null-pointer-checks -O2 -g -mcmodel=medany -march=rv64imf -mabi=lp64f -Wa,-march=rv64imf,-mabi=lp64f $MINION_DIAGS/common/crt.S $MIN_DIAGS_LD_ARGS $MTG_LD_ARGS -nostdlib -nostartfiles -o $2 $MINION_DIAGS/common/boot.S $MINION_DIAGS/common/prcm.c $2.cc

riscv64-unknown-elf-readelf -S base | grep -E -oh '\bmtgsk\w*' | while read -r line ; do
    pre_size=$(stat -c%s "$2" | numfmt --to=iec)
    riscv64-unknown-elf-objcopy --remove-section $line $2
    post_size=$(stat -c%s "$2" | numfmt --to=iec)
    echo "Removing section $line ... Old size: $pre_size , New size: $post_size"
done
