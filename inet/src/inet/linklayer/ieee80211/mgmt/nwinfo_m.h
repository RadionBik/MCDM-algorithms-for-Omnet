//
// Generated file, do not edit! Created by nedtool 4.6 from inet/linklayer/ieee80211/mgmt/nwinfo.msg.
//

#ifndef _NWINFO_M_H_
#define _NWINFO_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// dll export symbol
#ifndef INET_API
#  if defined(INET_EXPORT)
#    define INET_API  OPP_DLLEXPORT
#  elif defined(INET_IMPORT)
#    define INET_API  OPP_DLLIMPORT
#  else
#    define INET_API
#  endif
#endif



// cplusplus {{
//#include "inet/linklayer/ieee80211/mgmt/Ieee80211Primitives_m.h"
// }}

/**
 * Class generated from <tt>inet/linklayer/ieee80211/mgmt/nwinfo.msg:27</tt> by nedtool.
 * <pre>
 * // TODO generated message class
 * //
 * class NWinfo //extends Ieee80211Primitives 
 * {
 *     int idx;
 *     double rss;
 *     double rssthr;
 *     double SNR;
 *     double SNRthr;
 *     double AdDataRate;
 *     //double AdPckLossRate;
 *     double xusers;
 *     double mcost;
 *     simtime_t nnDelay;
 *     double jitter;
 *     string SSID;
 *     double speed;
 *     int Thrvolume;
 *     int Rxvolume;
 *     double srcportno = 1025;//TODO should be collected from running app
 *     int Result; // used to pass the result to a response
 * }
 * </pre>
 */
class INET_API NWinfo : public ::cObject
{
  protected:
    int idx_var;
    double rss_var;
    double rssthr_var;
    double SNR_var;
    double SNRthr_var;
    double AdDataRate_var;
    double xusers_var;
    double mcost_var;
    simtime_t nnDelay_var;
    double jitter_var;
    opp_string SSID_var;
    double speed_var;
    int Thrvolume_var;
    int Rxvolume_var;
    double srcportno_var;
    int Result_var;

  private:
    void copy(const NWinfo& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NWinfo&);

  public:
    NWinfo();
    NWinfo(const NWinfo& other);
    virtual ~NWinfo();
    NWinfo& operator=(const NWinfo& other);
    virtual NWinfo *dup() const {return new NWinfo(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getIdx() const;
    virtual void setIdx(int idx);
    virtual double getRss() const;
    virtual void setRss(double rss);
    virtual double getRssthr() const;
    virtual void setRssthr(double rssthr);
    virtual double getSNR() const;
    virtual void setSNR(double SNR);
    virtual double getSNRthr() const;
    virtual void setSNRthr(double SNRthr);
    virtual double getAdDataRate() const;
    virtual void setAdDataRate(double AdDataRate);
    virtual double getXusers() const;
    virtual void setXusers(double xusers);
    virtual double getMcost() const;
    virtual void setMcost(double mcost);
    virtual simtime_t getNnDelay() const;
    virtual void setNnDelay(simtime_t nnDelay);
    virtual double getJitter() const;
    virtual void setJitter(double jitter);
    virtual const char * getSSID() const;
    virtual void setSSID(const char * SSID);
    virtual double getSpeed() const;
    virtual void setSpeed(double speed);
    virtual int getThrvolume() const;
    virtual void setThrvolume(int Thrvolume);
    virtual int getRxvolume() const;
    virtual void setRxvolume(int Rxvolume);
    virtual double getSrcportno() const;
    virtual void setSrcportno(double srcportno);
    virtual int getResult() const;
    virtual void setResult(int Result);
};

inline void doPacking(cCommBuffer *b, NWinfo& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, NWinfo& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/linklayer/ieee80211/mgmt/nwinfo.msg:48</tt> by nedtool.
 * <pre>
 * message DInquiry extends NWinfo   //// to be sent to the decider
 * {
 *     NWinfo nw1;
 *     NWinfo nw2;
 * }
 * </pre>
 */
class INET_API DInquiry : public ::NWinfo
{
  protected:
    NWinfo nw1_var;
    NWinfo nw2_var;

  private:
    void copy(const DInquiry& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DInquiry&);

  public:
    DInquiry();
    DInquiry(const DInquiry& other);
    virtual ~DInquiry();
    DInquiry& operator=(const DInquiry& other);
    virtual DInquiry *dup() const {return new DInquiry(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual NWinfo& getNw1();
    virtual const NWinfo& getNw1() const {return const_cast<DInquiry*>(this)->getNw1();}
    virtual void setNw1(const NWinfo& nw1);
    virtual NWinfo& getNw2();
    virtual const NWinfo& getNw2() const {return const_cast<DInquiry*>(this)->getNw2();}
    virtual void setNw2(const NWinfo& nw2);
};

inline void doPacking(cCommBuffer *b, DInquiry& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, DInquiry& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/linklayer/ieee80211/mgmt/nwinfo.msg:54</tt> by nedtool.
 * <pre>
 * message updateMsg extends DInquiry
 * {
 *     int kind;
 * 
 * }
 * </pre>
 */
class INET_API updateMsg : public ::DInquiry
{
  protected:
    int kind_var;

  private:
    void copy(const updateMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const updateMsg&);

  public:
    updateMsg();
    updateMsg(const updateMsg& other);
    virtual ~updateMsg();
    updateMsg& operator=(const updateMsg& other);
    virtual updateMsg *dup() const {return new updateMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getKind() const;
    virtual void setKind(int kind);
};

inline void doPacking(cCommBuffer *b, updateMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, updateMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef _NWINFO_M_H_

