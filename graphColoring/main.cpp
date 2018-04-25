#include <stdio.h>  
#include <stdlib.h>  
#include <cstring>
#include <ctime>
#include "graphColoring.h"
#pragma warning(disable:4996)
#pragma warning(disable:4700)

int main()
{
	int i = 0, j = 0, n, k, s1, s2, fs = 1, iter = 0;
	int f[SolNum];
	int *s = NULL;
	int **sol = NULL;
	int *s1ColNum = NULL;
	int *s2ColNum = NULL;
	int *sSelect = NULL;
	int **adjTable = NULL;
	int **tabuTenure = NULL;
	vNode **adjVertics = NULL;
	Move currentMove;
	Move *bestMove, *tabuBestMove;
	time_t seconds;

	srand(100);

	freopen("inputs.txt", "r", stdin);
	freopen("outputs.txt", "w", stdout);

	scanf("%d", &n);
	scanf("%d", &k);

	initialization(currentMove, &bestMove, &tabuBestMove, &adjVertics, &tabuTenure, &adjTable, &sol, &s1ColNum, &s2ColNum, &sSelect, &s, n, k);

	seconds = time(NULL);

	for (i = 0; i < SolNum; i++)
	{
		tabuSearch(currentMove, bestMove, tabuBestMove, adjVertics, tabuTenure, adjTable, sol[i], n, k, f[i]);
		printf("F%d(S) = %d \n", i, f[i]);
	}

	while (fs && iter < 1000)
	{
		chooseParent(s1, s2, sol, f, n);
		crossover(s1ColNum, s2ColNum, sSelect, sol[s1], sol[s2], s, n, k);
		tabuSearch(currentMove, bestMove, tabuBestMove, adjVertics, tabuTenure, adjTable, s, n, k, fs);
		updatePopulation(adjVertics, s, sol, fs, f, n, k);
		iter++;
	}

	seconds = time(NULL) - seconds;

	printf("-------------\n");
	for (i = 0; i < SolNum; i++)
	{
		printf("F%d(S) = %d \n", i, f[i]);
		if (f[i] == 0)
		{
			j = i;
		}
	}
	printf("Iter is %d, using %d second(s), iteration velocity is %d/s\n", iter*MaxIter, ((int)seconds), iter*MaxIter / ((int)seconds));
	displayAdjTable(adjTable, sol[j], n, k);
	displayAdjVertice(adjVertics, sol[j], n, k);

	return EXIT_SUCCESS;
}