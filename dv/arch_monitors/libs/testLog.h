/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TESTLOG_H_
#define _TESTLOG_H_

#include <cstdint>
#include <sstream>
#include <iostream>

enum logLevel {LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERR, LOG_FTL, LOG_INFO_FORCE, NR_LOG_LEVELS};

void endSimAt(uint32_t extraTime=0); // time in time units (ps)
void set_stop_time(uint64_t t);
void endSim();
bool simEnded();

#define DEFAULT_CLOCK_PERIOD 1000
#define time2Cycles(x) (x/DEFAULT_CLOCK_PERIOD)

class testLog
{
public:
  testLog(const std::string &name = "", logLevel logLvl = LOG_INFO):
  name_(name)
  {
    msgStarted_ = false;
    msgInLogLevel_ = false;
    fatal_ = false;
    if (!logLevelsSet_) setLogLevels();
    setLogLevel(logLvl);
  }

  testLog( const testLog &l )
  : name_(l.name_), msgStarted_(l.msgStarted_), msgInLogLevel_(l.msgInLogLevel_),
    fatal_(l.fatal_), logLevel_(l.logLevel_)
  {
    os_ << l.os_.str();
  }

  //testLog& operator=(const testLog&) = delete;

  void setLogLevel(logLevel level) {
    logLevel_ = level;
    // cannot mask errors or fatal
    if (logLevel_ > LOG_ERR) logLevel_ = LOG_ERR;
  }
  enum logLevel getLogLevel() {  return logLevel_; }

  void setName (const std::string name) { name_ = name; }

  template <class T>
  testLog &operator<<(T x) {
    if (msgInLogLevel_) {
      os_<<x;
    }
    return *this;
  }
  testLog &operator<<(logLevel l) {
    if (msgStarted_) {
      std::cout << "previous msg did not finish => [" << name_.c_str() << "]" << os_.str() << std::endl;
    }

    msgStarted_ = true;

    if ((l >= globalLogLevel_) && (l >= logLevel_)) {
      if ((use_evl_lib_ != 0) && ((l == LOG_WARN) || (l == LOG_ERR) || (l == LOG_FTL))) {
        //
        // If the EVL package has been included, then DUT errors and warnings will be printed by the
        // EVL package (which also tracks the number of outstanding failures).  When the EVL
        // package prints errors, no timestamp information is neceesary.
        //
        os_ << "";
      }
      switch(l) {
        case LOG_DEBUG:      os_ << "DEBUG "; break;
        case LOG_INFO:       os_ << "INFO ";  break;
        case LOG_WARN:       os_ << "WARN ";  break;
        case LOG_ERR:        os_ << "ERROR "; break;
        case LOG_INFO_FORCE: os_ << "INFO ";  break;
        default:             os_ << "FATAL "; fatal_ = true;
      }
      os_ << name_.c_str() << ": ";
      curLogLevel_   = l;
      msgInLogLevel_ = true;
    } else {
      msgInLogLevel_ = false;
    }
    if ((l >= LOG_ERR) && (l != LOG_INFO_FORCE)) {
      errors_++;
    }
    return *this;
  }
  testLog & operator<<(testLog& m(testLog&)) {
    return m(*this);
  }
  void endl() {
    if (msgStarted_) {
      os_ << std::endl;
    }
  }

  void endm() {
    if (!msgStarted_) {
      std::cout << "endm without msg start (string=" << os_.str() << ")" << std::endl;
    } else if (msgInLogLevel_) {
      if ((use_evl_lib_ != 0) && (curLogLevel_ >= LOG_WARN)) {
        if (sendEVLMessage(curLogLevel_, os_.str()) <= 0) {
          std::cout << "" << os_.str() << std::endl;
        }
      } else {
        std::cout << "" << os_.str() << std::endl;
      }
    }

    os_.str("");
    os_.clear();
    if (use_evl_lib_ == 0) {
      if (fatal_) endSim();
      if (errors_ >= maxErrors_) {
        if ( !simEnded() )  {
          std::cout<<"Stopping simulation because max number of errors reached ("<<maxErrors_<<")"<<std::endl;
          endSim();
        }
      }
    }
    msgInLogLevel_ = true;
    msgStarted_    = false;
    curLogLevel_   = LOG_DEBUG;
  }

private:
  std::string name_;
  std::ostringstream os_;
  bool msgStarted_;
  bool msgInLogLevel_;
  bool fatal_;
  logLevel curLogLevel_ = LOG_DEBUG;
  static unsigned errors_;
  static logLevel globalLogLevel_;
  static logLevel defaultLogLevel_;
  static bool logLevelsSet_;
  static int use_evl_lib_;
public:
  static logLevel getGlobalLogLevel() {
    if (!logLevelsSet_) setLogLevels();
    return globalLogLevel_;
  }
  static logLevel getDefaultLogLevel() {
    if (!logLevelsSet_) setLogLevels();
    return defaultLogLevel_;
  }
  static void setGlobalLogLevel(int level) {
    if (!logLevelsSet_) setLogLevels();
    globalLogLevel_ = (logLevel) level;
  }
  static void setDefaultLogLevel(int level) {
    if (!logLevelsSet_) setLogLevels();
    defaultLogLevel_ = (logLevel) level;
  }

  static unsigned maxErrors_;
  static unsigned getErrors() {return errors_;}

  uint64_t simTime();
  unsigned simCycle();
  std::string simTimeStr();
  static void setLogLevels();
  static int sendEVLMessage(logLevel level, std::string str);

 private:
  logLevel logLevel_;
};


static inline testLog& endm( testLog& log) {
  log.endm();
  return log;
}

static inline testLog& endl( testLog& log) {
  log.endl();
  return log;
}

#endif
