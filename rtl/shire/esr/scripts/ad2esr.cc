////////////////////////////////////////////////////////////////////////////////
// compile with g++ ad2esr.cc esr_properties.cc -Wall -O3 -o ad2esr -std=c++11
// if will print information of the ESRs corresponding to the hex addresses
// read from stdin
////////////////////////////////////////////////////////////////////////////////

#include "esr_properties.h"
#include <string>
#include <iostream>


using namespace std;
int  main(){
  uint64_t ad;
  string line;
  for(;;) {
    cin >> line;
    if (! cin.good())  break;
    ad = std::stoll(line, nullptr, 16);
    cout << hex;
    cout << "----- " << ad << "-----" << endl;
    if (ESR_EXISTS(ad)) {
      cout << "REGNO="  << ESR_GET_REGNO(ad)  << endl
           << "NAME="   << ESR_GET_NAME(ad)   << endl
           << "INFO="   << ESR_GET_INFO(ad)   << endl
           << "ACCESS=" << ESR_GET_ACCESS(ad) << endl
           << "MOD="    << ESR_GET_MOD(ad)    << endl
           << "PROT="   << ESR_GET_PROT(ad)   << endl
           << "SHIRE="  << ESR_GET_SHIRE(ad)  << endl
           << "NEIGH="  << ESR_GET_NEIGH(ad)  << endl
           << "BANK="   << ESR_GET_BANK(ad)   << endl;
    }
    else {
      cout << "doesn't exist" << endl;
    }
  }
  
  return 0;

}
