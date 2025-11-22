#include "common.h"

#include "adpcm/iopadpcm.h"

#include "libsd.h"
#include "memory.h"
#include "sysmem.h"
#include "thread.h"

void GetPosCalc(ADPCM_POS_CALC* calcp);

static int IAdpcmMakeThread(u_char channel);

void IaInitDev(u_char channel)
{
    memset(&iop_adpcm[channel], 0, sizeof(IOP_ADPCM));

    while (IAdpcmMakeThread(channel))
        ;

    AdpcmIopBuf[channel] = (u_char*)AllocSysMemory(0, 266240, 0);
    if (!AdpcmIopBuf[channel]) {
        while (1)
            ;
    }

    if (channel == 0) {
        AdpcmSpuBuf[0] = (u_char*)0x1F3740;
        iop_adpcm[0].core = 0;
        iop_adpcm[0].vl = 0;
        iop_adpcm[0].vr = 2;
    } else {
        AdpcmSpuBuf[1] = (u_char*)0x1F6780;
        iop_adpcm[1].core = 1;
        iop_adpcm[1].vl = 44;
        iop_adpcm[1].vr = 46;
    }
}

static int IAdpcmMakeThread(u_char channel)
{
    struct ThreadParam param;

    param.attr = 0x2000000;

    if (channel == 0)
        param.entry = IAdpcmReadCh0;
    else
        param.entry = IAdpcmReadCh1;

    param.initPriority = 31;
    param.stackSize = 2048;
    param.option = 0;
    iop_adpcm[channel].thread_id = CreateThread(&param);
    if (iop_adpcm[channel].thread_id > 0) {
        StartThread(iop_adpcm[channel].thread_id, 0);
        return 0;
    } else {
        return 1;
    }
}

void IaInitEffect()
{
    sceSdSetCoreAttr(SD_C_EFFECT_ENABLE | SD_CORE_0, 0);
}

void IaInitVolume()
{
    sceSdSetParam(SD_P_MVOLL | SD_CORE_0, iop_mv.vol);
    sceSdSetParam(SD_P_MVOLR | SD_CORE_0, iop_mv.vol);
    sceSdSetSwitch(SD_S_VMIXL | SD_CORE_0, 0xFFFFFFu);
    sceSdSetSwitch(SD_S_VMIXR | SD_CORE_0, 0xFFFFFFu);
    sceSdSetSwitch(SD_S_VMIXEL | SD_CORE_0, 0);
    sceSdSetSwitch(SD_S_VMIXER | SD_CORE_0, 0);
    sceSdSetParam(SD_VP_VOLL | SD_VOICE(0) | SD_CORE_0, 0);
    sceSdSetParam(SD_VP_VOLR | SD_VOICE(0) | SD_CORE_0, 0);
    sceSdSetParam(SD_VP_VOLL | SD_VOICE(1) | SD_CORE_0, 0);
    sceSdSetParam(SD_VP_VOLR | SD_VOICE(1) | SD_CORE_0, 0);
}

void IaDbgMemoryCheck()
{
    int tmp;

    tmp = QueryMemSize();
    tmp = QueryTotalFreeMemSize();
    tmp = QueryMaxFreeMemSize();
}

void IaSetRegSsa(u_char channel)
{
    sceSdSetAddr(iop_adpcm[channel].core | iop_adpcm[channel].vl | SD_VA_SSA, (u_int)AdpcmSpuBuf[channel]);
    sceSdSetAddr(iop_adpcm[channel].core | iop_adpcm[channel].vr | SD_VA_SSA, (u_int)(AdpcmSpuBuf[channel] + 4096));
}

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegAdsr);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegVol);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegPitch);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegKon);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegKoff);

void IaSetWrkVolPanPitch(u_char channel, u_short pan, u_short master_vol, u_short pitch)
{
    ADPCM_POS_CALC pcalc;

    pcalc.pan = pan;
    pcalc.master_vol = master_vol;
    GetPosCalc(&pcalc);
    iop_adpcm[channel].pan = pcalc.pan;
    iop_adpcm[channel].vol = pcalc.master_vol;
    iop_adpcm[channel].vol_ll = pcalc.ll;
    iop_adpcm[channel].vol_lr = pcalc.lr;
    iop_adpcm[channel].vol_rl = pcalc.rl;
    iop_adpcm[channel].vol_rr = pcalc.rr;
    iop_adpcm[channel].pitch = pitch;
}

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetWrkFadeParam);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetWrkFadeMode);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetWrkFadeInit);

void GetPosCalc(ADPCM_POS_CALC* calcp)
{
    if (!iop_mv.mono) {
        if (calcp->pan < 0x280u) {
            calcp->ll = calcp->master_vol;
            calcp->lr = 0;
            calcp->rl = ((640 - calcp->pan) * calcp->master_vol) / 640;
            calcp->rr = calcp->pan * calcp->master_vol / 640;
        } else {
            calcp->ll = ((1279 - calcp->pan) * calcp->master_vol) / 640;
            calcp->lr = ((calcp->pan - 640) * calcp->master_vol) / 640;
            calcp->rl = 0;
            calcp->rr = calcp->master_vol;
        }
    } else {
        calcp->ll = calcp->master_vol >> 1;
        calcp->lr = calcp->master_vol >> 1;
        calcp->rl = calcp->master_vol >> 1;
        calcp->rr = calcp->master_vol >> 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetSteMono);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetStopBlock);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetMasterVol);
