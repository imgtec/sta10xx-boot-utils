/*
 * a command line tool for patch sta1095 M3 boot binary.
 *
 * This is not a replacement of A2BU.
 *
 * Copyright (c) 2016 Du Huanpeng<u74147@gmail.com>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int calcrc32(void *data, int size)
{
	
	fprintf(stderr, "TODO: CRC %p %d\n", data, size);
	return 0xdeadbeef;
}



/* 但是这个数组字址不以4对齐，可能挂了，这问题不解决算了。 */
char header[64] = {
0xEB,0x10,0x90,0x00,0x03,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x3B,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,
0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x80,0x0F,0x80,0x2F,0xAD,0x72};

char M3_xl[1<<16];

struct firmware {
	char unknow1[16];
	
	int filelen;
	char unknow2[12];
	
	char unknow3[16];
	
	char unknow4[8];
	int crc32prog;
	int crc32head;
	
	char *buffer; /* 32 or 64? */
};


int main(int argc, char *argv[])
{
	struct firmware *fw;
	int i,j;
	int ch;

	if(sizeof(*fw)-sizeof(fw->buffer) != 64) {
		fprintf(stderr, "???\n");
		return -1;
	}

	FILE *in = NULL, *out = NULL;
//	if(argc > 1) filein = argv[1];
//	if(argc > 2) fileout = argv[2];

	fprintf(stderr, "firmware size limit[%lu]\n", sizeof(M3_xl));

/* 初始化 fw 结构 */
	fw = (void *)M3_xl;

	memcpy(fw, header, 64);

	fw->filelen = -1;
	fw->crc32prog = 0;
	fw->crc32head = 0;
	fw->buffer = &M3_xl[64];

/* 简单朴素，使用标准输入输出就行 */
	in = stdin;
	out = stdout;	

/* 固件总大小不能超过 64KB */

	for(i=0; (ch=fgetc(in)) != EOF;i++) {
		if(i >= sizeof(M3_xl)-sizeof(header)) {
			fprintf(stderr, "file to large[%d]\n", i);
			i=0;
			j=1;
			while((ch=getc(in)) != EOF) {
				if(i++==j) {
					fputc('.', stderr);
					j<<=1;
				}
			}
			fprintf(stderr, "please reduce bytes[%d]\n", i);
			return -2;
		}
		fw->buffer[i] = ch;
	}

	fw->filelen = i;

	fprintf(stderr, "file size read[%d]\n", fw->filelen);

#ifdef PAD16
	while(fw->filelen % 16) {
		fw->buffer[fw->filelen] = '\xff';
		fw->filelen++;
		
		if(fw->filelen >= sizeof(M3_xl)-sizeof(header)) {
			fprintf(stderr, "pad failed[%d].\n", fw->filelen);
			return -3;
		}
	}
#endif


	
	fw->crc32prog = calcrc32(fw->buffer, fw->filelen);
	fw->crc32head = calcrc32(fw, sizeof(header)-sizeof(fw->crc32head));

	fprintf(stderr, "file size: [%d]\n", fw->filelen);

	for(i=0; i<fw->filelen + sizeof(header); i++) {
		int ch;
		ch = (unsigned char)M3_xl[i];
		putc(ch, out);
	}

}




