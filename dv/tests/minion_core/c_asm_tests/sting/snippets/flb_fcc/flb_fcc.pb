pb_start do_fcc(regc, counter_fcc)
    li regc, counter_fcc
    csrrw x0, fcc, regc
pb_end

pb_start do_fcc_nb(rega, regb, counter_fccnb, rand_num_inst)
if (counter_fccnb == 0):
    li regb, 0x0000ffff
else: ; counter_fccnb == 1
    li regb, 0xffff0000
endif
fcc_nb_start:
if ET_TEST_ENV.SILICON_MODE == 0:
    ; Generate a random sequence of instructions 
    ; Sequence lenght is >= 1 and <= 5
    RANDINST 0:rand_num_inst
endif
    csrrs rega, fccnb, x0

    ; Mask out the values we are not interested in
    and rega, rega, regb
    beq x0, rega, fcc_nb_start
    do_fcc(regc = rega, counter_fcc = counter_fccnb)
pb_end

pb_start flb_fcc_self_check(rega, regb, counter_fccnb)
if (counter_fccnb == 0):
    li regb, 0x0000ffff
else: ; counter_fccnb == 1
    li regb, 0xffff0000
endif
    csrrs rega, fccnb, x0
    ; Mask out the values we are not interested in
    and rega, rega, regb
    beq x0, rega, success

    test_fail(regz = rega)
success:
pb_end
