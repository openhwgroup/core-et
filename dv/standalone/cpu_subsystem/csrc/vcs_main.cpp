
#include "et_tb.hpp"

#include <iostream>

#define EXTERN_CPP extern "C"

EXTERN_CPP int vcs_main(int argc, char *argv[]);
EXTERN_CPP void vcs_atexit (void(*pfun)(int code));

et_tb * tb = nullptr;

EXTERN_CPP void vcs_startOfSim(){
   testLog log;
   if (tb == nullptr)
   {
      tb = new et_tb;
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "Minion: vcs_startofSim Sim TB - initialized TB " << (int *)tb << endm;
   }
   else
   {
      log << LOG_ERR << __PRETTY_FUNCTION__ << "Trying to instantiate Main_TB multiple times" << endm;
   }
}

EXTERN_CPP void vcs_endOfReset(){
   testLog log;
   if (tb == nullptr)
   {
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << "Minion: end of reset, but tb not initialized" << endm;
      exit(1);
   }
}

void simEnd(int code) {
  if (tb != nullptr)
  {
      delete tb;
      tb = nullptr;
      testLog log;
      log << LOG_INFO_FORCE << __PRETTY_FUNCTION__ << " tb destroyed" << endl; 
  }
}

EXTERN_CPP int main(int argc, char** argv, char**)
{
    vcs_atexit(simEnd);
    vcs_main(argc, argv);
    return 0;
}
