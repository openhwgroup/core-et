/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Global
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <svdpi.h>
// Local
#include "main_tb.h"

#ifdef ENABLE_UEVENT
   #include <uevent/neigh.h>
#endif


extern "C" void CoSim_agent_drop_objection(int result, int minion_id, int thread_id);
extern "C" void CoSim_agent_initialized();

main_tb *tb = NULL;
testLog *g_maxion_log = NULL;
uint64_t g_maxion_check_enable; // Indicates whether checks are enabled for a given maxion hart

void maxion_debug_log(int hartid, const char* fmt, ...) {
   char buf[256];
   va_list ap;
   va_start(ap, fmt);
   int n = vsnprintf(buf, sizeof(buf), fmt, ap);
   while (buf[n-1] == '\n') buf[--n] = '\0'; // trim extra EOL
   va_end(ap);
   (*g_maxion_log) << LOG_DEBUG << "MAX[" << hartid << "] " << buf << endm;
}

void maxion_error_log(int hartid, const char* fmt, ...) {
   char buf[256];
   va_list ap;
   va_start(ap, fmt);
   int n = vsnprintf(buf, sizeof(buf), fmt, ap);
   while (buf[n-1] == '\n') buf[--n] = '\0'; // trim extra EOL
   va_end(ap);
   (*g_maxion_log) << LOG_ERR << "MAX[" << hartid << "] " << buf << endm;
}

EXTERN_CPP void vcs_startOfSim(){
   testLog log;
   if (tb == NULL) {
      tb = new main_tb;
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "Minion: vcs_startofSim Sim TB - initialized TB " << (int *)tb << endm;
   } else {
      log << LOG_ERR << __PRETTY_FUNCTION__ << "Trying to instantiate Main_TB multiple times" << endm;
   }
}

EXTERN_CPP void vcs_endOfReset(){
   testLog log;
   if (tb == NULL) {
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "Minion: end of reset, but tb not initialized" << endm;
      exit(1);
   }
}

EXTERN_CPP void cosim_object_delete() {
   testLog log;
   if (tb) {
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "main_tb: simEnd - Delete Checker " << (int *)tb << endm;
      delete tb;
      tb = nullptr;
   } else {
      log << LOG_ERR << __PRETTY_FUNCTION__ << "main_tb: Unable to delete TB" << endm;
   }
   setEnded();
}


EXTERN_CPP void cosim_set_verbosity(int bemu_log_level) {
   #ifndef MAXION_SA
      testLog log;
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "main_tb: Setting Verbosity for BEMU to : " << bemu_log_level << endm;
      cosim::getInstance().set_verbosity(bemu_log_level);
   #endif
}

#ifdef ENABLE_UEVENT
   #ifdef USE_FAKE_UEVENT
      void uevent_send_minion_packet(const uevent_minion_packet*) {
         return;
      }

      void uevent_send_retire_event(const uevent_retire_event*) {
         return;
      }

      void uevent_send_exception_event(const uevent_exception_event*) {
         return;
      }

      void uevent_send_neigh_packet(const uevent_neigh_packet*) {
         return;
      }
   #endif
#endif

#ifdef ZEBU_DPI
   static bool get_env_variable(const char* key, const char* fmt, uint64_t* v)
   {
       const char* env = std::getenv(key);
       if (!env) return false;
       return sscanf(env, fmt, v) > 0;
   }
#endif

main_tb::main_tb()
{
   #ifndef ZEBU_DPI
      char *riscvemu_argv[64];
      char  riscvemu_elf[4096];
      char  bootrom_elf[4096];
      char  mmio_range[4096];
      char  riscvemu_args[4096];
      char  riscvemu_cfg[1024];
      char  maxion_start_pc[20];
      char  vharness_opt[32];
      char  reset_vector_opt[32];
      char  bootrom_opt[32];
      char  mmio_range_opt[32];
      #ifdef MAXION_SA
         uint64_t end_on_wfi   = 0;
         uint64_t shire_mask   = 0UL;
         uint64_t minion_mask  = 0UL;
         uint64_t thread_mask  = 0UL;
         uint64_t maxions_mask = 0xfUL;
      #else // MAXION_SA == 0
         // By default enable S0N0M0H0
         uint64_t end_on_wfi   = 0;
         uint64_t shire_mask   = 1UL;
         uint64_t minion_mask  = 1UL;
         uint64_t thread_mask  = 1UL;
         uint64_t maxions_mask = 0UL;
      #endif // MAXION_SA
      int maxion_log_level = -1;
   #else // ZEBU_DPI == 1
      // By default enable shire 0 and shire 32
      uint64_t end_on_wfi   = 0;
      uint64_t shire_mask   = 0x100000001UL;
      uint64_t minion_mask  = 0x0ffffffffUL;
      uint64_t thread_mask  = 3UL;
      uint64_t maxions_mask = 0UL;
   #endif // ZEBU_DPI

   g_maxion_check_enable = maxions_mask; // Default to checks enabled for all running maxion harts

   #ifndef ZEBU_DPI
      *riscvemu_elf = (char) 0;
      strcpy(vharness_opt, "vharness");
      strcpy(reset_vector_opt, "--reset_vector");
      strcpy(bootrom_opt, "--bootrom");
      strcpy(mmio_range_opt, "--mmio_range");
      memset(maxion_start_pc, 0, sizeof(maxion_start_pc));
      memset(riscvemu_argv, 0, sizeof(riscvemu_argv));
      memset(riscvemu_elf, 0, sizeof(riscvemu_elf));
      memset(bootrom_elf, 0, sizeof(bootrom_elf));
      memset(mmio_range, 0, sizeof(mmio_range));
      memset(riscvemu_cfg, 0, sizeof(riscvemu_cfg));
      // Retrieve runtime arguments for enabled threads
      if (valuePlusArgs("SHIRE_MASK", "%" SCNx64, &shire_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " SHIRE_MASK 0x" << std::hex << shire_mask << endm;
      }
      if (valuePlusArgs("MINION_MASK", "%" SCNx64, &minion_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " MINION_MASK 0x" << std::hex << (minion_mask) << endm;
      }
      if (valuePlusArgs("THREAD_MASK", "%" SCNx64, &thread_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " THREAD_MASK 0x" << std::hex << (thread_mask) << endm;
      }
      if (valuePlusArgs("END_ON_WFI", "%" SCNx64, &end_on_wfi)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " Test end mechanism set to WFI" << endm;
         testEndMonitor::getInstance().setEndOnWfi();
      }

      // Maxions mask
      if (valuePlusArgs("MAXIONS_MASK", "%" SCNx64, &maxions_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " MAXIONS_MASK 0x" << std::hex << maxions_mask << endm;
         g_maxion_check_enable = maxions_mask; // Default to checks enabled for all running harts
      }

      if (valuePlusArgs("maxion_check_enable", "%" SCNx64, &g_maxion_check_enable)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " maxion_check_enable 0x" << std::hex << g_maxion_check_enable << std::dec << endm;
      }

      if (valuePlusArgs("maxion_start_pc", "%s", riscvemu_args)) {
         int ii;

         strncpy(maxion_start_pc, riscvemu_args, 19);
         riscvemu_argv[0] = vharness_opt;
         for (ii = 1; ii < 64; ii++) {
            if (riscvemu_argv[ii] == NULL) {
               break;
            }
         }
         riscvemu_argv[ii++] = reset_vector_opt;
         riscvemu_argv[ii] = maxion_start_pc;
      }

      if (valuePlusArgs("maxion_elf", "%s", riscvemu_elf)) {
         int ii;

         riscvemu_argv[0] = vharness_opt;
      }

      if (valuePlusArgs("maxion_bootrom", "%s", bootrom_elf)) {
         int ii;

         riscvemu_argv[0] = vharness_opt;
         for (ii = 1; ii < 64; ii++) {
            if (riscvemu_argv[ii] == NULL) {
               break;
            }
         }
         riscvemu_argv[ii++] = bootrom_opt;
         riscvemu_argv[ii]   = bootrom_elf;
      }

      if (valuePlusArgs("maxion_mmio", "%s", mmio_range)) {
         int  ii;
         char *p;

         riscvemu_argv[0] = vharness_opt;
         for (ii = 1; ii < 64; ii++) {
            if (riscvemu_argv[ii] == NULL) {
               break;
            }
         }
         p = mmio_range;
         while ((*p != (char) 0) && (ii < 63)) {
            while ((*p == ' ') || (*p == '\t') || (*p == ',') || (*p == '=')) {
               p++;
            }
            if (*p != (char) 0) {
               riscvemu_argv[ii++] = mmio_range_opt;
               riscvemu_argv[ii++] = p;
               while ((*p != ' ') && (*p != '\t') && (*p != ',') && (*p != '=') && (*p != (char) 0)) {
                  p++;
               }
               if (*p != (char) 0) {
                  *p++ = (char) 0;
               }
            }
         }
      }

      g_maxion_log = new testLog("MAXION");
      if (valuePlusArgs("maxion_log_level", "%d", &maxion_log_level)) {
         g_maxion_log->setLogLevel((logLevel)maxion_log_level);
      }

      if (valuePlusArgs("riscvemu_args", "%s", riscvemu_args)) {
         int  ii;
         char *p;

         riscvemu_argv[0] = vharness_opt;
         for (ii = 1; ii < 64; ii++) {
            if (riscvemu_argv[ii] == NULL) {
               break;
            }
         }
         p = riscvemu_args;
         while ((*p != (char) 0) && (ii < 64)) {
            while ((*p == ' ') || (*p == '\t') || (*p == ',') || (*p == '=')) {
               p++;
            }
            if (*p != (char) 0) {
               riscvemu_argv[ii++] = p;
               while ((*p != ' ') && (*p != '\t') && (*p != ',') && (*p != '=') && (*p != (char) 0)) {
                  p++;
               }
               if (*p != (char) 0) {
                  *p++ = (char) 0;
               }
            }
         }
      }

      if (valuePlusArgs("riscvemu_cfg", "%s", riscvemu_cfg)) {
         int ii;

         riscvemu_argv[0] = vharness_opt;
         for (ii = 1; ii < 64; ii++) {
            if (riscvemu_argv[ii] == NULL) {
               break;
            }
         }
         riscvemu_argv[ii] = riscvemu_cfg;
      }

      if (valuePlusArgs("riscvemu_enable", "%s", riscvemu_args)) {
         if ((*riscvemu_args != (char) 0) && (*riscvemu_args != '0')) {
            riscvemu_argv[0] = vharness_opt;
         } else {
            riscvemu_argv[0] = NULL;
         }
      }

   #else // ZEBU_DPI == 1
      // Retrieve runtime arguments for enabled threads
      if (get_env_variable("ZEBU_SHIRE_MASK", "%llx", &shire_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " SHIRE_MASK 0x" << std::hex << shire_mask << endm;
      }
      if (get_env_variable("ZEBU_MINION_MASK", "%llx", &minion_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " MINION_MASK 0x" << std::hex << (minion_mask) << endm;
      }
      if (get_env_variable("ZEBU_THREAD_MASK", "%llx", &thread_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " THREAD_MASK 0x" << std::hex << (thread_mask) << endm;
      }
      if (get_env_variable("ZEBU_END_ON_WFI", "%llx", &end_on_wfi)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " Test end mechanism set to WFI" << endm;
         testEndMonitor::getInstance().setEndOnWfi();
      }

      // Maxions mask
      if (get_env_variable("ZEBU_MAXIONS_MASK", "%llx", &maxions_mask)) {
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " MAXIONS_MASK 0x" << std::hex << maxions_mask << endm;
      }
   #endif // ZEBU_DPI

   // Initialize testEndMon with the active threads of the test
   testEndMonitor::getInstance().initShires(shire_mask, minion_mask, thread_mask);
   log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " initShires() is complete" << endm;

   // Initialize testEndMon with the active maxions for the test
   testEndMonitor::getInstance().initMaxions(maxions_mask);
   log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " initMaxions() is complete" << endm;

   // Raise Objection to EVL that C Side Test Bench is alive - to wait for all Minions to be done
   #ifndef MAXION_SA
      #ifndef ZEBU_DPI
       #if !defined(ZEBU_DPI) && !defined(VERILATOR) && defined(EVL_SIMULATION)
         svScope scope;
         scope = svGetScopeFromName("evl_base_pkg");
         scope = svSetScope(scope);
         #endif
         CoSim_agent_initialized();
      #endif
   #endif

   #ifdef ENABLE_UEVENT
      #ifndef USE_FAKE_UEVENT
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " calling uevent_init()" << endm;
         uevent_init();
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " uevent_init() is complete" << endm;
         trace = uevent_open("uevent_retire.trace.lz4", "w");
         log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " uevent_open() is complete" << endm;
         if (trace) {
            log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " calling uevent_attach()" << endm;
            uevent_attach(UEVENT_TYPE_RETIRE, (uevent_callback)uevent_write, trace);
            uevent_attach(UEVENT_TYPE_EXCEPTION, (uevent_callback)uevent_write, trace);
            log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " uevent_attach() is complete" << endm;
         }
      #endif
   #endif
   log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " main_tb built" << endm;
}

main_tb::~main_tb (){

   //uint32_t results = log.getErrors();

   #ifdef ENABLE_UEVENT
      #ifndef USE_FAKE_UEVENT
         if (trace) {
            uevent_close(trace);
            trace = nullptr;
         }
         uevent_quit();
      #endif
   #endif
         log << LOG_INFO << "Calling main_tb destroyer" << endm;
    #if  !defined(MAXION_SA) 
      if (! cosim::getInstance().is_empty()) {
          if (cosim::getInstance().checker_is_on()) {
            log << (testMain::cleanEnd() ? LOG_ERR : LOG_WARN) << "test did not finish. Did not receive 'done' from all the minions" << endm;
            log << LOG_INFO << "Sending drop objection to EVL SV to shutdown simulation" << endm;
            #if !defined(ZEBU_DPI) && !defined(VERILATOR) && defined(EVL_SIMULATION)
               svScope scope;
               scope = svGetScopeFromName("evl_base_pkg");
               scope = svSetScope(scope);
               CoSim_agent_drop_objection(-1, -1, -1);
            #endif
         } else {
            log << LOG_WARN << "test did not finish.  Did not receive 'done' from all the minions" << endm;
            log << LOG_INFO << "Sending drop objection to EVL SV to shut down simulation" << endm;
            #if !defined(ZEBU_DPI) && !defined(VERILATOR) && defined(EVL_SIMULATION)
               svScope scope;
               scope = svGetScopeFromName("evl_base_pkg");
               scope = svSetScope(scope);
               CoSim_agent_drop_objection(0, -1, -1);
            #endif
         }
      }
      // FUTURE Use RAII to manage the lifetime of the cosim object instead
      cosim::deleteInstance();
   #endif
   if (g_maxion_log != nullptr) {
      delete g_maxion_log;
      g_maxion_log = nullptr;
   }
}

void main_tb::reset()
{
   log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "Main_TB : reset()" << endm;
   #ifndef MAXION_SA
      cosim::deleteInstance(); // We should replace this with something like cosim::reset(). See also: VERIF-3315
   #endif
   testEndMonitor::deleteInstance();
   dispatcher::deleteInstance();
}

void main_tb::sim()
{
   log << LOG_INFO << __PRETTY_FUNCTION__ << "Main_TB Sim called" << endm;
}
