#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;
static int EXIT = 0;
static int START = 0;
const int lenSpace = 10;
string Space(lenSpace, ' ');
vector<string> symbol;
vector<int> node;
struct State {
	//imp is implication
	string lstr;
	string rstr;
	int index_rule;//0(公理),1,2,3,4
				   //int rely;	
};
vector<State> res;
static int ig = 0;
struct Tree
{
	int index;
	int father;
	int lchild;
	int rchild;
};
vector<Tree> G;
bool isLetter(char c) {
	if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9')) {
		return true;
	}return false;
}
bool isAxiom(string & l, string & r) {
	//l 是=>左面的蕴含式, r是右边的蕴含式
	if ((l.find("!") != -1 || l.find("->") != -1 ||
		r.find("!") != -1 || r.find("->") != -1))
		return false;
	for (size_t i = 0; i < symbol.size(); i++) {
		string t = symbol.at(i);
		if (l.find(t) != -1 && r.find(t) != -1) return true;
	}
	return false;
}
bool isExist(const string & a) {
	for (size_t i = 0; i < symbol.size(); i++) {
		if (symbol.at(i) == a) {
			return true;
		}
	}
	return false;
}
void dealRight(string & lstr, string & rstr) {
	string ss;
	while (true) {
		int p = rstr.find(",");
		if (p == -1) ss = rstr;
		else
			ss = rstr.substr(0, p);

		int pos = -1;
		//getType(rstr, pos);
		if (pos == -1) return;
		//处理右边有非的形式,利用规则2
		if (pos == 0) {
			//将处理之前的结果放到res中
			State s = { lstr,rstr,2 };
			res.push_back(s);
			Tree t = { ig++ };
			//ss = ss.substr(1);
			if (lstr.size() > 0)
				lstr = lstr + "," + ss;
			else
				lstr = ss;
		}

	}


}
void getType1(string str, int& pos) {
	if (str.find("!") == -1 && str.find("->") == -1) return;
	string ss;	int flag = 1;
	int p = str.find(",");


	if (p == -1) ss = str;
	else
		ss = str.substr(0, p);
	if (str[0] == '!') {
		ss = str.substr(1); flag = 0;
	}

	if (isLetter(ss[0])) {
		pos = 1;
	}

	vector<char> stor;
	for (int i = 0; i < ss.size(); i++) {
		if (ss[i] == '(') stor.push_back(ss[i]);
		if (ss[i] == ')') stor.pop_back();
		if (stor.empty()) {
			if ((i == ss.size() - 1) && flag == 0) pos = 0;
			else if ((i != ss.size() - 1) && flag == 0) pos = i + 2;
			else pos = i + 1;
			break;
		}
	}
}
void getType(string str, int& pos) {
	if (str.find("!") == -1 && str.find("->") == -1) return;
	vector<int> p0;
	p0.push_back(-1);
	while (true) {
		int p = p0.back();
		p = str.find(",", p + 1);
		if (p == -1) break;
		p0.push_back(p);
	}
	p0.push_back(str.size());
	for (int j = 0; j < p0.size() - 1; j++) {
		string ss;
		ss = str.substr(p0.at(j) + 1, p0.at(j + 1) - p0.at(j) - 1);
		getType1(ss, pos);
		if (pos != -1) {
			pos += p0.at(j) + 1;
			return;
		}
	}
}
void printRes() {

}
void prove(int index) {
	//第一次碰到分开的节点才能开始START=1;
	if (EXIT == 1 && START == 1) {
		printRes();
		exit(0);
	}
	State cur = res.at(index);
	if (isAxiom(cur.lstr, cur.rstr)) {

	}

	int p1 = -1, p2 = -1;
	getType(cur.lstr, p1);
	getType(cur.rstr, p2);
	if (p2 >= 0) {//处理ii && iv两种情况
				  /*DEBUG*/
		if (p2 == cur.rstr.size()) {
			cur.rstr = cur.rstr.substr(1, cur.rstr.size() - 2);
			p2 = -1;
			getType(cur.rstr, p2);
		}
		int pos = cur.rstr.find(",");
		if (cur.rstr[p2] == '!') {

			string nextR = cur.rstr.substr(pos + 1);
			string nextL = cur.lstr;
			if (nextL.size() != 0) nextL += ",";
			if (cur.rstr[1] == '(')
				nextL += cur.rstr.substr(2, pos - 3);
			else
				nextL += cur.rstr[1];
			res.push_back({ nextL,nextR,2 });

			Tree t = { ig,G.back().index,-1,-1 };
			G.back().lchild = ig;
			G.push_back(t);
			ig++;

			prove(ig);
		}
		else {
			//iv情况
			//
			if (pos == -1) pos = cur.rstr.size();
			string s1 = "!" + cur.rstr.substr(0, p2);
			string s2;
			if (cur.rstr[p2 + 2] == '(') {
				s2 = cur.rstr.substr(p2 + 3, pos - p2 - 4);
			}
			else s2 = cur.rstr.substr(p2 + 2, pos - p2 - 2);

			string nextR = s1 + "," + s2;
			string nextL = cur.lstr;
			res.push_back({ nextL,nextR,4 });

			Tree t = { ig,G.back().index,-1,-1 };
			G.back().lchild = ig;
			G.push_back(t);
			ig++;

			prove(ig);
		}
	}
	else if (p1 >= 0) {
		int pos = cur.lstr.find(",");
		if (cur.lstr[p1] == '!') {

			string ss = cur.lstr.substr(p1 + 1, pos);
			string nextL = cur.lstr.substr(pos + 1);
			string nextR;
			if (cur.lstr.size() == 0) nextR = ss;
			else nextR = cur.lstr + "," + ss;
			//res.push_back({ nextL,nextR,2 });

			Tree t = { ig,G.back().index,-1,-1 };
			G.back().lchild = ig;
			G.push_back(t);
			ig++;

			prove(ig);
		}
	}
	else {
		return;
	}
}
void getRes(string & l, string & r, int father) {
	if (father < (int)res.size()) return;
	if (isAxiom(l, r)) {
		State t = { string(l + "=>" + r),0 };
		res.push_back(t);
		return;
	}
	dealRight(l, r);
	//dealLeft(l, r);
	while (true) {
		if (r.find("!") == -1 && r.find("->") == -1) break;
		//pos = r.find(",");
		dealRight(l, r);
	}
	while (true) {
		int pos = l.find("->");
		if (pos == -1) break;
		//dealLeft(l, r);
	}
}
//未完成(非必需)
//先不考虑'&'和'|'两边是()的形式.所以main()中不需要调用这个函数.
//void getPos(int & l, int & r, string & str, char flag,int pos) {
//	//取值方式: left: [ , ) right( , ]
//	if (str.at(pos - 1) != ')') {
//		l = pos - 1;
//	}
//	if (str.at(pos + 1) != '(') {
//		r = pos + 1;
//		if (l != -1)
//			return;
//	}
//	vector<char> bracket;
//	bracket.push_back(')');
//	int i = pos - 2;
//	while (!bracket.empty() && i>=0) {
//		char t = str.at(i);
//		if (t == ')' || t == '(') {
//			if (bracket.back() != t) {
//				bracket.pop_back();
//			}
//			else {
//				bracket.push_back(t);
//			}
//		}
//		i--;
//	}
//	
//}
int main()
{
	string str1 = "a->(b->a)";
	string str2 = "(a->b)->(!b->!a)";
	string str3 = "(a->b)->((a->c)->(a->(b&c)))";
	string str4 = "(a->c)->((b->c)->((a|b)->c))";
	string str5 = "(a<->b)->(a->b)";
	string str6 = "(p->(q->r))->((p->q)->(p->r))";
	string in = str6;
	//res.push(string(""));//把0号覆盖
	for (size_t i = 0; i < in.size();)
	{
		if (isLetter(in[i])) {
			string sym = "";
			while (isLetter(in[i])) {
				sym += in[i++];
			}
			if (!isExist(sym))
				symbol.push_back(sym);
		}
		else {
			i++;
		}
	}
	size_t pos1 = 0;
	size_t pos2 = 0;
	size_t pos3 = 0;
	//处理有多个&或者|的形式
	while (pos1 != -1 || pos2 != -1 || pos3 != -1) {
		pos1 = in.find("&");
		if (pos1 != -1)
			in = in.substr(0, pos1 - 1) + "!(" + in.at(pos1 - 1) + "->!" + in.substr(pos1 + 1);
		pos2 = in.find("|");
		if (pos2 != -1)
			in = in.substr(0, pos2 - 1) + "!" + in.at(pos2 - 1) + "->" + in.substr(pos2 + 1);
		pos3 = in.find("<->");
		if (pos3 != -1)
			// "(a<->b)->(a->b)"; (!((a->b)->!(b->a)))->(a->b)
			in = in.substr(0, pos3 - 1) + "!((" + in.at(pos3 - 1) + "->" +
			in.at(pos3 + 3) + ")->!(" + in.at(pos3 + 3) + "->" + in.at(pos3 - 1) + "))"
			+ in.substr(pos3 + 4);
	}
	cout << in << endl;
	//初始化
	res.push_back({ "",in,-1 }); G.push_back({ ig,-1,-1,-1 });
	//getRes(res.back().lstr,res.back().rstr,0);
	prove(0);
	system("pause");
	return 0;
}