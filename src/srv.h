#ifndef PORT_H
#define PORT_H

#include "common.h"
#include "kernel.h"

typedef struct _3DS HLE3DS;

typedef union {
    u32 w;
    struct {
        u32 paramsize_translate : 6;
        u32 paramsize_normal : 6;
        u32 unused : 4;
        u32 command : 16;
    };
} IPCHeader;

#define IPCHDR(normal, translate)                                              \
    ((IPCHeader){.paramsize_normal = normal, .paramsize_translate = translate} \
         .w)

typedef void (*PortRequestHandler)(HLE3DS* s, IPCHeader cmd, u32 cmd_addr);
typedef void (*PortRequestHandlerArg)(HLE3DS* s, IPCHeader cmd, u32 cmd_addr, u32 arg);

typedef struct {
    KObject hdr;

    PortRequestHandlerArg handler;
    u32 arg;
} KSession;

void init_services(HLE3DS* s);

KSession* session_create(PortRequestHandler f);
KSession* session_create_arg(PortRequestHandlerArg f, u32 arg);

#define DECL_PORT(name)                                                        \
    void port_handle_##name(HLE3DS* s, IPCHeader cmd, u32 cmd_addr)

#define DECL_PORT_ARG(name, arg)                                                        \
    void port_handle_##name(HLE3DS* s, IPCHeader cmd, u32 cmd_addr, u32 arg)

u32 srvobj_make_handle(HLE3DS* s, KObject* o);

DECL_PORT(srv);

DECL_PORT(errf);

#endif
