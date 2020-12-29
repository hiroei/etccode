#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

//Get Epoch seconds
double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 0.000001 * tv.tv_usec;
}

//Clear Cache
#define CACHE_SIZE (6*1024*1024) //キャッシュサイズ6MB
#define X_SIZE (2*CACHE_SIZE/sizeof(double))
double X[X_SIZE];
void clear_cache()
{
    int i;
    double sum = 0.0;
    FILE *fp;
    for(i=0; i<X_SIZE; i++) X[i] = drand48();

    for(i=0; i<X_SIZE; i++) sum += X[i];
    fp = fopen("/dev/null", "w");
    fprintf(fp, "%lf\n", sum);
    fclose(fp);

}

#define N 3000
#define REPEAT 5
double A[N][N];
double B[N][N];
double C[N][N] = {0};

void data_initialize()
{
    //A,Bにランダムな値を代入
    int i, j;
    for(i=0; i<N; i++) {
        for(j=0; j<N; j++) {
            A[i][j] = drand48();
            B[i][j] = drand48();
        }
    }
}

int main(void)
{
    FILE *fp;
    int i,j,k;
    double start, end;
    double total = 0, average;

    for(int count=0; count<REPEAT; count++) 
    {
        
        data_initialize();
        clear_cache(); //キャッシュクリア
        start = now(); //計測開始

        // for(i=0; i<N; i++) {
        //     for(k=0; k<N; k++) {
        //         for(j=0; j<N; j++){
        //             C[i][j] += A[i][k] * B[k][j];
        //         }
        //     }
        // }

        //行列積の計算
        for(i=0; i<N; i++) {
            for(j=0; j<N; j++) {
                for(k=0; k<N; k++){
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        end = now(); // 計測終了
        printf("time:%f\n", end-start);
        total += (end-start);

        //値を合計して出力
        double sum = 0.0;
        for(i=0; i<N; i++) {
            for(j=0; j<N; j++) {
                sum += C[i][j];
                // printf("%f\n", C[i][j]);
                // printf("B%f\n", B[i][j]);
            }
        }
        fp = fopen("/dev/null", "w");
        fprintf(fp, "%lf\n", sum);
        fclose(fp);
    }
    average = total / REPEAT;
    printf("average:%f\n", average);

    return 0;
}