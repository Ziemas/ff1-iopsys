#ifndef IOPCDVD_H_
#define IOPCDVD_H_

#include "common.h"

extern CDVD_STAT cdvd_stat;

void ICdvdLoadReqAdpcm(int lsn, u_int size_now, void* buf, u_char channel, int req_type, int endld_flg);

#endif // IOPCDVD_H_
