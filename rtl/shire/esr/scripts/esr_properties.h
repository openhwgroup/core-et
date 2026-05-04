#ifndef _ESR_PROPERTIES_
#define _ESR_PROPERTIES_

#include "esr_defines.h"
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// macros to get properties for a given address
////////////////////////////////////////////////////////////////////////////////

#define ESR_GET_MOD(ad)                                                    \
  ( ESR_IS_HART_REGION(ad) ? esr_region_HART :                           \
    ESR_IS_NEIGH_REGION(ad) ? esr_region_NEIGH :                         \
    ESR_IS_CACHE_REGION(ad) ? esr_region_CACHE :                         \
    ESR_IS_RBOX_REGION(ad)? esr_region_RBOX :                            \
    ESR_IS_SHIRE_REGION(ad)? esr_region_SHIRE :                          \
    esr_region_UNKNOWN)

#define ESR_GET_PROT(ad)                                                   \
  ( ESR_GET_MOD(ad) == esr_region_UNKNOWN ? PRV_INVALID :                  \
    esr_prot_t(( (ad) & ESR_REGION_PROT_MASK )  >> ESR_REGION_PROT_SHIFT) ) 

#define ESR_GET_SHIRE(ad)                                                  \
  ( ESR_GET_MOD(ad) == esr_region_UNKNOWN ? -1 :                           \
    (( (ad) & ESR_REGION_SHIRE_MASK )  >> ESR_REGION_SHIRE_SHIFT) )

#define ESR_GET_NEIGH(ad)                                                  \
  ( ESR_GET_MOD(ad) != esr_region_NEIGH ? -1 :                             \
    (( (ad) & ESR_NEIGH_MASK )  >>  ESR_NEIGH_SHIFT) )

#define ESR_GET_BANK(ad)                                                   \
  ( ESR_GET_MOD(ad) != esr_region_CACHE ? -1 :                             \
    (( (ad) & ESR_BANK_MASK )  >>  ESR_BANK_SHIFT) )

#define ESR_EXISTS(ad)      ESRRegisterList::getInstance().exists(ad)
#define ESR_GET_REGNO(ad)   ESRRegisterList::getInstance().get_regNo(ad)
#define ESR_GET_NAME(ad)    ESRRegisterList::getInstance().get_name(ad)      //returns reg name
#define ESR_GET_INFO(ad)    ESRRegisterList::getInstance().get_info(ad)  // returns reg name + shire + neigh/bank...
#define ESR_GET_ACCESS(ad)  ESRRegisterList::getInstance().get_access(ad)


////////////////////////////////////////////////////////////////////////////////
// Generic class holding ESR properties.
//  get_full_info is a pure virtual method to be implemented in subclasses
//     (see esr_properties.cc)
////////////////////////////////////////////////////////////////////////////////
class ESRProperties {
public:
  ESRProperties(const std::string name, const uint32_t regNo, const esr_prot_t prot, const esr_access_t access,
                const esr_address_t regionBase, const esr_address_t adMask ):
    name_(name),
    regNo_(regNo),
    prot_(prot),
    access_(access),
    address_ (computeAddress(regionBase, regNo, prot)),
    mask_ ( ~adMask)
  {
    assert ((address_ & adMask) == 0);
  }
  virtual ~ESRProperties() {}
  
  bool matches(esr_address_t ad) const { return (ad & mask_) == address_; }
  
  // return properties
  const std::string get_name() { return name_; }
  virtual const std::string get_info(esr_address_t ad) = 0;
  const uint32_t get_regNo() { return regNo_; }
  const esr_prot_t get_prot() { return prot_; }
  const esr_access_t get_access() { return access_; }
  const esr_address_t get_address() { return address_; }
  
protected:
  const std::string name_;
  const uint32_t regNo_;
  const esr_prot_t prot_;
  const esr_access_t access_;
  const esr_address_t address_;
  const esr_address_t mask_;

  esr_address_t computeAddress(const esr_address_t base, const uint64_t regNo, const esr_prot_t prot){
    return base | (uint64_t(prot) << ESR_REGION_PROT_SHIFT) | (regNo << 3);
  }
  
};

////////////////////////////////////////////////////////////////////////////////
// Specialization for the previous class, depending on the ESR block
////////////////////////////////////////////////////////////////////////////////

#define ESRPropertiesChild(name, BASE, MASK)                            \
  class ESRProperties##name : public ESRProperties {                    \
  public:                                                               \
  ESRProperties##name(const std::string name, const uint32_t regNo,     \
                      const esr_prot_t prot, const esr_access_t access) \
  : ESRProperties(name, regNo, prot, access, BASE, MASK){}              \
  const std::string get_info(esr_address_t ad);                         \
  };


ESRPropertiesChild(Neigh, ESR_NEIGH_REGION, ESR_REGION_SHIRE_MASK | ESR_NEIGH_MASK )
ESRPropertiesChild(Cache, ESR_CACHE_REGION, ESR_REGION_SHIRE_MASK | ESR_BANK_MASK  )
ESRPropertiesChild(Rbox,  ESR_RBOX_REGION,  ESR_REGION_SHIRE_MASK                  )
ESRPropertiesChild(Shire, ESR_SHIRE_REGION, ESR_REGION_SHIRE_MASK                  )

////////////////////////////////////////////////////////////////////////////////
// Singleton class holding a list of all the known ESRs
////////////////////////////////////////////////////////////////////////////////
class ESRRegisterList {
public:
  static ESRRegisterList & getInstance() {
    static ESRRegisterList instance;
    return instance;
  }

  bool exists(esr_address_t ad) { return getReg(ad) != NULL; }
  const std::string get_name(esr_address_t ad) {
    ESRProperties *r = getReg(ad);
    return r==NULL ? "UNKNOWN" : r->get_name();
  }
  const std::string get_info(esr_address_t ad) {
    ESRProperties *r = getReg(ad);
    return r==NULL ? "UNKNOWN" : r->get_info(ad);
  }
  const uint32_t get_regNo(esr_address_t ad) {
    ESRProperties *r = getReg(ad);
    return r==NULL ? (uint32_t) -1 : r->get_regNo();
  }
  const esr_access_t get_access(esr_address_t ad) {
    ESRProperties *r = getReg(ad);
    return r==NULL ? esr_access_INVALID : r->get_access();
  }
  
private:
  typedef std::vector<ESRProperties*>::iterator regs_it_t;
  std::vector<ESRProperties*> regs_;
  
  ESRProperties* getReg (esr_address_t ad) {
    for (regs_it_t i = regs_.begin(); i != regs_.end(); i++)
      if ((*i)->matches(ad)) return *i;
    return NULL;
  }

  ESRRegisterList();

  void add(ESRProperties* r) {
    esr_address_t base = r->get_address();
    assert ( getReg(base) == NULL );
    regs_.push_back(r);
  }
  
  ~ESRRegisterList() {
    for (regs_it_t i = regs_.begin(); i != regs_.end(); ++i)
      delete *i;
  }

};



#endif

