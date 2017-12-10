/*
 * 约瑟夫环 
 * 
 * 问题描述：
 * 约瑟夫问题的一种描述是：编号为1，2，…，n的n个人按顺时针方向围坐一圈，
 * 每人持一个密码（正整数）。一开始任选一个正整数作为报数上限值m，从第一
 * 个人开始按顺时针方向自1开始顺序报数，报到m时停止报数。报m的人出列，将
 * 他的密码作为新的m值，从他在顺时针方向上的下一个人开始重新从1报数，如此
 * 下去，直至所有人全部出列为止。试设计一个程序求出出列顺序。 
 * 
 * 基本要求:
 * 利用单向循环链表存储结构模拟此过程，按照出列的顺序印出各人的编号。 
 * 
 * 测试数据: 
 * M的初值为20；n=7，7各人的密码依次为3，1，7，2，4，8，4，首先m值
 * 为6（正确的出列顺序应为 6,1,4,7,2,3,5）。
 */

#include <iostream>
using namespace std;

#define Elemtype int

typedef struct LNode
{
	Elemtype	num;		//编号
	Elemtype	key;	//密码
	struct LNode *next;
}LNode;

int main(void) {
	unsigned int i, n, m;

	LNode *pHeadNode = new LNode();		//定义头节点
	LNode *pCurrNode = pHeadNode;		//当前节点，现在等于头节点
	LNode *pLastNode = NULL;			//前一个节点

	cout << "请输入初始的报数上限值m:";
	cin >> m;
	if (m < 1) {
		return -1;
	}
	cout << "请输入约瑟夫环的人的个数n:";
	cin >> n;
	if (n < 1) {
		return -1;
	}


	cout << "请输入第1个人的密码:";
	cin >> pCurrNode->key;
	if (pCurrNode->key < 1) {
		return -1;
	}
	pCurrNode->num = 1;

	//构建循环链表
	for (i = 1; i < n; i++) {
		pCurrNode->next = new LNode();		//初始化的时候已经建立了一个节点，所以需要少建立一个节点
		pCurrNode = pCurrNode->next;
		cout << "请输入第" << (i + 1) << "个人的密码:";
		cin >> pCurrNode->key;
		if (pCurrNode->key < 1) {
			return -1;
		}
		pCurrNode->num = i + 1;
	}
	pCurrNode->next = pHeadNode;			//循环链表，头尾相接

	//遍历链表
	pLastNode = pCurrNode;				//尾节点
	pCurrNode = pHeadNode;				//从头节点开始进行遍历
	while (pCurrNode->next != pCurrNode) {		//只剩下一个节点 
		for (i = 1; i < m; i++) {
			pLastNode = pCurrNode;
			pCurrNode = pCurrNode->next;
		}
		//删除第m个节点
		m = pCurrNode->key;
		pLastNode->next = pCurrNode->next;
		cout << pCurrNode->num << " ";
		delete pCurrNode;
		pCurrNode = pLastNode->next;
	}
	//释放空间
	cout << pCurrNode->num << endl;
	delete pCurrNode;
	return 1;
}