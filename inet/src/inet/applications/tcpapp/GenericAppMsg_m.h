//
// Generated file, do not edit! Created by nedtool 4.6 from inet/applications/tcpapp/GenericAppMsg.msg.
//

#ifndef _INET_GENERICAPPMSG_M_H_
#define _INET_GENERICAPPMSG_M_H_

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
 * Class generated from <tt>inet/applications/tcpapp/GenericAppMsg.msg:37</tt> by nedtool.
 * <pre>
 * //
 * // Message class for generic request-reply style applications.
 * // The client sends a ~GenericAppMsg which contains the number of
 * // bytes the client wants the server to send back as reply.
 * //
 * // This way intelligence (behaviour specific to the modelled application,
 * // e.g. HTTP, SMB, database protocol) needs only to be present in
 * // the client, and the server model can be kept simple and dumb.
 * //
 * // @see ~TCPGenericSrvApp, TCPGenericCliAppBase (C++ only)
 * //
 * packet GenericAppMsg
 * {
 *     int expectedReplyLength; // in bytes
 *     double replyDelay;       // reply after this many seconds
 *     bool serverClose;        // with TCP: if true, server should close the
 *                              // connection after sending the reply
 * }
 * </pre>
 */
class INET_API GenericAppMsg : public ::cPacket
{
  protected:
    int expectedReplyLength_var;
    double replyDelay_var;
    bool serverClose_var;

  private:
    void copy(const GenericAppMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GenericAppMsg&);

  public:
    GenericAppMsg(const char *name=NULL, int kind=0);
    GenericAppMsg(const GenericAppMsg& other);
    virtual ~GenericAppMsg();
    GenericAppMsg& operator=(const GenericAppMsg& other);
    virtual GenericAppMsg *dup() const {return new GenericAppMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getExpectedReplyLength() const;
    virtual void setExpectedReplyLength(int expectedReplyLength);
    virtual double getReplyDelay() const;
    virtual void setReplyDelay(double replyDelay);
    virtual bool getServerClose() const;
    virtual void setServerClose(bool serverClose);
};

inline void doPacking(cCommBuffer *b, GenericAppMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GenericAppMsg& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef _INET_GENERICAPPMSG_M_H_

