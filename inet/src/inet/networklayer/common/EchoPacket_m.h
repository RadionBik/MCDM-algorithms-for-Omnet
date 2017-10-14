//
// Generated file, do not edit! Created by nedtool 4.6 from inet/networklayer/common/EchoPacket.msg.
//

#ifndef _INET_ECHOPACKET_M_H_
#define _INET_ECHOPACKET_M_H_

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
#include "inet/common/INETDefs.h"
// }}


namespace inet {

/**
 * Enum generated from <tt>inet/networklayer/common/EchoPacket.msg:28</tt> by nedtool.
 * <pre>
 * //
 * // Determines the packet type.
 * //
 * enum EchoProtocolType
 * {
 * 
 *     ECHO_PROTOCOL_REQUEST = 0;
 *     ECHO_PROTOCOL_REPLY = 1;
 * }
 * </pre>
 */
enum EchoProtocolType {
    ECHO_PROTOCOL_REQUEST = 0,
    ECHO_PROTOCOL_REPLY = 1
};

/**
 * Class generated from <tt>inet/networklayer/common/EchoPacket.msg:37</tt> by nedtool.
 * <pre>
 * //
 * // Generic network protocol packet for testing network connectivity.
 * //
 * packet EchoPacket
 * {
 *     int type @enum(EchoProtocolType);
 * }
 * </pre>
 */
class INET_API EchoPacket : public ::cPacket
{
  protected:
    int type_var;

  private:
    void copy(const EchoPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const EchoPacket&);

  public:
    EchoPacket(const char *name=NULL, int kind=0);
    EchoPacket(const EchoPacket& other);
    virtual ~EchoPacket();
    EchoPacket& operator=(const EchoPacket& other);
    virtual EchoPacket *dup() const {return new EchoPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getType() const;
    virtual void setType(int type);
};

inline void doPacking(cCommBuffer *b, EchoPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, EchoPacket& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef _INET_ECHOPACKET_M_H_

