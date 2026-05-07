#include <bits/stdc++.h>
 using namespace std;
//定义NFA，中间状态DFA，最终状态DFA
 struct NFA{
    set<int> Q;//状态集
    set<int> U;//符号集
    map<pair<int,int>,set<int>> F;//转换关系
    int S;//开始状态集
    set<int> Z;//终止状态
};
NFA nfa1,nfa2;
struct DFA//中间态DFA
{
    set<set<int>> Q;
    set<int> U;
    map<pair<set<int>,int>,set<int>> F;
    set<int> S;
    set<set<int>> Z;
};
DFA dfa;
struct DFA1
{
    set<int> Q;
    set<int> U;
    map<pair<int,int>,int> F;
    int S;
    set<int> Z;
};
//读取NFA信息
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
        nfa1.Q.insert(shuru);
    }
    
    //读取符号表
    getline(file,line);
    ss.clear(); ss.str(line);
    while(ss>>shuru){
        nfa1.U.insert(shuru);
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
        nfa1.F[make_pair(from,sym)]=temps;
    }
    
    //读取开始状态 S
    getline(file,line);
    ss.clear(); ss.str(line);
    ss>>nfa1.S;
    
    //读取终止状态 Z
    getline(file,line);
    ss.clear(); ss.str(line);	
    while(ss>>shuru){
        nfa1.Z.insert(shuru);
    }	
    
    // 简单调试信息
    cout<<"ReadNFA: |Q|="<<nfa1.Q.size()<<" |U|="<<nfa1.U.size()<<" |F|="<<nfa1.F.size()<<" S="<<nfa1.S<<" |Z|="<<nfa1.Z.size()<<endl;
    
    file.close();
}
// 函数定义：输入两个NFA，返回合并后的新NFA（const&避免拷贝，提高效率）
NFA merge_nfas(const NFA& nfa1, const NFA& nfa2) {
    // 合并状态集：nfa1和nfa2的状态集取并集
    set<int> merged_states = nfa1.Q;
    merged_states.insert(nfa2.Q.begin(), nfa2.Q.end());  //  将nfa2的状态加入合并集

    // 合并字母表：取两个NFA字母表的并集
    set<char> merged_alphabet = nfa1.U;
    merged_alphabet.insert(nfa2.U.begin(), nfa2.U.end());  //  将nfa2的符号加入合并集
	
    // 新增合并后的起始状态（固定为0，假设原NFA无状态0；可扩展为动态分配）
    int merged_start_state = 0;

    //合并终止状态集：取两个NFA终止状态的并集
    set<int> merged_Z = nfa1.Z;
    merged_Z.insert(nfa2.Z.begin(), nfa2.Z.end());  // 合并终止状态

    // 合并转移函数：先复制nfa1的转移，再添加nfa2的转移
    map<pair<int,int>,set<int>> merged_transitions = nfa1.F;
    for (const auto& trans1 : nfa2.F) {  // 遍历nfa2的所有转移
        int from_state = trans1.first;
        for (const auto& trans2 : trans1.second) {  // 遍历该起始状态的符号转移
            char symbol = trans2.first;
            const set<int>& to_states = trans2.second;
            //将nfa2的转移添加到合并转移函数（set自动去重，避免重复转移）
            merged_transitions[from_state][symbol].insert(to_states.begin(), to_states.end());
        }
    }

    为新起始状态添加ε转移：可同时启动nfa1和nfa2
    map<int, set<int>> start_transitions;
    start_transitions[-1] = { nfa1.S, nfa2.S };  // 新起始状态0通过ε转移到两个原NFA的起始状态
    merged_transitions[merged_start_state] = start_transitions;  // 将新起始状态的转移加入转移函数

    // 构造并返回合并后的NFA
    return NFA(merged_states, merged_alphabet, merged_transitions, merged_start_state, merged_Z);
}
//确定ε-CLOSURE（I)
set<int> epsilonClosure(const set<int>& Q){
	
	set<int> closure = Q;  // 初始化闭包，包含当前状态集
	
	stack<int> s;  // 维护一个栈，进行深度优先遍历
	
	for(int i : Q){
		s.push(i);  // 把所有初始状态压入栈
	}
	
	while(!s.empty()){
		int st  = s.top();
		s.pop();
		
		// 如果状态 st 存在 ε 转移（-1 代表 ε）
		if(nfa1.F.count({st, -1})){
			for(int i : nfa1.F[{st, -1}]){
				// 如果状态 i 还未加入闭包，则加入并继续深度遍历
				if(closure.find(i) == closure.end()){
					closure.insert(i);
					s.push(i);
				}
			}
		}
	}
	return closure;  // 返回该状态集的 ε-闭包
}
//DFA确定化
void nfaToDfa(){
	
	queue<set<int>> s;  // 维护一个队列，用于逐步处理 DFA 状态
	
	dfa.U = nfa1.U;  // 继承 NFA 的输入符号集
	
	// 计算 DFA 的起始状态，即 NFA 初始状态的 ε-闭包
	set<int> dfaStart = epsilonClosure({nfa1.S});
	dfa.S = dfaStart;
	
	s.push(dfaStart);  // 将起始状态加入队列
	
	while(!s.empty()){
		set<int> now = s.front();  // 取出队列头部状态集
		s.pop();
		
		dfa.Q.insert(now);  // 记录该状态集为 DFA 的一个状态
		
		// 遍历所有输入符号
		for(int sym: dfa.U){
			set<int> next;  // 存储当前状态集在输入 `sym` 后能到达的状态
			
			for(int i : now){
				// 查找 NFA 中当前状态 `i` 在 `sym` 输入下的转移
				if(nfa1.F.count({i, sym})){
					next.insert(nfa1.F[{i, sym}].begin(), nfa1.F[{i, sym}].end());//把所有能够转移得到的状态加入next中
				}
			}
			
			// 计算该状态集的 ε-闭包
			next = epsilonClosure(next);
		
			if(!next.empty()){
				dfa.F[{now, sym}] = next;  // 记录 DFA 的转移关系
				
				// 如果该状态集还未在 DFA 状态集中，则加入队列进行后续处理
				if(!dfa.Q.count(next)){
					s.push(next);
				}
			}
		}
		
		// 检查当前状态集是否包含 NFA 的终止状态
		for(int i : now){
			if(nfa1.Z.count(i)){
				dfa.Z.insert(now);
				break;  // 只要有一个终止状态就可以确定该状态集是 DFA 终止状态
			}
		}	
	}
}

//DFA改名确定状态
DFA1 rename() {
	
	DFA1 sure_dfa;  // 创建新的 DFA 变量
	
	map<set<int>, int> stateMap;  // 记录 DFA 状态集合到整数编号的映射
	int count = 0;  // 记录新的状态编号
	
	// 遍历原 DFA 的所有状态集合，为每个集合分配一个整数编号
	for (auto i : dfa.Q) {
		sure_dfa.Q.insert(count);  // 直接将编号加入新的 DFA 状态集合
		stateMap[i] = count++;  // 记录映射关系，并自增编号
	}
	
	sure_dfa.U = dfa.U;  // 继承 DFA 的输入符号集
	
	// 处理终止状态，映射为新的编号
	for (auto i : dfa.Z) {
		sure_dfa.Z.insert(stateMap[i]);
	}
	
	// 处理状态转移关系
    for (auto [key, value] : dfa.F) {
        int from = stateMap[key.first];  // 获取起始状态的新编号
        int sym = key.second;  // 获取输入符号
        int to = stateMap[value];  // 获取目标状态的新编号
        sure_dfa.F[{from, sym}] = to;  // 记录新的转移关系
    }
 
	return sure_dfa;  // 返回重命名后的 DFA
}
//DFA最小化
DFA1 miniDFA(const DFA1& sure_dfa){
	set<set<int>> partition;  // 记录当前状态划分的集合（即等价状态类）
	queue<set<int>> worklist; // 存储待处理的状态集合
 
	// 初始划分：终止状态集合 和 非终止状态集合
	partition.insert(sure_dfa.Z);  
	worklist.push(sure_dfa.Z);  // 终止状态组加入队列
	
	set<int> nonState;  // 非终态状态集合
	for (int s : sure_dfa.Q) {
		if (!sure_dfa.Z.count(s)) // 如果 s 不是终止状态
			nonState.insert(s);
	}
	worklist.push(nonState);  // 非终态组加入队列
	partition.insert(nonState);
	
	// 迭代细分状态集合，直到无法进一步划分
	while (!worklist.empty()) {
		
		set<int> current = worklist.front(); // 取出当前待处理的状态集合
		worklist.pop();
		
		// 遍历所有输入符号
		for (int sym : sure_dfa.U) {
			map<set<int>, set<int>> split; // 用于存储按照符号划分后的子集
			
			// 遍历当前状态集合中的状态
			for (int state : current) {
				// 检查该状态在当前符号下的转移情况
				if (sure_dfa.F.count({state, sym})) {
					int next = sure_dfa.F.at({state, sym}); // 获取转移后状态
					
					// 根据转移后的状态所属的划分组，进行细分
					for (auto part : partition) {
						if (part.count(next)) { // 找到 next 所属的组
							split[part].insert(state); // 记录当前 state 属于该组
							break;
						}
					}
				}
			}
			
			// 如果当前状态集合被分割成了多个子集，则需要重新划分
			if (split.size() > 1) {
				for (auto [key, value] : split) {
					partition.insert(value); // 插入新的状态组
					worklist.push(value); // 加入工作队列，继续处理
				}
				
				partition.erase(current); // 移除原有的状态组
				break; // 重新开始迭代
			}
		}
	}
	
	// 构造最小化 DFA
	DFA1 min_dfa;
	map<set<int>, int> stateMap; // 用于存储等价类到新的编号映射
	int count = 0;
 
	// 遍历划分好的等价类，为每个等价类分配一个新的编号
	for (auto part : partition) {
		min_dfa.Q.insert(count);
		stateMap[part] = count++;
	}
	
	min_dfa.U = sure_dfa.U; // 继承输入符号集
	
	// 设定新的起始状态
	for (auto [part, id] : stateMap) {
		if (part.count(sure_dfa.S)) { // 起始状态在哪个等价类里，就赋值新的编号
			min_dfa.S = id;
			break;
		}	
	}
	
	// 设定新的终止状态
	for (auto [part, id] : stateMap) {
		for (int final : sure_dfa.Z) {
			if (part.count(final)) { // 只要等价类中包含终态，则整个类作为终态
				min_dfa.Z.insert(id);
				break;
			}
		}
	}
	
	// 重新构建最小化 DFA 的状态转移表
	for (auto [part, id] : stateMap) { // 遍历每个等价类
		for (int state : part) { // 遍历等价类中的每个状态
			for (int sym : sure_dfa.U) { // 遍历输入符号
				if (sure_dfa.F.count({state, sym})) {
					int next = sure_dfa.F.at({state, sym}); // 取出转移后的状态
					
					// 找到 next 状态属于哪个等价类
					for (auto [nextPart, nextID] : stateMap) {
						if (nextPart.count(next)) {
							min_dfa.F[{id, sym}] = nextID; // 记录转换关系
							break;
						}
					}
				}
			}
		}
	}
		
	return min_dfa; // 返回最小化 DFA
}
//主程序部分
int main(){
	
	ReadNFA();
	nfaToDfa();
	DFA1 sure_dfa=rename();
	DFA1 min_dfa=miniDFA(sure_dfa);
	cout<<"Min_DFA States: ";
	for(auto i:min_dfa.Q){
		cout<<i<<" ";
	}
	cout<<endl;
	cout<<"Min_DFA Symbols: ";
	for(auto i:min_dfa.U){
		cout<<i<<" ";
	}
	cout<<endl;
	cout<<"Min_DFA Transitions: "<<endl;
	for(const auto& entry:min_dfa.F){
		cout<<entry.first.first<<" -- "<<entry.first.second<<" --> "<<entry.second<<endl;
	}
	cout<<"Min_DFA Start State: "<<min_dfa.S<<endl;
	cout<<"Min_DFA Final States: ";
	for(auto i:min_dfa.Z){
		cout<<i<<" ";
	}
	cout<<endl;
	return 0;
}