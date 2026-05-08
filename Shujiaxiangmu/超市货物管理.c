#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#define LEN sizeof(struct shouhuo)


struct huowu
{
    char ID[10];//货号
    char brand[10];//名称
    double inprice;
    double outprice1,outprice2;//原定售价，实际售价
    int storage;//存储量
};
//存储货物信息的结构体
struct shouhuo
{
    char ID[10];
    int num;
    struct shouhuo *next;    
};
//存储出售的货物信息的结构体

int N;//文件里货物总种类
struct huowu goods[1000];//仓库
struct huowu inhuo[1000];//进货
struct huowu youhui[1000];//优惠
struct huowu chahuo[1000];
double ri[31]={0};//日收入
double yue[12]={0};//月收入
double nian[3][12]={0};//三年年收入
int a,nian0,yue0,ri0;//是否为第一次使用系统的判断标准，第一次使用系统的年月日
int year,month,day;//记录当前数据的年月日
double rishou;//计算当日日收入

int runnian(int a);
int menu();//菜单
void Cangku();//库存1
void Xiancun();//显示库存信息2
void Inhuo();//进货信息3
void Youhui();//今日优惠4
void Chushou();//今日出售情况5
void Chaxun();//查询某种货物信息6
void Rishouru();//本月日收入7
void Yueshouru();//本年月收入8
void Nianshouru();//近三年年收入9
void Jiahuo();//添加新货物库存10
void Cunchuqian();//计算日收入，并进行按日期进行存储

int main()//主函数
{
    
    printf("**********************************\n");
    printf("      欢迎使用超市信息管理系统      \n");
    printf("**********************************\n");
    year=0;
    printf("是否为第一次使用该系统：1.是 2.否\n");
    scanf("%d",&a);
    if(a==1)
    { printf("请输入初次使用时间（年 月 日）：");
        scanf("%d%d%d",&nian0,&yue0,&ri0);
        year=0;
    }
    //记录第一次使用系统的时间，方便以时间进行统计信息
    a=runnian(nian0);
    //记录判断闰年的结果
    while(1)
    {
        switch(menu())
        {
            case 1: Cangku(); break;
            case 2: Xiancun(); break;
            case 3: Inhuo(); break;
            case 4: Youhui(); break;
            case 5: Chushou(); break;
            case 6: Chaxun();break;
            case 7: Rishouru(); break;
            case 8: Yueshouru(); break;
            case 9: Nianshouru(); break;
            case 10: Jiahuo(); break;
            case 11: exit(0);
        }
    }
    return 0;
}

int runnian(int q)
{
    if((q/100)==0||(q/4)==0&&(q/400)!=0)
        return 2;
    else
        return 3;
}
//计算每年是否为闰年，确定每年2月的天数

int menu()
{
    int n;
    printf("\n请输入操作前数字进行操作：\n");
    printf("1.建立仓库信息\n");
    printf("2.显示仓库信息\n");
    printf("3.今日进货信息\n");
    printf("4.今日优惠信息\n");
    printf("5.今日出售情况\n");
    printf("6.查询特定货物信息\n");
    printf("7.查看本月日收入\n");
    printf("8.查看本年月收入\n");
    printf("9.查看近三年收入情况\n");
    printf("10.添加新货物信息\n");
    printf("11.退出\n");
    scanf("%d",&n);
    
    return n;
}//菜单

void Cangku()//建立库存文件
{
    FILE *fp;
    int i;
    printf("请输入货物种类数：");
    scanf("%d",&N);
    //确定循环输入货物信息的次数
    printf("\n请按指示输入货物信息：\n");
    for(i=0;i<N;i++)
    {
        printf("第%d种货物：\n",i+1);
        printf("货号：");
        scanf("%s",goods[i].ID);

        printf("品名：");
        scanf("%s",goods[i].brand);

        printf("进价：");
        scanf("%lf",&goods[i].inprice);

        printf("售价：");
        scanf("%lf",&goods[i].outprice1);
        goods[i].outprice2=0;

        printf("数量：");
        scanf("%d",&goods[i].storage);
        printf("\n");
    }
    //依次输入货物货号、品名、进价、售价、数量
    if((fp=fopen("kucun.txt","a+"))==NULL)
    {
        printf("创建文件失败\n");
        return ;
    }
    //创建存储货物信息的文件
    for(i=0;i<N;i++)
    {
        fprintf(fp,"%-11s,%-15s,%-8.3lf,%-8.3lf,%-7d\n",goods[i].ID,goods[i].brand,goods[i].inprice,goods[i].outprice2,goods[i].storage);
    }
    //将货物信息存储在文件中
    fclose(fp);
    //关闭该文件
    printf("库存信息创建成功\n");
    return ;
}

void Xiancun()//显示库存信息
{
    FILE *file;
    if((file=fopen("kucun.txt","r"))==NULL)
    {
        printf("目标文件打开失败\n");
        return ;
    }
    //打开库存文件
    int i;
    for(i=0;i<N;i++)
    {
        fscanf(file,"%s,%s,%lf,%lf,%d",goods[i].ID, goods[i].brand, &goods[i].inprice, &goods[i].outprice1, &goods[i].storage);  // bug
        printf("*********************************\n");
        printf("货号       品名           进价    售价    库存量\n");
        printf("%-11s%-15s%-8.3lf%-8.3lf%-7d\n",goods[i].ID,goods[i].brand,goods[i].inprice,goods[i].outprice1,goods[i].storage);
    }
    //循环输出各货物信息
    fclose(file);
    //关闭该文件
}

void Jiahuo()//记录从未有过的新货物数据
{
    int N1;
    FILE *fp;
    printf("请输入新货物总种类数：");
    scanf("%d",&N1);
    N1=N1+N;
    //记录原有货物加上新货物的总种类数
    printf("\n请按指示输入货物信息：\n");
    for(int i=N;i<N1;i++)
    {
        printf("第%d种货物：\n",i+1);
        printf("货号：");
        scanf("%s",goods[i].ID);

        printf("品名：");
        scanf("%s",goods[i].brand);

        printf("进价：");
        scanf("%lf",&goods[i].inprice);

        printf("售价：");
        scanf("%lf",&goods[i].outprice1);
        goods[i].outprice2=0;

        printf("数量：");
        scanf("%d",&goods[i].storage);
        printf("\n");
    }
    //输入新货物信息
    if((fp=fopen("kucun.txt","w"))==NULL)
    {
        printf("创建文件失败\n");
        return ;
    }
    //打开库存文件
    for(int i=N;i<N1;i++)
    {
        fprintf(fp,"%-11s%-15s%-8.3lf%-8.3lf%-7d\n",goods[i].ID,goods[i].brand,goods[i].inprice,goods[i].outprice2,goods[i].storage);
    }
    //将新货物信息输入进文件中
    fclose(fp);
    N=N1;
    //更新库存总货物种类数
    printf("货物信息更新成功\n");
    return ;
}

void Inhuo()//添加某些原有货物的数量
{
    FILE *fp,*fpp;
    printf("请输入进货信息种类总数：\n");
    int j;
    scanf("%d",&j);
    //输入有进货需求的货物总类
    int i,n;
    for(i=0;i<j;i++)
    {
        printf("第%d种货物：\n",i+1);
        printf("货号：");
        scanf("%s",inhuo[i].ID);
        printf("进货数量：");
        scanf("%d",&inhuo[i].storage);
        printf("\n");
    }
    //输入进货货物数量信息
    if((fp=fopen("jinhuo.txt","w"))==NULL)
    {
        printf("进货信息创建失败\n");
        return ;
    }
    //创建进货文件
    for(i=0;i<j;i++)
    {
        fprintf(fp,"%-11s%-7d\n",inhuo[i].ID,inhuo[i].storage);
    }
    //将数据输入到文件中
    fclose(fp);
    if((fp=fopen("jinhuo.txt","w"))==NULL)
    {
        printf("进货信息读取失败\n");
        return ;
    }
    for(i=0;i<j;i++)
    {
        fscanf(fp,"%s%d",inhuo[i].ID,inhuo[i].storage);
        if((fpp=fopen("kucun.txt","r"))==NULL)
        {
            printf("库存信息读取失败\n");
            return ;
        }
        //打开进货文件和库存文件
        else
        {for(n=0;n<N;n++)
        {
            if(strcmp(inhuo[i].ID,goods[n].ID)==0)
            {
                goods[n].storage=goods[n].storage+inhuo[i].storage;
            }
        }
        //将进货文件中的信息更新到库存文件中
        }  
    }
    printf("仓库信息更新成功\n");
    fclose(fp);
    fclose(fpp);
    //关闭两个文件
}

void Youhui()//记录每日优惠政策，方便计算实际每日售额
{
    FILE *fp;
    printf("请输入有优惠的货品种类：");
    int j,i,n;
    scanf("%d",&j);
    //输入有优惠需求的货物总类
    for(i=0;i<j;i++)
    {
        printf("第%d种优惠货物信息：\n",i+1);
        printf("货号：");
        scanf("%s",youhui[i].ID);
        printf("优惠后价格：");
        scanf("%lf",&youhui[i].outprice2);
    }
    //输入记录优惠货物售价信息
    for(i=0;i<j;i++)
    {
        if((fp=fopen("kucun.txt","w"))==NULL)
        {
            printf("库存信息读取失败\n");
            return ;
        }
        //打开库存文件
        else
        {for(n=0;n<N;n++)
        {
            fscanf(fp,"%s%lf",goods[n].ID,goods[n].outprice1);
            if(strcmp(youhui[i].ID,goods[n].ID)==0)
            {
                goods[n].outprice2=youhui[i].outprice2;
            }
            //找到存在优惠信息的货物，将优惠售价存到文件中
        }
        }  
    }
    printf("优惠信息更新成功\n");
    fclose(fp);
}

void Chushou()//计算每日售额
{   
    FILE *fp;
    rishou=0;
    //初始化当日售额
    struct shouhuo *head,*p1,*p2;
    //使用链表存储当日出售的货物种类和数量
    int n;
    n=0;
    p1=p2=(struct shouhuo *)malloc(LEN);
    printf("输入出售货物货号：");
    scanf("%s",p1->ID);
    printf("输入出售数量：");
    scanf("%d",&p1->num);
    //先输入第一个出售信息
    head=NULL;
    while (p1->num!=0)
    {
        n=n+1;
        if(n==1)
            head=p1;
        else
            p2->next=p1;
        p2=p1;
        p1=(struct shouhuo *)malloc(LEN);
        printf("输入出售货物货号：");
        scanf("%s",p1->ID);
        printf("输入出售数量：");
        scanf("%d",&p1->num);
    }
    //以链表存储出售的货物的货号及数量
    p2->next=NULL;
    struct shouhuo *p;
    int i;
    p=head;
    //获取头结点
    if(head!=NULL)
    {
        if((fp=fopen("kucun.txt","r"))==NULL)
        {
            printf("库存信息打开失败\n");
            return ;
        }
        //打开库存文件，方便接下来进行筛选
        else
       { 
        do
        {   for(i=0;i<N;i++)
            {if(strcmp(p->ID,goods[i].ID)==0)
               { goods[i].storage=goods[i].storage-p->num;
                //当出售货物链表中的货号与库存中的货物货号相等时，该货物数量为原本数量减去出售数量
                 if(goods[i].outprice2!=0)
                    rishou=rishou+goods[i].outprice2*p->num;
                else
                    rishou=rishou+goods[i].outprice1*p->num;
               }}
               //当出售货物链表中的货号与库存中的货物货号相等时，计算该货物相应出售数量的当日售额
            p=p->next;
            //移动到链表中的下一个节点
        } while (p!=NULL); 
       }
       Cunchuqian();//更新日收入、月收入、年收入信息
       printf("已更新库存信息\n");
    }
}

void Cunchuqian()//更新日收入、月收入、年收入信息
{   
    int i;
    int y;
    switch (yue0)
    //确定当前月份，找到相应天数
    {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: day=31; 
            break;
        case 4: case 6: case 9: case 11: day=30;break;
        case 2: if(a=2)
        //以闰年判断的a为依据，判断当前年份的2月天数
                    day=29;
                else
                    day=28;break;
    }
    if(ri0>day)
    {
        yue0=yue0+1;
        ri0=1;
        for(int i=0;i<31;i++)
            ri[i]=0;
    }
    //如果当前日期大于相应天数，月份加一，日期回到一，当前月份日收入全部初始化为零
    if(yue0==13)
    {
        yue0=1;
        year++;
        for(int i=0;i<12;i++)
        {
            yue[i]=0;
        }
        a=runnian(year);
    }
    //如果月份大于12，年份加一，月份回到一，当前年份月收入全部初始化为零，并判断当前年份是否为闰月，结果以a返回
    ri[ri0-1]=rishou;
    //将计算出的当前日收入存储到相应数组中
    ri0++;
    //日期加一
    if(ri0==2)
    {
        yue[yue0-1]=ri[0];
    }
    else
    {
        yue[yue0-1]+=ri[ri0-2];
    }
    //月收入为当前结果加前一天日收入
    nian[year][yue0-1]=yue[yue0-1];
    //存储每一年的月收入
    if(year==3)
    {
        for(i=0;i<3;i++)
        {
            for(y=0;y<12;y++)
            {if(i!=2)
                {nian[i][y]=nian[i+1][y];}
                else
                {
                    nian[i][y]=0;
                }
            }   
        }
    }
    //当前系统只能存储3年的出手信息，当年次大于三年时，将第一年数据覆盖为第二年
    //将第二年数据覆盖为第三年，第三年数据初始化为零，并存储当年收入结果
}

void Rishouru()//输出日收入
{
    int i;
    for(i=0;i<day;i++)
    {
        printf("%.2lf ",ri[i]);
        if(i%7==6)
        printf("\n");
    }
}

void Yueshouru()//输出月收入
{
    int i;
    for(i=0;i<12;i++)
    {
        printf("%.2lf ",yue[i]);
        if(i%4==3)
        printf("\n");
    }
}

void Nianshouru()//计算每年年总收入，并输出年总收入和月收入
{   double nia;
    int i;
    for(i=0;i<3;i++)
    {   nia=0;
        for(int y=0;y<12;y++)
        {
            printf("%.2lf ",nian[i][y]);
            //输出每年的每月收入
            nia+=nian[i][y];
            //计算每年总收入
            if(y%4==3)
            printf("\n");
            //每4个月份换一行
        }
        printf("%.2lf\n",nia);
        //输出每年的总收入
        printf("\n");
    }
}

void Chaxun()//查询某些货物的信息
{

    FILE *fp,*fpp;
    printf("请输入查货信息种类总数：\n");
    int j;
    scanf("%d",&j);
    //输入需要查询的货物的总类数
    int i,n;
    for(i=0;i<j;i++)
    {
        printf("第%d种货物：\n",i+1);
        printf("货号：");
        scanf("%s",chahuo[i].ID);
    }
    //输入需要查询的货物的货号
    if((fp=fopen("chahuo.txt","w"))==NULL)
    {
        printf("查货信息创建失败\n");
        return ;
    }
    for(i=0;i<j;i++)
    {
        fprintf(fp,"%-11s\n",chahuo[i].ID);
    }
    fclose(fp);
    //创建查货信息的文件
    if((fp=fopen("chahuo.txt","w"))==NULL)
    {
        printf("查货信息读取失败\n");
        return ;
    }
    for(i=0;i<j;i++)
    {
        fscanf(fp,"%s",chahuo[i].ID);
        if((fpp=fopen("kucun.txt","r"))==NULL)
        {
            printf("库存信息读取失败\n");
            return ;
        }
        //打开查货文件和库存文件
        else
        {for(n=0;n<N;n++)
        {
            if(strcmp(chahuo[i].ID,goods[n].ID)==0)
            {
                fscanf(fp,"%s%s%lf%lf%d",goods[i].ID,goods[i].brand,goods[i].inprice,goods[i].outprice1,goods[i].storage);
                printf("*********************************\n");
                printf("货号       品名           进价    售价    库存量\n");
                printf("%-11s%-15s%-8.3lf%-8.3lf%-7d\n",goods[i].ID,goods[i].brand,goods[i].inprice,goods[i].outprice1,goods[i].storage);
            }
            //当查货文件中货物货号和库存文件中货物货号一致时，
        }
        }  
    }
    fclose(fp);
    fclose(fpp);
    //关闭两个文件
}
