#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include "graphColoring.h"
#pragma warning(disable:4996)

void insertIntoAdjVertics(vNode** adjVertics, int start, int end)
{
	vNode* node = (vNode*)malloc(sizeof(vNode));
	vNode* head = adjVertics[start];

	node->value = end;
	node->next = NULL;

	if (head == NULL) 
	{
		adjVertics[start] = node;
		return;
	}

	if (head->next == NULL && head->value>end) 
	{
		node->next = head;
		adjVertics[start] = node;
		return;
	}

	while (head->next != NULL && head->next->value<end) 
	{
		head = head->next;
	}

	if (head->next == NULL) 
	{
		head->next = node;
		return;
	}

	node->next = head->next;
	head->next = node;
}

//打印邻接表 
void displayAdjTable(int** adjTable, int* sol, int n, int k)
{
	int i, j;
	printf("\n");
	for (i = 0; i < n; i++)
	{
		printf("Vertex%-3d: ", i);
		for (j = 0; j < k; j++)
		{
			if (sol[i] == j)
			{
				printf("[%3d]", adjTable[i][j]);
			}
			else
			{
				printf(" %3d ", adjTable[i][j]);
			}
		}
		printf("\n");
	}
	printf("-------------\n");
}

//打印邻接表  
void displayAdjVertice(vNode** adjVertics, int* sol, int n, int k)
{
	int i, j;
	vNode* head;
	printf("\n");
	for (i = 0; i < n; i++)
	{
		j = 0;
		head = adjVertics[i];
		printf("[Vertex%-3d] Color%-2d Neighbor: ", i, sol[i]);
		while (head != NULL)
		{
			j++;
			printf("->%d[Ve%-3d] Co%-2d ", j, head->value, sol[head->value]);
			head = head->next;
		}
		printf("\n");
	}
	printf("-------------\n");
}

//初始化
void initialization(Move &currentMove, Move **bestMove, Move **tabuBestMove, vNode ***adjVertics, int ***tabuTenure, int ***adjTable, int **sol, int n, int k,int &iter, int &f, int &bestF)
{
	int i, j, start, end;
	//初始化邻接链表  
	*adjVertics = (vNode**)malloc(sizeof(vNode*)*n);
	for (i = 0; i < n; i++)
	{
		(*adjVertics)[i] = NULL;
	}

	//输入定点，格式为 1 2  stop by -1
	while (1)
	{
		scanf("%d", &start);
		if (start == -1)
		{
			break;
		}
		scanf("%d", &end);
		insertIntoAdjVertics(*adjVertics, start - 1, end - 1);
		insertIntoAdjVertics(*adjVertics, end - 1, start - 1);
	}

	//初始化解数组
	*sol = (int*)malloc(sizeof(int)*n);
	for (i = 0; i < n; i++)
	{
		(*sol)[i] = (int)(rand() % k);
	}

	//初始化邻接表
	*adjTable = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
	{
		(*adjTable)[i] = (int*)malloc(sizeof(int)*k);
		for (j = 0; j < k; j++)
		{
			(*adjTable)[i][j] = 0;
		}
	}
	for (i = 0; i < n; i++)
	{
		vNode* head = (*adjVertics)[i];
		while (head != NULL)
		{
			++(*adjTable)[i][(*sol)[head->value]];
			head = head->next;
		}
	}

	//初始化禁忌表
	*tabuTenure = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
	{
		(*tabuTenure)[i] = (int*)malloc(sizeof(int)*k);
		for (j = 0; j < k; j++)
		{
			(*tabuTenure)[i][j] = 0;
		}
	}

	//初始化移动方向
	i = rand() % n;
	j = rand() % k;
	currentMove = { i,j,j,0 };

	*bestMove = (Move*)malloc(sizeof(Move)*n*k);
	for (i = 0; i < n*k; i++)
	{
		(*bestMove)[i] = { 0,0,0,0 };
	}
	*tabuBestMove = (Move*)malloc(sizeof(Move)*n*k);
	for (i = 0; i < n*k; i++)
	{
		(*tabuBestMove)[i] = { 0,0,0,0 };
	}

	iter = 0;
	f = objFunction(*adjVertics, *sol, n, k);
	bestF = f;
}

//目标函数
int objFunction(vNode** adjVertics, int* sol, int n, int k)
{
	int i, sum = 0;
	vNode* head;
	for (i = 0; i < n; i++)
	{
		head = adjVertics[i];
		while (head != NULL)
		{
			if (sol[i] == sol[head->value])
			{
				++sum;
			}
			head = head->next;
		}
	}
	sum /= 2;
	return sum;
}

//寻找移动方向
void findMove(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int f, int bestF)
{
	int i, j, delta, bestDelta = f + 1, tabuBestDelta = f + 1, foundFlag = 0;
	int sum = 0, tabuSum = 0;
	currentMove.u = rand() % n;
	currentMove.vi = sol[currentMove.u];
	currentMove.vj = rand() % k;
	currentMove.delta = adjTable[currentMove.u][currentMove.vj] - adjTable[currentMove.u][currentMove.vi];
	for (i = 0; i < n; i++)
	{
		if (adjTable[i][sol[i]])
		{
			for (j = 0; j < k; j++)
			{
				if (j != sol[i])
				{
					delta = adjTable[i][j] - adjTable[i][sol[i]];
					if (iter >= tabuTenure[i][j])
					{
						if (delta < bestDelta)
						{
							foundFlag = 1;
							bestDelta = delta;
							sum = 0;
							bestMove[sum].u = i;
							bestMove[sum].vi = sol[i];
							bestMove[sum].vj = j;
							bestMove[sum].delta = delta;
						}
						else if (delta == bestDelta)
						{
							sum++;
							bestMove[sum].u = i;
							bestMove[sum].vi = sol[i];
							bestMove[sum].vj = j;
							bestMove[sum].delta = delta;
						}
					}
					else
					{
						if (delta < tabuBestDelta)
						{
							tabuBestDelta = delta;
							tabuSum = 0;
							tabuBestMove[tabuSum].u = i;
							tabuBestMove[tabuSum].vi = sol[i];
							tabuBestMove[tabuSum].vj = j;
							tabuBestMove[tabuSum].delta = delta;
						}
						else if (delta == tabuBestDelta)
						{
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
	if ((tabuBestDelta < bestDelta) && (f + tabuBestDelta < bestF)) //特赦
	{
		tabuSum = rand() % (tabuSum + 1);
		currentMove = tabuBestMove[tabuSum];
	}
	else if (foundFlag)
	{
		sum = rand() % (sum + 1);
		currentMove = bestMove[sum];
	}
}

//执行移动并更新邻接表、禁忌表以及目标函数值
void makeMove(Move &currentMove, vNode **adjVertics,int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int &f, int &bestF)
{
	vNode* head = adjVertics[currentMove.u];
	sol[currentMove.u] = currentMove.vj;
	f += currentMove.delta;
	tabuTenure[currentMove.u][currentMove.vi] = iter + f + rand() % 10;
	while (head != NULL)
	{
		++(adjTable[head->value][currentMove.vj]);
		--(adjTable[head->value][currentMove.vi]);
		head = head->next;
	}
	if (bestF > f)
	{
		bestF = f;
	}
	iter++;
}

