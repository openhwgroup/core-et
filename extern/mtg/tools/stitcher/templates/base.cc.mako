#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "${crc16_file}"

#include "${MTG_DIR}/tools/mtg_printf/mtg_printf.h"

% if test_info.self_check == 1:
#include "${test_info.snip_dir}/mtg_export_checkpoints.h"
%endif

<%!import random%>
<%random.seed(test_info.seed)%>

<%def name="include_test(path, block_idx, snip_idx)">\
   <%
      include = "#include \"" + str(path) + "\""
      file_name = path.split("/")[-1]
      file_name = file_name[:-2]
      if "gen_signature" in file_name:
         file_name = file_name.replace("gen_signature_", "")
         add_print = "mtg_printf(\""+file_name+"\\n\");"
         post_add_print = "mtg_printf(\""+file_name+" pass\\n\");"

      elif "end_fcc_barrier" in file_name:
         add_print = 'mtg_printf("HART done!\\n");\n'
         post_add_print = '   int16_t lhid;\n   __asm__ __volatile__ ("add %[lhid], x0, x27\\n" : [lhid] "=r" (lhid) : : );\n'
         if test_info.self_check == 1:
             post_add_print = post_add_print + '   if (lhid == 0) \      print_checkpoints();\n   }\n'
      else:
         add_print = ""
         post_add_print = ""
   %>\
   __asm__("${file_name}_b${block_idx}_s${snip_idx}:");
   ${add_print}
   ${include}
   ${post_add_print}\
</%def>\

% if test_info.repeat_count != -1:
extern "C" void print_loop_iteration(int64_t* count);
void print_loop_iteration(int64_t* count) {
   volatile uint64_t loop_it;
   __asm__ __volatile__ (
   % if test_info.l2_only == True:
      "amoorl.d %[loop_it], %[ptr1], x0\n"
   % else:
      "amoorg.d %[loop_it], %[ptr1], x0\n"
   % endif
      : [loop_it] "=r" (loop_it)
      : [ptr1]  "r" (count)
      :
   );
   _mtg_hid_printf("Finished loop iteration %d/${test_info.repeat_count-1}\n", loop_it);
}
%endif

#include "${mem_map_file}"
#include "${test_info.snip_dir}/mtg_lhid_map.h"
#include "${test_info.snip_dir}/mtg_target_data_rand.h"
int main() {

   __asm__("add x1, x0, x0");

   // Preamble
   %for i in preamble_list:
      ${include_test(path=i, block_idx=0, snip_idx=loop.index)}
   %endfor

   // Init marker
   C_TEST_START;

   // Test Start
   mtg_printf("TEST_START\n");
   %for i in start_list:
      ${include_test(path=i, block_idx=0, snip_idx=loop.index)}
   %endfor

   // Test Main
   mtg_printf("TEST_MAIN\n");
   %for i in main_list:
      ${include_test(path=i, block_idx=0, snip_idx=loop.index)}
   %endfor

   // Test End
   mtg_printf("TEST_END\n");
   %for i in end_list:
      ${include_test(path=i, block_idx=0, snip_idx=loop.index)}
   %endfor

   __asm__ __volatile__ ("call mtg_glbl_test_pass\n" "mtg_glbl_test_pass:\n");

   // End marker
   C_TEST_PASS;

   // Test mhandlers, shandlers and uhandlers
   %for i in handler_list:
      ${include_test(path=i, block_idx=0, snip_idx=loop.index)}
   %endfor

   return 0;
}
