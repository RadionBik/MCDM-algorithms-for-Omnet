//
// Generated file, do not edit! Created by nedtool 4.6 from inet/networklayer/rsvp_te/IntServ.msg.
//

#ifndef _INET_INTSERV_M_H_
#define _INET_INTSERV_M_H_

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
// }}


namespace inet {

// cplusplus {{
typedef std::vector<IPv4Address> IPAddressVector;
typedef std::vector<struct EroObj_t> EroVector;
typedef std::vector<struct FlowDescriptor_t> FlowDescriptorVector;
// }}

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:44 by nedtool.
 */
struct INET_API SessionObj_t
{
    SessionObj_t();
    int Tunnel_Id;
    int Extended_Tunnel_Id;
    IPv4Address DestAddress;
    int setupPri;
    int holdingPri;
};

void INET_API doPacking(cCommBuffer *b, SessionObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, SessionObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:56 by nedtool.
 */
struct INET_API RsvpHopObj_t
{
    RsvpHopObj_t();
    IPv4Address Next_Hop_Address;
    IPv4Address Logical_Interface_Handle;
};

void INET_API doPacking(cCommBuffer *b, RsvpHopObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, RsvpHopObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:65 by nedtool.
 */
struct INET_API SenderTemplateObj_t
{
    SenderTemplateObj_t();
    IPv4Address SrcAddress;
    int Lsp_Id;
};

void INET_API doPacking(cCommBuffer *b, SenderTemplateObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, SenderTemplateObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:75 by nedtool.
 */
struct INET_API SenderTspecObj_t
{
    SenderTspecObj_t();
    double req_bandwidth;
};

void INET_API doPacking(cCommBuffer *b, SenderTspecObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, SenderTspecObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:83 by nedtool.
 */
struct INET_API FlowSpecObj_t : public ::inet::SenderTspecObj_t
{
    FlowSpecObj_t();
};

void INET_API doPacking(cCommBuffer *b, FlowSpecObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, FlowSpecObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:90 by nedtool.
 */
struct INET_API FilterSpecObj_t : public ::inet::SenderTemplateObj_t
{
    FilterSpecObj_t();
};

void INET_API doPacking(cCommBuffer *b, FilterSpecObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, FilterSpecObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:97 by nedtool.
 */
struct INET_API LabelRequestObj_t
{
    LabelRequestObj_t();
    int prot;
};

void INET_API doPacking(cCommBuffer *b, LabelRequestObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, LabelRequestObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:106 by nedtool.
 */
struct INET_API SenderDescriptor_t
{
    SenderDescriptor_t();
    SenderTemplateObj_t Sender_Template_Object;
    SenderTspecObj_t Sender_Tspec_Object;
};

void INET_API doPacking(cCommBuffer *b, SenderDescriptor_t& a);
void INET_API doUnpacking(cCommBuffer *b, SenderDescriptor_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:115 by nedtool.
 */
struct INET_API EroObj_t
{
    EroObj_t();
    bool L;
    IPv4Address node;
};

void INET_API doPacking(cCommBuffer *b, EroObj_t& a);
void INET_API doUnpacking(cCommBuffer *b, EroObj_t& a);

/**
 * Struct generated from inet/networklayer/rsvp_te/IntServ.msg:124 by nedtool.
 */
struct INET_API FlowDescriptor_t
{
    FlowDescriptor_t();
    FlowSpecObj_t Flowspec_Object;
    FilterSpecObj_t Filter_Spec_Object;
    IPAddressVector RRO;
    int label;
};

void INET_API doPacking(cCommBuffer *b, FlowDescriptor_t& a);
void INET_API doUnpacking(cCommBuffer *b, FlowDescriptor_t& a);

} // namespace inet

#endif // ifndef _INET_INTSERV_M_H_

