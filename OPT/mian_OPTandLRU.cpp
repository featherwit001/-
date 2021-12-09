#include"need.h"


//随机数生成
int randnum()
{
	return rand() % 100 + 1;
}

//进程代码填充
int ProcessPadding()
{
	srand(time(NULL));
	//随机填充1-100数字至process 中
	for (int i = 0; i < processlen - 1; i++)
	{
		process[i] = randnum();
	}

	//标记结束
	process[processlen - 1] = -1;

	return 0;
}

//执行顺序填充  exetype填充存在错位
void OrderPadding()
{
	int codeip = 0; //代码位置指针ip，指向下一条执行的指令的地址，不可超过processlen
	int code = 0; //指令的具体值
	exelen = 1;//exelen 重置为1


	//根据process中的数据确定执行顺序
	while (1)
	{
		if (codeip < processlen && code >= 0)  // codeip： 0 <--> processlen-1
		{
			code = process[codeip]; //取值
		}
		else
			break;// 超出范围 执行结束

		if (code == -1) //process 代码结束
		{
			break;
		}
		else if (code == 0)//随机执行
		{
			srand(time(NULL));
			codeip = rand() % processlen; // codeip： 0 <--> processlen-1
			exetype[exelen - 1] = 0;
			exeorder[exelen++] = codeip;
		}
		else if (0 < code && code <= 70) // 模拟顺序执行
		{
			codeip++;
			if (codeip >= processlen)
				break;
			exetype[exelen - 1] = 1;
			exeorder[exelen++] = codeip;
		}
		else if (70 < code && code <= 80) // 模拟分支
		{
			exetype[exelen - 1] = 2;
			if (code <= 85)
			{
				codeip++;
				if (codeip >= processlen)
					break;

				exeorder[exelen++] = codeip;
			}
			else
			{
				codeip += 16;
				if (codeip >= processlen)
					break;
				exeorder[exelen++] = codeip;
			}
		}
		else if (80 < code && code <= 90) // 模拟跳转
		{

			while (true)
			{
				int offset = rand()%(processlen /2) - (processlen / 4); // 随机向后或者向前跳 1/4 总举例，默认 -4*128 <--> 4*128
				if ((codeip + offset) < processlen && (codeip + offset) >= 0) //跳转范围合理
				{
					codeip += offset;
					break;
				}
			}
			
			
				
			exetype[exelen - 1] = 3;
			exeorder[exelen++] = codeip;
		}
		else if (90 < code && code <= 100)//模拟循环
		{
			exetype[exelen - 1] = 4;
			//srand(time(NULL));
			int looptime = (rand() % 5 + 1); //循环次数 1-5
			int loopcode = (rand() % 8 + 1); //循环代码数量 1-8
			//循环中代码超出范围
			if ((codeip + loopcode) >= processlen)
				break;

			for (int i = 0; i < looptime; i++)
			{
				//printf("循环\n");
				for (int j = 1; j < loopcode; j++)
				{
					codeip++;

					if (codeip >= processlen)
						break;

					exetype[exelen - 1] = 4; //循环体内
					exeorder[exelen++] = codeip;
				}
				exetype[exelen] = 4;//跳转
				codeip -= loopcode;
			}
			//循环出口位置
			codeip += loopcode;
			exeorder[exelen++] = codeip;
		}
	}
}



//虚拟页调入顺序填充，用于实现OPT算法
void PageOrderPadding()
{
	int codeip = 0;
	exepagelen = 0;
	for (int i = 0; i < exelen; i++)
	{
		codeip = exeorder[i];
		int codeVpage = codeip / pagelen;//获取虚拟页号
		//如果页发生改变则记录页顺序
		if (i == 0)
		{
			exepageorder[exepagelen++] = codeVpage;
		}
		if(exepagelen!=0 && codeVpage != exepageorder[exepagelen-1])
			exepageorder[exepagelen++] = codeVpage;
			
	}


}

//页信息初始化
int PageInit()
{
	//页框配置
	rCtl.rpagenum = ramlen / pagelen;//确定页框数
	rCtl.rpageUsednum = 0; //已使用的内存数量
	for (int i = 0; i < rCtl.rpagenum; i++)
	{
		rCtl.rpageptr[i] = &ram[i * pagelen]; //确定每个页框对应的内存起始地址
		rCtl.rtable[i].rampagenum = i;//内存管理表中确认页框号顺序
		rCtl.rtable[i].I = 1;//页框可用
	}


	//进程虚拟页配置
	pCtl.vpagenum = (processlen - 1) / pagelen + 1; //虚拟页数，向上取整
	for (int i = 0; i < pCtl.vpagenum; i++)
	{
		pCtl.vpageptr[i] = &process[i * pagelen]; //确定每个虚拟页在进程中起始地址
		pCtl.ptable[i].vpagenum = i;//内存管理表中确认虚拟页号顺序
		pCtl.ptable[i].I = 0;//虚拟页未调入
	}
	return 0;
}



void parainput()
{
	std::string s;
	std::string v = "";
	char str[10];
	char c;
	printf("请输入进程长度，默认%d：", processlen);
#ifndef DEBUG
	while ((c = getchar()), c != '\n')
	{
		s += c;
	}

	if (s == v)
		;
	else {
		processlen = stoi(s);
	}
#endif // DEBUG

	s = "";
	printf("\n请输入页长度，默认%d：", pagelen);
#ifndef DEBUG
	while ((c = getchar()), c != '\n')
	{
		s += c;
	}

	if (s == v)
		;
	else {
		pagelen = stoi(s);
	}
#endif // !DEBUG
	printf("\n");

	
}


//初始化
void init()
{
	

	PageInit();

	ProcessPadding();

#ifndef DEBUG
	OrderPadding(); 
	PageOrderPadding();
#else
	int codeipOrder[] = { 0,128,256,512,1024,1,2,513, 257};
	int size = 9;
	for (int i = 0; i < size; i++)
	{
		exeorder[exelen++] =  codeipOrder[i];
	}
	PageOrderPadding();
#endif // !DEBUG
	memset(ram, 0, ramlen); //清理内存

}

//LRU调度算法
int  LRU()
{
	int selectedrpagenum = -1;
	int farthest = 0;

	for (int i = 0; i < rCtl.rpagenum; i++)
	{
		if (rCtl.RpageUsedTimes[i] >= farthest)
		{
			farthest = rCtl.RpageUsedTimes[i];
			selectedrpagenum = i;
		}

	}

	return selectedrpagenum;
}


//OPT调度算法，返回淘汰虚拟页所在的页框号
int OPT()
{
	//最佳调度算法，淘汰最远不需要的页面（不再需要的表示为无限远）
	int selectedrpagenum = -1;
	
	const int RamPageNum = rCtl.rpagenum; //内存页框

	//int vPageInrPage[16] = { 0 }; //每个页框中的虚拟页号，废弃
	//int WaitToFindvPagenum = RamPageNum; //等待寻找的虚拟页数，\
										一次扫描后续页数可以使用，此处采用多次扫描，故废弃
	
	int vPageInterval[16] = { 0 };//每个页框中所存的虚拟页面下一次使用的间隔,大于
	int FarthestTime = 0; //使用页框中虚拟页的最远距离

	int nowcodeptr = exeorderptr;//当前指令位置
	int nowpageorderptr = exepageptr; //当前指令所在页的次序


	int nowcode = -1;// 当前指令

	//int codenext = -1;//下一条指令
	//int nowvpageptr = exeorderptr;//当前指令所在页,废弃

	int vpagetofind = -1; //当前需要寻找的虚拟页号


	printf("\n--------------------------------\n");
	printf("raddr\tvaddr\n");
	for (int i = 0; i < RamPageNum; i++)
	{
		printf("%d\t%d\n", i, rCtl.rtable[i].vpagenum);
	}
	printf("\n--------------------------------\n");



	//逐一寻找每个虚拟页的使用间隔
	for (int i = 0; i < RamPageNum; i++)
	{
		//vPageInrPage[i] = rCtl.rtable[i].vpagenum;
		vpagetofind= rCtl.rtable[i].vpagenum;

		//先比较即将执行的下一条指令是否在已经在此页中,即nowcode
		if (nowcodeptr< exelen)
		{
			
			//可能会出现缺页中断，于是直接从process 中调指令，在实际中会损失速度，但是此处只是模拟
			//nowcode = process[exeorder[nowcodeptr]];//当前指令
			
			int nowvpagenum = exeorder[exeorderptr] / pagelen; //当前指令的页号

			if (nowvpagenum == vpagetofind) 
			{   
				

				//下一条指令的页号就是我们要找的页号，则间隔为0
				vPageInterval[i] = 0;
			}	
			else
			{
				//下一条指令不在我们要找的页中，则需要借助 exepageorder向后寻找
				int interval = 1;

				//寻找多少比较难判断
				while (true)
				{
					//间隔页号
					int vpagenumInterval = exepageorder[nowpageorderptr + interval];

					if (vpagenumInterval == vpagetofind)
					{
						//找到该页号的位置
						vPageInterval[i] = interval;

						break;
					}
					else
					{
						interval++;
					}

					if (nowpageorderptr + interval >= exepagelen)
					{
						break;
					}
				}



			}
		}
		else
		{
			//没有下一条指令，则当前指令为最后一条指令，直接选择0号也替换
			selectedrpagenum = 0;
			return selectedrpagenum;

		}

	}

	//选择间隔最大的淘汰
	int farthest = 0;
	for (int i = 0; i < RamPageNum; i++)
	{
		if (vPageInterval[i] >= farthest)
		{
			farthest = vPageInterval[i];
			selectedrpagenum = i;
		}

	}


	
	return selectedrpagenum;
}

//虚拟页调入，调入虚拟页进入对应的页框
int VpageToRpage(int vpnum, int rnum)
{
	if (vpnum<0 || rnum <0 || vpnum >= pCtl.vpagenum || rnum >=rCtl.rpagenum)
	{
		//不在范围内
		return -1; //报错
	}

	//合法数据，则执行调入
	int* v = pCtl.vpageptr[vpnum];
	int* r = rCtl.rpageptr[rnum];
	for (int i = 0; i < pagelen; i++)
	{
		*(r + i) = *(v + i);
	}

	int lastvpnum = rCtl.rtable[rnum].vpagenum; // 原先在页框中的虚拟页号
	if (lastvpnum >= 0 && lastvpnum < pCtl.vpagenum)
		pCtl.ptable[lastvpnum].I = 0;//已被覆盖，中断位置0

	pCtl.ptable[vpnum].I = 1; //已调入内存，中断位置1
	pCtl.ptable[vpnum].rampagenum = rnum;
	rCtl.rtable[rnum].I = 0; //页框已被占用，可能本来是被占用的
	rCtl.rtable[rnum].vpagenum = vpnum;

	return 0;
}

//缺页中断处理,返回输入的虚拟页号对应的页框号
int do_page_fault(int pnum)
{
	int selectedrpagenum = -1;
	//頁框是否有剩余
	if (rCtl.rpagenum > rCtl.rpageUsednum)
	{
		//有剩余选择一个页框号，调入虚拟页，并返回该页框号
		
		//顺序遍历
		for (int i = 0; i <rCtl.rpagenum ; i++)
		{
			if (rCtl.rtable[i].I == 1)
			{
				selectedrpagenum = i;
				break;
			}
		}
		VpageToRpage(pnum, selectedrpagenum);//虚拟页调入该页框
		rCtl.rpageUsednum++;//页框使用数增加
	}
	else
	{
		//无剩余，通过淘汰算法选择出页框号，调入虚拟页并返回该页框号
		selectedrpagenum = selectAlg[selectAlgnum]();
		VpageToRpage(pnum, selectedrpagenum);//虚拟页调入该页框
	}
	return selectedrpagenum;
}

//通过虚拟页号找页框号
int Findrampage(int pnum)
{
	findtimes++;//访问页次数自增
	int rrpagenum = -1;
	if (pCtl.ptable[pnum].I == 0)
	{
		//虚拟页尚未调入内存
		pagefault++;//缺页错误

		printf("缺页处理,虚拟页号 %d\n",pnum);

		//缺页处理
		rrpagenum = do_page_fault(pnum);
		
		//返回调入的虚拟页所在的页框号
		//return rrpagenum;
	}
	else
	{
		//虚拟页已在内存中
		rrpagenum = pCtl.ptable[pnum].rampagenum;
		
		if (rrpagenum == -1)
		{
			printf("ERROR!\n");
		}
		
	}

	if (selectAlgnum == 1)
	{
		for (int i = 0; i < rCtl.rpagenum; i++)
		{
			rCtl.RpageUsedTimes[i]++;//未使用页++
		}

		rCtl.RpageUsedTimes[rrpagenum] = 0;//最近使用的清0
	}

	return rrpagenum; // 返回页框号
}

//虚拟地址转为物理地址,输入虚拟地址返回物理地址
int vaddrToraddr(int vaddr)
{
	int P = 0; //页号
	int W = 0; //页内偏移
	int rP = 0; //页框号

	P = vaddr / pagelen;
	if (exepageorder[exepageptr] != P)
	{
		if (exepageorder[exepageptr + 1] == P)
		{
			exepageptr++;
		}
	}
	W = vaddr % pagelen;
	rP =  Findrampage(P);


	return rP * pagelen + W;
}



//根据执行顺序exeorder执行
int exe()
{
	//执行步骤
	//当前执行指令的地址（虚拟地址）确定页号和页内偏移
	//判断是否在内存中，是则执行，否，缺页错误，根据淘汰算法，选择已有的一页淘汰，换入所需的页。
	//int vpage = (processlen - 1) / pagelen + 1; //虚拟页数，向上取整
	int codeVaddr = 0; // 指令虚拟地址
	int codeRaddr = 0; // 指令物理地址

	exeorderptr = 0;
	exepageptr = 0;

	for (int i = 0; i < exelen; i++,exeorderptr++)
	{
		//打印执行序号， 指令虚拟地址， 指令物理地址，指令值
		if (i % 15 == 0)
			printf("\n序号\tvaddr\traddr\trvalue\tvvalue\n");
		//执行序号， 指令地址， 指令操作，指令值
		//printf("%d\t %d\t %s\t %d\n", i,exeorder[i],ordertype[exetype[i]].c_str(), process[exeorder[i]]);
		codeVaddr = exeorder[i];
		codeRaddr = vaddrToraddr(codeVaddr);
		

		if (0 <= codeRaddr && codeRaddr <= ramlen)
			printf("%d\t%d\t%d\t%d\t%d\n", i, codeVaddr, codeRaddr, ram[codeRaddr],process[codeVaddr]);
		else
		{
			printf("%d\t%d\t%d\n", i, codeVaddr, codeRaddr);
			printf("地址越界\n");
		}
	}
	return 0;
}


void test01()
{
	parainput();
	init();
	findtimes = 0;//访问页次数
	pagefault = 0;//缺页次数统计
	selectAlgnum = 0;
	exe();
	double OPTfaultrate = double(pagefault) / double(findtimes);

	findtimes = 0;//访问页次数
	pagefault = 0;//缺页次数统计
	selectAlgnum = 1;
	exe();
	double LRUfaultrate = double(pagefault) / double(findtimes);

	printf("OPT缺页率%f\nLRU缺页率%f\n", OPTfaultrate, LRUfaultrate);
}

//存在严重的内存管理问题
void test02()
{
	int testnum = 5;
	double OPTfaultrate = 0;
	double LRUfaultrate = 0;
	parainput();

	for (int i = 0; i < testnum; i++)
	{
		init();
		findtimes = 0;//访问页次数
		pagefault = 0;//缺页次数统计
		selectAlgnum = 0;
		exe();
		OPTfaultrate += double(pagefault) / double(findtimes);

		findtimes = 0;//访问页次数
		pagefault = 0;//缺页次数统计
		selectAlgnum = 1;
		exe();
		LRUfaultrate += double(pagefault) / double(findtimes);
	}

	printf("OPT缺页率%f\nLRU缺页率%f\n", OPTfaultrate / testnum, LRUfaultrate / testnum);

}


int main()
{
	
	test01();

	return 0;
}
