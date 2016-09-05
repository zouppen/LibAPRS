#ifndef PROTOCOL_AX25_H
#define PROTOCOL_AX25_H

#include <stdio.h>
#include <stdbool.h>
#include "device.h"

#ifndef CUSTOM_FRAME_SIZE
    #define AX25_MAX_FRAME_LEN 330
#else
    #define AX25_MAX_FRAME_LEN CUSTOM_FRAME_SIZE
#endif

#define AX25_CTRL_UI      0x03
#define AX25_PID_NOLAYER3 0xF0

struct AX25Ctx;     // Forward declarations

typedef struct AX25Ctx {
    uint16_t crc_out;
} AX25Ctx;


#define AX25_MAX_RPT 8

#define CALL_OVERSPACE 1

typedef struct AX25Call {
    char call[6+CALL_OVERSPACE];
    //char STRING_TERMINATION = 0;
    uint8_t ssid;
} AX25Call;

void ax25_sendVia(AX25Ctx *ctx, const AX25Call *path, size_t path_len, const void *_buf, size_t len);
#define ax25_send(ctx, dst, src, buf, len) ax25_sendVia(ctx, ({static AX25Call __path[]={dst, src}; __path;}), 2, buf, len)


void ax25_sendRaw(AX25Ctx *ctx, void *_buf, size_t len);
void ax25_init(AX25Ctx *ctx);

#endif
