#include "common.h"

void GetPosCalc(ADPCM_POS_CALC* calcp);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaInitDev);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IAdpcmMakeThread);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaInitEffect);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaInitVolume);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaDbgMemoryCheck);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegSsa);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegAdsr);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegVol);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegPitch);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegKon);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetRegKoff);

// INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", IaSetWrkVolPanPitch);
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

// INCLUDE_ASM("asm/nonmatchings/adpcm/ia_func", GetPosCalc);
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
