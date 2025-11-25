#ifndef IOPSE_H_
#define IOPSE_H_

#include "common.h"

extern u_int snd_buf_top[];

u_int SeGetSndBufTop(int pos) ;
void SeSetStartPoint(u_char type, u_int no);
int ICdvdTransSeEnd();
void SeSetMasterVol(u_short mvol);

SE_PARAM* SeGetSeParamTbl(int num);
int GetSeAdrs(int se_no);

#endif // IOPSE_H_
