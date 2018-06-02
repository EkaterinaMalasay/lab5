#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define N 1024

//специальная структура для данных потока
typedef struct{
	int j; //номер обрабатываемой строки
	int weidht; //размер строки
	//указатели на матрицы
	int** img;
	int** img2;
} pthrData;

void *thread_function(void* thread_data) {
	int j, k, i;
	int f1, f2, f;
	int a[9];
	int z[9];
	int G_x[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
	int G_y[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	pthrData *data = (pthrData*) thread_data;

	data->img2[data->j][0] = data->img[data->j][0];
	for (k = 1; k < data->weidht-1; k++)
	{
		pthrData *data = (pthrData*) thread_data;
		a[0] = data->img[data->j-1][k-1];
		a[1] = data->img[data->j-1][k];
		a[2] = data->img[data->j-1][k+1];

		a[3] = data->img[data->j][k-1];
		a[4] = data->img[data->j][k];
		a[5] = data->img[data->j][k+1];

		a[6] = data->img[data->j+1][k-1];
		a[7] = data->img[data->j+1][k];
		a[8] = data->img[data->j+1][k+1];

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
		data->img2[data->j][k] = f;
	}

	data->img2[data->j][data->weidht-1] = data->img[data->j][data->weidht-1];
	pthread_exit("Thank you for the CPU time");
}


int main(int argc, char const *argv[])
{
	int res, i, num_thr;
	pthread_t a_thread[5];
	void *thread_result;
	int j, k, hight = 0, weidht = 0;
	//int img[N][N];
	char file_out[N] = {0};
	char file_in[N] = {0};

	FILE *in;
	FILE *out;
	int** img = (int**) malloc(N * sizeof(int*));
	int** img2 = (int**) malloc(N * sizeof(int*));
	for(i = 0; i < N; i++){
		img[i] = (int*) malloc(N * sizeof(int));
		img2[i] = (int*) malloc(N * sizeof(int));
	}

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
		for(j = 0; j < 5; j++) {
			k = fgetc(in);
			fputc(k,out);
		}

	for (j = 0; j < hight; j++)
		for (k = 0; k < weidht; k++)
				img[j][k] = fgetc(in);
	}


	num_thr = hight - 2;
	for ( j = 0; j < weidht; j++)
		fputc(img[0][j],out);

	//сколько потоков - столько и структур с потоковых данных
	pthrData* threadData = (pthrData*) malloc(num_thr * sizeof(pthrData));

	for (i = 0; i < num_thr; i++)
	{
			j = i + 1;
			threadData[i].j = j;
			threadData[i].weidht = weidht;
			threadData[i].img = img;
			threadData[i].img2 = img2;
			res = pthread_create(&a_thread[i], NULL, thread_function,
										&threadData[i]);
			if (res != 0) {
				perror("Thread creatoin failed");
				exit(EXIT_FAILURE);
			}
	}

	for(i = 0; i < num_thr; i++) {
		res = pthread_join(a_thread[i], &thread_result);
		if (res != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
	}

	for (j = 1; j < hight-1; j++)
	{
		for (k = 0; k < weidht; k++)
		{
			fputc(img2[j][k],out);
		}
	}
	for ( j = 0; j < weidht; j++)
		fputc(img[hight - 1][j],out);


	free(threadData);
	for(i = 0; i < N; i++){
		free(img[i]);
		free(img2[i]);
	}
	free(img2);
	free(img);
	printf("%ld %d\n", clock(), num_thr);
	exit(EXIT_SUCCESS);
}
