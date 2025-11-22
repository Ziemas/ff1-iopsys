#ifndef IOPADPCM_H_
#define IOPADPCM_H_

#include "common.h"

extern IOP_ADPCM iop_adpcm[2];
extern u_char* AdpcmIopBuf[2];
extern u_char* AdpcmSpuBuf[2];
extern ADPCM_CMD now_cmd;
extern ADPCM_CMD cmd_buf[8];

void SetLoopFlgSize(u_int size_byte, u_int* start, u_short core);
void IAdpcmLoadEndStream(int channel);
void IAdpcmLoadEndPreOnly(int channel);

void IAdpcmPlay(ADPCM_CMD* acp);
void IAdpcmStop(ADPCM_CMD *acp);
void IAdpcmPreLoad(ADPCM_CMD *acp);
void IaSetWrkVolPanPitch(u_char channel, u_short pan, u_short master_vol, u_short pitch);
void IaSetWrkFadeParam(u_char channel, int fade_flm, u_short target_vol);
void IaSetWrkFadeMode(u_char channel, u_char mode, u_short target_vol, int fade_flm);
void DbgDispAdpcmCmdWrk(ADPCM_CMD* acp);

void IAdpcmReadCh0();
void IAdpcmReadCh1();

#endif // IOPADPCM_H_
