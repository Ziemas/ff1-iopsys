#include "common.h"

#include "libsd.h"
#include "se/iopse.h"

#include "libcdvd.h"
#include "memory.h"
#include "sif.h"
#include "string.h"
#include "sysmem.h"

/* bss bc0 */ CDVD_STAT cdvd_stat;
/* bss c60 */ CDVD_REQ_BUF cdvd_req[32];
/* bss ac0 */ CDVD_LOAD_STAT load_stat[32];
/* bss 1060 */ CDVD_TRANS_STAT cdvd_trans[2];
/* bss 20 */ u_int* load_buf_table[2];

static void ICdvdInitOnce();
static void ICdvdInitSoftReset();
static void ICdvdAddCmd(IOP_COMMAND* icp);
static void ICdvdTransSe(IOP_COMMAND* icp);
static void ICdvdTransSeInit();
static void ICdvdSetRetStat(int id, u_char stat);
static int ICdvdExecCmdSub(CDVD_REQ_BUF* req_bufp);
static int ICdvdCheckBufStatus();

void ICdvdCmd(IOP_COMMAND* icp)
{
    switch (icp->cmd_no) {
    case IC_CDVD_INIT:
        ICdvdInit(icp->data1);
        break;
    case IC_CDVD_LOAD:
    case IC_CDVD_LOAD_SECT:
    case IC_CDVD_SEEK:
        ICdvdAddCmd(icp);
        break;
    case IC_CDVD_SE_TRANS:
        ICdvdTransSe(icp);
        break;
    case IC_CDVD_SE_TRANS_RESET:
        ICdvdTransSeInit();
        break;
    case IC_CDVD_BREAK:
        ICdvdBreak();
        break;
    }
}

void ICdvdInit(int reset)
{
    if (reset)
        ICdvdInitSoftReset();
    else
        ICdvdInitOnce();
}

INCLUDE_ASM("asm/nonmatchings/cdvd/iopcdvd", ICdvdInitOnce);
// static void ICdvdInitOnce()
//{
//     int tmp;
//     char ld_error = 0;
//     u_int ld_cnt = 0;
//     sceCdlFILE cdlf;
//     sceSifDmaData sdd;
//     u_int dma_id;
//     int oldstat;
//     int i;
//     u_int ret_count0;
//     u_int ret_count1;
//     char fname[80];
//
//     memset(&cdvd_stat, 0, sizeof(cdvd_stat));
//     memset(cdvd_req, 0, sizeof(cdvd_req));
//     memset(&iop_stat.cdvd, 0, sizeof(iop_stat.cdvd));
//     memset(cdvd_trans, 0, sizeof(cdvd_trans));
//
//     load_buf_table[0] = AllocSysMemory(0, 0x64000, 0);
//     if (!load_buf_table[0]) {
//     } else {
//         load_buf_table[1] = (u_int)load_buf_table[0] + 0x32000;
//         iop_stat.cdvd.ld_addr = (u_int)load_buf_table[0];
//     }
//
//     if (sceCdSync(1)) {
//         while (!sceCdBreak())
//             ;
//     }
//
//     strcpy(fname, "\\IMG_HD.BIN;1");
//     if (!sceCdSearchFile(&cdlf, fname)) {
//         while (1)
//             ;
//     }
//     strcpy(fname, "\\IMG_BD.BIN;1");
//     if (!sceCdSearchFile(&cdvd_stat.cdlf, fname)) {
//         while (1)
//             ;
//     }
//
//     cdvd_stat.rmode.trycount = 0;
//     cdvd_stat.rmode.spindlctrl = 1;
//     cdvd_stat.rmode.datapattern = 0;
// }

INCLUDE_ASM("asm/nonmatchings/cdvd/iopcdvd", ICdvdInitSoftReset);

INCLUDE_ASM("asm/nonmatchings/cdvd/iopcdvd", ICdvdMain);

INCLUDE_ASM("asm/nonmatchings/cdvd/iopcdvd", ICdvdTransCheck);

INCLUDE_ASM("asm/nonmatchings/cdvd/iopcdvd", ICdvdTransFinishedData);

static int ICdvdSeTransCB(int channel, void* data)
{
    cdvd_stat.vtrans_flg = 0;
    sceSdSetTransIntrHandler(1, 0, 0);
    return 1;
}

static void ICdvdDataReadOnceSector()
{
    cdvd_stat.now_size = cdvd_stat.size - cdvd_stat.end_size;

    if (cdvd_stat.now_size > 204800) {
        cdvd_stat.now_size = 204800;
        cdvd_trans[cdvd_stat.now_lbuf].ltrans = 0;
    } else {
        cdvd_trans[cdvd_stat.now_lbuf].ltrans = 1;
    }

    sceCdRead(
        cdvd_stat.end_size / 2048 + cdvd_stat.start,
        (cdvd_stat.now_size + 2047) / 2048,
        load_buf_table[cdvd_stat.now_lbuf],
        &cdvd_stat.rmode);
}

INCLUDE_ASM("asm/nonmatchings/cdvd/iopcdvd", ICdvdOnPreLoadEnd);

static void ICdvdAdpcmLoad()
{
    u_char pos;

    pos = 0xff;
    if (cdvd_stat.adpcm[0].req_type >= cdvd_stat.adpcm[1].req_type) {
        pos = 0;
    } else {
        pos = 1;
    }

    sceCdRead(
        cdvd_stat.adpcm[pos].start,
        (cdvd_stat.adpcm[pos].size_now + 2047) / 2048,
        cdvd_stat.adpcm[pos].taddr,
        &cdvd_stat.rmode);
    cdvd_stat.adpcm[pos].now_load = 1;
    cdvd_stat.adpcm_req = 1;
}

static void ICdvdStrLoad() {}

static int ICdvdCheckLoadError()
{
    int result;

    result = sceCdGetError();

    switch (result) {
    case 0:
        return 0;
    case -1:
        return -1;
    }

    return 1;
}

void ICdvdBreak()
{
    while (!sceCdBreak())
        ;
    cdvd_stat.stat = 0;
    cdvd_stat.adpcm_req = 0;
}

static int ICdvdSectorLoad(CDVD_REQ_BUF* req_bufp)
{
    if (!ICdvdCheckBufStatus())
        return 0;

    cdvd_stat.start = req_bufp->start_sector + cdvd_stat.cdlf.lsn;
    cdvd_stat.size = req_bufp->size_sector;
    cdvd_stat.file_no = req_bufp->file_no;
    cdvd_stat.end_size = 0;
    cdvd_stat.id = req_bufp->id;
    if (req_bufp->tmem == TRANS_MEM_SPU)
        SeSetStartPoint(req_bufp->se_buf_no, req_bufp->file_no);

    ICdvdDataReadOnceSector();
    cdvd_stat.cmd_on = 1;
    ICdvdSetRetStat(cdvd_stat.id, CDVD_LS_LOADING);
    return 1;
}

static int ICdvdCheckBufStatus()
{
    int i;

    for (i = 0; i < 2; i++) {
        if (cdvd_trans[cdvd_stat.now_lbuf].stat == 0) {
            return 1;
        } else if (cdvd_trans[cdvd_stat.now_lbuf].stat == 4) {
            continue;
        } else if (cdvd_trans[cdvd_stat.now_lbuf].stat == 3) {
            return 1;
        }

        cdvd_stat.now_lbuf = (cdvd_stat.now_lbuf + 1) % 2;
    }

    return 0;
}

void ICdvdSeek()
{
    cdvd_stat.cmd_on = 1;
}

static void ICdvdAddCmd(IOP_COMMAND* icp)
{
    CDVD_REQ_BUF req_buf;

    req_buf.req_type = icp->cmd_no;
    req_buf.file_no = icp->data1;
    req_buf.start_sector = icp->data2;
    req_buf.size_sector = icp->data3;

    if (icp->data5 >= TRANS_MEM_NUM)
        req_buf.tmem = TRANS_MEM_EE;
    else
        req_buf.tmem = icp->data5;

    if (req_buf.tmem == TRANS_MEM_SPU) {
        req_buf.taddr = (u_int*)SeGetSndBufTop(icp->data4);
        req_buf.se_buf_no = icp->data4;
    } else {
        req_buf.taddr = (u_int*)icp->data4;
    }

    req_buf.id = icp->data6;
    if (cdvd_stat.buf_use_num < 32) {
        cdvd_req[cdvd_stat.req_pos] = req_buf;
        cdvd_stat.req_pos = (cdvd_stat.req_pos + 1) % 32;
        ++cdvd_stat.buf_use_num;
        ICdvdSetRetStat(req_buf.id, CDVD_STAT_LOADING);
    } else {
    }
}

static void ICdvdTransSe(IOP_COMMAND* icp)
{
    u_int size;
    u_int addr;

    size = icp->data1;
    addr = snd_buf_top[icp->data2];
    while (sceSdVoiceTrans(1, 0, (u_char*)load_buf_table[0], (u_int*)addr, size) < 0)
        ;
    iop_stat.cdvd.se_trans = 1;
    SeSetStartPoint(icp->data2, icp->data3);
}

int ICdvdTransSeEnd()
{
    if (iop_stat.cdvd.se_trans == 1) {
        if (sceSdVoiceTransStatus(1, 0) == 1) {
            iop_stat.cdvd.se_trans = 2;
        }
    } else if (iop_stat.cdvd.se_trans == 2) {
    }
}

static void ICdvdTransSeInit()
{
    iop_stat.cdvd.se_trans = 0;
}

static void ICdvdExecCmd()
{
    if (cdvd_stat.buf_use_num) {
        if (ICdvdExecCmdSub(&cdvd_req[cdvd_stat.start_pos])) {
            cdvd_stat.error_cnt = 0;
        } else {
        }
    } else {
    }
}

static int ICdvdExecCmdSub(CDVD_REQ_BUF* req_bufp)
{
    int ret_param = 0;

    switch (req_bufp->req_type) {
    case IC_CDVD_LOAD:
        cdvd_stat.stat = CDVD_STAT_LOADING;
        break;
    case IC_CDVD_LOAD_SECT:
        if (ICdvdSectorLoad(req_bufp)) {
            cdvd_stat.stat = CDVD_STAT_LOADING;
            ret_param = 1;
        }
        break;
    case IC_CDVD_SEEK:
        cdvd_stat.stat = CDVD_STAT_SEEKING;
        break;
    }

    return ret_param;
}

static void ICdvdSetRetStat(int id, u_char stat)
{

    if (id >= 0 && id < 32) {
        iop_stat.cdvd.fstat[id].stat = stat;
    } else {
    }
}

static void ICdvdSetRetStatFinish(int id)
{
    if (id >= 0 && id < 32) {
        iop_stat.cdvd.fstat[id].stat = 0;
        cdvd_stat.id = (cdvd_stat.id + 1) % 32;
    } else {
    }
}

void ICdvdSetRetStatClear()
{
    CDVD_LOAD_STAT* clsp;
    int i;

    for (i = 0, clsp = iop_stat.cdvd.fstat; i < 32; i++, clsp++) {
        clsp->stat = CDVD_LS_FINISHED;
    }
}

void ICdvdLoadReqPcm(u_int lsn, u_int size_sec, void* buf, u_char pre)
{
    cdvd_stat.pcm.start = lsn;
    cdvd_stat.pcm.size_now = size_sec;
    cdvd_stat.pcm.taddr = (u_int*)buf;
    if (pre) {
        cdvd_stat.pcm_pre = 1;
        cdvd_stat.pcm_pre_end = 0;
    } else {
        cdvd_stat.pcm_pre = 0;
    }
    cdvd_stat.pcm_req = 1;
}

void ICdvdLoadReqAdpcm(int lsn, u_int size_now, void* buf, u_char channel, u_char req_type, u_char endld_flg)
{
    cdvd_stat.adpcm[channel].start = lsn + cdvd_stat.cdlf.lsn;
    cdvd_stat.adpcm[channel].size_now = size_now;
    cdvd_stat.adpcm[channel].read_now = 0;
    cdvd_stat.adpcm[channel].taddr = (u_int*)buf;
    cdvd_stat.adpcm[channel].req_type = req_type;
    cdvd_stat.adpcm[channel].endld_flg = endld_flg;
    cdvd_stat.adpcm_req = 1;
}
