#ifndef _FUCK_CHEATS
#define _FUCK_CHEATS

#include "linux/fs.h" 
#include "ini.h" 
#include "asm/uaccess.h"

const unsigned long 原码 = 0x358947;

uint32_t get_str_crc32(const char* s)
{
	int i = 0;
	uint8_t j, byte;
	uint32_t crc = 0xffffffff;
	
	if(NULL==s)
	{
		return 6666;
	}
	


	while (s[i] != '\0')
	{
		byte = s[i];
		crc = crc ^ byte;
		for (j = 8; j > 0; --j)
		{
			crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
		}
		i++;
	}
	return crc ^ 0xffffffff;
}


#endif /* _FUCK_CHEATS */