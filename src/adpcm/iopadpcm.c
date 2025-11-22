#include "common.h"

#include "memory.h"

/* bss a00 */ IOP_ADPCM iop_adpcm[2];
/* bss 10 */ u_char* AdpcmIopBuf[2];
/* bss 18 */ u_char* AdpcmSpuBuf[2];
/* bss 9e0 */ ADPCM_CMD now_cmd;
/* bss 8e0 */ ADPCM_CMD cmd_buf[8];

void IAdpcmInit(int dev_init);
void IAdpcmInit(int dev_init);
void IAdpcmAddCmd(IOP_COMMAND*);
void IAdpcmFadeVol(IOP_COMMAND*);
void IAdpcmPos(IOP_COMMAND*);
void IAdpcmMvol(IOP_COMMAND*);

// INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmCmd);
void IAdpcmCmd(IOP_COMMAND* icp)
{
    switch (icp->cmd_no) {
    case IC_ADPCM_PLAY:
    case IC_ADPCM_PRELOAD:
    case IC_ADPCM_LOADEND_PLAY:
    case IC_ADPCM_STOP:
    case IC_ADPCM_PAUSE:
    case IC_ADPCM_RESTART:
        IAdpcmAddCmd(icp);
        break;
    case IC_ADPCM_INIT:
        IAdpcmInit(icp->data1);
        break;
    case IC_ADPCM_FADE_VOL:
        IAdpcmFadeVol(icp);
        break;
    case IC_ADPCM_POS:
        IAdpcmPos(icp);
        break;
    case IC_ADPCM_MVOL:
        IAdpcmMvol(icp);
        break;
    case IC_ADPCM_QUIT:
    }
}

// INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmAddCmd);
void IAdpcmAddCmd(IOP_COMMAND* icp)
{
    ADPCM_CMD cmd;
    ADPCM_CMD* nc;
    ADPCM_CMD* ac;
    int i;
    int j;
    int k;

    memset(&cmd, 0, sizeof(cmd));

    cmd.cmd_type = 0;
    switch (icp->cmd_no) {
    case IC_ADPCM_PLAY:
        cmd.cmd_type = AC_PLAY;
        cmd.fade_flm = icp->data6 & 0xffff;
        cmd.first = icp->data2;
        cmd.size = icp->data3;
        cmd.start_cnt = icp->data7;
        cmd.tune_no = icp->data1;
        cmd.vol = icp->data5 & 0xffff;
        cmd.target_vol = cmd.vol;
        cmd.pan = icp->data5 >> 16;
        cmd.pitch = icp->data6 >> 16;
        cmd.loop = icp->data4 & 2;
        cmd.channel = icp->data4 & 1;
        if (cmd.fade_flm) {
            cmd.fade_mode = ADPCM_FADE_IN_PLAY;
            cmd.vol = 0;
        } else {
            cmd.fade_mode = 0;
        }
        break;
    case IC_ADPCM_PRELOAD:
        cmd.cmd_type = AC_PRELOAD;
        cmd.tune_no = icp->data1;
        cmd.first = icp->data2;
        cmd.size = icp->data3;
        cmd.start_cnt = icp->data7;
        cmd.channel = icp->data4 & 1;
        break;
    case IC_ADPCM_LOADEND_PLAY:
        cmd.cmd_type = AC_PLAY;
        cmd.fade_flm = icp->data6 & 0xffff;
        cmd.start_cnt = icp->data7;
        cmd.tune_no = icp->data1;
        cmd.vol = icp->data5 & 0xffff;
        cmd.target_vol = cmd.vol;
        cmd.pan = icp->data5 >> 16;
        cmd.pitch = icp->data6 >> 16;
        cmd.loop = icp->data4 & 2;
        cmd.channel = icp->data4 & 1;
        if (cmd.fade_flm) {
            cmd.fade_mode = ADPCM_FADE_IN_PLAY;
            cmd.vol = 0;
        } else {
            cmd.fade_mode = 0;
        }
        break;
    case IC_ADPCM_STOP:
        cmd.cmd_type = AC_STOP;
        cmd.fade_flm = icp->data6 & 0xffff;
        cmd.tune_no = icp->data1;
        cmd.channel = icp->data4 & 1;
        break;
    case IC_ADPCM_PAUSE:
        cmd.cmd_type = AC_PAUSE;
        cmd.fade_flm = icp->data6 & 0xffff;
        cmd.tune_no = icp->data1;
        cmd.channel = icp->data4 & 1;
        break;
    case IC_ADPCM_RESTART:
        cmd.cmd_type = AC_RESTART;
        cmd.fade_flm = icp->data6 & 0xffff;
        cmd.tune_no = icp->data1;
        cmd.channel = icp->data4 & 1;
        break;
    }

    // Insert into cmd buffer
    if (cmd.cmd_type)
    {
        nc = 0;
        for (i = 0; i < 8; i++)
        {
            if (cmd_buf[i].cmd_type == 0)
            {
                cmd_buf[i] = cmd;
                nc = &cmd_buf[i];
                break;
            }
        }

        if (i < 8) {}

        if (nc == 0) {} else {}
    }

}

// INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmSetSpu2Irq);
void IAdpcmSetSpu2Irq()
{
    static u_short pitch;
}

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmMain);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmMain2);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmInit);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmPreLoad);

// INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmPreLoadEnd);
void IAdpcmPreLoadEnd(int channel)
{
    int i;
    static int cnt;
}

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmPlay);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmStop);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmFadeVol);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmPos);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmMvol);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", AdpcmSpu2IntrHander);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", AdpcmTransCB);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmReadCh0);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmTuneEndStop);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmTuneEndLoop);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmReadCh1);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", SetLoopFlag);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", SetLoopFlgAll);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", SetLoopFlgAll2);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", SetLoopFlgSize);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmLoadEndStream);

INCLUDE_ASM("asm/nonmatchings/adpcm/iopadpcm", IAdpcmLoadEndPreOnly);
