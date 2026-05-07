#include <bits/stdc++.h>
 #include<iostream>
using namespace std;
 
// 定义一个结构体 NFA，用于表示一个非确定性有限自动机
struct NFA {
    set<int> Q; // 状态集
    set<int> U; // 符号表
    map<pair<int, char>, set<int>> F; // 转换关系，存储每个状态在符号输入下可以转移到的状态集合
    int S; // 起始状态
    set<int> Z; // 终止状态集合
} nfa;
 
int id = 0; // 用于给状态分配唯一的编号
 
// 处理正则表达式中的中缀表达式，为了保证正确性，自动在适当位置插入连接符 '.' (拼接操作符)
string zhongzhui(const string& infix) {
    string handleInfix;
    size_t a;
    for (a = 0; a < infix.size(); a++) {
        handleInfix += infix[a]; // 将原始正则表达式逐个字符加入新字符串

        if ((a + 1) < infix.size()) {
            // 如果是字母后面跟着字母或者括号，插入拼接符 '.'
            if (isalnum(infix[a]) && (isalnum(infix[a + 1]) || infix[a + 1] == '(')) {
                handleInfix += '.';
            }
            // 处理括号后面跟字母或括号的情况
            else if (infix[a] == ')' && (isalnum(infix[a + 1]) || infix[a + 1] == '(')) {
                handleInfix += '.';
            }
            // 处理 '*' 后面跟字母或括号的情况
            else if (infix[a] == '*' && (isalnum(infix[a + 1]) || infix[a + 1] == '(')) {
                handleInfix += '.';
            }
        }
    }
    return handleInfix;
}
 
// 优先级规则，返回运算符的优先级
int priority(char op) {
    if (op == '*') return 3; // Kleene 星号的优先级最高
    if (op == '.') return 2; // 拼接操作符的优先级次之
    if (op == '|') return 1; // 或操作符的优先级最低
    return 0;
}
 
// 将中缀表达式转换为后缀表达式
string houzhui(const string& infix) {
    string postfix;
    stack<char> opStack;
 
    for (char c : infix) {
        // 如果是字母，直接加入后缀表达式
        if (isalnum(c)) {
            postfix += c;
        }
        // 如果是左括号，压入栈
        else if (c == '(') {
            opStack.push(c);
        }
        // 如果是右括号，弹出栈内的运算符，直到遇到左括号
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop(); // 弹出左括号
        }
        // 处理运算符
        else {
            while (!opStack.empty() && priority(opStack.top()) >= priority(c)) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
    }
 
    // 将栈内剩余的运算符弹出
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
 
    return postfix;
}
 
// 创建一个 NFA 用于表示单个字符
NFA createNFA(char c) {
    NFA nfa;
    int s1 = id++, s2 = id++; // 创建两个新的状态
    nfa.Q = {s1, s2}; // 状态集
    nfa.S = s1; // 起始状态
    nfa.U = {c}; // 符号表
    nfa.Z = {s2}; // 终止状态
    nfa.F[{s1, c}].insert(s2); // 从状态 s1 通过字符 c 转移到状态 s2
    return nfa;
}
 
// 拼接两个 NFA（串联操作）.
NFA pin(NFA nfa1, NFA nfa2) {
    // 将 nfa1 的终止状态通过 #（空字符）转移到 nfa2 的起始状态
    for (int q : nfa1.Z) {
        nfa1.F[{q, '#'}].insert(nfa2.S);
    }
    // 合并状态集
    nfa1.Q.insert(nfa2.Q.begin(), nfa2.Q.end());
    // 合并转换关系
    nfa1.F.insert(nfa2.F.begin(), nfa2.F.end());
    nfa1.Z = nfa2.Z; // 设置 nfa1 的终止状态为 nfa2 的终止状态
    nfa1.U.insert(nfa2.U.begin(), nfa2.U.end()); // 合并符号表
    return nfa1;
}
 
// 或操作（联合操作），返回 nfa1 和 nfa2 的并集 |
NFA bing(NFA nfa1, NFA nfa2) {
    int s = id++, f = id++; // 创建新的起始和终止状态
    NFA nfa;
    nfa.Q = {s, f}; // 状态集
    nfa.Q.insert(nfa1.Q.begin(), nfa1.Q.end());
    nfa.Q.insert(nfa2.Q.begin(), nfa2.Q.end());
    // 从新的起始状态可以转移到 nfa1 和 nfa2 的起始状态
    nfa.F[{s, '#'}] = {nfa1.S, nfa2.S};
    nfa.F.insert(nfa1.F.begin(), nfa1.F.end());
    nfa.F.insert(nfa2.F.begin(), nfa2.F.end());
    // 对于 nfa1 和 nfa2 的终止状态，都可以转移到新的终止状态
    for (int q : nfa1.Z) {
        nfa.F[{q, '#'}].insert(f);
    }
    for (int q : nfa2.Z) {
        nfa.F[{q, '#'}].insert(f);
    }
    nfa.S = s; // 设置起始状态
    nfa.Z = {f}; // 设置终止状态
    nfa.U.insert(nfa1.U.begin(), nfa1.U.end()); // 合并符号表
    nfa.U.insert(nfa2.U.begin(), nfa2.U.end());
    return nfa;
}
 
// Kleene 星号操作，表示零次或多次重复*
NFA xing(NFA nfa) {
    int s = id++, f = id++; // 创建新的起始和终止状态
    NFA result;
    result.Q = {s, f}; // 状态集
    result.Q.insert(nfa.Q.begin(), nfa.Q.end());
    result.F.insert(nfa.F.begin(), nfa.F.end());
    // 新的起始状态可以转移到 nfa 的起始状态或者终止状态
    result.F[{s, '#'}].insert(nfa.S);
    result.F[{s, '#'}].insert(f);
    // nfa 的终止状态可以回到起始状态或转移到终止状态
    for (int c : nfa.Z) {
        result.F[{c, '#'}].insert(nfa.S);
        result.F[{c, '#'}].insert(f);
    }
    result.S = s; // 设置新的起始状态
    result.Z = {f}; // 设置新的终止状态
    result.U.insert(nfa.U.begin(), nfa.U.end()); // 合并符号表
    return result;
}
 
// 将后缀表达式转换为 NFA
NFA reToNFA(const string& postfix) {
    stack<NFA> stk;
    for (char c : postfix) {
        if (isalnum(c)) {
            stk.push(createNFA(c)); // 如果是字母，创建对应的 NFA
        } else if (c == '.') {
            NFA nfa2 = stk.top(); stk.pop();
            NFA nfa1 = stk.top(); stk.pop();
            stk.push(pin(nfa1, nfa2)); // 拼接操作
        } else if (c == '|') {
            NFA nfa2 = stk.top(); stk.pop();
            NFA nfa1 = stk.top(); stk.pop();
            stk.push(bing(nfa1, nfa2)); // 并操作
        } else if (c == '*') {
            NFA nfa = stk.top(); stk.pop();
            stk.push(xing(nfa)); // 星号操作
        }
    }
    return stk.top(); // 返回栈顶的 NFA，表示整个正则表达式的 NFA
}
 
int main() {
	
	string regex;
 
	cin>>regex;
	
	string infix;
    infix=zhongzhui(regex);
	
	cout<<"After preprocessing:"<<endl;
	cout<<infix<<endl;
	
	string postfix = houzhui(infix);
	
	cout<<"Postfix expression:"<<endl;
	cout<<postfix<<endl;
	
	NFA nfa=reToNFA(postfix);
	
	cout<<"NFA States:"<<endl;
	for(int q: nfa.Q){
		cout<<q<<" ";
	}
	
	cout<<endl<<"NFA Symbol"<<endl;
	for(char c: nfa.U){
		cout<<c<<" ";
	}
	
 
	cout<<endl<<"NFA Transition"<<endl;
	for(auto [key, value] : nfa.F){
		cout<<key.first<<"--"<<key.second<<"-->";
		for(int q : value){
			cout<<q<<" ";
		}
		cout<<endl;
	}
	
	cout<<"NFA Start"<<endl;
	cout<<nfa.S<<endl;
	
	cout<<"NFA Finals"<<endl;
	for(int q : nfa.Z){
		cout<<q<<endl;
	}
		
    return 0;
}