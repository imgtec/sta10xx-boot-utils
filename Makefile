sta10xx-boot-utils:
	gcc crc32.c sta10xx-boot-utils.c -g -o $@ -include config.h
