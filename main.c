#include <stdlib.h>
#include "netpbm/ppm.h"
int main(int argc, const char **argv)
{
	FILE *fp;
	int rows, cols, format;
	int row;
	pixval maxval;
	pixel **pixels;
	int p,q;

	fp = fopen(argv[1], "rb");
	ppm_readppminit(fp, &cols, &rows, &maxval, &format);
	if (PPM_FORMAT_TYPE(format) != PPM_TYPE) return -1;
	pm_close(fp);

	return 0;
}
