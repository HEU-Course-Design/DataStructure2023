#include<stdio.h>
#include<stdlib.h>
#include "5SavagesAndMonks.h"
#include "5defs.h"


bool isSafe(Boat B) { //检测当前修道士是否安全
	if ((B.Monk >= B.Savage || B.Monk == 0) && 
		((N - B.Monk) >= (N - B.Savage) || N == B.Monk) && B.Monk >= 0 && B.Monk <= N && B.Savage >= 0 && B.Savage <= N)
		return true;
	else
		return false;
}

void solveOne(Algraph* G, int n) { //输出一种可行方案
	Boat T[1000];
	int cnt = 0;
	while (n >= 0) {
		T[cnt++] = G->vertices[n].data;
		n = G->vertices[n].pre;
	}
	for (int i = cnt - 1; i >= 0; i--) {
		printf("( %d %d %d )", T[i].Monk, T[i].Savage, T[i].state);
		if (!(T[i].Monk == 0 && T[i].Savage == 0 && T[i].state == 0)) {
			if (T[i].state == 1)
				printf(" -->-->-->-->[%d %d]-->-->-->--> (%d %d 0)\n", T[i].Monk - T[i - 1].Monk, T[i].Savage - T[i - 1].Savage, T[i - 1].Monk, T[i - 1].Savage);
			else
				printf(" -->-->-->-->[%d %d]-->-->-->-->(%d %d 1)\n", (T[i].Monk - T[i - 1].Monk) * (-1), (-1) * (T[i].Savage - T[i - 1].Savage), T[i - 1].Monk, T[i - 1].Savage);
		}
		else
			printf("\n");
	}
	system("pause");
	// system("cls");
}

int solve(Boat B) {//生成可行的行船方案，便于后续的bfs进行
	int cnt = 0, flag;
	if (B.state == 1)flag = 1;
	else flag = -1;

	for (int i = 1; i <= C; i++)
		for (int j = 0; j <= C; j++) {
			if (i >= j && ((i + j) <= C)) {
				Boatsafe[cnt].Monk = i * flag;
				Boatsafe[cnt++].Savage = j * flag;
			}
		}
	for (int i = 1; i <= C; i++) {
		Boatsafe[cnt].Monk = 0;
		Boatsafe[cnt++].Savage = i * flag;
	}
	return cnt;
}

void bfs(Algraph* G) {
	Boat tmp;
	int total, flag, cnt = 0, n = 0, i = 0, j = 0;
	while (G->vertices[i].data.state != -1) {

		total = solve(G->vertices[i].data);//生成可行行船方案

		for (int t = 0; t < total; t++) { //挨个方案尝试并建立邻接表
			tmp.Monk = G->vertices[i].data.Monk - Boatsafe[t].Monk;
			tmp.Savage = G->vertices[i].data.Savage - Boatsafe[t].Savage;
			tmp.state = 1 - G->vertices[i].data.state;
			if (isSafe(tmp)) {
				flag = 1;
				j = i;//标记已有顶点数量

				while (j >= 0) { //避免出现重复的状态
					if (tmp.Monk == G->vertices[j].data.Monk && tmp.Savage == G->vertices[j].data.Savage && tmp.state == G->vertices[j].data.state) {
						flag = 0;
						break;
					}
					j--;
				}
				if (flag) { //可行状态插入到邻接表
					n++;
					G->vertices[n].pre = i;
					G->insert(n, tmp);
					G->insertEdge(i, n);

					if (tmp.Monk == 0 && tmp.Savage == 0 && tmp.state == 0) { //目标状态输出可行方案
						cnt++;
						printf("第%d种方案:\n", cnt);
						solveOne(G, n);

					}
				}
			}
		}
		i++;
	}

	if (cnt == 0)
		printf("没有可行的方案可以渡河！");
}

int main(int argc , char ** argv) {
	printf("请输入修道士和野人的数量：");
	scanf("%d", &N);
	printf("请输入小船可乘人数：");
	scanf("%d", &C);

	Boat start;
	Algraph G;
	start.Monk = N;
	start.Savage = N;
	start.state = 1;
	G.insert(0, start);//插入第一个顶点 

	bfs(&G);

	return 0;
}
