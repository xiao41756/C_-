#include <bits/stdc++.h>
using namespace std;
set<char> zhongjie;//终结符
set<char> feizhongjie;//非终结符
map<char,vector<string>> mp;//存储文法
map<char,set<char>> first;//first集
map<char,set<char>> follow;//follow集
map<string,set<char>> select;//select集
char start;//开始符号

//从文件中读取文法
void readFile(){
    ifstream file("E:\\Shujiaxiangmu\\lisan\\shiyan4.txt");
    if(!file){
        cout<<"File Error"<<endl;
        return;
    }
    string line;
    //读取非终结符
    getline(file,line);
    for(char c:line){
        if(c!=' '){
            feizhongjie.insert(c);
        }
    }
    //读取终结符
    getline(file,line);
    for(char c:line){
        if(c!=' '){
            zhongjie.insert(c);
        }
    }
    //读取文法规则数
    int n;
    file>>n;
    file.get();//吸收换行符
    //读取文法规则
    for(int i=0;i<n;i++){
        getline(file,line);
        char left=line[0];//左部
        string right;//右部
        for(int j=4;j<line.size();j++){
            if(line[j]!=' '){
                right+=line[j];
            }
        }
        mp[left].push_back(right);
    }
    //读取开始符号
    file>>start;
    file.ignore();
    file.close();
}
// 计算每个非终结符的FIRST集
void findFirst() {
    bool changed = true;
 
    // 迭代直到FIRST集不再变化
    while (changed) {
        changed = false;
 
        // 遍历所有文法规则
        for (auto [left, right] : mp) {
            for (auto formula : right) {
                bool allEpsilon = true; // 标记是否所有符号都能推导出ε
 
                // 遍历产生式右部的每个符号
                for (int i = 0; i < formula.size(); i++) {
                    char c = formula[i];
 
                    // 如果是终结符或ε
                    if (zhongjie.count(c)|| c == '#') {
                        if (first[left].insert(c).second) {
                            changed = true; // 如果插入成功，标记变化
                        }
                        allEpsilon = false;
                        break;
                    }
                    // 如果是非终结符
                    else {
                        if (first.count(c)) {
                            // 将FIRST(c)中的非ε元素加入FIRST(left)
                            for (char f : first[c]) {
                                if (f != '#') {
                                    if (first[left].insert(f).second) {
                                        changed = true;
                                    }
                                }
                            }
                            // 如果FIRST(c)不包含ε，停止遍历
                            if (first[c].count('#') == 0) {
                                allEpsilon = false;
                                break;
                            }
                        } else {
                            allEpsilon = false;
                        }
                    }
                }
 
                // 如果所有符号都能推导出ε，将ε加入FIRST(left)
                if (allEpsilon) {
                    if (first[left].insert('#').second) {
                        changed = true;
                    }
                }
            }
        }
    }
}
// 计算每个非终结符的FOLLOW集
void findFollow() {
    bool changed = true;
    //直到FOLLOW集不再变化
    while(changed){
        changed=false;
        //遍历所有文法规则
        for(auto [left,right]:mp){
            for(auto formula:right){
                //遍历产生式右部的每个符号
                for(int i=0;i<formula.size();i++){
                    char c=formula[i];
                    //如果是非终结符
                    if(feizhongjie.count(c)){
                        bool allEpsilon=true;//标记后续符号是否都能推导出ε
                        //遍历c后面的符号
                        for(int j=i+1;j<formula.size();j++){
                            char next=formula[j];
                            //如果是终结符或ε
                            if(zhongjie.count(next)||next=='#'){
                                if(follow[c].insert(next).second){
                                    changed=true;
                                }
                                allEpsilon=false;
                                break;
                            }
                            //如果是非终结符
                            else{
                                if(first.count(next)){
                                    //将FIRST(next)中的非ε元素加入FOLLOW(c)
                                    for(char f:first[next]){
                                        if(f!='#'){
                                            if(follow[c].insert(f).second){
                                                changed=true;
                                            }
                                        }
                                    }
                                    //如果FIRST(next)不包含ε，停止遍历
                                    if(first[next].count('#')==0){
                                        allEpsilon=false;
                                        break;
                                    }
                                }else{
                                    allEpsilon=false;
                                }
                            }
                        }
                        //如果后续符号都能推导出ε，将FOLLOW(left)加入FOLLOW(c)
                        if(allEpsilon){
                            for(char f:follow[left]){
                                if(follow[c].insert(f).second){
                                    changed=true;
                                }
                            }
                        }
                    }
                }
            }
            if(left==start){
                //将$加入开始符号的FOLLOW集
                if(follow[left].insert('#').second){
                    changed=true;
                }}

        }
    }
}
//计算每个产生式的SELECT集
void findSelect(){
    for(auto [left,right]:mp){
        for(auto formula:right){
            set<char> selectSet;
            bool allEpsilon=true;//标记是否所有符号都能推导出ε
            //遍历产生式右部的每个符号
            for(int i=0;i<formula.size();i++){
                char c=formula[i];
                //如果是终结符或ε
                if(zhongjie.count(c)||c=='#'){
                    selectSet.insert(c);
                    allEpsilon=false;
                    break;
                }
                //如果是非终结符
                else{
                    if(first.count(c)){
                        //将FIRST(c)中的非ε元素加入SELECT集
                        for(char f:first[c]){
                            if(f!='#'){
                                selectSet.insert(f);
                            }
                        }
                        //如果FIRST(c)不包含ε，停止遍历
                        if(first[c].count('#')==0){
                            allEpsilon=false;
                            break;
                        }
                    }else{
                        allEpsilon=false;
                    }
                }
            }
            //如果所有符号都能推导出ε，将FOLLOW(left)加入SELECT集
            if(allEpsilon){
                for(char f:follow[left]){
                    selectSet.insert(f);
                }
            }
            //存储SELECT集
            {
                string prod = string(1, left) +"->" + formula;
                select[prod] = selectSet;
            }
        }
    }
}
//主函数
int main(){
    readFile();
    findFirst();
    findFollow();
    findSelect();
    cout<<"CFG=(V,T,P,S)"<<endl;
    //输出非终结符和终结符
    cout<<"V: ";
    for(char c:feizhongjie){
        cout<<c<<" ";
    }
    cout<<endl;
    cout<<"T: ";
    for(char c:zhongjie){
        cout<<c<<" ";
    }
    cout<<endl;
    //输出文法规则
    cout<<"Productions: "<<endl;
    int i=0;
    for(auto [left,right]:mp){
        for(auto formula:right){
            cout<<i++<<": "<<left<<" -> "<<formula<<endl;
        }
    }
    //输出开始符号
    cout<<"Start Symbol: "<<start<<endl;
    //输出FIRST集
    cout<<endl;
    cout<<"FIRST:"<<endl;
    for(auto [c,s]:first){
        cout<<c<<": { ";
        for(char f:s){
            cout<<f<<" ";
        }
        cout<<"}"<<endl;
    }
    //输出FOLLOW集
    cout<<"FOLLOW:"<<endl;
    for(auto [c,s]:follow){
        cout<<c<<": { ";
        for(char f:s){
            cout<<f<<" ";
        }
        cout<<"}"<<endl;
    }
    //输出SELECT集
    cout<<"SELECT:"<<endl;
    for(auto [prod,s]:select){
        cout<<prod<<": { ";
        for(char f:s){
            cout<<f<<" ";
        }
        cout<<"}"<<endl;
    }
    return 0;
}
