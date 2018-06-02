#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define N 1024

int main(int argc, char const *argv[])
{
	int G_x[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
	int G_y[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int j, k, hight = 0, weidht = 0;
	int f1, f2, f;
	int i;
	int img[N][N];
	int a[9];
	int z[9];
	char file_out[N] = {0};
	char file_in[N] = {0};

	FILE *in;
	FILE *out;

	strcat(file_in, argv[1]);
	strcat(file_out, argv[2]);

	in = fopen(file_in, "rb");
	out = fopen(file_out, "wb");

	if(in!=NULL){
		for(j = 0; j < 3; j++) {
			k = fgetc(in);
			fputc(k,out);
		}
		fscanf(in, "%d", &weidht);
		fprintf(out, "%d", weidht);
		k = fgetc(in);
		fputc(k,out);
		fscanf(in, "%d", &hight);
		fprintf(out, "%d", hight);
		for(j = 0; j < 4; j++) {
			k = fgetc(in);
			fputc(k,out);
		}

		for (j = 0; j < hight; j++)
			for (k = 0; k < weidht; k++)
				img[j][k] = fgetc(in);
	}

	for ( j = 0; j < weidht; j++)
		fputc(img[0][j],out);

	for (j = 1; j < hight-1; j++)
	{
		fputc(img[j][0],out);
		for (k = 1; k < weidht-1; k++)
		{
			a[0] = img[j-1][k-1];
			a[1] = img[j-1][k];
			a[2] = img[j-1][k+1];

			a[3] = img[j][k-1];
			a[4] = img[j][k];
			a[5] = img[j][k+1];

			a[6] = img[j+1][k-1];
			a[7] = img[j+1][k];
			a[8] = img[j+1][k+1];

			for(i = 0; i<9; i++)
				z[i] = G_y[i] * a[i];

			for(f1 =0, i = 0; i<9; i++)
				f1 = f1 +z[i];

			for(i = 0; i<9; i++)
				z[i] = G_x[i] * a[i];

			for(f2 =0, i = 0; i<9; i++)
				f2 = f2 +z[i];
			f1 = abs(f1);
			f2 = abs(f2);
			f1 = f1*f1;
			f2 = f2*f2;
			f = f1 + f2;
			f = sqrt(f);
			fputc(f,out);
		}
		fputc(img[j][weidht-1],out);
	}

	for ( j = 0; j < weidht; j++)
		fputc(img[hight - 1][j],out);

	fclose(out);
	fclose(in);
	printf("%ld %d\n", clock(), 1);
	return 0;
}