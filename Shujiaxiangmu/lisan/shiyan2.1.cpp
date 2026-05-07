#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct NFA{
    set<int> Q;//状态集
    set<int> U;//符号集
    map<pair<int,int>,set<int>> F;//转换关系
    int S = -1;//开始状态集（初始化为 -1，避免未定义行为）
    set<int> Z;//终止状态
};

void ReadNFA(){

    ifstream file("E:\\Shujiaxiangmu\\lisan\\shiyan2.txt");

    if(!file){
        cout<<"文件读取错误! 请确认 E:\\Shujiaxiangmu\\lisan\\shiyan2.txt 是否存在"<<endl;
        exit(1); // 文件不存在时直接退出，避免后续 UB
    }
    
    string line;
    
    //读取状态集
    getline(file, line);
    stringstream ss(line);
    int shuru;
    while(ss>>shuru){
        nfa.Q.insert(shuru);
    }
    
    //读取符号表
    getline(file,line);
    ss.clear(); ss.str(line);
    while(ss>>shuru){
        nfa.U.insert(shuru);
    }
    
    //读取状态转移函数
    int numTrans;
    getline(file, line);
    ss.clear(); ss.str(line);
    ss>>numTrans; //读取状态转移行数
    
    for(int i=0;i<numTrans;i++){
        getline(file, line);
        ss.clear(); ss.str(line);
        int from, sym, to;
        set<int> temps;
        ss>>from>>sym;
        while(ss>>to){
            temps.insert(to);
        }
        nfa.F[make_pair(from,sym)]=temps;
    }
    
    //读取开始状态 S
    getline(file,line);
    ss.clear(); ss.str(line);
    ss>>nfa.S;
    
    //读取终止状态 Z
    getline(file,line);
    ss.clear(); ss.str(line);	
    while(ss>>shuru){
        nfa.Z.insert(shuru);
    }	
    
    // 简单调试信息
    cout<<"ReadNFA: |Q|="<<nfa.Q.size()<<" |U|="<<nfa.U.size()<<" |F|="<<nfa.F.size()<<" S="<<nfa.S<<" |Z|="<<nfa.Z.size()<<endl;
    
    file.close();
}

void PrintNFA(){
    cout<<"NFA: Q="<<nfa.Q<<endl;
    cout<<"NFA: U="<<nfa.U<<endl;
    cout<<"NFA: F="<<nfa.F<<endl;
    cout<<"NFA: S="<<nfa.S<<endl;
    cout<<"NFA: Z="<<nfa.Z<<endl;
}

void NFA::printNFA(){
    cout<<"NFA: Q="<<Q<<endl;
    cout<<"NFA: U="<<U<<endl;
    cout<<"NFA: F="<<F<<endl;
    cout<<"NFA: S="<<S<<endl;
    cout<<"NFA: Z="<<Z<<endl;
}