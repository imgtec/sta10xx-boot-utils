1. The header format

00000000  eb 10 90 00 03 00 00 0a  00 00 00 00 00 00 00 00  |................|
00000010  48 f4 00 00 01 00 00 00  00 00 00 00 40 00 00 00  |H...........@...|
00000020  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  |................|
00000030  00 00 00 00 00 00 00 00  23 9f a7 e0 0f 06 4c d1  |........#.....L.|
00000040  00 58 01 00 a9 02 00 00  b5 02 00 00 b1 02 00 00  |.X..............|
00000050  b7 02 00 00 b9 02 00 00  bb 02 00 00 00 00 00 00  |................|
00000060  00 00 00 00 00 00 00 00  00 00 00 00 c5 04 00 00  |................|
00000070  bd 02 00 00 00 00 00 00  71 05 00 00 ad 05 00 00  |........q.......|
00000080  bf 02 00 00 c1 02 00 00  71 1b 00 00 c5 02 00 00  |........q.......|
00000090  c7 02 00 00 c9 02 00 00  cb 02 00 00 cd 02 00 00  |................|

10h, 11h, 12h, 13h: binary file without header length in little-endian
38h, 39h, 3Ah, 3Bh: crc of the binary file without header (40h-EOF)
3Ch, 3Dh, 3Eh, 3Fh: crc of the header (00h-3Ch)

others still don't know and seems fixed for nor flash boot.

2. How to use this tool
 a) GNU/Linux command line or Makefile
	$ sta10xx-boot-utils <M3_xl.bin >M3_xlsig.bin
	  or
	$ sta10xx-boot-utils M3_xl.bin M3_xlsig.bin
 b) Windows command line or Makefile
	$ sta10xx-boot-utils M3_xl.bin M3_xlsig.bin

 c) emIDE
	Project-> Build options-> Pre/post build steps

 d) Keil
	Project-> Options for Target '...'->User ->After Build/Rebuild

 e) Source Insight
	Options-> Custom Commands-> Add

3. If this tool not works, please 'tell' me.
 Du Huanpeng <u74147@gmail.com>

- - -
2016.11.20
