#pragma once

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
void initialization(Move &currentMove, Move **bestMove, Move **tabuBestMove, vNode ***adjVertics, int ***tabuTenure, int ***adjTable, int **sol, int n, int k, int &iter, int &f, int &bestF);
//目标函数
int objFunction(vNode** adjVertics, int* sol, int n, int k);
//寻找移动方向
void findMove(Move &currentMove, Move *bestMove, Move *tabuBestMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int f, int bestF);
//执行移动并更新邻接表、禁忌表以及目标函数值
void makeMove(Move &currentMove, vNode **adjVertics, int **tabuTenure, int **adjTable, int *sol, int n, int k, int &iter, int &f, int &bestF);