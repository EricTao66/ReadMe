#pragma once

#define SolNum 10
#define MaxIter 10000

//ͼ�ı�ʾ������ڵ�����ͱߣ�����ͼ���ڽӾ���  
typedef struct vNode
{
	int value;
	vNode* next;
}vNode;

//�ƶ�����ṹ��
typedef struct Move
{
	int u, vi, vj, delta;
}Move;

//���ڽӱ��в���һ�����ݣ�����֤�ڽӱ��������  
void insertIntoAdjVertics(vNode** adjVertics, int start, int end);
//��ӡ�ڽӱ� 
void displayAdjTable(int** adjTable, int* sol, int n, int k);
//��ӡ�ڽӱ�  
void displayAdjVertice(vNode** adjVertics, int* sol, int n, int k);
//��ʼ��
void initialization(Move &currentMove, Move **bestMove, Move **tabuBestMove, vNode ***adjVertics, int ***tabuTenure, int ***adjTable, int ***sol, int **s, int **s1ColNum, int **s2ColNum, int **sSelect, int n, int k);
//Ŀ�꺯��
int objFunction(vNode** adjVertics, int* sol, int n, int k);
//Ѱ���ƶ�����
void findMove(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int f, int bestF);
//ִ���ƶ��������ڽӱ����ɱ��Լ�Ŀ�꺯��ֵ
void makeMove(Move &currentMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int &f, int &bestF);
//��������
int tabuSearch(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &f);
//��������
void crossover(int *s1ColNum, int *s2ColNum, int *sSelect, int *s1, int *s2, int *s, int n, int k);
//�Ҹ�ĸ
void chooseParent(int &s1, int &s2, int **sol, int *f, int n);
//����Ⱥ��
void updatePopulation(vNode **adjVertics, int *s, int **sol, int fs, int *f, int n, int k);