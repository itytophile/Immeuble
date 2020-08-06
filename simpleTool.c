#include <fxlib.h>
#include "simpleTool.h"

void intToString(int n, char **s) {
	int i = 1, m = n;
	while(m/=10) ++i;
	*s = (char *)realloc(*s, sizeof(char)*(i+1));
	(*s)[i] = 0;
	do (*s)[--i] = '0' + n%10;
	while(n/=10);
}
