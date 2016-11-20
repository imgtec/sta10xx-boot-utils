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


extern unsigned crc32(unsigned int crc, const void *buf, int size);

unsigned int calcrc32(void *data, int size)
{

#if 1
	return crc32(0, data, size);
#else	
	fprintf(stderr, "TODO: CRC %p %d\n", data, size);
	return 0x4d435243;
#endif
}



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
	
	char buffer[sizeof(M3_xl)-sizeof(header)];
};


int main(int argc, char *argv[])
{
	struct firmware *fw;
	unsigned int i,j;
	int ch;
	FILE *in, *out;
	char filein[250];
	char fileout[250];

	if(sizeof(*fw)-sizeof(fw->buffer) != 64) {
		fprintf(stderr, "???\n");
		return -1;
	}

	if(argc > 1) {
		strcpy(filein, argv[1]);
		in = fopen(filein, "rb");
		if(in == NULL) {
			fprintf(stderr, "can't read file[%s]\n", filein);
			return -4;
		} else {
			fprintf(stderr, "read from file[%s]\n", filein);
		}

	} else {
		in = stdin;
#ifdef WIN32
		fprintf(stderr, "usage: %s infile outfile", argv[0]);
		return -10;
#endif
	}


	if(argc > 2) {
		strcpy(fileout, argv[2]);
		out = fopen(fileout, "wb");
		if(out) {
			fprintf(stderr, "write to file[%s]\n", fileout);
		} else {
			fprintf(stderr, "can't write to file[%s]\n", fileout);
			return -5;
		}
	} else {
		out = stdout;
#ifdef WIN32
		fprintf(stderr, "usage: %s infile outfile", argv[0]);
		return -11;
#endif
	}

	fprintf(stderr, "firmware size limit[%lu]\n", sizeof(M3_xl));

	fw = (void *)M3_xl;
	memcpy(fw, header, 64);
	fw->filelen = -1;
	fw->crc32prog = 0;
	fw->crc32head = 0;

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
	i = 0;
	while(fw->filelen % 16) {
		fw->buffer[fw->filelen] = '\xff';
		fw->filelen++;
		i++;
		
		if(fw->filelen >= sizeof(M3_xl)-sizeof(header)) {
			fprintf(stderr, "pad failed[%d]\n", fw->filelen);
			return -3;
		}
	}
	fprintf(stderr, "pad count[%d]\n", i);
#endif
	
	fw->crc32prog = calcrc32(fw->buffer, fw->filelen);
	fw->crc32head = calcrc32(fw, sizeof(header)-sizeof(fw->crc32head));

	fprintf(stderr, "file size[%08X]\n", fw->filelen);

	for(i=0; i<fw->filelen + sizeof(header); i++) {
		int ch;
		ch = 0xFF & M3_xl[i];
		putc(ch, out);
	}
	fprintf(stderr, "writen bytes[%d]\n", i);
	return 0;
}

