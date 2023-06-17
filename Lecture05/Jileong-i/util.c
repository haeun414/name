#include "util.h"

void CHECKERROR(int e)
{
	if (e == 1)
	{
		printf("\n[line:%d, %s] ¿¡·¯°¡ ¹ß»ýÇÏ¿´½À´Ï´Ù.\n", __LINE__, __func__);
		exit(1);
	}
}