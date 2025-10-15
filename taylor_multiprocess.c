#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#define _USE_MATH_DEFINES
#include <math.h>
#define N 4
#define TERMS 10

double sinx_taylor_single(int terms, double x)
{
    double value = x;
    double numer = x * x * x;
    double denom = 6.0; // 3!
    int sign = -1;

    for (int j = 1; j <= terms; j++) {
        value += (double)sign * numer / denom;
        numer *= x * x;
        denom *= (2.0 * (double)j + 2.0) * (2.0 * (double)j + 3.0);
        sign *= -1;
    }
    return value;
}

int main()
{
    double x[N] = {0, M_PI / 6., M_PI / 3., 0.134}; 
    double res[N];                            
    pid_t pids[N];              
    int pipes[N][2];                  

    for (int i = 0; i < N; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe 생성 오류");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork 생성 오류");
            exit(EXIT_FAILURE);
        }
        else if (pids[i] == 0) {
            close(pipes[i][0]);

            for(int j = 0; j < N; j++) {
                if (i != j) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            double child_result = sinx_taylor_single(TERMS, x[i]);

            if (write(pipes[i][1], &child_result, sizeof(double)) == -1) {
                perror("자식 프로세스 write 오류");
                exit(EXIT_FAILURE);
            }

            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < N; i++) {
        close(pipes[i][1]);

        if (read(pipes[i][0], &res[i], sizeof(double)) == -1) {
            perror("부모 프로세스 read 오류");
            exit(EXIT_FAILURE);
        }
        close(pipes[i][0]);
    }

    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    printf("--- 병렬 계산 결과 ---\n");
    for (int i = 0; i < N; i++) {
        printf("sin(%.4f) [테일러 급수] = %f\n", x[i], res[i]);
        printf("sin(%.4f) [math.h]     = %f\n", x[i], sin(x[i]));
        printf("----------------------------------------\n");
    }

    return 0;
}
				   
