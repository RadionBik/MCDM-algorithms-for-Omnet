//
// Generated file, do not edit! Created by nedtool 4.6 from inet/applications/netperfmeter/NetPerfMeter.msg.
//

#ifndef _INET_NETPERFMETER_M_H_
#define _INET_NETPERFMETER_M_H_

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
#include "inet/transportlayer/contract/sctp/SCTPCommand_m.h"
// }}


namespace inet {

/**
 * Class generated from <tt>inet/applications/netperfmeter/NetPerfMeter.msg:38</tt> by nedtool.
 * <pre>
 * message NetPerfMeterTransmitTimer
 * {
 *     unsigned int streamID;
 * }
 * </pre>
 */
class INET_API NetPerfMeterTransmitTimer : public ::cMessage
{
  protected:
    unsigned int streamID_var;

  private:
    void copy(const NetPerfMeterTransmitTimer& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NetPerfMeterTransmitTimer&);

  public:
    NetPerfMeterTransmitTimer(const char *name=NULL, int kind=0);
    NetPerfMeterTransmitTimer(const NetPerfMeterTransmitTimer& other);
    virtual ~NetPerfMeterTransmitTimer();
    NetPerfMeterTransmitTimer& operator=(const NetPerfMeterTransmitTimer& other);
    virtual NetPerfMeterTransmitTimer *dup() const {return new NetPerfMeterTransmitTimer(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual unsigned int getStreamID() const;
    virtual void setStreamID(unsigned int streamID);
};

inline void doPacking(cCommBuffer *b, NetPerfMeterTransmitTimer& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, NetPerfMeterTransmitTimer& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/applications/netperfmeter/NetPerfMeter.msg:44</tt> by nedtool.
 * <pre>
 * message NetPerfMeterDataMessage extends SCTPSimpleMessage
 * {
 * }
 * </pre>
 */
class INET_API NetPerfMeterDataMessage : public ::inet::SCTPSimpleMessage
{
  protected:

  private:
    void copy(const NetPerfMeterDataMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NetPerfMeterDataMessage&);

  public:
    NetPerfMeterDataMessage();
    NetPerfMeterDataMessage(const NetPerfMeterDataMessage& other);
    virtual ~NetPerfMeterDataMessage();
    NetPerfMeterDataMessage& operator=(const NetPerfMeterDataMessage& other);
    virtual NetPerfMeterDataMessage *dup() const {return new NetPerfMeterDataMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, NetPerfMeterDataMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, NetPerfMeterDataMessage& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef _INET_NETPERFMETER_M_H_

