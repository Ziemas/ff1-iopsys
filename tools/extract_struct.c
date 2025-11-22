#include <stdio.h>

typedef struct { // 0x10
    /* 0x0:00 */ unsigned int adrs : 32;
    /* 0x4 */ short unsigned int attribute;
    /* 0x6 */ short unsigned int pitch;
    /* 0x8 */ short unsigned int adsr1;
    /* 0xa */ short unsigned int adsr2;
    /* 0xc */ char efct;
    /* 0xd */ char vol;
    /* 0xe */ char pan;
} SE_PARAM;

SE_PARAM param[548];

int main(int argc, const char** argv)
{
    SE_PARAM* p;
    FILE* fp;
    int i;

    fp = fopen("in/IOPSYS.IRX", "r");

    fseek(fp, 0xb490, SEEK_SET);
    fread(param, sizeof(SE_PARAM), 548, fp);

    for (i = 0; i < 548; i++) {
        p = &param[i];

        printf("{ .adrs = 0x%x, .attribute = 0x%x, .pitch = 0x%x, .adsr1 = 0x%x, .adsr2 = 0x%x, .efct = 0x%x, .vol = 0x%x, .pan = 0x%x },\n", p->adrs, p->attribute, p->pitch, p->adsr1, p->adsr2, p->efct, p->vol, p->pan);
    }
}
