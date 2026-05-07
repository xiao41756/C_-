#include <bits/stdc++.h>
 
using namespace std;
 
map<char, vector<string>> grammar;//存储文法规则
set<char> zhongzhi;  // 终结符集合
set<char> feizhongzhi;  // 非终结符集合
char start;          // 开始符号
string input;    // 输入串
 //LR(0)项目结构体
struct Item{
	char left;// 产生式左部
	string right;// 产生式右部
	int dot;// 点的位置
	// 定义小于号运算符以便在set中使用
    bool operator<(const Item& other) const {
	    if (left != other.left) return left < other.left;
	    if (right != other.right) return right < other.right;
	    return dot < other.dot;
	}
	
	bool operator==(const Item& other) const {
        return left == other.left && right == other.right && dot == other.dot;
    }
};
 
vector<set<Item>> C;// 项目集规范族
map<pair<int,char>, pair<char, string>> actionTable;// 动作表
map<pair<int,char>, int> gotoTable;// GOTO表
 
// 从文件中读取文法规则和其他信息
void ReadFile() {
    ifstream file("E:\\Shujiaxiangmu\\lisan\\shiyan6.txt"); // 打开文件
    if (!file) {
        cout << "文件读取错误!" << endl;
        return;
    }
 
    string line;
 
    // 读取非终结符
    getline(file, line);
    for (char c : line) {
        if (c != ' ') feizhongzhi.insert(c);
    }
 
    // 读取终结符
    getline(file, line);
    for (char c : line) {
        if (c != ' ') zhongzhi.insert(c);
    }
 
    // 读取文法规则数量
    int numGrammar;
    file >> numGrammar;
    file.ignore(); // 忽略换行符
 
    // 读取每条文法规则
    for (int i = 0; i < numGrammar; i++) {
        getline(file, line);
        char left = line[0]; // 产生式左部
        string right;         // 产生式右部
 
        // 去掉空格，提取右部
        for (int j = 4; j < line.size(); j++) {
            if (line[j] != ' ') right += line[j];
        }
 
        grammar[left].push_back(right); // 存储文法规则
    }
 
    // 读取开始符号
    file >> start;
    file.ignore();
 
    // 读取输入串
    getline(file, input);
 
    file.close(); // 关闭文件
}
 // 计算项目的闭包
set<Item> closure(set<Item> I){
	
	stack<Item> stk;
	for(auto i : I){
		stk.push(i);
	}// 初始化栈
	
	set<Item> result = I;// 结果集初始化为输入集
	
	while(!stk.empty()){
		
		Item item = stk.top();// 取栈顶项目
		stk.pop();
		
		if(item.dot<item.right.size()){
			char next = item.right[item.dot];// 点后符号
			if(feizhongzhi.count(next)){// 如果点后是非终结符，展开该非终结符的产生式
				for(auto formula : grammar[next]){
					Item newItem = {next, formula, 0};// 新项目，点在最前面
					if(!result.count(newItem)){
						result.insert(newItem);// 插入新项目
						stk.push(newItem);// 将新项目压入栈中
					}
				}
			}
		}
	}
	
	return result;
}
 
set<Item> Go(set<Item> I, char X){// 计算项目集I在符号X上的GO函数
	
	set<Item> J;
	
	for(auto item : I){
        // 如果点后符号是X，点向后移动一位
		if(item.dot<item.right.size()&&item.right[item.dot]==X){
			J.insert({item.left, item.right, item.dot+1});
		}
	}
	
	return closure(J);// 返回J的闭包
}
 // 构造项目集规范族
void constructItems(){
	
	set<Item> startItem = closure({{'S', string(1, start), 0}});// 增加新的开始符号S'
	
	start='S';
	
	C.push_back(startItem);// 初始化项目集规范族
	
	bool changed = true;
	
	while(changed){
		
		changed=false;
		
		for(auto I: C){// 遍历当前所有项目集
			for(char t : zhongzhi){// 对每个终结符计算GO
				set<Item> J = Go(I, t);// 计算GO
					
				if(!J.empty()){// 如果GO不为空，检查是否已存在于C中
					
					bool sign=false;
					
					for(auto c : C){// 检查是否已存在于C中
						if(c==J){
							sign=true;// 已存在于C中
							break;
						}
					}
					
					if(sign==false){
						C.push_back(J);// 将新的项目集加入规范族
						changed=true;
					}
				}
			}
			
			for(char n : feizhongzhi){// 对每个非终结符计算GO
				set<Item> J = Go(I, n);
				if(!J.empty()){
					
					bool sign=false;
					
					for(auto c : C){
						if(c==J){
							sign=true;
							break;
						}
					}
					
					if(sign==false){
						C.push_back(J);
						changed=true;
					}
				}
			}
		}
	}
}
 // 构造ACTION和GOTO表
void constructActionGoto(){
	// 遍历所有项目集
	for(int i=0;i<C.size();i++){
		for(auto item : C[i]){// 遍历项目集中的每个项目
			if(item.dot == item.right.size()){
				// 点在产生式右部末尾，规约或接受动作
				if(item.left==start){// 如果是增强的开始符号，接受动作
					actionTable[{i, '$'}]={'a', "accept"};
				}else{
					for(auto t : zhongzhi){// 对每个终结符添加规约动作
						actionTable[{i, t}] = {'r', string(1, item.left)+"->"+item.right};
					}
					// 对$添加规约动作
					actionTable[{i,'$'}] = {'r', string(1, item.left)+"->"+item.right};					
				}
			}else{
				// 点后是终结符，移进动作
				char next=item.right[item.dot];
				if(zhongzhi.count(next)){
					set<Item> nextItem = Go(C[i], next);// 计算GO
					for(int j = 0;j<C.size();j++){// 查找对应的项目集编号
						if(nextItem==C[j]){
							actionTable[{i, next}]={'s', to_string(j)};// 添加移进动作
							break;
						}
					}
				}else{// 点后是非终结符，GOTO动作
					set<Item> nextItems = Go(C[i], next);
                    for (int j = 0; j < C.size(); ++j) {
                        if (nextItems == C[j]) {
                            gotoTable[{i,next}]=j;// 添加GOTO动作
                            break;
                        }
                    }
					
				}
			}
		}
	}
	// 填充未定义项为错误
    for (int i = 0; i < C.size(); i++) {
	    for (char t : zhongzhi) {
	        if (actionTable.find({i, t}) == actionTable.end()) {
	            actionTable[{i, t}] = {'e', "error"};  // 未定义项填充 "error"
	        }
	    }
	    for (char nt : feizhongzhi) {
	        if (gotoTable.find({i, nt}) == gotoTable.end()) {
	            gotoTable[{i, nt}] = -1;  // -1 代表 GOTO 错误
	        }
	    }
	}
}
void predict() {
    stack<int> stateStack;  // 状态栈
    stack<char> symbolStack; // 符号栈
    stateStack.push(0);      // 初始状态
    symbolStack.push('$');   // 栈底符号
 
    input += '$';            // 输入串末尾添加 $
    int idx = 0;             // 输入指针
 
    cout << "预测分析过程" << endl;
    cout << "------------+--------+----+-------------------------------------+-----------" << endl;
    cout << "栈顶\t\t输入\t查表\t动作\t\t\t\t注释" << endl;
    cout << "------------+--------+----+-------------------------------------+-----------" << endl;
 
    while (true) {
 
        int state = stateStack.top(); // 当前状态
        char current = input[idx];   // 当前输入符号
        
        cout<<state<<'\t'<<symbolStack.top()<<'\t'<<current<<'\t';
 
        // 查找 ACTION 表
        if (actionTable.find({state, current}) != actionTable.end()) {
            auto action = actionTable[{state, current}];
            char actionType = action.first;// 动作类型s/r
            string actionValue = action.second;// 动作值
 
            if (actionType == 's') { // 移进动作
                int nextState = stoi(actionValue);// 下一个状态
                stateStack.push(nextState);
                symbolStack.push(current);
                idx++;
                cout << "s" << nextState << "\t移进\t进栈 " << " " << current << endl;
            } else if (actionType == 'r') { // 规约动作
                char left = actionValue[0];
                string right = actionValue.substr(3);// 产生式右部
                int count = right.size();
 
                // 弹出栈顶的符号和状态
                for (int i = 0; i < count; i++) {
                    stateStack.pop();
                    symbolStack.pop();
                }
 
                // 获取规约后的状态
                int newState = stateStack.top();
                if (gotoTable.find({newState, left}) != gotoTable.end()) {
                    int gotoState = gotoTable[{newState, left}];// GOTO 状态
                    stateStack.push(gotoState);
                    symbolStack.push(left);
                    cout << "r" << gotoState << "\t规约\t出栈 " << count << " 个符号和状态，进栈 " << " " << left << "\t" << left << " -> " << right << endl;
                } else {
                    cout << "error\t错误\tGOTO 表未定义" << endl;
                    return;
                }
            } else if (actionType == 'a') { // 接受动作
                cout << "acc\t接受\t成功接收！" << endl;
                return;
            } else { // 其他动作
                cout << actionValue << "\t未知动作" << endl;
                return;
            }
        } else {
            cout << "error\t错误\tACTION 表未定义" << endl;
            return;
        }
    }
}
 
int main() {
	system("chcp 65001");
    ReadFile(); 
    
    constructItems();
    
    constructActionGoto();
    //解决中文输出问题
    system("chcp 65001");
 
	cout << "非终结符: ";
    for (const auto &nt : feizhongzhi) {
        cout << nt << " ";
    }
    cout << endl;
 
    cout << "终结符: ";
    for (const auto &t : zhongzhi) {
        cout << t << " ";
    }
    cout << endl;
 
    cout << "文法规则:" << endl;
    for (auto entry : grammar) {
        char lhs = entry.first;
        cout << lhs << " -> ";
        for (auto rhs : entry.second) {
            cout << rhs << " | ";
        }
        cout << endl;
    }
 
    cout << "起始符号: " << start << endl;
    
        // 打印项目集
    cout << "LR(0) 规范簇：" << endl;
    for (int i = 0; i < C.size(); i++) {
        cout << "I" << i << ":\n";
        for (const auto& item : C[i]) {
            cout << "  " << item.left << " -> ";
			cout<<item.right<<" "<<item.dot<<endl;
        }
 
    }
    
    cout << "\nACTION 表:\n";
    cout << "状态\t";
    for (char t : zhongzhi) cout << t << "\t";
    cout << "$\n";
 
    for (int i = 0; i < C.size(); i++) {
        cout << i << "\t";
        for (char t : zhongzhi) {
            if (actionTable[{i, t}].first != 'e') {
                cout << actionTable[{i, t}].first << actionTable[{i, t}].second << "\t";
            } else {
                cout << "error\t";
            }
        }
        if (actionTable.find({i, '$'}) != actionTable.end()) {
            cout << actionTable[{i, '$'}].first << actionTable[{i, '$'}].second;
        } else {
            cout << "error";
        }
        cout << "\n";
    }
 
    cout << "\nGOTO 表:\n";
    cout << "状态\t";
    for (char nt : feizhongzhi) cout << nt << "\t";
    cout << "\n";
 
    for (int i = 0; i < C.size(); i++) {
        cout << i << "\t";
        for (char nt : feizhongzhi) {
            if (gotoTable[{i, nt}] != -1) {
                cout << gotoTable[{i, nt}] << "\t";
            } else {
                cout << "error\t";
            }
        }
        cout << "\n";
    }
 
	predict();// 进行预测分析
 
    return 0;
}