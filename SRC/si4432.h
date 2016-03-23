#ifndef __SI4432_H__
#define __SI4432_H__

#include "bsp.h"

typedef enum{
  IPOR      = 0x0001,
  ICHIPRDY  = 0x0002,
  ILBD      = 0x0004,
  IWUT      = 0x0008,
  IRSSI     = 0x0010,
  IPREAINVAL= 0x0020,
  IPREAVAL  = 0x0040,
  ISWDET    = 0x0080,
  ICRCERR   = 0x0100,
  IPKVALID  = 0x0200,
  IPKSENT   = 0x0400,
  IEXT      = 0x0800,
  IRXFFAFULL= 0x1000,
  ITXFFAEM  = 0x2000,
  ITXFFAFLL = 0x4000,
  IFFERR    = 0x8000
} eSI4432_INT_STATE;

typedef enum{
  ENPOR      = 0x0001,
  ENCHIPRDY  = 0x0002,
  ENLBD      = 0x0004,
  ENWUT      = 0x0008,
  ENRSSI     = 0x0010,
  ENPREAINVAL= 0x0020,
  ENPREAVAL  = 0x0040,
  ENSWDET    = 0x0080,
  ENCRCERR   = 0x0100,
  ENPKVALID  = 0x0200,
  ENPKSENT   = 0x0400,
  ENEXT      = 0x0800,
  ENRXFFAFULL= 0x1000,
  ENTXFFAEM  = 0x2000,
  ENTXFFAFLL = 0x4000,
  ENFFERR    = 0x8000
} eSI4432_INT_EN;

#define DEVICE_TYPE     0x00
#define DEVICE_VERSION  0x01
#define DEVICE_STATUS   0x02
#define INT_STATUS      0x03
#define INT_ENALBE      0x05


void si4432_init(void);\
void set_channel(uint8_t ch);
void set_header(uint8_t header);
void header_enable(void);
void header_disable(void);
void broadcast_enable(void);
uint8_t get_header(void);
void si4432_tx(uint8_t *buf, uint8_t len);
void si4432_rx(void);
uint8_t si4432_rcv(uint8_t *buf);
void rf_isr(void);
void txfae_isr(void);
void rxfaf_isr(void);


#endif
