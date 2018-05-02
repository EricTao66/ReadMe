#include <stdio.h>  
#include <stdlib.h>  
#include <cstring>
#include <algorithm>
#include "graphColoring.h"
#pragma warning(disable:4996)

//向邻接表中插入一个数据，并保证邻接表的有序性  
void insertIntoAdjVertics(vNode **adjVertics, int start, int end){
	vNode* node = (vNode*)malloc(sizeof(vNode));
	vNode *head = adjVertics[start];
	node->value = end;
	node->next = NULL;

	if (head == NULL) {
		adjVertics[start] = node;
		return;
	}
	if (head->next == NULL && head->value>end) {
		node->next = head;
		adjVertics[start] = node;
		return;
	}
	while (head->next != NULL && head->next->value<end) {
		head = head->next;
	}
	if (head->next == NULL) {
		head->next = node;
		return;
	}
	node->next = head->next;
	head->next = node;
}

//打印邻接冲突表 
void displayAdjTable(int adjTable[n][k], int sol[n]){
	int i, j;
	printf("\n");
	for (i = 0; i < n; ++i){
		printf("Vertex%-3d: ", i);
		for (j = 0; j < k; ++j){
			if (sol[i] == j){
				printf("[%3d]", adjTable[i][j]);
			}
			else{
				printf(" %3d ", adjTable[i][j]);
			}
		}
		printf("\n");
	}
	printf("-------------\n");
}

//打印邻接节点表  
void displayAdjVertice(vNode **adjVertics, int sol[n]){
	int i;
	vNode* head;
	printf("\n");
	for (i = 0; i < n; ++i){
		head = adjVertics[i];
		printf("[Vertex%-3d] Color%-2d Neighbor: ", i, sol[i]);
		while (head != NULL){
			printf("->[Ve%-3d] Co%-2d ", head->value, sol[head->value]);
			head = head->next;
		}
		printf("\n");
	}
	printf("-------------\n");
}

//初始化
void initialization(vNode ***adjVertics, int sol[SolNum][n]){
	int i, j, start, end;
	//初始化邻接链表  
	*adjVertics = (vNode**)malloc(sizeof(vNode*)*n);
	for (i = 0; i < n; ++i){
		(*adjVertics)[i] = NULL;
	}
	//输入定点，格式为 1 2  stop by -1
	while (1){
		scanf("%d", &start);
		if (start == -1){
			break;
		}
		scanf("%d", &end);
		insertIntoAdjVertics(*adjVertics, start - 1, end - 1);
		insertIntoAdjVertics(*adjVertics, end - 1, start - 1);
	}
	//初始化解数组
	for (i = 0; i < SolNum; ++i){
		for (j = 0; j < n; ++j){
			sol[i][j] = (int)(rand() % k);
		}
	}
}

//目标函数
int objFunction(vNode** adjVertics, int sol[n]){
	int i, sum = 0;
	vNode *head;
	for (i = 0; i < n; ++i){
		head = adjVertics[i];
		while (head != NULL){
			if (sol[i] == sol[head->value]){
				++sum;
			}
			head = head->next;
		}
	}
	sum /= 2;
	return sum;
}

//寻找移动方向
void findMove(Move &currentMove, vNode **adjVertics, int tabuTenure[n][k], int adjTable[n][k], int sol[n], int &iter, int f, int bestF){
	int i, j, delta, bestDelta = f + 1, tabuBestDelta = f + 1, foundFlag = 0;
	int sum = 0, tabuSum = 0;
	Move bestMove[n*k], tabuBestMove[n*k];
	currentMove.u = rand() % n;
	currentMove.vi = sol[currentMove.u];
	currentMove.vj = rand() % k;
	currentMove.delta = adjTable[currentMove.u][currentMove.vj] - adjTable[currentMove.u][currentMove.vi];
	for (i = 0; i < n; ++i){
		if (adjTable[i][sol[i]]){
			for (j = 0; j < k; ++j){
				if (j != sol[i]){
					delta = adjTable[i][j] - adjTable[i][sol[i]];
					if (iter >= tabuTenure[i][j]){
						if (delta < bestDelta){
							foundFlag = 1;
							bestDelta = delta;
							sum = 0;
							bestMove[sum].u = i;
							bestMove[sum].vi = sol[i];
							bestMove[sum].vj = j;
							bestMove[sum].delta = delta;
						}
						else if (delta == bestDelta){
							sum++;
							bestMove[sum].u = i;
							bestMove[sum].vi = sol[i];
							bestMove[sum].vj = j;
							bestMove[sum].delta = delta;
						}
					}
					else
					{
						if (delta < tabuBestDelta){
							tabuBestDelta = delta;
							tabuSum = 0;
							tabuBestMove[tabuSum].u = i;
							tabuBestMove[tabuSum].vi = sol[i];
							tabuBestMove[tabuSum].vj = j;
							tabuBestMove[tabuSum].delta = delta;
						}
						else if (delta == tabuBestDelta){
							tabuSum++;
							tabuBestMove[tabuSum].u = i;
							tabuBestMove[tabuSum].vi = sol[i];
							tabuBestMove[tabuSum].vj = j;
							tabuBestMove[tabuSum].delta = delta;
						}
					}
				}
			}
		}
	}
	if ((tabuBestDelta < bestDelta) && (f + tabuBestDelta < bestF)){
		currentMove = tabuBestMove[rand() % (tabuSum + 1)];
	}
	else if (foundFlag){
		currentMove = bestMove[rand() % (sum + 1)];
	}
}

//执行移动并更新邻接表、禁忌表以及目标函数值
void makeMove(Move &currentMove, vNode **adjVertics,int tabuTenure[n][k], int adjTable[n][k], int sol[n], int &iter, int &f, int &bestF){
	vNode* head = adjVertics[currentMove.u];
	sol[currentMove.u] = currentMove.vj;
	f += currentMove.delta;
	tabuTenure[currentMove.u][currentMove.vi] = iter + f + rand() % 10;
	while (head != NULL){
		++(adjTable[head->value][currentMove.vj]);
		--(adjTable[head->value][currentMove.vi]);
		head = head->next;
	}
	if (bestF > f){
		bestF = f;
	}
	++iter;
}

//禁忌搜索
int tabuSearch(vNode **adjVertics, int sol[n], int &f, int &tabuIter){
	int i, bestF, fLast, iter = 0, fNChangeTimes = 0;
	int adjTable[n][k] = { 0 }, tabuTenure[n][k] = { 0 };
	Move currentMove = { 0 };
	vNode *head = NULL;
	//初始化冲突表
	for (i = 0; i < n; ++i){
		head = adjVertics[i];
		while (head != NULL){
			++adjTable[i][sol[head->value]];
			head = head->next;
		}
	}
	f = objFunction(adjVertics, sol);
	bestF = f;
	fLast = f;
	while (f && fNChangeTimes < fNChangeTimesMax){
		findMove(currentMove, adjVertics, tabuTenure, adjTable, sol, iter, f, bestF);
		makeMove(currentMove, adjVertics, tabuTenure, adjTable, sol, iter, f, bestF);
		if (f < fLast){
			fNChangeTimes = 0;
		}
		else{
			++fNChangeTimes;
		}
		fLast = f;
		++tabuIter;
	}
	return f;
}

//交叉算子
void crossover(int s1[n], int s2[n], int s[n]){
	int i, j, l, s1MaxColor = 0, s1MaxSum = 0, s2MaxColor = 0, s2MaxSum = 0;
	int s1ColNum[k] = { 0 }, s2ColNum[k] = { 0 };
	int sSelect[n] = { 0 };
	for (i = 0; i < n; ++i) {
		++s1ColNum[s1[i]];
		++s2ColNum[s2[i]];
	}

	for (l = 0; l < k/2+1; ++l){
		s1MaxSum = 0;
		for (j = 0; j < k; ++j){
			if (s1ColNum[j] >= s1MaxSum){
				s1MaxSum = s1ColNum[j];
				s1MaxColor = j;
			}
		}
		for (i = 0; i < n; ++i){
			if (s1[i] == s1MaxColor && sSelect[i] == 0){
				sSelect[i] = 1;
				s[i] = s1MaxColor;
				--s2ColNum[s2[i]];
			}
		}
		s1ColNum[s1MaxColor] = 0;

		s2MaxSum = 0;
		for (j = 0; j < k; ++j){
			if (s2ColNum[j] >= s2MaxSum){
				s2MaxSum = s2ColNum[j];
				s2MaxColor = j;
			}
		}
		for (i = 0; i < n; ++i){
			if (s2[i] == s2MaxColor && sSelect[i] == 0){
				sSelect[i] = 1;
				s[i] = s2MaxColor;
				--s1ColNum[s1[i]];
			}
		}
		s2ColNum[s2MaxColor] = 0;
	}
	for (i = 0; i < k; ++i){
		if (sSelect[i] == 0){
			s[i] = rand() % k;
		}
	}
}

//找父母
void chooseParent(int &i1, int &i2, int sol[SolNum][n], int f[SolNum]){
	int i, sum = 0;
	int fSort[SolNum];
	int s[SolNum];
	std::copy(f, f + SolNum, fSort);
	std::sort(fSort, fSort + SolNum);
	if (fSort[0] != fSort[1]){
		if (fSort[1] != fSort[2]){
			for (i = 0; i < SolNum; ++i){
				if (f[i] == fSort[0]){
					i1 = i;
				}
				if (f[i] == fSort[1]){
					i2 = i;
				}
			}
		}
		else{
			for (i = 0; i < SolNum; ++i){
				if (f[i] == fSort[0]){
					i1 = i;
				}
				if (f[i] == fSort[1]){
					s[sum] = i;
					sum++;
				}
			}
			i2 = s[rand() % sum];
		}
	}
	else{
		for (i = 0; i < SolNum; ++i){
			if (f[i] == fSort[0]){
				s[sum] = i;
				sum++;
			}
		}
		std::random_shuffle(s, s + sum);
		i1 = s[0];
		i2 = s[1];
	}
}

//更新群体
void updatePopulation(int s[n], int sol[SolNum][n], int fs, int f[SolNum]){
	int i, fWorst = 0, iWorst;
	for (i = 0; i < SolNum; ++i){
		if (f[i] > fWorst){
			fWorst = f[i];
			iWorst = i;
		}
	}
	if (fs < fWorst){
		for (i = 0; i < n; ++i){
			sol[iWorst][i] = s[i];
		}
		f[iWorst] = fs;
	}
	else if (fs == fWorst && rand() % 2){
		for (i = 0; i < n; ++i){
			sol[iWorst][i] = s[i];
		}
		f[iWorst] = fs;
	}
}
