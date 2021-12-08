#pragma once
#include <stdlib.h>
#include <time.h>
#include<string>
#include<iostream>
#include<cstdlib>
//#define DEBUG

const int Maxproclen = 10000;// �����10000
int process[Maxproclen];
int processlen = 2048; //Ĭ�ϳ���2048

const int Maxexelen = 50000;// ִ���20000
int exeorder[Maxexelen] = { 0 };//ִ��˳��
int exetype[Maxexelen] = { 0 };
int exelen = 1; //ִ�д����������һ��ָ��λ��Ϊ0
int exeorderptr = 0; //ִ��ָ�����ڵĴ���λ�ã���̭�㷨ʹ��

const int MaxExePageLen = 2000;
int exepageorder[MaxExePageLen] = { 0 }; //ִ��ʱҳ˳��
int exepagelen = 0;
int exepageptr = 0; //ִ��ָ����������ҳ����λ�ã�OPTʹ��

// 0"���", 1"˳��",2"��֧",3"��ת",4"ѭ��"
std::string ordertype[5] = { "���","˳��","��֧","��ת","ѭ��" };

const int ramlen = 512; //�ڴ泤��512
int ram[ramlen]; //�ڴ�
int pagelen = 128; //ÿҳĬ�ϳ���128
//int pagenum = 4;   //Ĭ�Ϸ�4ҳ

//����ҳ��ṹ
typedef struct vPagetable {
	int vpagenum = -1; //ҳ�� ������ʡ�ԣ�Ϊ����ʽ��Ӧ��ӣ�
	int rampagenum = -1; //ҳ��ţ�
	int I = 0; //�ж�λ�����ڴ���Ϊ1������Ϊ0
};

//���̿��ƿ�
typedef struct ProcCtl
{
	int vpagenum = 16; //Ĭ��16
	vPagetable ptable[32];//����ҳ��������أ�ͨ������ҳ��ֱ���ҵ���Ӧ��ҳ���
	int* vpageptr[32]; //ָ������ҳ����ʼ��ַ
};

//ҳ������
typedef struct rPagetable {
	int rampagenum = -1; //ҳ��ţ�����ʡ�ԣ�Ϊ����ʽ��Ӧ��ӣ�
	int vpagenum = -1; //ҳ��
	int I = 1; // �Ƿ����
};

//�ڴ����ṹ
typedef struct RamCtl
{
	//int isvacant = 1; //�Ƿ���ڿ��õ�ҳ��
	int rpagenum = 4; //ҳ��������Ĭ��Ϊ4
	int rpageUsednum = 0; //��ʹ��ҳ����
	rPagetable rtable[10];//�ڴ����� ҳ��Ŷ�Ӧ�Ľ�������ҳ��
	int* rpageptr[10];//ָ�����ҳ����ʼ��ַ
};

ProcCtl pCtl;//���̿��ƿ�
RamCtl rCtl;//�ڴ���ƿ�


int findtimes = 0;//����ҳ����
int pagefault = 0;//ȱҳ����ͳ��

//LRU�����㷨
int  LRU();
//OPT�����㷨
int OPT();

int (*selectAlg[2])() = { LRU,OPT };
int  selectAlgnum = 0; //ѡ��������̭�㷨
int do_page_fault(int pnum);
int vaddrToraddr(int vaddr);
int Findrampage(int pnum);
int VpageToRpage(int vpnum, int rnum);
void init();