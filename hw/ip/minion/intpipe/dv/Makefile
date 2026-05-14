REPO_ROOT := $(shell git rev-parse --show-toplevel)

PKGS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv

TESTS := alu \
         int_scoreboard \
         fp_scoreboard \
         mask_scoreboard \
         debug_breakpoint \
         csr_replay_vpu \
         csr_replay_no_vpu \
         csr_msgs

alu_TOP := intpipe_alu_tb
alu_SRCS := $(PKGS) \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_alu.sv \
            $(CURDIR)/intpipe_alu_tb.sv \
            $(CURDIR)/intpipe_alu_test.cc

int_scoreboard_TOP := intpipe_int_scoreboard_tb
int_scoreboard_SRCS := $(PKGS) \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_int_scoreboard.sv \
                       $(CURDIR)/intpipe_int_scoreboard_tb.sv \
                       $(CURDIR)/intpipe_int_scoreboard_test.cc

fp_scoreboard_TOP := intpipe_fp_scoreboard_tb
fp_scoreboard_SRCS := $(PKGS) \
                      $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_fp_scoreboard.sv \
                      $(CURDIR)/intpipe_fp_scoreboard_tb.sv \
                      $(CURDIR)/intpipe_fp_scoreboard_test.cc

mask_scoreboard_TOP := intpipe_mask_scoreboard_tb
mask_scoreboard_SRCS := $(PKGS) \
                        $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mask_scoreboard.sv \
                        $(CURDIR)/intpipe_mask_scoreboard_tb.sv \
                        $(CURDIR)/intpipe_mask_scoreboard_test.cc

debug_breakpoint_TOP := debug_breakpoint_tb
debug_breakpoint_SRCS := $(PKGS) \
                         $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/debug_breakpoint.sv \
                         $(CURDIR)/debug_breakpoint_tb.sv \
                         $(CURDIR)/debug_breakpoint_test.cc

csr_replay_vpu_TOP := intpipe_csr_replay_tb
csr_replay_vpu_SRCS := $(PKGS) \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_replay.sv \
                       $(CURDIR)/intpipe_csr_replay_tb.sv \
                       $(CURDIR)/intpipe_csr_replay_test.cc

csr_replay_no_vpu_TOP := intpipe_csr_replay_tb
csr_replay_no_vpu_SRCS := $(csr_replay_vpu_SRCS)
csr_replay_no_vpu_FLAGS := -GVpuEn=0

csr_msgs_TOP := intpipe_csr_msgs_tb
csr_msgs_SRCS := $(PKGS) \
                 $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_msgs.sv \
                 $(CURDIR)/intpipe_csr_msgs_tb.sv \
                 $(CURDIR)/intpipe_csr_msgs_test.cc

LINT_SRCS := $(PKGS) \
             $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_alu.sv \
             $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_int_scoreboard.sv \
             $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_fp_scoreboard.sv \
             $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mask_scoreboard.sv \
             $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/debug_breakpoint.sv \
             $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_replay.sv \
             $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_msgs.sv \
             $(CURDIR)/intpipe_alu_tb.sv \
             $(CURDIR)/intpipe_int_scoreboard_tb.sv \
             $(CURDIR)/intpipe_fp_scoreboard_tb.sv \
             $(CURDIR)/intpipe_mask_scoreboard_tb.sv \
             $(CURDIR)/debug_breakpoint_tb.sv \
             $(CURDIR)/intpipe_csr_replay_tb.sv \
             $(CURDIR)/intpipe_csr_msgs_tb.sv
LINT_FLAGS := -Wno-MULTITOP

include $(REPO_ROOT)/mk/verilator.mk
