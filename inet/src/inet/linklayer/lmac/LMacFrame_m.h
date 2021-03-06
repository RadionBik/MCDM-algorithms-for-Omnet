//
// Generated file, do not edit! Created by nedtool 4.6 from inet/linklayer/lmac/LMacFrame.msg.
//

#ifndef _INET_LMACFRAME_M_H_
#define _INET_LMACFRAME_M_H_

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
    #include "inet/linklayer/base/MACFrameBase_m.h"
// }}


namespace inet {

/**
 * Class generated from <tt>inet/linklayer/lmac/LMacFrame.msg:15</tt> by nedtool.
 * <pre>
 * packet LMacFrame extends MACFrameBase
 * {
 *     int mySlot; // the node's current slot number
 *     MACAddress occupiedSlots[]; // currently known occupied slots
 * }
 * </pre>
 */
class INET_API LMacFrame : public ::inet::MACFrameBase
{
  protected:
    int mySlot_var;
    MACAddress *occupiedSlots_var; // array ptr
    unsigned int occupiedSlots_arraysize;

  private:
    void copy(const LMacFrame& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const LMacFrame&);

  public:
    LMacFrame(const char *name=NULL, int kind=0);
    LMacFrame(const LMacFrame& other);
    virtual ~LMacFrame();
    LMacFrame& operator=(const LMacFrame& other);
    virtual LMacFrame *dup() const {return new LMacFrame(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getMySlot() const;
    virtual void setMySlot(int mySlot);
    virtual void setOccupiedSlotsArraySize(unsigned int size);
    virtual unsigned int getOccupiedSlotsArraySize() const;
    virtual MACAddress& getOccupiedSlots(unsigned int k);
    virtual const MACAddress& getOccupiedSlots(unsigned int k) const {return const_cast<LMacFrame*>(this)->getOccupiedSlots(k);}
    virtual void setOccupiedSlots(unsigned int k, const MACAddress& occupiedSlots);
};

inline void doPacking(cCommBuffer *b, LMacFrame& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, LMacFrame& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef _INET_LMACFRAME_M_H_

