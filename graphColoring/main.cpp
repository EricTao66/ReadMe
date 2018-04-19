#include <stdio.h>  
#include <stdlib.h>  
#include <cstring>
#include <ctime>
#include "graphColoring.h"
#pragma warning(disable:4996)
#pragma warning(disable:4700)

int main()
{
	int n, k, f, bestF, iter;
	int* sol = NULL;
	int** adjTable = NULL;
	int** tabuTenure = NULL;
	vNode** adjVertics = NULL;
	Move currentMove;
	Move *bestMove, *tabuBestMove;
	time_t seconds;

	srand(10);

	freopen("inputs.txt", "r", stdin);
	freopen("outputs.txt", "w", stdout);

	scanf("%d", &n);
	scanf("%d", &k);

	initialization(currentMove, &bestMove, &tabuBestMove, &adjVertics, &tabuTenure, &adjTable, &sol, n, k, iter, f, bestF);

	seconds = time(NULL);

	while (f)
	{
		findMove(currentMove, bestMove, tabuBestMove, adjVertics, tabuTenure, adjTable, sol, n, k, iter, f, bestF);
		makeMove(currentMove, adjVertics, tabuTenure, adjTable, sol, n, k, iter, f, bestF);
		if (iter > 25000000)break;
	}

	seconds = time(NULL) - seconds;

	displayAdjTable(adjTable, sol, n, k);
	displayAdjVertice(adjVertics, sol, n, k);
	printf("F(S) is %d, iter is %d, using %d second(s), iteration velocity is %dK/s\n", objFunction(adjVertics, sol, n, k), iter, ((int)seconds), iter/1000/((int)seconds));
	printf("-------------\n");

	return EXIT_SUCCESS;
}