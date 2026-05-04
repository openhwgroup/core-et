
      
//+define+ET_NEIGH_STANDALONE

// Do not compile any TBOX, any RBOX EMU, or any neighborhood DPI code
+define+ET_NO_TBOX
+define+ET_NO_RBOX_EMU
+define+ET_NO_NEIGH_DPI
+define+SIM_ALLOW_STUB
+define+STOP_COND=0
+define+RANDOMIZE
+define+RANDOMIZE_MEM_INIT
+define+RANDOMIZE_REG_INIT
+define+RANDOMIZE_GARBAGE_ASSIGN
+define+RANDOMIZE_INVALID_ASSIGN
+define+VPU
+define+GFX
+define+ET_PRINT_CONFIG
+define+EVL_TOP_LEVEL_HAS_FINISH
+define+EVL_ENABLE_QUIESCENT_CHECK
+define+VIRAGE_IGNORE_RESET


+define+ET_SIMULATION
+define+ENABLE_BEMU      
+libext+.v+.sv+.vh+.svh+.svp
+systemverilogext+.sv+.svh+.svp

+incdir+$REPOROOT
+incdir+$RTLROOT/soctop
+incdir+$REPOROOT/dv/libs
+incdir+$REPOROOT/dv/standalone/cpu_subsystem
+incdir+$REPOROOT/dv/standalone/cpu_subsystem/vsrc
+incdir+$REPOROOT/dv/ip_stub/

$REPOROOT/dv/standalone/cpu_subsystem/config_build.vh
      
// needed for testend monitor
-y $REPOROOT/dv/soc/

// Needed for endSimAt
-y $REPOROOT/test/common/vsrc

// APB injector and erbium_pkg
$REPOROOT/dv/standalone/cpu_subsystem/vsrc/erbium_pkg.sv
$REPOROOT/dv/standalone/cpu_subsystem/vsrc/cpu_subsystem_apb_injector.v
$REPOROOT/dv/standalone/cpu_subsystem/vsrc/cpu_subsystem_clock_gen.v      

//IRQ Injector
$REPOROOT/dv/standalone/cpu_subsystem/vsrc/cpu_subsystem_irq_injector.v

// Add RTL      
-f $RTLROOT/cpu_subsystem/cpu_subsystem_top_vcs.f      

// needed by archstate monitor
-f $REPOROOT/dv/arch_monitors/common/soc_monitors.f

// IP models/stubs
-y $REPOROOT/dv/ip_stub/

// top
$REPOROOT/dv/standalone/cpu_subsystem/vsrc/top.sv
