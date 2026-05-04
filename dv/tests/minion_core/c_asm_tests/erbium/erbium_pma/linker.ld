/* Script for -z combreloc */
/* Copyright (C) 2014-2025 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */

/* -------------------------------------------------------------------------- */
/* Memory regions                                                             */
/* -------------------------------------------------------------------------- */
MEMORY
{
    bootrom  (rx)  : ORIGIN = 0x40000000, LENGTH = 0x400        /* 1 KB */
    mram     (rwx) : ORIGIN = 0x40000400, LENGTH = 0x00BFFC00   /* free region */
    mram_msu (rwx) : ORIGIN = 0x40C00000, LENGTH = 0x00400000   /* M/S/U accessible */
    stack    (rw)  : ORIGIN = 0x40FC0000, LENGTH = 0x40000      /* 256 KB */
}

ENTRY(_boot)

/* -------------------------------------------------------------------------- */
/* Sections                                                                   */
/* -------------------------------------------------------------------------- */
SECTIONS
{
    /* Boot ROM at the top of the MRAM */
    .bootrom :
    {
        *(.bootrom)
    } > bootrom

    /* All executable code goes into MRAM */
    .text :
    {
        *(.text)
        *(.text.*)
    } > mram_msu

    /* Read‑only data */
    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    } > mram_msu

    /* Initialized data */
    .data :
    {
        *(.data)
        *(.data.*)
    } > mram_msu

    /* Small data (requires GP) */
    .sdata :
    {
        __global_pointer$ = . + 0x800;   /* required for RISC‑V */
        *(.sdata)
        *(.sdata.*)
    } > mram_msu

    /* Uninitialized data */
    .bss (NOLOAD) :
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    } > mram_msu

    /* Stack region at the end of the MRAM */
    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        *(.stack)
    } > stack
}
