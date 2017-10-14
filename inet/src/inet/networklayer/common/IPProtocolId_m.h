//
// Generated file, do not edit! Created by nedtool 4.6 from inet/networklayer/common/IPProtocolId.msg.
//

#ifndef _INET_IPPROTOCOLID_M_H_
#define _INET_IPPROTOCOLID_M_H_

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
 * Enum generated from <tt>inet/networklayer/common/IPProtocolId.msg:31</tt> by nedtool.
 * <pre>
 * //
 * // Protocol field: taken from RFC 1700.
 * // Replacing old RFC1700 with newer (28/06/01) Protocol Numbers
 * //
 * enum IPProtocolId
 * {
 * 
 *     //  IP_PROT_HOPOPT    = 0;
 *     IP_PROT_ICMP = 1;
 *     IP_PROT_IGMP = 2;
 *     IP_PROT_IP = 4; // used for IP tunneling. FIXME: RFC1700 specifies =3 (old IPSuite) ???
 *     IP_PROT_TCP = 6;
 *     IP_PROT_EGP = 8;
 *     IP_PROT_IGP = 9;
 *     IP_PROT_UDP = 17;
 *     IP_PROT_XTP = 36;
 *     IP_PROT_IPv6 = 41;
 *     IP_PROT_RSVP = 46;
 *     IP_PROT_IPv6_ICMP = 58;
 *     IP_PROT_NONE = 59;
 *     IP_PROT_OSPF = 89;
 *     IP_PROT_PIM = 103;
 *     IP_PROT_SCTP = 132;
 *     IP_PROT_DSR = 48;
 *     IP_PROT_MANET = 138;
 * 
 *     IP_PROT_IPv6EXT_HOP = 0;       // Hop-by-hop option header.
 *     IP_PROT_IPv6EXT_DEST = 60;     // Destination options header.
 *     IP_PROT_IPv6EXT_ROUTING = 43;  // Routing header.
 *     IP_PROT_IPv6EXT_FRAGMENT = 44; // Fragmentation/reassembly header.
 *     IP_PROT_IPv6EXT_AUTH = 51;     // Authentication header.
 *     IP_PROT_IPv6EXT_ESP = 50;      // Encapsulating security payload.
 *     IP_PROT_IPv6EXT_MOB = 135;      // Mobility Header for MIPv6 support.
 * }
 * </pre>
 */
enum IPProtocolId {
    IP_PROT_ICMP = 1,
    IP_PROT_IGMP = 2,
    IP_PROT_IP = 4,
    IP_PROT_TCP = 6,
    IP_PROT_EGP = 8,
    IP_PROT_IGP = 9,
    IP_PROT_UDP = 17,
    IP_PROT_XTP = 36,
    IP_PROT_IPv6 = 41,
    IP_PROT_RSVP = 46,
    IP_PROT_IPv6_ICMP = 58,
    IP_PROT_NONE = 59,
    IP_PROT_OSPF = 89,
    IP_PROT_PIM = 103,
    IP_PROT_SCTP = 132,
    IP_PROT_DSR = 48,
    IP_PROT_MANET = 138,
    IP_PROT_IPv6EXT_HOP = 0,
    IP_PROT_IPv6EXT_DEST = 60,
    IP_PROT_IPv6EXT_ROUTING = 43,
    IP_PROT_IPv6EXT_FRAGMENT = 44,
    IP_PROT_IPv6EXT_AUTH = 51,
    IP_PROT_IPv6EXT_ESP = 50,
    IP_PROT_IPv6EXT_MOB = 135
};

/**
 * Class generated from <tt>inet/networklayer/common/IPProtocolId.msg:62</tt> by nedtool.
 * <pre>
 * class IPRegisterProtocolCommand
 * {
 *     int protocol @enum(IPProtocolId);
 * }
 * </pre>
 */
class INET_API IPRegisterProtocolCommand : public ::cObject
{
  protected:
    int protocol_var;

  private:
    void copy(const IPRegisterProtocolCommand& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const IPRegisterProtocolCommand&);

  public:
    IPRegisterProtocolCommand();
    IPRegisterProtocolCommand(const IPRegisterProtocolCommand& other);
    virtual ~IPRegisterProtocolCommand();
    IPRegisterProtocolCommand& operator=(const IPRegisterProtocolCommand& other);
    virtual IPRegisterProtocolCommand *dup() const {return new IPRegisterProtocolCommand(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getProtocol() const;
    virtual void setProtocol(int protocol);
};

inline void doPacking(cCommBuffer *b, IPRegisterProtocolCommand& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, IPRegisterProtocolCommand& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef _INET_IPPROTOCOLID_M_H_

