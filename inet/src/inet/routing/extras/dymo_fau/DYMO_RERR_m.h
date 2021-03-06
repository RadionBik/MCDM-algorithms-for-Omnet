//
// Generated file, do not edit! Created by nedtool 4.6 from inet/routing/extras/dymo_fau/DYMO_RERR.msg.
//

#ifndef _INET__INETMANET_DYMO_RERR_M_H_
#define _INET__INETMANET_DYMO_RERR_M_H_

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
#include "inet/routing/extras/dymo_fau/DYMO_PacketBBMessage_m.h"
#include "inet/routing/extras/dymo_fau/DYMO_AddressBlock.h"

#include <vector>
#include <sys/types.h>
#include "inet/routing/extras/dymo_fau/InspectorHelpers.h"
// }}


namespace inet {
namespace inetmanet {

// cplusplus {{
typedef std::vector<DYMO_AddressBlock> VectorOfDYMOAddressBlocks;
// }}

/**
 * Class generated from <tt>inet/routing/extras/dymo_fau/DYMO_RERR.msg:44</tt> by nedtool.
 * <pre>
 * message DYMO_RERR extends DYMO_PacketBBMessage
 * {
 *     VectorOfDYMOAddressBlocks unreachableNodes;
 * }
 * </pre>
 */
class INET_API DYMO_RERR : public ::inet::inetmanet::DYMO_PacketBBMessage
{
  protected:
    VectorOfDYMOAddressBlocks unreachableNodes_var;

  private:
    void copy(const DYMO_RERR& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DYMO_RERR&);

  public:
    DYMO_RERR(const char *name=NULL, int kind=0);
    DYMO_RERR(const DYMO_RERR& other);
    virtual ~DYMO_RERR();
    DYMO_RERR& operator=(const DYMO_RERR& other);
    virtual DYMO_RERR *dup() const {return new DYMO_RERR(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual VectorOfDYMOAddressBlocks& getUnreachableNodes();
    virtual const VectorOfDYMOAddressBlocks& getUnreachableNodes() const {return const_cast<DYMO_RERR*>(this)->getUnreachableNodes();}
    virtual void setUnreachableNodes(const VectorOfDYMOAddressBlocks& unreachableNodes);
};

inline void doPacking(cCommBuffer *b, DYMO_RERR& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, DYMO_RERR& obj) {obj.parsimUnpack(b);}

} // namespace inetmanet
} // namespace inet

#endif // ifndef _INET__INETMANET_DYMO_RERR_M_H_

