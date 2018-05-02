#include <stdio.h>  
#include <stdlib.h>  
#include <cstring>
#include <ctime>
#include "graphColoring.h"
#pragma warning(disable:4996)
#pragma warning(disable:4700)

int main(){
	int i = 0, j = 0, i1 = 0, i2 = 0, evoIter = 0, tabuIter = 0;
	int fs = 1, f[SolNum] = { 0 };
	int s[n] = { 0 }, sol[SolNum][n] = { 0 };
	vNode **adjVertics = NULL;
	time_t seconds = time(NULL);

	freopen("inputs.txt", "r", stdin);
	freopen("outputs.txt", "w", stdout);
	//srand(seconds);
	srand(1525266756); //17s
	initialization(&adjVertics, sol);

	//printf("-------------\n");
	for (i = 0; i < SolNum; ++i){
		tabuSearch(adjVertics, sol[i], f[i], tabuIter);
		//printf("F%d(S) = %d \n", i, f[i]);
	}

	while (fs && evoIter < 10000){
		chooseParent(i1, i2, sol, f);
		crossover(sol[i1], sol[i2], s);
		//printf("\n Parent1 F%d(S) = %d \n Parent2 F%d(S) = %d \n Spring F(S) = %d \n", i1, f[i1], i2, f[i2], objFunction(adjVertics, s));
		tabuSearch(adjVertics, s, fs, tabuIter);
		//printf(" LocalSearch F(S) = %d \n", fs);
		updatePopulation(s, sol, fs, f);
		++evoIter;
	}

	printf("-------------\n");
	for (i = 0; i < SolNum; ++i){
		printf("F%d(S) = %d \n", i, f[i]);
		if (f[i] == 0){
			j = i;
		}
	}
	printf("-------------\n");
	printf("srand() seed is %lld.\n", 1525266756);
	seconds = time(NULL) - seconds;
	printf("Total iter is %d, using %d second(s), iteration velocity is %d K/s.\n", tabuIter, ((int)seconds), (int)tabuIter / 1000 / ((int)seconds));
	//displayAdjVertice(adjVertics, sol[j]);

	return EXIT_SUCCESS;
}