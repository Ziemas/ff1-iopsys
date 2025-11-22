#include "common.h"

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", InitAdpcmCmdBuf);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmCmdSlide);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmChkCmdExist);

//INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmCmdPlay);
void IAdpcmCmdPlay() {
	ADPCM_CMD ac;
	static int c;
}

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmCmdStop);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmCmdPreLoad);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmCmdPause);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", IAdpcmCmdRestart);

INCLUDE_ASM("asm/nonmatchings/adpcm/ia_cmd", DbgDispAdpcmCmdWrk);
