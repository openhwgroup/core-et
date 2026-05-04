/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_BASE_H_
#define _TEST_BASE_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <cstdio>
#include "testLog.h"
#include "testDpi.h"
#include "plusArgs.h"

class testBase {
public:
  testBase(const std::string &name) :
    name_(name),
      log(name, getLogLevel()){
    }
  virtual ~testBase() {}
  const std::string getName() const { return name_; }
  void setLogLevel(logLevel level) { log.setLogLevel(level); }
protected:
  const std::string name_;
  testLog log;

  virtual logLevel getLogLevel() {
    int logLvl;
    if( valuePlusArgs(name_ + "_LOG_LEVEL","%i", &logLvl) )
      return  (logLevel) logLvl;
    else return testLog::getDefaultLogLevel();
  }
};

// class with dpi functions bound to a module instance
// import dpi functions need to be context
// can call export functions
class testMod : public testBase {
 public:
  testMod(const std::string &name, const std::string &path):
    testBase(name),
    path_(path)
  {
    scope_ = svGetScopeFromName (path.c_str());
    if ( scope_ == NULL) log<<LOG_ERR<<"cannot determine sv scope for path "<<path<<endm;
  }
  virtual ~testMod() {}
  svScope getScope () { return scope_; }
  const std::string getPath() const { return path_;}
 protected:
  svScope scope_;
  const std::string path_;
};

template<class T> class testDriver;

template<class transaction_t>
class testChannel: public testBase {
public:
  typedef boost::function<void (const transaction_t &t)> callback_t;
  testChannel(const std::string &name) :
    testBase (name),
    waiting_(false)
  {}
  virtual ~testChannel() {}
  virtual void addTransaction(const transaction_t &t) {
    bool idle = empty();
    enqueueTransaction(t);
    if ( idle || waiting_) tryStartTransaction();
  }

  void attachDriver (testDriver<transaction_t> &drv) {
    callback_ = boost::bind(&testDriver<transaction_t>::startTransaction, &drv,_1);
    drv.setChannel(this);
  }

  void endTransaction() {
    assert(!transactions_.empty());
    preDequeue();
    transactions_.pop_front();
    if(!transactions_.empty()) tryStartTransaction();
  }

  bool empty() { return transactions_.empty(); }
  unsigned size()  { return transactions_.size(); }

  void tryStartTransaction() {
    waiting_ = !getNextTransaction();
    if (!waiting_) startTransaction(transactions_.front());
  }
protected:

  void startTransaction(const transaction_t &t) {
    if (callback_) callback_(t);
  }
  callback_t callback_;
  bool waiting_;
  std::list<transaction_t> transactions_;

  // function called to enqueue a new transaction
  virtual void enqueueTransaction(const transaction_t &t){
    transactions_.push_back(t);
  }
  // function called before dequeing the current transaction (in front() )
  virtual void preDequeue() {}

  // function to put next transaction in front()... returns false if no transactions ready
  // if no transactions ready, a call to 'tryStartTransaction' will restart the transactions
  virtual bool getNextTransaction(){
    return  true;
  }
};

template<class T>
class testDriver : public testMod {
  friend class testChannel<T>;
public:
  testDriver(const std::string &name, const std::string &path) :
    testMod(name, path),
    channel_(NULL)
  {}

  virtual ~testDriver() {}
protected:
  void setChannel( testChannel<T> *c) { channel_ = c;}
  virtual void startTransaction(const T &t) = 0;
  void transactionDone() {
    if (channel_!=NULL) channel_->endTransaction();
  }
  testChannel<T> *channel_;
};

template<class transaction_t>
class testMonitor : public testMod {
public:

  typedef boost::function<void (const transaction_t &t)> callback_t;

  testMonitor(const std::string &name, const std::string &path, unsigned nrEvents = 1) :
    testMod(name, path),
    nrEvents_(nrEvents)
  {
    if(nrEvents==0) log<<LOG_FTL<<"nrEvents has to be > 0"<<endm;
    callbacks_ = new std::vector<callback_t> [nrEvents];
  }

  virtual ~testMonitor() {
    delete[] callbacks_;
  }

  void addCallback ( callback_t f, unsigned eventNr=0) {
    if(eventNr>=nrEvents_) log<<LOG_FTL<<"bad eventNr "<<eventNr<<" when installing callback"<<endm;
    else callbacks_[eventNr].push_back(f);
  }

  //TODO: add capability to wait for event
private:
  const unsigned nrEvents_;
  std::vector<callback_t> *callbacks_;
protected:
  void setEvent(const transaction_t &t, unsigned eventNr = 0){
    if(eventNr>=nrEvents_) log<<LOG_FTL<<"bad eventNr "<<eventNr<<" when setting event"<<endm;
    else {
      typename std::vector<callback_t>::iterator it;
      for( it = callbacks_[eventNr].begin(); it != callbacks_[eventNr].end(); ++it)
        (*it)(t);
    }
  }
};

class testMain : public testBase {
public:
  typedef boost::function<bool ()> clean_end_f;

  static testMod* getMod(svScope scope) {
    if (modMap_.count(scope) == 0) return NULL;
    else return modMap_[scope];
  }
  virtual void sim() = 0;
  static bool timeOut_;

  static void addTestMod(testMod& m, bool destroy=false);
  static bool cleanEnd() {
    if (cleanEnd_) return cleanEnd_();
    else return true;
  }
  static void setCleanEnd(clean_end_f f) {
    cleanEnd_ = f;
  }
protected:
  testMain() :
    testBase("testMain")
  {
    // set global log level
    if (instanceCount_ > 0) log<<LOG_FTL<<"only one instance of testMain allowed"<<endm;
    instanceCount_ ++;

    // read random seed
    unsigned seed = time(0);
    valuePlusArgs("SEED","%d",&seed);
    log<<LOG_INFO<<"SEED: "<<std::dec<<seed<<endm;
    srand(seed);
    srand48(seed);

    // read max number of allowed errors
    int maxErrors;
    if (valuePlusArgs("maxErrorCount","%d",&maxErrors)) {
      if (maxErrors<=0) {
        std::cout<<"MAX_ERRORS <=0. Ignoring and setting to 1"<<std::endl;
        testLog::maxErrors_=1;
      }
      else{
        testLog::maxErrors_=maxErrors;
      }
    }
  }

  virtual ~testMain() {
    //destroy objects that have been allocated and marked as to be destroyed upon sim end
    //this is for objects allocated from the verilog with 'new', where there is no destructor to be called
    while( !modToDestroy_.empty() ) {
      log<<LOG_DEBUG<<"Mods to destroy: "<<endm;

      for (std::vector<testMod*>::iterator it = modToDestroy_.begin() ; it != modToDestroy_.end(); ++it)
      {
        log<<LOG_DEBUG<<" "<<(*it)->getName().c_str()<<endm;
      }

      log<<LOG_DEBUG<<"Destroying "<<modToDestroy_.back()->getName().c_str()<<endm;
      delete (modToDestroy_.back());
      modToDestroy_.pop_back();
    }

    if (timeOut_) log<<LOG_INFO_FORCE<<"ERROR TEST FAILED: TIMEOUT ( with " << std::dec<< testLog::getErrors() << " errors )"<<endm;
    #ifndef COCOTB_TEST_END
    else if (testLog::getErrors() == 0) print_pass_nekko();
    #endif
    else if (testLog::getErrors() == 1) log<<LOG_INFO_FORCE<<"TEST FAILED: " << std::dec << testLog::getErrors() << " error"<<endm;
    else log<<LOG_INFO_FORCE<<"TEST FAILED: " << std::dec << testLog::getErrors() << " errors"<<endm;
  }

    void print_pass_nekko() 
    {
        log << LOG_INFO_FORCE << "*** TEST HAS PASSED *** \\    /\\ " << endm;
        log << LOG_INFO_FORCE << "*** TEST HAS PASSED ***  )  ( ')" << endm;
        log << LOG_INFO_FORCE << "*** TEST HAS PASSED *** (  /  ) " << endm;
        log << LOG_INFO_FORCE << "*** TEST HAS PASSED ***  \(__)| " << endm;
        log << LOG_INFO_FORCE << "***   PURRRRRFECT!  ***         " << endm;
    }

  static std::map<svScope, testMod*> modMap_;
  static std::vector<testMod*> modToDestroy_;
  static int instanceCount_;
  static clean_end_f cleanEnd_;
};

// other useful macros
#define LOG2(x) log2_<x>::value
#define MASK(x) ((1ULL<<(x))-1)


template <unsigned x>
struct log2_ { enum { value = 1 + log2_<(x>>1)>::value }; };
template <> struct log2_<1> { enum { value = 0 }; };



// and some useful macros
#ifndef TB_MAX
#define  TB_MAX(a,b) ( (a) > (b) ? (a) : (b) )
#endif

#ifndef TB_MIN
#define  TB_MIN(a,b) ( (a) < (b) ? (a) : (b) )
#endif

#endif
