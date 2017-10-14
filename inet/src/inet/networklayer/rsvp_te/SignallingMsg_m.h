//
// Generated file, do not edit! Created by nedtool 4.6 from inet/networklayer/rsvp_te/SignallingMsg.msg.
//

#ifndef _INET_SIGNALLINGMSG_M_H_
#define _INET_SIGNALLINGMSG_M_H_

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
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
#include "inet/networklayer/rsvp_te/IntServ.h"

#define MSG_PSB_TIMER               1
#define MSG_PSB_TIMEOUT             2

#define MSG_RSB_REFRESH_TIMER       3
#define MSG_RSB_COMMIT_TIMER        4
#define MSG_RSB_TIMEOUT             5

#define MSG_HELLO_TIMER             6
#define MSG_HELLO_TIMEOUT           7

#define MSG_PATH_NOTIFY             8

#define PATH_CREATED                1
#define PATH_UNFEASIBLE             2
#define PATH_FAILED                 3
#define PATH_PREEMPTED              4
#define PATH_RETRY                  5
// }}


namespace inet {

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:51</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message SignallingMsg
 * {
 *     int command = 0;
 * }
 * </pre>
 */
class INET_API SignallingMsg : public ::cMessage
{
  protected:
    int command_var;

  private:
    void copy(const SignallingMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SignallingMsg&);

  public:
    SignallingMsg(const char *name=NULL, int kind=0);
    SignallingMsg(const SignallingMsg& other);
    virtual ~SignallingMsg();
    SignallingMsg& operator=(const SignallingMsg& other);
    virtual SignallingMsg *dup() const {return new SignallingMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, SignallingMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, SignallingMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:59</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message PsbTimerMsg extends SignallingMsg
 * {
 *     int id;
 * 
 *     int command = MSG_PSB_TIMER;
 * }
 * </pre>
 */
class INET_API PsbTimerMsg : public ::inet::SignallingMsg
{
  protected:
    int id_var;
    int command_var;

  private:
    void copy(const PsbTimerMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PsbTimerMsg&);

  public:
    PsbTimerMsg(const char *name=NULL, int kind=0);
    PsbTimerMsg(const PsbTimerMsg& other);
    virtual ~PsbTimerMsg();
    PsbTimerMsg& operator=(const PsbTimerMsg& other);
    virtual PsbTimerMsg *dup() const {return new PsbTimerMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getId() const;
    virtual void setId(int id);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, PsbTimerMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, PsbTimerMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:69</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message PsbTimeoutMsg extends SignallingMsg
 * {
 *     int id;
 * 
 *     int command = MSG_PSB_TIMEOUT;
 * }
 * </pre>
 */
class INET_API PsbTimeoutMsg : public ::inet::SignallingMsg
{
  protected:
    int id_var;
    int command_var;

  private:
    void copy(const PsbTimeoutMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PsbTimeoutMsg&);

  public:
    PsbTimeoutMsg(const char *name=NULL, int kind=0);
    PsbTimeoutMsg(const PsbTimeoutMsg& other);
    virtual ~PsbTimeoutMsg();
    PsbTimeoutMsg& operator=(const PsbTimeoutMsg& other);
    virtual PsbTimeoutMsg *dup() const {return new PsbTimeoutMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getId() const;
    virtual void setId(int id);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, PsbTimeoutMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, PsbTimeoutMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:79</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message RsbRefreshTimerMsg extends SignallingMsg
 * {
 *     int id;
 * 
 *     int command = MSG_RSB_REFRESH_TIMER;
 * }
 * </pre>
 */
class INET_API RsbRefreshTimerMsg : public ::inet::SignallingMsg
{
  protected:
    int id_var;
    int command_var;

  private:
    void copy(const RsbRefreshTimerMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RsbRefreshTimerMsg&);

  public:
    RsbRefreshTimerMsg(const char *name=NULL, int kind=0);
    RsbRefreshTimerMsg(const RsbRefreshTimerMsg& other);
    virtual ~RsbRefreshTimerMsg();
    RsbRefreshTimerMsg& operator=(const RsbRefreshTimerMsg& other);
    virtual RsbRefreshTimerMsg *dup() const {return new RsbRefreshTimerMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getId() const;
    virtual void setId(int id);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, RsbRefreshTimerMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, RsbRefreshTimerMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:89</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message RsbCommitTimerMsg extends SignallingMsg
 * {
 *     int id;
 * 
 *     int command = MSG_RSB_COMMIT_TIMER;
 * }
 * </pre>
 */
class INET_API RsbCommitTimerMsg : public ::inet::SignallingMsg
{
  protected:
    int id_var;
    int command_var;

  private:
    void copy(const RsbCommitTimerMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RsbCommitTimerMsg&);

  public:
    RsbCommitTimerMsg(const char *name=NULL, int kind=0);
    RsbCommitTimerMsg(const RsbCommitTimerMsg& other);
    virtual ~RsbCommitTimerMsg();
    RsbCommitTimerMsg& operator=(const RsbCommitTimerMsg& other);
    virtual RsbCommitTimerMsg *dup() const {return new RsbCommitTimerMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getId() const;
    virtual void setId(int id);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, RsbCommitTimerMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, RsbCommitTimerMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:99</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message RsbTimeoutMsg extends SignallingMsg
 * {
 *     int id;
 * 
 *     int command = MSG_RSB_TIMEOUT;
 * }
 * </pre>
 */
class INET_API RsbTimeoutMsg : public ::inet::SignallingMsg
{
  protected:
    int id_var;
    int command_var;

  private:
    void copy(const RsbTimeoutMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RsbTimeoutMsg&);

  public:
    RsbTimeoutMsg(const char *name=NULL, int kind=0);
    RsbTimeoutMsg(const RsbTimeoutMsg& other);
    virtual ~RsbTimeoutMsg();
    RsbTimeoutMsg& operator=(const RsbTimeoutMsg& other);
    virtual RsbTimeoutMsg *dup() const {return new RsbTimeoutMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getId() const;
    virtual void setId(int id);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, RsbTimeoutMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, RsbTimeoutMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:109</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message HelloTimerMsg extends SignallingMsg
 * {
 *     IPv4Address peer;
 * 
 *     int command = MSG_HELLO_TIMER;
 * }
 * </pre>
 */
class INET_API HelloTimerMsg : public ::inet::SignallingMsg
{
  protected:
    IPv4Address peer_var;
    int command_var;

  private:
    void copy(const HelloTimerMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const HelloTimerMsg&);

  public:
    HelloTimerMsg(const char *name=NULL, int kind=0);
    HelloTimerMsg(const HelloTimerMsg& other);
    virtual ~HelloTimerMsg();
    HelloTimerMsg& operator=(const HelloTimerMsg& other);
    virtual HelloTimerMsg *dup() const {return new HelloTimerMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual IPv4Address& getPeer();
    virtual const IPv4Address& getPeer() const {return const_cast<HelloTimerMsg*>(this)->getPeer();}
    virtual void setPeer(const IPv4Address& peer);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, HelloTimerMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, HelloTimerMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:119</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message HelloTimeoutMsg extends SignallingMsg
 * {
 *     IPv4Address peer;
 * 
 *     int command = MSG_HELLO_TIMEOUT;
 * }
 * </pre>
 */
class INET_API HelloTimeoutMsg : public ::inet::SignallingMsg
{
  protected:
    IPv4Address peer_var;
    int command_var;

  private:
    void copy(const HelloTimeoutMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const HelloTimeoutMsg&);

  public:
    HelloTimeoutMsg(const char *name=NULL, int kind=0);
    HelloTimeoutMsg(const HelloTimeoutMsg& other);
    virtual ~HelloTimeoutMsg();
    HelloTimeoutMsg& operator=(const HelloTimeoutMsg& other);
    virtual HelloTimeoutMsg *dup() const {return new HelloTimeoutMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual IPv4Address& getPeer();
    virtual const IPv4Address& getPeer() const {return const_cast<HelloTimeoutMsg*>(this)->getPeer();}
    virtual void setPeer(const IPv4Address& peer);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, HelloTimeoutMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, HelloTimeoutMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/rsvp_te/SignallingMsg.msg:129</tt> by nedtool.
 * <pre>
 * //
 * // FIXME missing documentation
 * //
 * message PathNotifyMsg extends SignallingMsg
 * {
 *     SessionObj_t session;
 *     SenderTemplateObj_t sender;
 * 
 *     int status;
 * 
 *     int command = MSG_PATH_NOTIFY;
 * }
 * </pre>
 */
class INET_API PathNotifyMsg : public ::inet::SignallingMsg
{
  protected:
    SessionObj_t session_var;
    SenderTemplateObj_t sender_var;
    int status_var;
    int command_var;

  private:
    void copy(const PathNotifyMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PathNotifyMsg&);

  public:
    PathNotifyMsg(const char *name=NULL, int kind=0);
    PathNotifyMsg(const PathNotifyMsg& other);
    virtual ~PathNotifyMsg();
    PathNotifyMsg& operator=(const PathNotifyMsg& other);
    virtual PathNotifyMsg *dup() const {return new PathNotifyMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual SessionObj_t& getSession();
    virtual const SessionObj_t& getSession() const {return const_cast<PathNotifyMsg*>(this)->getSession();}
    virtual void setSession(const SessionObj_t& session);
    virtual SenderTemplateObj_t& getSender();
    virtual const SenderTemplateObj_t& getSender() const {return const_cast<PathNotifyMsg*>(this)->getSender();}
    virtual void setSender(const SenderTemplateObj_t& sender);
    virtual int getStatus() const;
    virtual void setStatus(int status);
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, PathNotifyMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, PathNotifyMsg& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef _INET_SIGNALLINGMSG_M_H_

