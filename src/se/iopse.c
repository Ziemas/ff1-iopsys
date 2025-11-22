#include "common.h"

/* data 10 */ u_int snd_buf_top[] = {
    0x5400,
    0x5F400,
    0x68800,
    0x78400,
    0x84900,
    0x90E00,
    0x9D300,
    0x9F600,
    0xA1900,
    0xA3C00,
    0xA5F00,
    0xA8200,
    0xAA500,
    0xAC800,
    0xAEB00,
    0xC2F00,
    0xD7300,
    0x115700,
    0x153B00,
    0x191F00,
    0x1A4F00,
    0x1B7F00,
    0x1BF700,
    0x1CC700,
    0x1D9700,
    0x1E6700,
    0x1F3700,
    0x1F3740,
    0x1F6740,
    0x1F6780
};

/* bss 0 */ static SE_WRK_SET se_wrk_set[24];
/* bss 8a0 */ SE_START_POINT se_start_point;
/* bss 8 */ u_int se_start_flg;
/* bss 4 */ u_int se_stop_flg;

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeInit);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeMain);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeEndCheck);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeWrkUpdate);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeCmd);

INCLUDE_ASM("asm/nonmatchings/se/iopse", IopSoundMasterVolChange);

INCLUDE_ASM("asm/nonmatchings/se/iopse", IopSoundSteMonoChange);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISePlay);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeStop);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeVol);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeEfctVol);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISePitch);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeAllStop);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISeQuit);

INCLUDE_ASM("asm/nonmatchings/se/iopse", ISePos);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeChangeSetDataPlay);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeChangeSetDataPos);

INCLUDE_ASM("asm/nonmatchings/se/iopse", GetPrimAndBufNo);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeSetStartPoint);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeGenerateVolPich);

INCLUDE_ASM("asm/nonmatchings/se/iopse", GetSeVstat);

INCLUDE_ASM("asm/nonmatchings/se/iopse", GetSeWrkSetP);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeSetSeWrk);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeSetMix);

INCLUDE_ASM("asm/nonmatchings/se/iopse", CidAndVnum);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeGetSndBufTop);

INCLUDE_ASM("asm/nonmatchings/se/iopse", SeSetMasterVol);
