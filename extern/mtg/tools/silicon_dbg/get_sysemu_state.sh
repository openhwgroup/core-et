#!/usr/bin/bash

ELF_NAME=base
FAIL_PC="80009114e4"
FAIL_HARTID=808
X13="80a135f6c0"
SYS_LOG_FILE="sysemu.log"

if [ ! -f "$ELF_NAME" ]; then
   echo "'$ELF_NAME' ELF does not exist."
   exit -1
fi

riscv64-unknown-elf-objdump -D $ELF_NAME > ${ELF_NAME}.dasm
PC=$(grep "^0.*b0.*:\|$FAIL_PC:" ${ELF_NAME}.dasm | grep -C1 "$FAIL_PC:" | grep "^0" | cut -d " " -f1)

PC_HEAD=$(echo $PC | cut -d " " -f1)
PC_TAIL=$(echo $PC | cut -d " " -f2)

PC_HEAD="${PC_HEAD#"${PC_HEAD%%[!0]*}"}"
PC_TAIL="${PC_TAIL#"${PC_TAIL%%[!0]*}"}"

if [ ! -f "$SYS_LOG_FILE" ]; then
   echo "'$SYS_LOG_FILE' log file does not exist."
   exit -1
fi

grep "H$FAIL_HARTID" $SYS_LOG_FILE > h${FAIL_HARTID}.out

LINE=$(grep -n "$PC_HEAD\|$PC_TAIL" h${FAIL_HARTID}.out | cut -d ":" -f1)

LINE_HEAD=$(echo $LINE | cut -d " " -f1)
LINE_TAIL=$(echo $LINE | cut -d " " -f2)

awk "NR >= $LINE_HEAD && NR <= $LINE_TAIL" h${FAIL_HARTID}.out > h${FAIL_HARTID}.out.cut

LBU_PC=$(grep -m1 -B2 "MEM8\[0x$X13\] :" h${FAIL_HARTID}.out.cut | grep "lbu" | grep -oh "0x.......... ")

OUTPUT_FILE="sys.out"
grep -A2 "$LBU_PC" h${FAIL_HARTID}.out.cut | grep -oh "MEM.*" > $OUTPUT_FILE &&
echo "SYS_EMU architectural state succesfully written in '$OUTPUT_FILE' file"
