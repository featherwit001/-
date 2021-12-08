#pragma once
#include <stdlib.h>
#include <time.h>
#include<string>
#include<iostream>
#include<cstdlib>
//#define DEBUG

const int Maxproclen = 10000;// 进程最长10000
int process[Maxproclen];
int processlen = 2048; //默认长度2048

const int Maxexelen = 50000;// 执行最长20000
int exeorder[Maxexelen] = { 0 };//执行顺序
int exetype[Maxexelen] = { 0 };
int exelen = 1; //执行代码次数，第一条指令位置为0
int exeorderptr = 0; //执行指令所在的次序位置，淘汰算法使用

const int MaxExePageLen = 2000;
int exepageorder[MaxExePageLen] = { 0 }; //执行时页顺序
int exepagelen = 0;
int exepageptr = 0; //执行指令所在虚拟页次序位置，OPT使用

// 0"随机", 1"顺序",2"分支",3"跳转",4"循环"
std::string ordertype[5] = { "随机","顺序","分支","跳转","循环" };

const int ramlen = 512; //内存长度512
int ram[ramlen]; //内存
int pagelen = 128; //每页默认长度128
//int pagenum = 4;   //默认分4页

//虚拟页表结构
typedef struct vPagetable {
	int vpagenum = -1; //页号 （可以省略，为了显式对应添加）
	int rampagenum = -1; //页框号；
	int I = 0; //中断位，在内存中为1，不在为0
};

//进程控制块
typedef struct ProcCtl
{
	int vpagenum = 16; //默认16
	vPagetable ptable[32];//虚拟页表，进程相关，通过虚拟页号直接找到对应的页框号
	int* vpageptr[32]; //指向虚拟页的起始地址
};

//页框管理表
typedef struct rPagetable {
	int rampagenum = -1; //页框号（可以省略，为了显式对应添加）
	int vpagenum = -1; //页号
	int I = 1; // 是否可用
};

//内存管理结构
typedef struct RamCtl
{
	//int isvacant = 1; //是否存在可用的页框
	int rpagenum = 4; //页框数量，默认为4
	int rpageUsednum = 0; //已使用页框数
	rPagetable rtable[10];//内存管理表， 页框号对应的进程虚拟页号
	int* rpageptr[10];//指向各个页框起始地址
};

ProcCtl pCtl;//进程控制块
RamCtl rCtl;//内存控制块


int findtimes = 0;//访问页次数
int pagefault = 0;//缺页次数统计

//LRU调度算法
int  LRU();
//OPT调度算法
int OPT();

int (*selectAlg[2])() = { LRU,OPT };
int  selectAlgnum = 0; //选择哪种淘汰算法
int do_page_fault(int pnum);
int vaddrToraddr(int vaddr);
int Findrampage(int pnum);
int VpageToRpage(int vpnum, int rnum);
void init();