#include <bits/stdc++.h>
using namespace std;
set<char> zhongjie;//终结符
set<char> feizhongjie;//非终结符
map<char,vector<string>> mp;//存储文法
map<char,set<char>> first;//first集
map<char,set<char>> follow;//follow集
char start;//开始符号
string input; // 输入串
map<char, map<char, string>> parseTable; // 预测分析表

//从文件中读取文法
void readFile(){
    ifstream file("E:\\Shujiaxiangmu\\lisan\\shiyan5.txt");
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
    file>>input;
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
                    if (zhongjie.count(c) || c == '#') {
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
                if(follow[left].insert('$').second){
                    changed=true;
                }}

        }
    }
}
 
// 构建预测分析表
void constructParseTable() {
    // 遍历所有文法规则
    for (auto [left, right] : mp) {
        for (auto formula : right) {
            set<char> firstSingle; // 存储产生式右部的FIRST集
            bool hasEpsilon = true; // 标记是否包含ε
 
            // 计算产生式右部的FIRST集
            for (char sym : formula) {
                // 如果是终结符
                if (zhongjie.count(sym)) {
                    firstSingle.insert(sym);
                    hasEpsilon = false;
                    break;
                } else if (sym != '#') {// 如果是非终结符，且不是ε
                    for (char f : first[sym]) {
                        if (f != '#') {
                            firstSingle.insert(f);
                        }
                    }
                    if (first[sym].count('#') == 0) {
                        hasEpsilon = false;
                        break;
                    }
                }
            }
 
            // 将FIRST集中的每个符号与产生式关联
            for (char c : firstSingle) {
                parseTable[left][c] = formula;
            }
 
            // 如果产生式右部可以推导出ε，将FOLLOW(left)中的符号与ε关联
            if (hasEpsilon) {
                for (char c : follow[left]) {
                    parseTable[left][c] = "#";
                }
            }
        }
    }
 
    // 填充错误项
    for (char n : feizhongjie) {
        for (char t : zhongjie) {
            if (parseTable[n].find(t) == parseTable[n].end()) {//检查是否已有条目
                parseTable[n][t] = "error";
            }
        }
        if (parseTable[n].find('$') == parseTable[n].end()) {
            parseTable[n]['$'] = "error";
        }
    }
}
 
// 使用预测分析表进行语法分析
void predict() {
    stack<char> stk;
    stk.push('$'); // 栈底符号
    stk.push(start); // 开始符号
 
    input += '$'; // 输入串末尾添加$
 
    int idx = 0; // 输入串指针
 
    while (!stk.empty()) {
        char top = stk.top();
 
        // 如果栈顶和输入符号匹配
        if (top == '$' && input[idx] == '$') {
            cout << "出栈" << top << ", 输入" << input[idx] << "，匹配成功。" << endl;
            stk.pop();
            idx++;
        } else if (top == input[idx]) {
            cout << "出栈" << top << ", 输入" << input[idx] << "，输入指针后移。" << endl;
            stk.pop();
            idx++;
        }
        // 如果栈顶是非终结符
        else if (feizhongjie.count(top)) {
            if (parseTable[top].find(input[idx]) != parseTable[top].end()) {
                string formula = parseTable[top][input[idx]];
                stk.pop();
                cout << "出栈" << top << ", 输入" << input[idx] << "查表，" << top << "->" << formula << "，产生式右部逆序入栈" << endl;
                if (formula != "#") {
                    for (int i = formula.size() - 1; i >= 0; i--) {
                        stk.push(formula[i]);
                    }
                }
            } else {
                cout << "错误：在状态 " << top << " 遇到符号 " << input[idx] << " 时无法匹配！" << endl;
                return;
            }
        }
        // 其他情况报错
        else {
            cout << "错误：栈顶符号与输入不匹配！" << endl;
            return;
        }
    }
 
    // 检查输入串是否完全消耗
    if (idx != input.size()) {
        cout << "错误：输入串未完全消耗完！" << endl;
    }
}
 
// 主函数
int main() {
    //解决中文乱码
    system("chcp 65001");
    readFile(); // 读取文件
    findFirst(); // 计算FIRST集
    findFollow(); // 计算FOLLOW集
    constructParseTable(); // 构建预测分析表
 
    // 打印非终结符、终结符、文法规则、FIRST集、FOLLOW集和预测分析表
    cout << "非终结符: ";
    for (const auto &nt : feizhongjie) {
        cout << nt << " ";
    }
    cout << endl;
 
    cout << "终结符: ";
    for (const auto &t : zhongjie) {
        cout << t << " ";
    }
    cout << endl;
 
    cout << "文法规则:" << endl;
    for (auto entry : mp) {
        char lhs = entry.first;
        cout << lhs << " -> ";
        for (auto rhs : entry.second) {
            cout << rhs << " | ";
        }
        cout << endl;
    }
 
    cout << "起始符号: " << start << endl;
 
    cout << "FIRST集:" << endl;
    for (auto [key, value] : first) {
        cout << key << "      ";
        for (auto v : value) {
            cout << v << " ";
        }
        cout << endl;
    }
 
    cout << "FOLLOW集:" << endl;
    for (auto [key, value] : follow) {
        cout << key << "      ";
        for (auto v : value) {
            cout << v << " ";
        }
        cout << endl;
    }
 
    cout << "\n预测分析表:" << endl;
    cout << "  ";
    for (const char &t : zhongjie) {
        cout << t << "\t";
    }
    cout << "$" << endl;
 
    for (const char &n : feizhongjie) {
        cout << n << " ";
        for (const char &t : zhongjie) {
            cout << parseTable[n][t] << "\t";
        }
        cout << parseTable[n]['$'] << endl;
    }
 
    predict(); // 进行语法分析
 
    return 0;
}