
#include <stdio.h>
#include <math.h>
#include <string.h>
 
//计数真值结果
void CountTF(char c, int* countT, int*countF)
{
	if(c == 'T')
	{
		(*countT)++;
	}
	else if(c == 'F')	//考虑到空白字符等干扰
	{
		(*countF)++;
	}
}
 
//输出真值表
void OutPutTruthTable(char** truthTable, char* valueRet, int col, int row)
{	
	char ch = 'P';
	for(int i = 1; i <= col; i++)	//n 相当于列
	{
		printf("%-10c",ch++);
		if(i == col)
		{
			printf("%-10c\n",'A');
		}
	}
	
	for(int i = 1; i <= col; i++)
	{
		printf( "-----------" );
	}
	printf("\n");
	
	ch = 'P';   	//用P等字母代替变元
	
	for(int i = 1; i <= row; i++)
	{
		for(int j = col; j >= 1; j--)
		{
			if(*((int *)truthTable +i*col +j) == 1)	//二级指针调用
			{
				printf("%-10c",'T');
			}
			else
			{
				printf("%-10c",'F');				
			}
		}
		printf("%-10c\n",valueRet[i]);
	}
}
 
//主析取范式
void MasterDisjunction(char** truthTable, char* valueRet, int col, int row,int countT,int* count)
{
	for(int i = 1; i <= row; i++)
	{
		char ch = 'P';
		if(valueRet[i] == 'T')
		{
			(*count)++;
			
			printf("(");
			
			for(int j = col; j >= 1; j--)
			{				
				if(*((int *)truthTable +i*col +j) == 1)
				{
					printf("%c",ch++);
				}
				else
				{
					printf( "┓%c",ch++);
				}
				
				if(j != 1)
				{
					printf( "∧");
				}
			}
			
			printf( ")");
			
			if((*count) < countT)
			{
				printf("∨");
			}
		}
	}
}
 
//主合取范式
void MasterConjunction(char** truthTable, char* valueRet, int col, int row,int countF,int* count)
{
	for(int i = 1; i <= row; i++)
	{
		char ch = 'P';
		if(valueRet[i] == 'F')
		{
			(*count)++;
			
			printf("(");
			
			for(int j = col; j >= 1; j--)
			{
				if(*((int *)truthTable +i*col +j) == 0)
				{
					printf("%c",ch++) ;
				}
				else
				{
					printf("┓%c", ch++);
				}
				if(j != 1){
					printf( """");
				}
			}
			
			printf(")");
			
			if((*count) < row)
			{
				printf("∧");
			}	
		}
	}
}
 
 
int main()
{
	//1 输入变量个数
	int n = 0;	//变量个数
	printf("请输入变量个数:> ");
	scanf("%d",&n);
	
	//2 输入真值结果
	int totalNumber=(int)pow(2,n);
	//共2^n个真值结果
	
	char valueRet[totalNumber+1];		//存放输入的真值结果
	memset(valueRet,0,sizeof(valueRet));
	
	int countT = 0;	
	int countF = 0; 
    printf("请输入%n个结果：",totalNumber);
	for(int i = 1; i <= totalNumber; i++)	//从1开始，和后面对齐
	{
		scanf("%c",&valueRet[i]);
		CountTF(valueRet[i], &countT, &countF);
	}
	
	//测试代码
	//cout << countT;
	//cout << countF;
	
	printf("\n");
	
	//3 真值表赋值
	int truthTable[totalNumber+1][n+1];	//真值表数组
	memset(truthTable, 0, sizeof(truthTable));	//初始化	
	int col = 0;	
	int row = col + 1;
	int i = totalNumber;
	
	//真值表命题变元赋值
	//将0到2^n-1的所有数转换为2进制
	//用行标控制0到2^n-1这些数值
	while(i--)
	{
		int truthValue = i;
		while(col < n)
		{
			truthTable[row][++col] = truthValue % 2;	//存入1或0
			truthValue /= 2;
		}
		row++;
		col = 0;	//注意每一行都要从首列开始赋值
	}
	
	//5 主析取范式输出
	int count = 0;
	printf( "主析取范式:\n");
	MasterDisjunction((char**)truthTable,valueRet,n,totalNumber,countT,&count);
	printf("\n");
	
	//6 主合取范式输出
	printf( "主合取范式:\n");
	MasterConjunction((char**)truthTable,valueRet,n,totalNumber,countF,&count);
	printf("\n");
	
	return 0;
}