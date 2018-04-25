#include <stdio.h>  
#include <stdlib.h>  
#include <cstring>
#include <algorithm>
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

//��ӡ�ڽӱ� 
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

//��ӡ�ڽӱ�  
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

//��ʼ��
void initialization(Move &currentMove, Move **bestMove, Move **tabuBestMove, vNode ***adjVertics, int ***tabuTenure, int ***adjTable, int ***sol, int **s, int **s1ColNum, int **s2ColNum, int **sSelect, int n, int k)
{
	int i, j, start, end;
	//��ʼ���ڽ�����  
	*adjVertics = (vNode**)malloc(sizeof(vNode*)*n);
	for (i = 0; i < n; i++)
	{
		(*adjVertics)[i] = NULL;
	}

	//���붨�㣬��ʽΪ 1 2  stop by -1
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

	//��ʼ��������
	*sol = (int**)malloc(sizeof(int)*SolNum);
	for (i = 0; i < SolNum; i++)
	{
		(*sol)[i] = (int*)malloc(sizeof(int*)*n);
		for (j = 0; j < n; j++)
		{
			(*sol)[i][j] = (int)(rand() % k);
		}
	}

	*s = (int*)malloc(sizeof(int)*n);
	for (i = 0; i < n; i++)
	{
		(*s)[i] = 0;// (int)(rand() % k);
	}
	
	//��ʼ����ͻ��
	*adjTable = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
	{
		(*adjTable)[i] = (int*)malloc(sizeof(int)*k);
		for (j = 0; j < k; j++)
		{
			(*adjTable)[i][j] = 0;
		}
	}

	//��ʼ�����ɱ�
	*tabuTenure = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
	{
		(*tabuTenure)[i] = (int*)malloc(sizeof(int)*k);
		for (j = 0; j < k; j++)
		{
			(*tabuTenure)[i][j] = 0;
		}
	}

	//��ʼ���ƶ�����
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

	*s1ColNum = (int*)malloc(sizeof(int)*k);
	*s2ColNum = (int*)malloc(sizeof(int)*k);
	*sSelect = (int*)malloc(sizeof(int)*n);
	for (j = 0; j < k; j++)
	{
		(*s1ColNum)[j] = 0;
		(*s2ColNum)[j] = 0;
	}
	for (i = 0; i < n; i++)
	{
		(*sSelect)[i] = 0;
	}
}

//Ŀ�꺯��
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

//Ѱ���ƶ�����
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
	if ((tabuBestDelta < bestDelta) && (f + tabuBestDelta < bestF)) //����
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

//ִ���ƶ��������ڽӱ����ɱ��Լ�Ŀ�꺯��ֵ
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

//��������
int tabuSearch(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &f)
{
	int i, j, bestF, iter = 0;
	vNode* head;
	//��ʼ����ͻ��
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < k; j++)
		{
			adjTable[i][j] = 0;
		}
	}
	for (i = 0; i < n; i++)
	{
		head = adjVertics[i];
		while (head != NULL)
		{
			++adjTable[i][sol[head->value]];
			head = head->next;
		}
	}
	//��ʼ�����ɱ�
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < k; j++)
		{
			tabuTenure[i][j] = 0;
		}
	}
	f = objFunction(adjVertics, sol, n, k);
	bestF = f;
	iter = 0;
	while (f && iter < MaxIter)
	{
		findMove(currentMove, bestMove, tabuBestMove, adjVertics, tabuTenure, adjTable, sol, n, k, iter, f, bestF);
		makeMove(currentMove, adjVertics, tabuTenure, adjTable, sol, n, k, iter, f, bestF);
	}
	return f;
}

//��������
void crossover(int *s1ColNum, int *s2ColNum, int *sSelect, int *s1, int *s2, int *s, int n, int k)
{
	int i, j, l, sum, s1Max=0, s1MaxSum=0, s2Max=0, s2MaxSum=0;
	
	for (j = 0; j < k; j++)
	{
		s1ColNum[j] = 0;
		s2ColNum[j] = 0;
	}
	for (i = 0; i < n; i++)
	{
		sSelect[i] = 0;
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < k; j++)
		{
			if (s1[i] == j)
			{
				++s1ColNum[j];
			}
			if (s2[i] == j)
			{
				++s2ColNum[j];
			}
		}
	}

	for (l = 0; l < k/2+1; l++)
	{
		s1MaxSum = 0;
		for (j = 0; j < k; j++)
		{
			if (j % 2 == 1 && s1ColNum[j] >= s1MaxSum)
			{
				s1MaxSum = s1ColNum[j];
				s1Max = j;
			}
		}
		for (i = 0; i < n; i++)
		{
			if (s1[i] == s1Max && sSelect[i] == 0)
			{
				sSelect[i] = 1;
				s[i] = s1Max;
				for (j = 0; j < k; j++)
				{
					if (s2[i] == j)
					{
						--s2ColNum[j];
					}
				}
			}
		}
		s1ColNum[s1Max] = 0;

		s2MaxSum = 0;
		for (j = 0; j < k; j++)
		{
			if (j % 2 == 0 && s2ColNum[j] >= s2MaxSum)
			{
				s2MaxSum = s2ColNum[j];
				s2Max = j;
			}
		}
		for (i = 0; i < n; i++)
		{
			if (s2[i] == s2Max && sSelect[i] == 0)
			{
				sSelect[i] = 1;
				s[i] = s2Max;
				for (j = 0; j < k; j++)
				{
					if (s1[i] == j)
					{
						--s1ColNum[j];
					}
				}
			}
		}
		s2ColNum[s2Max] = 0;
	}
	for (i = 0; i < k; i++)
	{
		if (sSelect[i] == 0)
		{
			s[i] = rand() % k;
		}
	}
}

//�Ҹ�ĸ
void chooseParent(int &s1, int &s2, int **sol, int *f, int n)
{
	int i, fBest1 = 100000, fBest2 = 100000, sum = 0;
	int fSort[SolNum];
	int s[SolNum];
	std::copy(f, f + SolNum, fSort);
	std::sort(fSort, fSort + SolNum);
	if (fSort[0] != fSort[1])
	{
		if (fSort[1] != fSort[2])
		{
			for (i = 0; i < SolNum; i++)
			{
				if (f[i] == fSort[0])
				{
					s1 = i;
				}
				if (f[i] == fSort[1])
				{
					s2 = i;
				}
			}
		}
		else
		{
			for (i = 0; i < SolNum; i++)
			{
				if (f[i] == fSort[0])
				{
					s1 = i;
				}
				if (f[i] == fSort[1])
				{
					s[sum] = i;
					sum++;
				}
			}
			s2 = s[rand() % sum];
		}
	}
	else
	{
		for (i = 0; i < SolNum; i++)
		{
			if (f[i] == fSort[0])
			{
				s[sum] = i;
				sum++;
			}
		}
		std::random_shuffle(s, s + sum);
		s1 = s[0];
		s2 = s[1];
	}
}

//����Ⱥ��
void updatePopulation(vNode **adjVertics, int *s, int **sol, int fs, int *f, int n, int k)
{
	int i, fWorst = 0, iWorst;
	for (i = 0; i < SolNum; i++)
	{
		if (f[i] > fWorst)
		{
			fWorst = f[i];
			iWorst = i;
		}
	}
	if (fs < fWorst)
	{
		for (i = 0; i < n; i++)
		{
			sol[iWorst][i] = s[i];
		}
		f[iWorst] = fs;
	}
	else if (fs == fWorst && rand() % 2 == 0)
	{
		for (i = 0; i < n; i++)
		{
			sol[iWorst][i] = s[i];
		}
		f[iWorst] = fs;
	}
}
