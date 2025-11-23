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
void IAdpcmStop(ADPCM_CMD* acp);
void IAdpcmPreLoad(ADPCM_CMD* acp);
void IaSetWrkVolPanPitch(u_char channel, u_short pan, u_short master_vol, u_short pitch);
void IaSetWrkFadeParam(u_char channel, int fade_flm, u_short target_vol);
void IaSetWrkFadeMode(u_char channel, u_char mode, u_short target_vol, int fade_flm);
void IaSetRegVol(u_char channel);
void DbgDispAdpcmCmdWrk(ADPCM_CMD* acp);
void IaDbgMemoryCheck();
void IaSetRegKoff(u_char channel);
void IaInitEffect();
void IaInitVolume();
void IaInitDev(u_char channel);
void IaSetRegPitch(u_char channel);
void IaSetRegAdsr(u_char channel);
void IaSetRegSsa(u_char channel);
void IaSetRegKon(u_char channel);

void IAdpcmReadCh0();
void IAdpcmReadCh1();

void IaSetWrkFadeInit(u_char channel);
void IaSetWrkFadeParam(u_char channel, int fade_flm, u_short target_vol);
void IaSetMasterVol(u_short mvol);

u_char IAdpcmChkCmdExist();
void InitAdpcmCmdBuf();
void IAdpcmCmdSlide();
void IAdpcmCmdPlay();
void IAdpcmCmdStop();
void IAdpcmCmdPreLoad();
void IAdpcmCmdPause();
void IAdpcmCmdRestart();

#endif // IOPADPCM_H_
