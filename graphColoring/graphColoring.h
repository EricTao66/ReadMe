#pragma once

#define n 500
#define k 49
#define SolNum 8
#define fNChangeTimesMax 150

//ͼ�ı�ʾ������ڵ�����ͱߣ�����ͼ���ڽӾ���  
typedef struct vNode{
	int value;
	vNode* next;
}vNode;

//�ƶ�����ṹ��
typedef struct Move{
	int u, vi, vj, delta;
}Move;

//���ڽӱ��в���һ�����ݣ�����֤�ڽӱ��������  
void insertIntoAdjVertics(vNode **adjVertics, int start, int end);
//��ӡ�ڽӳ�ͻ�� 
void displayAdjTable(int adjTable[n][k], int sol[n]);
//��ӡ�ڽӽڵ��  
void displayAdjVertice(vNode **adjVertics, int sol[n]);
//��ʼ��
void initialization(vNode ***adjVertics, int sol[SolNum][n]);
//Ŀ�꺯��
int objFunction(vNode** adjVertics, int sol[n]);
//Ѱ���ƶ�����
void findMove(Move &currentMove, vNode **adjVertics, int tabuTenure[n][k], int adjTable[n][k], int sol[n], int &iter, int f, int bestF);
//ִ���ƶ��������ڽӱ����ɱ��Լ�Ŀ�꺯��ֵ
void makeMove(Move &currentMove, vNode **adjVertics, int tabuTenure[n][k], int adjTable[n][k], int sol[n], int &iter, int &f, int &bestF);
//��������
int tabuSearch(vNode **adjVertics, int sol[n], int &f, int &tabuIter);
//��������
void crossover(int s1[n], int s2[n], int s[n]);
//�Ҹ�ĸ
void chooseParent(int &i1, int &i2, int sol[SolNum][n], int f[SolNum]);
//����Ⱥ��
void updatePopulation(int s[n], int sol[SolNum][n], int fs, int f[SolNum]);