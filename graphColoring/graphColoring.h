#pragma once

#define n 500
#define k 49
#define SolNum 8
#define fNChangeTimesMax 150

//图的表示，输入节点个数和边，构造图的邻接矩阵  
typedef struct vNode{
	int value;
	vNode* next;
}vNode;

//移动方向结构体
typedef struct Move{
	int u, vi, vj, delta;
}Move;

//向邻接表中插入一个数据，并保证邻接表的有序性  
void insertIntoAdjVertics(vNode **adjVertics, int start, int end);
//打印邻接冲突表 
void displayAdjTable(int adjTable[n][k], int sol[n]);
//打印邻接节点表  
void displayAdjVertice(vNode **adjVertics, int sol[n]);
//初始化
void initialization(vNode ***adjVertics, int sol[SolNum][n]);
//目标函数
int objFunction(vNode** adjVertics, int sol[n]);
//寻找移动方向
void findMove(Move &currentMove, vNode **adjVertics, int tabuTenure[n][k], int adjTable[n][k], int sol[n], int &iter, int f, int bestF);
//执行移动并更新邻接表、禁忌表以及目标函数值
void makeMove(Move &currentMove, vNode **adjVertics, int tabuTenure[n][k], int adjTable[n][k], int sol[n], int &iter, int &f, int &bestF);
//禁忌搜索
int tabuSearch(vNode **adjVertics, int sol[n], int &f, int &tabuIter);
//交叉算子
void crossover(int s1[n], int s2[n], int s[n]);
//找父母
void chooseParent(int &i1, int &i2, int sol[SolNum][n], int f[SolNum]);
//更新群体
void updatePopulation(int s[n], int sol[SolNum][n], int fs, int f[SolNum]);