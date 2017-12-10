/*
 * 哈夫曼编/译码器
 * 
 * 问题描述：
 * 利用哈夫曼编码进行通信可以大大提高信道的利用率，缩短信息传输时间，
 * 降低传输成本。但是，这要求在发送端通过一个编码系统对待传数据预先
 * 编码，在接收端将传来的数据进行译码（复原）。对于双工信道（即可以
 * 双向传输信息的信道），每端都需要一个完整的编/译码系统。试为这样
 * 的信息收发站设计一个哈夫曼码的编/译码系统。
 * 
 * 基本要求：
 * 一个完整的系统应具备以下功能：
 * （1）I：初始化（Initialization）。从终端读入字符集大小n，以及n
 * 个字符和n个权值，建立哈夫曼树，并将它存于文件hfmTree中。  
 * （2）E：编码（Encoding）。利用已建好的哈夫曼树（如不在内存，
 * 则从文 件hfmTree中读入），对文件ToBeTran中的正文进行编码，然后将
 * 结果存入文件CodeFile中。  
 * （3）D：译码（Decoding）。利用已建好的哈夫曼树将文件CodeFile中
 * 的代码进行译码，结果存入文件TextFile中。  
 * （4）P：印代码文件（Print）。将文件CodeFile以紧凑格式显示在终
 * 端上，每行50个代码。同时将此字符形式的编码文件写入文件CodePrin中。  
 * （5）T：印哈夫曼树（Tree printing）。将已在内存中的哈夫曼树以
 * 直观的 方式（树或凹入表形式或广义表）显示在终端上，同时将此字符
 * 形式的哈夫曼树写入文件TreePrint中。
 * 
 * 测试数据  
 * （1）利用下面这道题中的数据调试程序。  
 * 某系统在通信联络中只可能出现八种字符，其概率分别为0.25，0.29，
 * 0.07，0.08，0.14，0.23，0.03，0.11，试设计哈夫曼编码。  
 * （2）用下表给出的字符集和频度的实际统计数据建立哈夫曼树，并实现
 * 以下报文的编码和译码：“THIS PROGRAM IS MY FAVORITE”。  
 * 字符  空格  A   B   C   D   E   F   G   H   I   J   K   L   M 
 * 频度  186  64  13  22  32 103  21  15  47  57  1   5   32  20 
 * 字符  N    O   P   Q   R   S   T   U   V   W   X   Y   Z  
 * 频度  57   63  15  1   48  51  80  23  8   18  1   16  1 
 */

#include <iostream>  
#include <string.h>  
using namespace std;

#define N 27        /*叶子结点数*/  
#define M 2*N-1     /*树中结点总数*/  
//存储哈夫曼树
typedef struct {
	char data;      //节点
	int weight;     //权重
	int parent;     
	int lchild;     
	int rchild;     
} HTNode, *HuffmanTree;

//存储哈夫曼编码
typedef struct {
	char cd[N];     
	int start;
} HCode, *HuffmanCode;

void CreateHT(HuffmanTree ht, int n);
void Select(HuffmanTree pHT, int k, int &min1, int &min2);
int min(HuffmanTree pHT, int k);

//创建哈夫曼树
void CreateHT(HuffmanTree ht, int n) {
	int i;

	for (i = 0; i < 2*n-1; i++) {	//初始化序列
		ht[i].parent = -1;
		ht[i].lchild = -1;
		ht[i].rchild = -1;
	}
	int min1, min2;			//权值最小的数的序列
	for(i = n; i < 2 * n - 1; i++) {
		Select(ht, i, min1, min2);	//选择出没有双亲的且权值最小的两个节点的序号
		ht[min1].parent = i;		//连接这两个叶子节点的双亲
		ht[min2].parent = i;
		ht[i].lchild = min1;		//建立新节点的左右孩子
		ht[i].rchild = min2;
		ht[i].weight = ht[min1].weight + ht[min2].weight;	
	}
}

//查找weight最小的两个节点的序号
void Select(HuffmanTree ht, int k, int &min1, int &min2) {
	min1 = min(ht, k);
	min2 = min(ht, k);
}

//从前k个元素中找到weight最小的点的序号
int min(HuffmanTree pHT, int k) {
	int i = 0;
	int minIndex;
	int minWeight;

	while (pHT[i].parent != -1) {	//查找第一个没有被选择过的叶子节点
		i++;
	}
	minIndex = i;
	minWeight = pHT[i].weight;

	for (; i < k; i++) {
		if (pHT[i].weight < minWeight && pHT[i].parent == -1) {
			minIndex = i;
			minWeight = pHT[i].weight;
		}
	}
	pHT[minIndex].parent = -2;
	return minIndex;
}

//从叶子到根逆向求每个字符的哈夫曼编码
void CreateHCode(HTNode ht[], HCode hcd[], int n) {    
	int i, f, c;
	HCode hc;
	for (i = 0; i < n; i++) {
		hc.start = n; c = i;
		f = ht[i].parent;
		while (f != -1) {
			if (ht[f].lchild == c)
				hc.cd[hc.start--] = '0';
			else
				hc.cd[hc.start--] = '1';
			c = f; f = ht[f].parent;
		}
		hc.start++;
		hcd[i] = hc;
	}
}

//显示建立完后的每个字符的哈夫曼编码
void DispHCode(HuffmanTree ht, HuffmanCode hcd, int n) {  
	int i, k;
	int sum = 0, m = 0, j;
	cout << "该字符集的哈夫曼编码如下: " << endl;
	for (i = 0; i < n; i++) {
		j = 0;
		cout << ht[i].data << " ";
		for (k = hcd[i].start; k <= n; k++) {
			cout << hcd[i].cd[k];
		}
		cout << endl;
	}
}

//显示输入字符串的哈夫曼编码
void DisplayHuffmanCode(char *s, HTNode HT[], HCode hcd[], int n) {
	int i = 0;
	int j;
	int k = 0;
	cout << "该字符串的哈夫曼编码如下: " << endl;
	while (s[i] != '\0') {
		for (j = 0; j < n; j++) {
			if (s[i] == HT[j].data) {
				for (k = hcd[j].start; k <= n; k++)
					cout << hcd[j].cd[k];
				//cout << "_";
				break;
			}
		}
		i++;
	}
	cout << endl;
}

//查找哈夫曼编码对应的字符
void FindHuffmanCode(char *s, HuffmanTree ht) {
	int i = 0, j = 0;
	int index;
	int root = 0;

	while (ht[root].parent != -1) {	//查找第一个没有被选择过的叶子节点
		root++;
	}

	while (s[i] != '\0') {
		index = s[i] - '0';
	//	cout << index << endl;
		j = root;
		while (ht[j].lchild != -1 || ht[j].rchild != -1) {
			if (index == 0) {
				j = ht[j].lchild;
			}
			else
				j = ht[j].rchild;
			i++;
			index = s[i] - '0';	//获取下一个字符
		}
		cout << ht[j].data;
	//	j = root;
	//	i++;
	}
}

int main() {
	int n = 27, i;
	char s[100] = "THIS PROGRAM IS MY FAVORITE";	//输入字符串
	char code[200];
	char str[] = { ' ','A','B','C','D','E','F','G','H','I','J','K','L',
		'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' }; /*字符集*/
	int fnum[] = { 186,64,13,22,32,103,21,15,47,57,1,5,32,20,57,
		63,15,1,48,51,80,23,8,18,1,16,1 }; /*字符集对应的权值*/

	HTNode ht[M];		//哈夫曼树
	HCode hcd[N];		//哈夫曼编码

	//节点和节点的权值
	for (i = 0; i < n; i++) {
		ht[i].data = str[i];
		ht[i].weight = fnum[i];
	}

	CreateHT(ht, n);	//创建哈夫曼树

	CreateHCode(ht, hcd, n);	//创建哈夫曼编码
	DispHCode(ht, hcd, n);
	
//	*s =  "THIS PROGRAM IS MY FAVORITE";
	cout << "***********************************" << endl;

	DisplayHuffmanCode(s, ht, hcd, n);

	
	cout << "请输入你要查找的字符的哈夫曼编码:" << endl;
	scanf("%s", code);
//	cout <<  << endl;

	FindHuffmanCode(code, ht);
	cout << endl;
	return 0;
}

//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//
////动态分类数组存储哈夫曼树
//typedef struct {
//	char			data;	
//	unsigned int	weight;
//	unsigned int	parent, lchild, rchild;
//}HTNode, *HuffmanTree;
////动态分配数组存储哈夫曼编码
//typedef char HTCode, **HuffmanCode;
//
//
//void CreateHT(HTNode pHT[], int n);
//void Select(HuffmanTree pHT, int k, int &min1, int &min2);
//int min(HuffmanTree pHT, int k);
//void HuffmanCoding(HuffmanTree HT, HuffmanCode &HC, int n);
//void DisplayHuffmanCode(char *s, HuffmanTree HT, HuffmanCode &HC, int n);
//
////创建哈夫曼树
//void CreateHT(HuffmanTree pHT, int n) {
//	int i;
//	int m = 2 * n - 1;	//节点数目
////	HuffmanTree HT = (HuffmanTree)malloc(m * sizeof(HTNode));
//	for (i = 0; i < n; i++) {	//初始化序列
////		pHT[i].weight = *w;		w++;
//		pHT[i].parent = -1;
//		pHT[i].lchild = -1;
//		pHT[i].rchild = -1;
//	}	
//	int min1, min2;
//	for (i = n; i < m; i++) {	// 设置n个叶子节点的根节点
//		Select(pHT, i, min1, min2);
//		pHT[min1].parent = i;
//		pHT[min2].parent = i;
//		pHT[i].lchild = min1;
//		pHT[i].rchild = min2;
//		pHT[i].weight = pHT[min1].weight + pHT[min2].weight;
//	}
//}
//
////查找weight最小的两个节点的序号
//void Select(HuffmanTree pHT, int k, int &min1, int &min2) {
//	min1 = min(pHT, k);
//	min2 = min(pHT, k);
//}
//
////从前k个元素中找到weight最小的点的序号
//int min(HuffmanTree pHT, int k) {
//	int i = 0;
//	unsigned int minIndex;
//	unsigned int minWeight;
//
//	while (pHT[i].parent != -1) {	//查找第一个没有被选择过的叶子节点
//		i++;
//	}
//	minIndex = i;
//	minWeight = pHT[i].weight;
//
//	for (; i < k; i++) {
//		if (pHT[i].weight < minWeight && pHT[i].parent == -1) {
//			minIndex = i;
//			minWeight = pHT[i].weight;
//		}
//	}
//
//	pHT[minIndex].parent = 0;
//
//	return minIndex;
//}
//
////从叶子到根逆向求每个字符的哈夫曼编码
//void HuffmanCoding(HuffmanTree HT, HuffmanCode &HC, int n) {
//	int i;
////	HC = (HuffmanCode)malloc(n * sizeof(char *));	//分配n个字符编码的头指针向量
//	char * cd = (char *)malloc(n * sizeof(char));	//分配求编码的工作空间 
//	cd[n - 1] = '\0';		//编码结束符
//
//	//求每个字符的哈夫曼编码
//	for (i = 0; i < n; i++) {
//		int start = n - 1;		//编码结束符位置
//		int father = HT[i].parent;
//		int current = i;
//		for (; father != -1; current = father, father = HT[father].parent) {	//逐个字符求哈夫曼编码
//			if (HT[father].lchild == current)
//				cd[--start] = '0';
//			else
//				cd[--start] = '1';
//		}
//		HC[i] = (char *)malloc((n - start) * sizeof(char));	//为第i个字符编码分配空间
//		strcpy(HC[i], &cd[start]);
//	}
//	free(cd);
//}
//
//void DisplayHuffmanCode(char *s, HuffmanTree HT, HuffmanCode &HC, int n) {
//	int i = 0;
//	int j;
//	while (s[i] != '\0') {
//		for (j = 0; j < n; j++) {
//			if (s[i] == HT[j].data) {
//				printf("%s\n",HC[j]);
//				break;
//			}
//		}
//		i++;
//	}
//}
//
//int main() {
//	int n = 27;
//	int m = 2 * n - 1;
//	int i;
//	char str[] = { ' ','A','B','C','D','E','F','G','H','I','J','K','L',
//		'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
//	int sweight[] = { 186,64,13,22,32,103,21,15,47,57,1,5,32,20,57,
//		63,15,1,48,51,80,23,8,18,1,16,1};		/*字符集对应的权值*/
//	char word[] = "THIS PROGRAM IS MY FAVORITE";
//
//
//	HuffmanTree HT = (HuffmanTree)malloc(m * sizeof(HTNode));
//	for (i = 0; i < n; i++) {	//初始化序列
//		HT[i].data		= str[i];
//		HT[i].weight	= sweight[i];
//	}
//	CreateHT(HT, n);
//
//	HuffmanCode HC = (HuffmanCode)malloc(n * sizeof(char *));
//	HuffmanCoding(HT, HC, n);
//	DisplayHuffmanCode(word, HT, HC, n);
//	return 0;
//
//
//}

//#include "iostream"
//using namespace std;
//
//#define N	50		/*叶子节点数目*/
//#define M	2*N-1	/*树的节点总数*/
//typedef struct {
//	char	data;		/*节点值*/
//	int		weight;		/*权重*/
//	int		parent;		/*双亲节点*/
//	int		lchild;		/*左孩子节点*/
//	int		rchild;		/*右孩子节点*/
//}HuffmanTreeNode;
//
//typedef struct {
//	char	cd[N];		/*存放哈夫曼编码*/
//	int		start;		/*编码结束符位置*/
//}HuffmanCodingNode;
//
//int main() {
//	int n = 27, i;
//	char str[] = { ' ','a','b','c','d','e','f','g','h','i','j','k','l',
//		'm','n','o','p','q','r','s','t','u','v','w','x','y','z' };	/*字符集*/
//	int sweight[] = {186,64,13,22,32,103,21,15,47,57,1,5,32,20,57,
//		63,15,1,48,51,80,23,8,18,1,16,1};		/*字符集对应的权值*/
//	char key[] = { "THIS PROGRAM IS MY FAVORITE" };
//
//	HuffmanTreeNode ht[M];
//	HuffmanCodingNode hcd[N];
//
//	for (i = 0; i < n; i++) {
//		ht[i].data = str[i];
//		ht[i].weight = sweight[i];
//	}
//
//	//创建哈夫曼树
//	CreateHuffmanTree(ht, n);
//	CreateHuffmanCoding(ht, hcd, n);
//	DisplayHuffmanCoding(key, ht, hcd, n);
//	return 0;
//
//
//}
//
///*创建哈夫曼树*/
//void CreateHuffmanTree(HuffmanTreeNode ht[], int n) {
//	int i, k;
//	int min1, min2;			//min1标识当前weight最小的结点，位于二叉树的左孩子结点。
//	int lnode, rnode;
//	for (i = n; i <= 2 * n - 1; i++) {
//		//从ht中选出parent为0且wight最小的两个结点
//		min1 = 32767, min2 = 32767;
//		lnode = rnode = -1;
//		for (k = 0; k < i - 1; k++) {
//			if (ht[k].parent == -1) {
//				if (ht[k].weight < min1) {
//					min2 = min1;
//					rnode = lnode;
//					min1 = ht[k].weight;
//					lnode = k;
//				}
//				else if (ht[k].weight < min2) {
//					min2 = ht[k].weight;
//					rnode = k;
//				}
//			}
//		}
//		ht[i].weight = min1 + min2;
//		ht[i].lchild = lnode;
//		ht[i].rchild = rnode;
//		ht[lnode].parent = i;
//		ht[rnode].parent = i;
//	}
//}
//
///*从叶子节点到根逆向求每个字符的哈夫曼编码*/
//void CreateHuffmanCoding(HuffmanTreeNode ht[], HuffmanCodingNode hcd[], int n) {
//	int i, p, c;
//	HuffmanCodingNode hc;
//	for (i = 0; i < n; i++) {
//		hc.start = n;
//		c = i;
//		p = ht[i].parent;
//		while (p != -1) {
//			if (ht[p].lchild == c) {
//				hc.cd[hc.start--] = '0';
//			}
//			else
//				hc.cd[hc.start--] = '1';
//			c = p;
//			p = ht[p].parent;
//		}
//		hc.start++;
//		hcd[i] = hc;
//	}
//}
//
///*显示各个字符的哈夫曼编码*/
//void DisplayHuffmanCoding(char *s, HuffmanTreeNode ht[], HuffmanCodingNode hcd[], int n) {
//	int i = 0, k;
//
//	while (s[i] != '\0') {
//		if (s[i] == ht[i].data) {
//			cout << s[i] << "的哈夫曼编码是" << end;
//			for (k = hcd[i].start; k <= n; k++) {
//				cout << hcd[i].cd[k];
//			}
//			cout << endl;
//		}
//		i++;
//	}
//}