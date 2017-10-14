//
// Generated file, do not edit! Created by nedtool 4.6 from inet/routing/extras/batman/BatmanMsg.msg.
//

#ifndef _INET__INETMANET_BATMANMSG_M_H_
#define _INET__INETMANET_BATMANMSG_M_H_

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
#include "inet/networklayer/common/L3Address.h"
#define DIRECTLINK 0x40
#define UNIDIRECTIONAL 0x80

#ifdef __USE_MINHOP__
#define BatPacketSize 17
#else
#define BatPacketSize 16
#endif

#define BATMAN_HNA_MSG_SIZE 5
// }}


namespace inet {
namespace inetmanet {

/**
 * Struct generated from inet/routing/extras/batman/BatmanMsg.msg:42 by nedtool.
 */
struct INET_API vis_data
{
    vis_data();
    uint8_t type;
    uint8_t data;
    L3Address ip;
};

void INET_API doPacking(cCommBuffer *b, vis_data& a);
void INET_API doUnpacking(cCommBuffer *b, vis_data& a);

/**
 * Struct generated from inet/routing/extras/batman/BatmanMsg.msg:49 by nedtool.
 */
struct INET_API HnaElement
{
    HnaElement();
    L3Address addr;
    uint8_t netmask;
};

void INET_API doPacking(cCommBuffer *b, HnaElement& a);
void INET_API doUnpacking(cCommBuffer *b, HnaElement& a);

/**
 * Class generated from <tt>inet/routing/extras/batman/BatmanMsg.msg:55</tt> by nedtool.
 * <pre>
 * packet BatmanPacket
 * {
 *     byteLength = BatPacketSize;
 *     uint8_t version;  // batman version field
 *     uint8_t flags;    // 0x80: UNIDIRECTIONAL link, 0x40: DIRECTLINK flag, ...
 *     uint8_t ttl;
 *     uint8_t gatewayFlags;  // flags related to gateway functions: gateway class
 *     unsigned short seqNumber;
 *     unsigned short gatewayPort;
 *     L3Address orig;
 *     L3Address prevSender;
 *     uint8_t tq;           // Quality
 *     uint8_t hops;
 *     HnaElement hnaMsg[];    ///< Packet body.
 * }
 * </pre>
 */
class INET_API BatmanPacket : public ::cPacket
{
  protected:
    uint8_t version_var;
    uint8_t flags_var;
    uint8_t ttl_var;
    uint8_t gatewayFlags_var;
    unsigned short seqNumber_var;
    unsigned short gatewayPort_var;
    L3Address orig_var;
    L3Address prevSender_var;
    uint8_t tq_var;
    uint8_t hops_var;
    HnaElement *hnaMsg_var; // array ptr
    unsigned int hnaMsg_arraysize;

  private:
    void copy(const BatmanPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BatmanPacket&);

  public:
    BatmanPacket(const char *name=NULL, int kind=0);
    BatmanPacket(const BatmanPacket& other);
    virtual ~BatmanPacket();
    BatmanPacket& operator=(const BatmanPacket& other);
    virtual BatmanPacket *dup() const {return new BatmanPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual uint8_t getVersion() const;
    virtual void setVersion(uint8_t version);
    virtual uint8_t getFlags() const;
    virtual void setFlags(uint8_t flags);
    virtual uint8_t getTtl() const;
    virtual void setTtl(uint8_t ttl);
    virtual uint8_t getGatewayFlags() const;
    virtual void setGatewayFlags(uint8_t gatewayFlags);
    virtual unsigned short getSeqNumber() const;
    virtual void setSeqNumber(unsigned short seqNumber);
    virtual unsigned short getGatewayPort() const;
    virtual void setGatewayPort(unsigned short gatewayPort);
    virtual L3Address& getOrig();
    virtual const L3Address& getOrig() const {return const_cast<BatmanPacket*>(this)->getOrig();}
    virtual void setOrig(const L3Address& orig);
    virtual L3Address& getPrevSender();
    virtual const L3Address& getPrevSender() const {return const_cast<BatmanPacket*>(this)->getPrevSender();}
    virtual void setPrevSender(const L3Address& prevSender);
    virtual uint8_t getTq() const;
    virtual void setTq(uint8_t tq);
    virtual uint8_t getHops() const;
    virtual void setHops(uint8_t hops);
    virtual void setHnaMsgArraySize(unsigned int size);
    virtual unsigned int getHnaMsgArraySize() const;
    virtual HnaElement& getHnaMsg(unsigned int k);
    virtual const HnaElement& getHnaMsg(unsigned int k) const {return const_cast<BatmanPacket*>(this)->getHnaMsg(k);}
    virtual void setHnaMsg(unsigned int k, const HnaElement& hnaMsg);
};

inline void doPacking(cCommBuffer *b, BatmanPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, BatmanPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/routing/extras/batman/BatmanMsg.msg:71</tt> by nedtool.
 * <pre>
 * packet visPacket
 * {
 *     L3Address senderIp;
 *     unsigned char version;
 *     unsigned char gwClass;
 *     unsigned char tqMax;
 *     vis_data visData[];
 * }
 * </pre>
 */
class INET_API visPacket : public ::cPacket
{
  protected:
    L3Address senderIp_var;
    unsigned char version_var;
    unsigned char gwClass_var;
    unsigned char tqMax_var;
    vis_data *visData_var; // array ptr
    unsigned int visData_arraysize;

  private:
    void copy(const visPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const visPacket&);

  public:
    visPacket(const char *name=NULL, int kind=0);
    visPacket(const visPacket& other);
    virtual ~visPacket();
    visPacket& operator=(const visPacket& other);
    virtual visPacket *dup() const {return new visPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual L3Address& getSenderIp();
    virtual const L3Address& getSenderIp() const {return const_cast<visPacket*>(this)->getSenderIp();}
    virtual void setSenderIp(const L3Address& senderIp);
    virtual unsigned char getVersion() const;
    virtual void setVersion(unsigned char version);
    virtual unsigned char getGwClass() const;
    virtual void setGwClass(unsigned char gwClass);
    virtual unsigned char getTqMax() const;
    virtual void setTqMax(unsigned char tqMax);
    virtual void setVisDataArraySize(unsigned int size);
    virtual unsigned int getVisDataArraySize() const;
    virtual vis_data& getVisData(unsigned int k);
    virtual const vis_data& getVisData(unsigned int k) const {return const_cast<visPacket*>(this)->getVisData(k);}
    virtual void setVisData(unsigned int k, const vis_data& visData);
};

inline void doPacking(cCommBuffer *b, visPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, visPacket& obj) {obj.parsimUnpack(b);}

} // namespace inetmanet
} // namespace inet

#endif // ifndef _INET__INETMANET_BATMANMSG_M_H_

