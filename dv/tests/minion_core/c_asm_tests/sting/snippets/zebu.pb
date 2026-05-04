pb_start zebu_pass()
zebu_pass_hint:
    slti x0,x0,0x7fe
pb_end

pb_start zebu_test_fail(regx)
   ; Be careful with adding or deleting instructions of this param block
   ; Some snippets are assuming that this block contains 3 instructions
   ;
   ; For example:
   ;    Some mhandlers will mret to a PC that skips this param block therefore
   ;    is necessary to know the number of instructions contained in this block
   lui   regx, 0x50BAD
   csrw  validation0, regx
   zebu_fail()
pb_end

pb_start zebu_fail()
zebu_fail_hint:
    slti x0,x0,0x7ff
pb_end
