/*
author:albert wang(wangzhuo)
Email:keepwz@163.com or keep@dlmu.edu.cn
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
typedef vector<string> String;
struct STATE {
	String left;
	String right;
	STATE* father;
	STATE* lchild;
	STATE* rchild;
	int index;
	int rule;
};
typedef STATE CNode;
class Solve
{
public:
	void print() {
		print(&this->head);
	}
	Solve(string init) {
		switSym(init);
		CNode t;
		t.father = t.lchild = t.rchild = NULL;
		t.rule = 0; //t.index = ++size;
		t.right.push_back(init);
		this->head = t;
		buffSize = t.right.size() + 40;
		CreatBiTree(&this->head);
	}
private:
	CNode head;
	int buffSize = 0;
	int size = 0;//当前需要扩展的节点的索引值
				 //vector<STATE> res;
				 //vector<int> node2Index;
	static bool cmp(string& a, string& b) {
		return a.size() < b.size();
	}
	static bool isLetter(char c) {
		if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9')) {
			return true;
		}return false;
	}
	bool isAxiom(CNode* nn) {
		//判断当前的节点(String类型)是不是公理
		CNode* st = nn;
		String& l = st->left;
		String& r = st->right;
		String::reverse_iterator itR = needDeal(&r);
		String::reverse_iterator itL = needDeal(&l);
		if (itR == r.rend() && itL == l.rend())
			return true;
		return false;
	}
	void switSym(string & in) {
		//switch & and |
		size_t pos1 = 0;
		size_t pos2 = 0;
		size_t pos3 = 0;
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
	}
	void DealNode(CNode *T, CNode *lchild, CNode * rchild) {
		CNode* cur = T;
		String lstr = cur->left;
		String rstr = cur->right;
		sort(rstr.begin(), rstr.end(), cmp);
		String::reverse_iterator itR = needDeal(&rstr);
		if (itR != rstr.rend()) {
			string sonR = rstr.back();
			rstr.pop_back();
			int be = getType(sonR);//首先需要处理的情况
								   //iv
			if (sonR[be] == '-') {
				string s1 = sonR.substr(0, be);
				string s2 = sonR.substr(be + 2, sonR.size());
				if (s1[0] != '!') s1 = "!" + s1;
				else s1 = s1.substr(1);
				if (s2[0] == '(') {
					s2 = s2.substr(1, s2.size() - 2);
				}
				rstr.push_back(s2);
				rstr.push_back(s1);
				T->rule = 4;
			}//ii
			else
			{
				string s = sonR.substr(1, sonR.size());
				if (s[0] == '(') s = s.substr(1, s.size() - 2);
				lstr.push_back(s);
				T->rule = 2;
			}
			lchild = new CNode();
			//lchild->index = ++size;
			T->lchild = lchild;
			lchild->father = T;
			lchild->lchild = lchild->rchild = NULL;
			lchild->left = lstr;
			lchild->right = rstr;
			return;
		}
		sort(lstr.begin(), lstr.end(), cmp);
		String::reverse_iterator itL = needDeal(&lstr);

		if (itL != lstr.rend()) {
			string sonL = lstr.back();
			lstr.pop_back();
			int be = getType(sonL);//首先需要处理的情况
			if (sonL[be] == '-') {//iii
				string s1 = sonL.substr(0, be);
				string s2 = sonL.substr(be + 2, sonL.size());
				s1 = "!" + s1;
				if (s2[0] == '(') {
					s2 = s2.substr(1, s2.size() - 2);
				}
				String lstr1 = lstr, lstr2 = lstr;
				lstr1.push_back(s1);
				lstr2.push_back(s2);
				lchild = new CNode();
				lchild->father = T;
				lchild->lchild = lchild->rchild = NULL;
				lchild->right = rstr;
				lchild->left = lstr1;
				lchild->index = ++size;

				rchild = new CNode();
				rchild->father = T;
				rchild->lchild = lchild->rchild = NULL;
				rchild->right = rstr;
				rchild->left = lstr2;
				//rchild->index = ++size;

				T->lchild = lchild;
				T->rchild = rchild;
				T->rule = 3;
			}
			else {
				string s = sonL.substr(1, sonL.size());
				if (s[0] == '(') s = s.substr(1, s.size() - 2);
				rstr.push_back(s);
				lchild = new CNode();
				T->lchild = lchild;
				T->rule = 1;
				T->rchild = NULL;
				//lchild->index = ++size;
				lchild->father = T;
				lchild->lchild = lchild->rchild = NULL;
				lchild->left = lstr;
				lchild->right = rstr;
			}
		}

	}
	void CreatBiTree(CNode * T = NULL) {
		if (T == NULL) return;
		if (isAxiom(T)) {
			T->lchild = T->rchild = NULL;
			return;
		}
		CNode* lchild = NULL; CNode* rchild = NULL;
		DealNode(T, lchild, rchild);
		CreatBiTree(T->lchild);
		CreatBiTree(T->rchild);
	}
	String::reverse_iterator needDeal(String* current) {
		//如果 返回值等于 current->end() 说明不需要处理了. 说明是公理了
		String::reverse_iterator nn = current->rbegin();
		for (; nn != current->rend(); ++nn) {
			if (nn->size() == 1)
				continue;
			if ((*nn).find("!") != -1 || (*nn).find("->") != -1)
				break;
		}
		return nn;
	}
	int getType(string str) {
		//传入的str:没有"," 
		if (str[0] == '!' && str[1] == '(') return 0;
		if (str[0] == '!' && isLetter(str[1])) return 2;
		int pos = -1;
		int bra = 0;
		int i = 0;
		do {
			if (str[i] == '(') bra++;
			if (str[i] == ')') bra--;
			i++;
		} while (bra != 0 && i < str.size());
		if (i < str.size() - 1) pos = i;
		return pos;
	}

	void print(CNode* nn) {
		if (nn == NULL) {
			return;
		}

		print(nn->lchild);
		print(nn->rchild);
		static int no = 0;
		int linsize = 0;
		printf("(%2d) ", ++no);
		nn->index = no;
		for (String::iterator i = (*nn).left.begin(); i != (*nn).left.end(); ++i) {
			cout << (*i);
			linsize += i->size();
			if (i != (*nn).left.end() - 1) {
				cout << ","; linsize++;
			}

		}
		cout << "=>";
		for (String::iterator j = (*nn).right.begin(); j != (*nn).right.end(); ++j) {
			cout << (*j);
			linsize += j->size();
			if (j != (*nn).right.end() - 1) {
				cout << ","; linsize++;
			}

		}
		int lenSpace = buffSize - linsize;
		string Space(lenSpace, ' ');
		cout << Space;
		if (nn->rchild == NULL && nn->lchild == NULL) {
			printf("公理\n");
			return;
		}
		int r1 = nn->lchild->index;
		printf("由[ %d ]", r1);
		if (nn->rchild != NULL) {
			int r2 = nn->rchild->index;
			printf("[ %d ]", r2);
		}
		printf("得出,依据规则%d", nn->rule);
		cout << endl;
	}
};

int main()
{
	string str1 = "a->(b->a)";
	string str2 = "(a->b)->(!b->!a)";
	string str3 = "(a->b)->((a->c)->(a->(b&c))))";
	string str4 = "(a->c)->((b->c)->((a|b)->c))";
	string str5 = "(a<->b)->(a->b)";
	string str6 = "(p->(q->r))->((p->q)->(p->r))";
	string test = str3;
	Solve s(test);
	s.print();
	system("pause");
	return 0;
}
