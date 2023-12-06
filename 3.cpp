#include<stdio.h>

const int N = 10000;
int array[N][N] = {0};  //将二维数组先全部初始化为零

void MagicPrint(int m ){

	array[0][m / 2] = 1;  //首先将1写在第一行的中间
	int row = 0;  //初始化行row
	int col = m / 2;  //初始化列column 
    int num;

	for(num = 2 ; num <= m * m ; ++num){
		row = (row - 1 + m) % m;  //上移
		col = (col + 1) % m;  //右移 
		if(array[row][col] != 0){  //判断该格子是否被别的数占了。
			row = (row + 2) % m;  //下移 
			col = (col - 1 + m) % m;  //左移 
		}
		array[row][col] = num ;  //依次编号 
	}

    int i,j;
	for(i = 0 ; i < m ; i++){
		for(j = 0 ; j < m ;j++) printf("%-4d ", array[i][j]);  //输出的时候每个数字占4个位置
		printf("\n");  //输出二维矩阵 
    }

}
int main(){
    int m;
    printf("请输入一个奇数:");
    scanf("%d",&m);
    if(m%2!=0){
        printf("得到的魔方阵为:\n");
        MagicPrint(m);
    }
    else printf("请重新输入一个奇数!");
    return 0;
}