#pragma once

#define SolNum 10
#define MaxIter 10000

//图的表示，输入节点个数和边，构造图的邻接矩阵  
typedef struct vNode
{
	int value;
	vNode* next;
}vNode;

//移动方向结构体
typedef struct Move
{
	int u, vi, vj, delta;
}Move;

//向邻接表中插入一个数据，并保证邻接表的有序性  
void insertIntoAdjVertics(vNode** adjVertics, int start, int end);
//打印邻接表 
void displayAdjTable(int** adjTable, int* sol, int n, int k);
//打印邻接表  
void displayAdjVertice(vNode** adjVertics, int* sol, int n, int k);
//初始化
void initialization(Move &currentMove, Move **bestMove, Move **tabuBestMove, vNode ***adjVertics, int ***tabuTenure, int ***adjTable, int ***sol, int **s, int **s1ColNum, int **s2ColNum, int **sSelect, int n, int k);
//目标函数
int objFunction(vNode** adjVertics, int* sol, int n, int k);
//寻找移动方向
void findMove(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int f, int bestF);
//执行移动并更新邻接表、禁忌表以及目标函数值
void makeMove(Move &currentMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int &f, int &bestF);
//禁忌搜索
int tabuSearch(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &f);
//交叉算子
void crossover(int *s1ColNum, int *s2ColNum, int *sSelect, int *s1, int *s2, int *s, int n, int k);
//找父母
void chooseParent(int &s1, int &s2, int **sol, int *f, int n);
//更新群体
void updatePopulation(vNode **adjVertics, int *s, int **sol, int fs, int *f, int n, int k);