//
// Generated file, do not edit! Created by nedtool 4.6 from inet/linklayer/ieee80211/mgmt/nwinfo.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "nwinfo_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(NWinfo);

NWinfo::NWinfo() : ::cObject()
{
    this->idx_var = 0;
    this->rss_var = 0;
    this->rssthr_var = 0;
    this->SNR_var = 0;
    this->SNRthr_var = 0;
    this->AdDataRate_var = 0;
    this->xusers_var = 0;
    this->mcost_var = 0;
    this->nnDelay_var = 0;
    this->jitter_var = 0;
    this->SSID_var = 0;
    this->speed_var = 0;
    this->Thrvolume_var = 0;
    this->Rxvolume_var = 0;
    this->srcportno_var = 1025;
    this->Result_var = 0;
}

NWinfo::NWinfo(const NWinfo& other) : ::cObject(other)
{
    copy(other);
}

NWinfo::~NWinfo()
{
}

NWinfo& NWinfo::operator=(const NWinfo& other)
{
    if (this==&other) return *this;
    ::cObject::operator=(other);
    copy(other);
    return *this;
}

void NWinfo::copy(const NWinfo& other)
{
    this->idx_var = other.idx_var;
    this->rss_var = other.rss_var;
    this->rssthr_var = other.rssthr_var;
    this->SNR_var = other.SNR_var;
    this->SNRthr_var = other.SNRthr_var;
    this->AdDataRate_var = other.AdDataRate_var;
    this->xusers_var = other.xusers_var;
    this->mcost_var = other.mcost_var;
    this->nnDelay_var = other.nnDelay_var;
    this->jitter_var = other.jitter_var;
    this->SSID_var = other.SSID_var;
    this->speed_var = other.speed_var;
    this->Thrvolume_var = other.Thrvolume_var;
    this->Rxvolume_var = other.Rxvolume_var;
    this->srcportno_var = other.srcportno_var;
    this->Result_var = other.Result_var;
}

void NWinfo::parsimPack(cCommBuffer *b)
{
    doPacking(b,this->idx_var);
    doPacking(b,this->rss_var);
    doPacking(b,this->rssthr_var);
    doPacking(b,this->SNR_var);
    doPacking(b,this->SNRthr_var);
    doPacking(b,this->AdDataRate_var);
    doPacking(b,this->xusers_var);
    doPacking(b,this->mcost_var);
    doPacking(b,this->nnDelay_var);
    doPacking(b,this->jitter_var);
    doPacking(b,this->SSID_var);
    doPacking(b,this->speed_var);
    doPacking(b,this->Thrvolume_var);
    doPacking(b,this->Rxvolume_var);
    doPacking(b,this->srcportno_var);
    doPacking(b,this->Result_var);
}

void NWinfo::parsimUnpack(cCommBuffer *b)
{
    doUnpacking(b,this->idx_var);
    doUnpacking(b,this->rss_var);
    doUnpacking(b,this->rssthr_var);
    doUnpacking(b,this->SNR_var);
    doUnpacking(b,this->SNRthr_var);
    doUnpacking(b,this->AdDataRate_var);
    doUnpacking(b,this->xusers_var);
    doUnpacking(b,this->mcost_var);
    doUnpacking(b,this->nnDelay_var);
    doUnpacking(b,this->jitter_var);
    doUnpacking(b,this->SSID_var);
    doUnpacking(b,this->speed_var);
    doUnpacking(b,this->Thrvolume_var);
    doUnpacking(b,this->Rxvolume_var);
    doUnpacking(b,this->srcportno_var);
    doUnpacking(b,this->Result_var);
}

int NWinfo::getIdx() const
{
    return idx_var;
}

void NWinfo::setIdx(int idx)
{
    this->idx_var = idx;
}

double NWinfo::getRss() const
{
    return rss_var;
}

void NWinfo::setRss(double rss)
{
    this->rss_var = rss;
}

double NWinfo::getRssthr() const
{
    return rssthr_var;
}

void NWinfo::setRssthr(double rssthr)
{
    this->rssthr_var = rssthr;
}

double NWinfo::getSNR() const
{
    return SNR_var;
}

void NWinfo::setSNR(double SNR)
{
    this->SNR_var = SNR;
}

double NWinfo::getSNRthr() const
{
    return SNRthr_var;
}

void NWinfo::setSNRthr(double SNRthr)
{
    this->SNRthr_var = SNRthr;
}

double NWinfo::getAdDataRate() const
{
    return AdDataRate_var;
}

void NWinfo::setAdDataRate(double AdDataRate)
{
    this->AdDataRate_var = AdDataRate;
}

double NWinfo::getXusers() const
{
    return xusers_var;
}

void NWinfo::setXusers(double xusers)
{
    this->xusers_var = xusers;
}

double NWinfo::getMcost() const
{
    return mcost_var;
}

void NWinfo::setMcost(double mcost)
{
    this->mcost_var = mcost;
}

simtime_t NWinfo::getNnDelay() const
{
    return nnDelay_var;
}

void NWinfo::setNnDelay(simtime_t nnDelay)
{
    this->nnDelay_var = nnDelay;
}

double NWinfo::getJitter() const
{
    return jitter_var;
}

void NWinfo::setJitter(double jitter)
{
    this->jitter_var = jitter;
}

const char * NWinfo::getSSID() const
{
    return SSID_var.c_str();
}

void NWinfo::setSSID(const char * SSID)
{
    this->SSID_var = SSID;
}

double NWinfo::getSpeed() const
{
    return speed_var;
}

void NWinfo::setSpeed(double speed)
{
    this->speed_var = speed;
}

int NWinfo::getThrvolume() const
{
    return Thrvolume_var;
}

void NWinfo::setThrvolume(int Thrvolume)
{
    this->Thrvolume_var = Thrvolume;
}

int NWinfo::getRxvolume() const
{
    return Rxvolume_var;
}

void NWinfo::setRxvolume(int Rxvolume)
{
    this->Rxvolume_var = Rxvolume;
}

double NWinfo::getSrcportno() const
{
    return srcportno_var;
}

void NWinfo::setSrcportno(double srcportno)
{
    this->srcportno_var = srcportno;
}

int NWinfo::getResult() const
{
    return Result_var;
}

void NWinfo::setResult(int Result)
{
    this->Result_var = Result;
}

class NWinfoDescriptor : public cClassDescriptor
{
  public:
    NWinfoDescriptor();
    virtual ~NWinfoDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NWinfoDescriptor);

NWinfoDescriptor::NWinfoDescriptor() : cClassDescriptor("NWinfo", "cObject")
{
}

NWinfoDescriptor::~NWinfoDescriptor()
{
}

bool NWinfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NWinfo *>(obj)!=NULL;
}

const char *NWinfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NWinfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 16+basedesc->getFieldCount(object) : 16;
}

unsigned int NWinfoDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<16) ? fieldTypeFlags[field] : 0;
}

const char *NWinfoDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "idx",
        "rss",
        "rssthr",
        "SNR",
        "SNRthr",
        "AdDataRate",
        "xusers",
        "mcost",
        "nnDelay",
        "jitter",
        "SSID",
        "speed",
        "Thrvolume",
        "Rxvolume",
        "srcportno",
        "Result",
    };
    return (field>=0 && field<16) ? fieldNames[field] : NULL;
}

int NWinfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "idx")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rss")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "rssthr")==0) return base+2;
    if (fieldName[0]=='S' && strcmp(fieldName, "SNR")==0) return base+3;
    if (fieldName[0]=='S' && strcmp(fieldName, "SNRthr")==0) return base+4;
    if (fieldName[0]=='A' && strcmp(fieldName, "AdDataRate")==0) return base+5;
    if (fieldName[0]=='x' && strcmp(fieldName, "xusers")==0) return base+6;
    if (fieldName[0]=='m' && strcmp(fieldName, "mcost")==0) return base+7;
    if (fieldName[0]=='n' && strcmp(fieldName, "nnDelay")==0) return base+8;
    if (fieldName[0]=='j' && strcmp(fieldName, "jitter")==0) return base+9;
    if (fieldName[0]=='S' && strcmp(fieldName, "SSID")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+11;
    if (fieldName[0]=='T' && strcmp(fieldName, "Thrvolume")==0) return base+12;
    if (fieldName[0]=='R' && strcmp(fieldName, "Rxvolume")==0) return base+13;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcportno")==0) return base+14;
    if (fieldName[0]=='R' && strcmp(fieldName, "Result")==0) return base+15;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NWinfoDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "double",
        "double",
        "double",
        "double",
        "double",
        "simtime_t",
        "double",
        "string",
        "double",
        "int",
        "int",
        "double",
        "int",
    };
    return (field>=0 && field<16) ? fieldTypeStrings[field] : NULL;
}

const char *NWinfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NWinfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NWinfo *pp = (NWinfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NWinfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NWinfo *pp = (NWinfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getIdx());
        case 1: return double2string(pp->getRss());
        case 2: return double2string(pp->getRssthr());
        case 3: return double2string(pp->getSNR());
        case 4: return double2string(pp->getSNRthr());
        case 5: return double2string(pp->getAdDataRate());
        case 6: return double2string(pp->getXusers());
        case 7: return double2string(pp->getMcost());
        case 8: return double2string(pp->getNnDelay());
        case 9: return double2string(pp->getJitter());
        case 10: return oppstring2string(pp->getSSID());
        case 11: return double2string(pp->getSpeed());
        case 12: return long2string(pp->getThrvolume());
        case 13: return long2string(pp->getRxvolume());
        case 14: return double2string(pp->getSrcportno());
        case 15: return long2string(pp->getResult());
        default: return "";
    }
}

bool NWinfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NWinfo *pp = (NWinfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setIdx(string2long(value)); return true;
        case 1: pp->setRss(string2double(value)); return true;
        case 2: pp->setRssthr(string2double(value)); return true;
        case 3: pp->setSNR(string2double(value)); return true;
        case 4: pp->setSNRthr(string2double(value)); return true;
        case 5: pp->setAdDataRate(string2double(value)); return true;
        case 6: pp->setXusers(string2double(value)); return true;
        case 7: pp->setMcost(string2double(value)); return true;
        case 8: pp->setNnDelay(string2double(value)); return true;
        case 9: pp->setJitter(string2double(value)); return true;
        case 10: pp->setSSID((value)); return true;
        case 11: pp->setSpeed(string2double(value)); return true;
        case 12: pp->setThrvolume(string2long(value)); return true;
        case 13: pp->setRxvolume(string2long(value)); return true;
        case 14: pp->setSrcportno(string2double(value)); return true;
        case 15: pp->setResult(string2long(value)); return true;
        default: return false;
    }
}

const char *NWinfoDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *NWinfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NWinfo *pp = (NWinfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(DInquiry);

DInquiry::DInquiry() : ::NWinfo()
{
}

DInquiry::DInquiry(const DInquiry& other) : ::NWinfo(other)
{
    copy(other);
}

DInquiry::~DInquiry()
{
}

DInquiry& DInquiry::operator=(const DInquiry& other)
{
    if (this==&other) return *this;
    ::NWinfo::operator=(other);
    copy(other);
    return *this;
}

void DInquiry::copy(const DInquiry& other)
{
    this->nw1_var = other.nw1_var;
    this->nw2_var = other.nw2_var;
}

void DInquiry::parsimPack(cCommBuffer *b)
{
    ::NWinfo::parsimPack(b);
    doPacking(b,this->nw1_var);
    doPacking(b,this->nw2_var);
}

void DInquiry::parsimUnpack(cCommBuffer *b)
{
    ::NWinfo::parsimUnpack(b);
    doUnpacking(b,this->nw1_var);
    doUnpacking(b,this->nw2_var);
}

NWinfo& DInquiry::getNw1()
{
    return nw1_var;
}

void DInquiry::setNw1(const NWinfo& nw1)
{
    this->nw1_var = nw1;
}

NWinfo& DInquiry::getNw2()
{
    return nw2_var;
}

void DInquiry::setNw2(const NWinfo& nw2)
{
    this->nw2_var = nw2;
}

class DInquiryDescriptor : public cClassDescriptor
{
  public:
    DInquiryDescriptor();
    virtual ~DInquiryDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DInquiryDescriptor);

DInquiryDescriptor::DInquiryDescriptor() : cClassDescriptor("DInquiry", "NWinfo")
{
}

DInquiryDescriptor::~DInquiryDescriptor()
{
}

bool DInquiryDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DInquiry *>(obj)!=NULL;
}

const char *DInquiryDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DInquiryDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int DInquiryDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND | FD_ISCOBJECT,
        FD_ISCOMPOUND | FD_ISCOBJECT,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *DInquiryDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "nw1",
        "nw2",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int DInquiryDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nw1")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nw2")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DInquiryDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "NWinfo",
        "NWinfo",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *DInquiryDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int DInquiryDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DInquiry *pp = (DInquiry *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DInquiryDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DInquiry *pp = (DInquiry *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getNw1(); return out.str();}
        case 1: {std::stringstream out; out << pp->getNw2(); return out.str();}
        default: return "";
    }
}

bool DInquiryDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DInquiry *pp = (DInquiry *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *DInquiryDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(NWinfo));
        case 1: return opp_typename(typeid(NWinfo));
        default: return NULL;
    };
}

void *DInquiryDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DInquiry *pp = (DInquiry *)object; (void)pp;
    switch (field) {
        case 0: return (void *)static_cast<cObject *>(&pp->getNw1()); break;
        case 1: return (void *)static_cast<cObject *>(&pp->getNw2()); break;
        default: return NULL;
    }
}

Register_Class(updateMsg);

updateMsg::updateMsg() : ::DInquiry()
{
    this->kind_var = 0;
}

updateMsg::updateMsg(const updateMsg& other) : ::DInquiry(other)
{
    copy(other);
}

updateMsg::~updateMsg()
{
}

updateMsg& updateMsg::operator=(const updateMsg& other)
{
    if (this==&other) return *this;
    ::DInquiry::operator=(other);
    copy(other);
    return *this;
}

void updateMsg::copy(const updateMsg& other)
{
    this->kind_var = other.kind_var;
}

void updateMsg::parsimPack(cCommBuffer *b)
{
    ::DInquiry::parsimPack(b);
    doPacking(b,this->kind_var);
}

void updateMsg::parsimUnpack(cCommBuffer *b)
{
    ::DInquiry::parsimUnpack(b);
    doUnpacking(b,this->kind_var);
}

int updateMsg::getKind() const
{
    return kind_var;
}

void updateMsg::setKind(int kind)
{
    this->kind_var = kind;
}

class updateMsgDescriptor : public cClassDescriptor
{
  public:
    updateMsgDescriptor();
    virtual ~updateMsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(updateMsgDescriptor);

updateMsgDescriptor::updateMsgDescriptor() : cClassDescriptor("updateMsg", "DInquiry")
{
}

updateMsgDescriptor::~updateMsgDescriptor()
{
}

bool updateMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<updateMsg *>(obj)!=NULL;
}

const char *updateMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int updateMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int updateMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *updateMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "kind",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int updateMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='k' && strcmp(fieldName, "kind")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *updateMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *updateMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int updateMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    updateMsg *pp = (updateMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string updateMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    updateMsg *pp = (updateMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getKind());
        default: return "";
    }
}

bool updateMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    updateMsg *pp = (updateMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setKind(string2long(value)); return true;
        default: return false;
    }
}

const char *updateMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *updateMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    updateMsg *pp = (updateMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


