
#include <iostream>

#include "testBase.h"
#include "testEndMonitor.h"
#include "cosim.h"


extern "C" void CoSim_agent_drop_objection(int result, int minion_id, int thread_id);
extern "C" void CoSim_agent_initialized();

class et_tb : public testMain
{

public:
    uint64_t end_on_wfi   = 0;
    uint64_t shire_mask   = 0x1;
    uint64_t minion_mask  = 0x0ffffffffUL;
    uint64_t thread_mask  = 3UL;

    void parse_valueargs()
    {
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

    }

    et_tb() {
        parse_valueargs();
        // Initialize testEndMon with the active threads of the test
        testEndMonitor::getInstance().initShires(shire_mask, minion_mask, thread_mask);
        log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " initShires() is complete" << endm;
        testEndMonitor::getInstance().initMaxions(0);         // TODO: This is required for now - remove it when testendmonitr ir updated
        // Init Cosim
        (void)cosim::getInstance();


        log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " et_tb built" << endm;
    }
    ~et_tb()
    {
      cosim::deleteInstance();
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " et_tb destroyed" << endl;
    }



    void sim() {};

};
