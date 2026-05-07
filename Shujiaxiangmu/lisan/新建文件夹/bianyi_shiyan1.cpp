#include <bits/stdc++.h>
#include<iostream>
 
using namespace std;
string code;//输入串
 
unordered_map<string, int> word = {
{"main",1}, {"if",2}, {"then",3}, {"while",4},{"do",5},{"static",6},{"ID",25},{"NUM",26},{"+",27},
{"-",28},{"*",29},{"/",30},{":",31},{":=",32},{"<",33},{"<>",34},{"<=",35},{">",36},{">=",37},{"=",38},
{"default",39},{"define",40},{";",41},{"(",42},{")",43},{"{",44},{"}",45},{"int",7},{"double",8},{"struct",9},{"break",10},
{"else",11},{"long",12},{"switch",13},{"case",14},{"typedef",15},{"char",16},{"return",17},{"const",18},
{"float",19},{"short",20},{"continue",21},{"for",22},{"void",23},{"sizeof",24},{"#",0}	
};//关键字表
 
map<string,int> token; 
 
void wordAnalysis(){
	
	int s=0;
	
	while(s<code.length()){
		
		//去除空格、制表符、和换行符
		if(code[s]==' '||code[s]=='\n'||code[s]=='\t'){ 
			s++;	
			continue;
		}
		
		if(code[s]=='/'&&s+1<code.length()&&code[s+1]=='/'){
			//去除单行注释
			s+=2;
			
			while(s<code.length()&&code[s]!='\n'){
				s++;
			}
			
			continue;
		}
		
		if(code[s]=='/'&&s+1<code.length()&&code[s+1]=='*'){
			//去除多行注释
			s+=3;
			
			while(s<code.length()&&(code[s-1]!='*'||code[s]!='/')){
				s++;
			}
			
			continue;
		}
		
		if(isalpha(code[s])||code[s]=='_'){
			//识别标识符和关键字
			string temp;
			temp+=code[s];
			
			s++;
			
			while(s<code.length()&&(isalpha(code[s])||code[s]=='_'||isdigit(code[s]))){
				temp+=code[s];
				s++;				
			}
			
			if(word.count(temp))
			token[temp]=word[temp];
			else
			token[temp]=word["ID"];
			
			continue;
		}
		
		if(isdigit(code[s])){
			
			string temp;
			temp+=code[s];
			//识别普通数字
			s++;
			
			while(s<code.length()&&isdigit(code[s])){
				temp+=code[s];
				s++;
			}
			
			if (s < code.length() && code[s] == '.') {
                temp += code[s];  // 识别浮点数
                s++;
 
                while (s < code.length() && isdigit(code[s])) {
                    temp += code[s];
                    s++;
                }
 
                token[temp] = word["NUM"]; 
            }
			
			else if(s<code.length()&&(isalpha(code[s])||code[s]=='_')){
				//识别错误的以数字开头的标识符
				temp+=code[s];
				s++;
				
				while(s<code.length()&&(isalpha(code[s])||code[s]=='_'||isdigit(code[s]))){
					temp+=code[s];
					s++;				
				}
					
				token[temp]=-1;
			}else{
				token[temp]=word["NUM"];
			}
			
			continue;
		}
		
		string temp(1,code[s]);
        if (word.count(temp)) {
            // 检查双字符操作符
            if ((temp == ":" || temp == "<" || temp == ">") && s + 1 < code.length()) {
                string twoChar = temp + code[s + 1];
                if (word.count(twoChar)) {
                    token[twoChar] = word[twoChar];
                    s += 2; // 跳过两个字符
                    continue;
                }
            }
 
            token[temp] = word[temp];
            s++; // 跳过单字符
        } else {
        	token[temp]=-1;
            s++; 
        }		
			
	}
}
 
int main(){
	
	ifstream file("E:\\Shujiaxiangmu\\lisan\\shiyan1.txt");
	
	if(!file){
		cout<<"file read error!"<<endl;
	}
	
	string line;
	//文件读取
	while(getline(file, line)){
		code+=line;
		code+='\n';
	}
	
	cout<<code;
	
	file.close();
	
	wordAnalysis();
	//打印词法分析后的token
	for(auto i : token){
		if(i.second!=-1)
		cout<<"token: "<<i.first<<" "<<"syn: "<<i.second<<endl;
		else
		cout<<"token: "<<i.first<<" "<<"Invalid Token !!!"<<endl;
	}
 
	return 0;
}