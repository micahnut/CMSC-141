//We hereby declare, upon our honor, that we made this MPA by ourselves, with the help of references.

/*
 * Bojos, Ena Margarita 
 * Chu Im, Micah Nicole
 * Lim, Alyssa Mae
 * Marturillas, Jeza Marie
 * 
 * list of references:
 * 
 * Thompson's Algorithm for RegEx to NFA
 * 	> http://www.cs.may.ie/staff/jpower/Courses/Previous/parsing/node5.html
 * 	> https://xysun.github.io/posts/regex-parsing-thompsons-algorithm.html
 * 	> https://github.com/kennyledet/Algorithm-Implementations/blob/master/Thompsons_Construction_Algorithm/C%2B%2B/AlexMathew/RE_to_NFA.cpp
 * 
 * NFA to DFA
 * 	> https://kartikkukreja.wordpress.com/2013/04/18/nfa-to-dfa-conversion/
 * 	> http://etnichols.com/nfa-to-dfa/
 * 	> https://github.com/swapagarwal/automata-from-regex
 * 	> https://www.sanfoundry.com/cpp-program-construct-dfa-from-nfa/
 * 
 * 
*/
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <set>		
#include <utility>	//http://www.cplusplus.com/reference/utility/pair/
#include <map>		//http://www.cplusplus.com/reference/map/map/
#include <string>
#include <algorithm>
using namespace std;

struct nst{
    vector<int> a[2], e;
    bool f=0;
};
struct dst{
    int a[2] = {-1,-1};
    bool f=0;
};

vector<nst> nfa;
vector<dst> dfa;
stack<int> state;
int nfa_size, dfa_size;
struct nst init_nfa_state;
struct dst init_dfa_state;


void addChara(int);
void union_();
void concatenation();
void kleeneStar();
void postfixToNFA(string);
int priority(char);
string addConcatSymbol(string);
string regexToPostfix(string);
void epsilonClosure(int, set<int>&);
set<int> stateChange(int, set<int>&);
void nfaToDFA(set<int>&, queue<set<int>>&, int);
pair<int,vector<tuple<int,int,bool>>> minimize_dfa(vector<dst>);
bool simulate(int, vector<tuple<int,int,bool>>, string);

int main() {
	
  	ifstream input("mp3.in");
	ofstream output;
	output.open("chuim.out");
	
	string regex, testcase, numOfRegex, numTests, postfix, regexIn; 
	
	getline(input, numOfRegex);
	
	for (int i = 0; i < stoi(numOfRegex); i++) {
		getline(input, regex);
		getline(input, numTests);
		for (int j = 0; j < stoi(numTests); j++) {
			getline(input, testcase);
			
			/*
			cout << "Regex no. " << i+1 << endl;
			cout << "Regex: " << regex << endl;
			cout << "Test no. " << j+1 << " == " << testcase << endl << endl;
			*/
			
			regex.erase( remove( regex.begin(), regex.end(), ' ' ), regex.end() );
			
			if (testcase == "e") { testcase = ""; } //converts empty symbol to "" 
			
			//cout<<regex<<endl;
			
			regexIn = addConcatSymbol(regex);				//adds . as a symbol for concatenation
			
			postfix = regexToPostfix(regexIn);		//translates regex to postfix
			
			postfixToNFA(postfix);					//makes nfa out of the postfix expression
			int final_state = state.top();state.pop();
			int start_state = state.top();state.pop();
			nfa[final_state].f=1;

			set<int> si;
			queue<set<int> > que;
			nfaToDFA(si,que,start_state);
			pair<int,vector<tuple<int,int,bool> > > min_dfa_tmp = minimize_dfa(dfa);
			vector<tuple<int,int,bool> >  min_dfa= min_dfa_tmp.second;
			int start_st = min_dfa_tmp.first;
			bool regOrNo = simulate(start_st,min_dfa, testcase);
			
			
			if(regOrNo){
				output<<"yes"<<endl;
			}else{	
				output<<"no"<<endl;
			}
		}
	}
	input.close();
	output.close();
	return 0;
}

string addConcatSymbol(string regex){ //adds the . symbol for concatenation
    string ret = "";
    char char_1, char_2;
    for(int i=0; i<regex.size(); i++){
        char_1 = regex[i];
        if(i+1 < regex.size()){
            char_2 = regex[i+1];
            ret += char_1;
            if(char_1 != '(' && char_2 != ')' && char_1 != 'U' && char_2 != 'U' && char_2 != '*'){
                ret+='.';
            }
        }
    }
    ret += regex[regex.size()-1];
    return ret;
}

void addChara(int i){				//adds the character to the nfa
    nfa.push_back(init_nfa_state);
    nfa.push_back(init_nfa_state);
    nfa[nfa_size].a[i].push_back(nfa_size+1);
    state.push(nfa_size);
    nfa_size++;
    state.push(nfa_size);
    nfa_size++;
}

void union_(){						//adds the union to the nfa
    nfa.push_back(init_nfa_state);
    nfa.push_back(init_nfa_state);
    int d = state.top(); state.pop();
    int c = state.top(); state.pop();
    int b = state.top(); state.pop();
    int a = state.top(); state.pop();
    nfa[nfa_size].e.push_back(a);
    nfa[nfa_size].e.push_back(c);
    nfa[b].e.push_back(nfa_size+1);
    nfa[d].e.push_back(nfa_size+1);
    state.push(nfa_size);
    nfa_size++;
    state.push(nfa_size);
    nfa_size++;
}

void concatenation(){				//adds the concat to the nfa
    int d = state.top(); state.pop();
    int c = state.top(); state.pop();
    int b = state.top(); state.pop();
    int a = state.top(); state.pop();
    nfa[b].e.push_back(c);
    state.push(a);
    state.push(d);
}

void kleeneStar(){					//adds kleene star to the nfa
    nfa.push_back(init_nfa_state);
    nfa.push_back(init_nfa_state);
    int b = state.top();
    state.pop();
    int a = state.top();
    state.pop();
    nfa[nfa_size].e.push_back(a);
    nfa[nfa_size].e.push_back(nfa_size+1);
    nfa[b].e.push_back(a);
    nfa[b].e.push_back(nfa_size+1);
    state.push(nfa_size);
    nfa_size++;
    state.push(nfa_size);
    nfa_size++;
}

void postfixToNFA(string postfix){	//converts the postfix to nfa
    for(unsigned int i = 0; i < postfix.size(); i++){
		if(postfix[i] == 'a' || postfix[i] == 'b'){
			addChara(postfix[i]-'a'); 
		}else if(postfix[i] == '*'){
			kleeneStar();
		}else if(postfix[i] == '.'){
			concatenation(); 
		}else if(postfix[i] == 'U'){
			union_();
        }
    }
}

int priority(char symbol){			//precedence where kleene star has higher priority
	if(symbol == '*'){
		return 3;
	}else if(symbol == '.'){
		return 2;
	}else if(symbol == 'U'){
		return 1;
	}
    return 0;
}

string regexToPostfix(string regex){	//converts the regex to postfix
    string postfix="";
    stack<char> op;
    char chara;
    for(unsigned int i=0; i<regex.size(); i++){
        if(regex[i] == 'a' || regex[i] == 'b'){
			postfix += regex[i]; 
		}else if(regex[i] == '('){
			op.push(regex[i]);
		}else if(regex[i] == ')'){
			while(op.top()!='('){
				postfix+=op.top();
				op.pop();
			}
			op.pop();
		}else{
			while(!op.empty()){
				chara = op.top();
				if(priority(chara)>=priority(regex[i])){
					postfix+=op.top();
					op.pop();
				}else{ break; }
			}
			op.push(regex[i]);
        }
    }
    while(!op.empty()){
        postfix += op.top();
        op.pop();
    }
    return postfix;
}

void epsilonClosure(int state,set<int>&si){		
    for(unsigned int i=0;i<nfa[state].e.size();i++){
        if(si.count(nfa[state].e[i])==0){
            si.insert(nfa[state].e[i]);
            epsilonClosure(nfa[state].e[i],si);
        }
    }
}

set<int> stateChange(int c,set<int>&si){			//changes the states
    set<int> temp;
    if(c==1){
        for (std::set<int>::iterator it=si.begin(); it!=si.end(); ++it){
            for(unsigned int j=0;j<nfa[*it].a[0].size();j++){
                temp.insert(nfa[*it].a[0][j]);
            }
        }
    }
    else{
        for (std::set<int>::iterator it=si.begin(); it!=si.end(); ++it){
            for(unsigned int j=0;j<nfa[*it].a[1].size();j++){
                temp.insert(nfa[*it].a[1][j]);
            }
        }
    }
    return temp;
}

void nfaToDFA(set<int>&si,queue<set<int> >&que,int start_state){ //converts the nfa to dfa
    map<set<int>, int> mp;
    mp[si]=-1;
    set<int> temp1;
    set<int> temp2;
    int ct=0;
    si.clear();
    si.insert(0);
    epsilonClosure(start_state,si);
    if(mp.count(si)==0){
        mp[si]=ct++;
        que.push(si);
    }
    int p=0;
    bool f1=false;
    while(que.size()!=0){
        dfa.push_back(init_dfa_state);
        si.empty();
        si=que.front();
        f1=false;
        for (set<int>::iterator it=si.begin(); it!=si.end(); ++it){
            if(nfa[*it].f==true)
                f1=true;
        }
        dfa[p].f=f1;
        temp1=stateChange(1,si);
        si=temp1;
        for (set<int>::iterator it=si.begin(); it!=si.end(); ++it){
            epsilonClosure(*it,si);
        }
        if(mp.count(si)==0){
            mp[si]=ct++;
            que.push(si);
            dfa[p].a[0]=ct-1;
        }
        else{
            dfa[p].a[0]=mp.find(si)->second;
        }
        temp1.clear();

        si=que.front();
        temp2=stateChange(2,si);
        si=temp2;
        for (set<int>::iterator it=si.begin(); it!=si.end(); ++it){
            epsilonClosure(*it,si);
        }
        if(mp.count(si)==0){
            mp[si]=ct++;
            que.push(si);
            dfa[p].a[1]=ct-1;
        }
        else{
            dfa[p].a[1]=mp.find(si)->second;
        }
        temp2.clear();
        que.pop();
        p++;
    }
    for(int i=0;i<p;i++){
        if(dfa[i].a[0]==-1)dfa[i].a[0]=p;
        if(dfa[i].a[1]==-1)dfa[i].a[1]=p;
    }
    dfa.push_back(init_dfa_state);
    dfa[p].a[0]=p;
    dfa[p].a[1]=p;
}

//simplifies the dfa
pair<int,vector<tuple<int,int,bool> > > minimize_dfa(vector<dst> dfa) {
    vector<int> grp(dfa.size());    /// Group number for states
    vector<vector<int> > part(2, vector<int>());   

    part[0].push_back(0);
    for(int i=1; i<(int)grp.size(); i++) {
        if(dfa[i].f==dfa[0].f) {
            grp[i]=0;
            part[0].push_back(i);
        } else {
            grp[i]=1;
            part[1].push_back(i);
        }
    }

    if(!part[1].size()) part.erase(part.end());

    //it will loop and will stop when no partitions are created
    bool chk=true;  //checks if new partitions are made
    int strt = 0;   
    while(chk) {
        chk=false;
        //Iterate over partitions and alphabets
        for(int i=0; i<part.size(); i++) {
            for(int j=0; j<2; j++) {
                vector<pair<int,int> > trans(part[i].size());   //Transitions for the states of partitions
                for(int k=0; k<part[i].size(); k++) {
                    if(dfa[part[i][k]].a[j] >= 0)
                        trans[k] = make_pair(grp[dfa[part[i][k]].a[j]],part[i][k]);
                    else
                        trans[k] = make_pair(-1,part[i][k]);
                }
                sort(trans.begin(), trans.end());

                if(trans[0].first!=trans[trans.size()-1].first) {
                    chk=true;

                    int k, m = part.size()-1;

                    part[i].clear();
                    part[i].push_back(trans[0].second);
                    for(k=1; k<trans.size() and (trans[k].first==trans[k-1].first); k++) {
                        part[i].push_back(trans[k].second);
                    }

                    while(k<trans.size()) {
                        if(trans[k].first!=trans[k-1].first) {
                            part.push_back(vector<int> ());
                            m++;
                        }
                        grp[trans[k].second] = m;
                        part[m].push_back(trans[k].second);
                        k++;
                    }
                }
            }
        }
    }

    for(int i=0; i<part.size(); i++) {
        for(int j=0; j<part[i].size(); j++) {
            if(part[i][j]==0) strt=i;
        }
    }

    vector<tuple<int,int,bool> > ret(part.size());
    for(int i=0; i<(int)part.size(); i++) {
        get<0>(ret[i]) = (dfa[part[i][0]].a[0]>=0)?grp[dfa[part[i][0]].a[0]]:-1;
        get<1>(ret[i]) = (dfa[part[i][0]].a[1]>=0)?grp[dfa[part[i][0]].a[1]]:-1;
        get<2>(ret[i]) = dfa[part[i][0]].f;
    }
    return make_pair(strt, ret);
}

bool simulate(int start_st,vector<tuple<int,int,bool> >min_dfa, string input){
	bool yesOrNo = true;
    int curr_state,next_state;
    curr_state=start_st;
    for(unsigned i=0;i<input.size();i++){
        if(input[i]=='a')
            next_state=get<0>(min_dfa[curr_state]);
        else
            next_state=get<1>(min_dfa[curr_state]);
        curr_state=next_state;
    }
    
    if(curr_state >= 0 && get<2>(min_dfa[curr_state])){
		yesOrNo = true;
	}else{
		yesOrNo = false;
	}
    return yesOrNo;
}
