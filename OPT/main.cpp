#include"need.h"


//���������
int randnum()
{
	return rand() % 100 + 1;
}

//���̴������
int ProcessPadding()
{
	srand(time(NULL));
	//������1-100������process ��
	for (int i = 0; i < processlen - 1; i++)
	{
		process[i] = randnum();
	}

	//��ǽ���
	process[processlen - 1] = -1;

	return 0;
}

//ִ��˳�����  exetype�����ڴ�λ
void OrderPadding()
{
	int codeip = 0; //����λ��ָ��ip��ָ����һ��ִ�е�ָ��ĵ�ַ�����ɳ���processlen
	int code = 0; //ָ��ľ���ֵ
	exelen = 1;//exelen ����Ϊ1


	//����process�е�����ȷ��ִ��˳��
	while (1)
	{
		if (codeip < processlen && code >= 0)  // codeip�� 0 <--> processlen-1
		{
			code = process[codeip]; //ȡֵ
		}
		else
			break;// ������Χ ִ�н���

		if (code == -1) //process �������
		{
			break;
		}
		else if (code == 0)//���ִ��
		{
			srand(time(NULL));
			codeip = rand() % processlen; // codeip�� 0 <--> processlen-1
			exetype[exelen - 1] = 0;
			exeorder[exelen++] = codeip;
		}
		else if (0 < code && code <= 70) // ģ��˳��ִ��
		{
			codeip++;
			if (codeip >= processlen)
				break;
			exetype[exelen - 1] = 1;
			exeorder[exelen++] = codeip;
		}
		else if (70 < code && code <= 80) // ģ���֧
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
		else if (80 < code && code <= 90) // ģ����ת
		{

			while (true)
			{
				int offset = rand()%(processlen /2) - (processlen / 4); // �����������ǰ�� 1/4 �ܾ�����Ĭ�� -4*128 <--> 4*128
				if ((codeip + offset) < processlen && (codeip + offset) >= 0) //��ת��Χ����
				{
					codeip += offset;
					break;
				}
			}
			
			
				
			exetype[exelen - 1] = 3;
			exeorder[exelen++] = codeip;
		}
		else if (90 < code && code <= 100)//ģ��ѭ��
		{
			exetype[exelen - 1] = 4;
			//srand(time(NULL));
			int looptime = (rand() % 5 + 1); //ѭ������ 1-5
			int loopcode = (rand() % 8 + 1); //ѭ���������� 1-8
			//ѭ���д��볬����Χ
			if ((codeip + loopcode) >= processlen)
				break;

			for (int i = 0; i < looptime; i++)
			{
				//printf("ѭ��\n");
				for (int j = 1; j < loopcode; j++)
				{
					codeip++;

					if (codeip >= processlen)
						break;

					exetype[exelen - 1] = 4; //ѭ������
					exeorder[exelen++] = codeip;
				}
				exetype[exelen] = 4;//��ת
				codeip -= loopcode;
			}
			//ѭ������λ��
			codeip += loopcode;
			exeorder[exelen++] = codeip;
		}
	}
}



//����ҳ����˳����䣬����ʵ��OPT�㷨
void PageOrderPadding()
{
	int codeip = 0;
	exepagelen = 0;
	for (int i = 0; i < exelen; i++)
	{
		codeip = exeorder[i];
		int codeVpage = codeip / pagelen;//��ȡ����ҳ��
		//���ҳ�����ı����¼ҳ˳��
		if (i == 0)
		{
			exepageorder[exepagelen++] = codeVpage;
		}
		if(exepagelen!=0 && codeVpage != exepageorder[exepagelen-1])
			exepageorder[exepagelen++] = codeVpage;
			
	}


}

//ҳ��Ϣ��ʼ��
int PageInit()
{
	//ҳ������
	rCtl.rpagenum = ramlen / pagelen;//ȷ��ҳ����
	rCtl.rpageUsednum = 0; //��ʹ�õ��ڴ�����
	for (int i = 0; i < rCtl.rpagenum; i++)
	{
		rCtl.rpageptr[i] = &ram[i * pagelen]; //ȷ��ÿ��ҳ���Ӧ���ڴ���ʼ��ַ
		rCtl.rtable[i].rampagenum = i;//�ڴ�������ȷ��ҳ���˳��
		rCtl.rtable[i].I = 1;//ҳ�����
	}


	//��������ҳ����
	pCtl.vpagenum = (processlen - 1) / pagelen + 1; //����ҳ��������ȡ��
	for (int i = 0; i < pCtl.vpagenum; i++)
	{
		pCtl.vpageptr[i] = &process[i * pagelen]; //ȷ��ÿ������ҳ�ڽ�������ʼ��ַ
		pCtl.ptable[i].vpagenum = i;//�ڴ�������ȷ������ҳ��˳��
		pCtl.ptable[i].I = 0;//����ҳδ����
	}
	return 0;
}



void parainput()
{
	std::string s;
	std::string v = "";
	char str[10];
	char c;
	printf("��������̳��ȣ�Ĭ��%d��", processlen);
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
	printf("\n������ҳ���ȣ�Ĭ��%d��", pagelen);
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


//��ʼ��
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
	memset(ram, 0, ramlen); //�����ڴ�

}

//LRU�����㷨
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


//OPT�����㷨��������̭����ҳ���ڵ�ҳ���
int OPT()
{
	//��ѵ����㷨����̭��Զ����Ҫ��ҳ�棨������Ҫ�ı�ʾΪ����Զ��
	int selectedrpagenum = -1;
	
	const int RamPageNum = rCtl.rpagenum; //�ڴ�ҳ��

	//int vPageInrPage[16] = { 0 }; //ÿ��ҳ���е�����ҳ�ţ�����
	//int WaitToFindvPagenum = RamPageNum; //�ȴ�Ѱ�ҵ�����ҳ����\
										һ��ɨ�����ҳ������ʹ�ã��˴����ö��ɨ�裬�ʷ���
	
	int vPageInterval[16] = { 0 };//ÿ��ҳ�������������ҳ����һ��ʹ�õļ��,����
	int FarthestTime = 0; //ʹ��ҳ��������ҳ����Զ����

	int nowcodeptr = exeorderptr;//��ǰָ��λ��
	int nowpageorderptr = exepageptr; //��ǰָ������ҳ�Ĵ���


	int nowcode = -1;// ��ǰָ��

	//int codenext = -1;//��һ��ָ��
	//int nowvpageptr = exeorderptr;//��ǰָ������ҳ,����

	int vpagetofind = -1; //��ǰ��ҪѰ�ҵ�����ҳ��


	printf("\n--------------------------------\n");
	printf("raddr\tvaddr\n");
	for (int i = 0; i < RamPageNum; i++)
	{
		printf("%d\t%d\n", i, rCtl.rtable[i].vpagenum);
	}
	printf("\n--------------------------------\n");



	//��һѰ��ÿ������ҳ��ʹ�ü��
	for (int i = 0; i < RamPageNum; i++)
	{
		//vPageInrPage[i] = rCtl.rtable[i].vpagenum;
		vpagetofind= rCtl.rtable[i].vpagenum;

		//�ȱȽϼ���ִ�е���һ��ָ���Ƿ����Ѿ��ڴ�ҳ��,��nowcode
		if (nowcodeptr< exelen)
		{
			
			//���ܻ����ȱҳ�жϣ�����ֱ�Ӵ�process �е�ָ���ʵ���л���ʧ�ٶȣ����Ǵ˴�ֻ��ģ��
			//nowcode = process[exeorder[nowcodeptr]];//��ǰָ��
			
			int nowvpagenum = exeorder[exeorderptr] / pagelen; //��ǰָ���ҳ��

			if (nowvpagenum == vpagetofind) 
			{   
				

				//��һ��ָ���ҳ�ž�������Ҫ�ҵ�ҳ�ţ�����Ϊ0
				vPageInterval[i] = 0;
			}	
			else
			{
				//��һ��ָ�������Ҫ�ҵ�ҳ�У�����Ҫ���� exepageorder���Ѱ��
				int interval = 1;

				//Ѱ�Ҷ��ٱȽ����ж�
				while (true)
				{
					//���ҳ��
					int vpagenumInterval = exepageorder[nowpageorderptr + interval];

					if (vpagenumInterval == vpagetofind)
					{
						//�ҵ���ҳ�ŵ�λ��
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
			//û����һ��ָ���ǰָ��Ϊ���һ��ָ�ֱ��ѡ��0��Ҳ�滻
			selectedrpagenum = 0;
			return selectedrpagenum;

		}

	}

	//ѡ����������̭
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

//����ҳ���룬��������ҳ�����Ӧ��ҳ��
int VpageToRpage(int vpnum, int rnum)
{
	if (vpnum<0 || rnum <0 || vpnum >= pCtl.vpagenum || rnum >=rCtl.rpagenum)
	{
		//���ڷ�Χ��
		return -1; //����
	}

	//�Ϸ����ݣ���ִ�е���
	int* v = pCtl.vpageptr[vpnum];
	int* r = rCtl.rpageptr[rnum];
	for (int i = 0; i < pagelen; i++)
	{
		*(r + i) = *(v + i);
	}

	int lastvpnum = rCtl.rtable[rnum].vpagenum; // ԭ����ҳ���е�����ҳ��
	if (lastvpnum >= 0 && lastvpnum < pCtl.vpagenum)
		pCtl.ptable[lastvpnum].I = 0;//�ѱ����ǣ��ж�λ��0

	pCtl.ptable[vpnum].I = 1; //�ѵ����ڴ棬�ж�λ��1
	pCtl.ptable[vpnum].rampagenum = rnum;
	rCtl.rtable[rnum].I = 0; //ҳ���ѱ�ռ�ã����ܱ����Ǳ�ռ�õ�
	rCtl.rtable[rnum].vpagenum = vpnum;

	return 0;
}

//ȱҳ�жϴ���,�������������ҳ�Ŷ�Ӧ��ҳ���
int do_page_fault(int pnum)
{
	int selectedrpagenum = -1;
	//퓿��Ƿ���ʣ��
	if (rCtl.rpagenum > rCtl.rpageUsednum)
	{
		//��ʣ��ѡ��һ��ҳ��ţ���������ҳ�������ظ�ҳ���
		
		//˳�����
		for (int i = 0; i <rCtl.rpagenum ; i++)
		{
			if (rCtl.rtable[i].I == 1)
			{
				selectedrpagenum = i;
				break;
			}
		}
		VpageToRpage(pnum, selectedrpagenum);//����ҳ�����ҳ��
		rCtl.rpageUsednum++;//ҳ��ʹ��������
	}
	else
	{
		//��ʣ�࣬ͨ����̭�㷨ѡ���ҳ��ţ���������ҳ�����ظ�ҳ���
		selectedrpagenum = selectAlg[selectAlgnum]();
		VpageToRpage(pnum, selectedrpagenum);//����ҳ�����ҳ��
	}
	return selectedrpagenum;
}

//ͨ������ҳ����ҳ���
int Findrampage(int pnum)
{
	findtimes++;//����ҳ��������
	int rrpagenum = -1;
	if (pCtl.ptable[pnum].I == 0)
	{
		//����ҳ��δ�����ڴ�
		pagefault++;//ȱҳ����

		printf("ȱҳ����,����ҳ�� %d\n",pnum);

		//ȱҳ����
		rrpagenum = do_page_fault(pnum);
		
		//���ص��������ҳ���ڵ�ҳ���
		//return rrpagenum;
	}
	else
	{
		//����ҳ�����ڴ���
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
			rCtl.RpageUsedTimes[i]++;//δʹ��ҳ++
		}

		rCtl.RpageUsedTimes[rrpagenum] = 0;//���ʹ�õ���0
	}

	return rrpagenum; // ����ҳ���
}

//�����ַתΪ�����ַ,���������ַ���������ַ
int vaddrToraddr(int vaddr)
{
	int P = 0; //ҳ��
	int W = 0; //ҳ��ƫ��
	int rP = 0; //ҳ���

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



//����ִ��˳��exeorderִ��
int exe()
{
	//ִ�в���
	//��ǰִ��ָ��ĵ�ַ�������ַ��ȷ��ҳ�ź�ҳ��ƫ��
	//�ж��Ƿ����ڴ��У�����ִ�У���ȱҳ���󣬸�����̭�㷨��ѡ�����е�һҳ��̭�����������ҳ��
	//int vpage = (processlen - 1) / pagelen + 1; //����ҳ��������ȡ��
	int codeVaddr = 0; // ָ�������ַ
	int codeRaddr = 0; // ָ�������ַ

	exeorderptr = 0;
	exepageptr = 0;

	for (int i = 0; i < exelen; i++,exeorderptr++)
	{
		//��ӡִ����ţ� ָ�������ַ�� ָ�������ַ��ָ��ֵ
		if (i % 15 == 0)
			printf("\n���\tvaddr\traddr\trvalue\tvvalue\n");
		//ִ����ţ� ָ���ַ�� ָ�������ָ��ֵ
		//printf("%d\t %d\t %s\t %d\n", i,exeorder[i],ordertype[exetype[i]].c_str(), process[exeorder[i]]);
		codeVaddr = exeorder[i];
		codeRaddr = vaddrToraddr(codeVaddr);
		

		if (0 <= codeRaddr && codeRaddr <= ramlen)
			printf("%d\t%d\t%d\t%d\t%d\n", i, codeVaddr, codeRaddr, ram[codeRaddr],process[codeVaddr]);
		else
		{
			printf("%d\t%d\t%d\n", i, codeVaddr, codeRaddr);
			printf("��ַԽ��\n");
		}
	}
	return 0;
}


void test01()
{
	parainput();
	init();
	findtimes = 0;//����ҳ����
	pagefault = 0;//ȱҳ����ͳ��
	selectAlgnum = 0;
	exe();
	double OPTfaultrate = double(pagefault) / double(findtimes);

	findtimes = 0;//����ҳ����
	pagefault = 0;//ȱҳ����ͳ��
	selectAlgnum = 1;
	exe();
	double LRUfaultrate = double(pagefault) / double(findtimes);

	printf("OPTȱҳ��%f\nLRUȱҳ��%f\n", OPTfaultrate, LRUfaultrate);
}

//�������ص��ڴ��������
void test02()
{
	int testnum = 5;
	double OPTfaultrate = 0;
	double LRUfaultrate = 0;
	parainput();

	for (int i = 0; i < testnum; i++)
	{
		init();
		findtimes = 0;//����ҳ����
		pagefault = 0;//ȱҳ����ͳ��
		selectAlgnum = 0;
		exe();
		OPTfaultrate += double(pagefault) / double(findtimes);

		findtimes = 0;//����ҳ����
		pagefault = 0;//ȱҳ����ͳ��
		selectAlgnum = 1;
		exe();
		LRUfaultrate += double(pagefault) / double(findtimes);
	}

	printf("OPTȱҳ��%f\nLRUȱҳ��%f\n", OPTfaultrate / testnum, LRUfaultrate / testnum);

}


int main()
{
	
	test01();

	return 0;
}