#include <stdio.h>
#include <time.h>
#include "nqueens.h"
FILE *worseResF, *worseVerF, *bestVerF, *worseFirsSolve, *spatialleVerification;

void initWorseResF()
{
    worseResF = fopen("Resolution.csv", "w+");
    fprintf(worseResF, "N; ExecTime (ms) \n");
}

void writeWorseResF(int n, long long ExecTime)
{
    fprintf(worseResF, "%d; %lld\n", n, ExecTime);
}

void initWorseVerF()
{
    worseVerF = fopen("Verification_worse_case.csv", "w+");
    fprintf(worseVerF, "N; ExecTime (ns) \n");
}

void writeWorseVerF(int n, long long ExecTime)
{
    fprintf(worseVerF, "%d; %lld\n", n, ExecTime);
}

void initBestVerF()
{
    bestVerF = fopen("Verification_best_case.csv", "w+");
    fprintf(bestVerF, "N; ExecTime (ns) \n");
}

void writeBestVerF(int n, long long ExecTime)
{
    fprintf(bestVerF, "%d; %lld\n", n, ExecTime);
}

void initWorseFirsSolve()
{
    worseFirsSolve = fopen("Resolution_Une_Solution.csv", "w+");
    fprintf(worseFirsSolve, "N; ExecTime (ns) \n");
}

void writeWorseFirsSolve(int n, long long ExecTime)
{
    fprintf(worseFirsSolve, "%d; %lld\n", n, ExecTime);
}

void initSpatialVerification()
{
    worseFirsSolve = fopen("Resolution_Une_Solution_space.csv", "w+");
    fprintf(worseFirsSolve, "N; space (octet) \n");
}

void writeSpatialVerification(int n, long long space)
{
    fprintf(worseFirsSolve, "%d; %lld\n", n, space);
}

long long timePassed(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000000000 + end.tv_nsec - start.tv_nsec;
}
void main()
{
    int choice = 1, bool , n;
    struct timespec start, end;
    initWorseResF();
    initWorseVerF();
    initBestVerF();
    initWorseFirsSolve();
    initSpatialVerification();
    int *solution;
    printf("please choose an operation : \n1 - print solutions of n queens for n from 1 to 15 and save execution time \n2 - save time of finding one solution to the n queens problem (n from 1 to 30)\n3 - save the time and space of the n queens solution verification algorithm (n from 1 to 30 worse and best case)\n4 - all above (n from 1 to 15) \n5 - find a solution \n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        for (int i = 1; i < 16; i++)
        {
            clock_gettime(CLOCK_REALTIME, &start);
            printf("nb solutions of %d queens is %d \n", i, getNbSolutionsOfNQueen(i));
            clock_gettime(CLOCK_REALTIME, &end);
            writeWorseResF(i, timePassed(start, end) / 1000000);
        }
        break;
    case 2:
        for (int i = 1; i < 31; i++)
        {
            if (i % 10 == 0)
                printf("i am in i = %d\n", i);
            clock_gettime(CLOCK_REALTIME, &start);
            solution = getFirstSolutionVect(i);
            clock_gettime(CLOCK_REALTIME, &end);
            writeWorseFirsSolve(i, timePassed(start, end));
        }
        break;
    case 3:
        for (int i = 1; i < 31; i++)
        {
            if (i % 10 == 0)
                printf("i am in i = %d\n", i);
            solution = getFirstSolutionVect(i);

            clock_gettime(CLOCK_REALTIME, &start);
            bool = checkSolution(solution, i);
            clock_gettime(CLOCK_REALTIME, &end);
            writeWorseVerF(i, timePassed(start, end));
            if (solution != NULL)
                solution[0] = -1;
            clock_gettime(CLOCK_REALTIME, &start);
            bool = checkSolution(solution, i);
            clock_gettime(CLOCK_REALTIME, &end);
            writeBestVerF(i, timePassed(start, end));
            writeSpatialVerification(i, sizeof(int) + i * sizeof(int) + i * i * sizeof(int) + 2 * sizeof(int));
        }
        break;
    case 5:
        printf("n : \n");
        scanf("%d",&n);
        getFirstSolution(n);
        break;
    default:
        for (int i = 1; i < 16; i++)
        {
            clock_gettime(CLOCK_REALTIME, &start);
            printf("nb solutions of %d queens is %d \n", i, getNbSolutionsOfNQueen(i));
            clock_gettime(CLOCK_REALTIME, &end);
            writeWorseResF(i, timePassed(start, end) / 1000000);
            clock_gettime(CLOCK_REALTIME, &start);
            solution = getFirstSolutionVect(i);
            clock_gettime(CLOCK_REALTIME, &end);
            writeWorseFirsSolve(i, timePassed(start, end));
            clock_gettime(CLOCK_REALTIME, &start);
            bool = checkSolution(solution, i);
            clock_gettime(CLOCK_REALTIME, &end);
            writeWorseVerF(i, timePassed(start, end));
            if (bool)
            {
                printf("the given solution (from the resolution algo) is valid for n = %d \n", i);
            }
            else
            {
                printf("the given solution (from the resolution algo) is not valid for n = %d \n", i);
            }
            if (solution != NULL)
                solution[0] = -1;
            clock_gettime(CLOCK_REALTIME, &start);
            bool = checkSolution(solution, i);
            clock_gettime(CLOCK_REALTIME, &end);
            writeBestVerF(i, timePassed(start, end));
            if (bool)
            {
                printf("the given solution (a wrong solution) is valid for n = %d \n", i);
            }
            else
            {
                printf("the given solution (a wrong solution) is not valid for n = %d \n", i);
            }
        }
        break;
    }
}
// gcc main.c -o main.exe && ./main.exe

