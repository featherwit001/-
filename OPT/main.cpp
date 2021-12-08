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

//��ʼ��
void init()
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
	

	PageInit();

	ProcessPadding();

#ifndef DEBUG
	OrderPadding(); 
	PageOrderPadding();
#else
	int codeipOrder[5] = { 0,128,256,512,1024 };

	for (int i = 0; i < 5; i++)
	{
		exeorder[exelen++] =  codeipOrder[i];
	}
	PageOrderPadding();
#endif // !DEBUG
	
}

//LRU�����㷨
int  LRU()
{
	int selectedrpagenum = -1;
	return selectedrpagenum;
}


//OPT�����㷨��������̭����ҳ���ڵ�ҳ���
int OPT()
{
	//��ѵ����㷨����̭��Զ����Ҫ��ҳ�棨������Ҫ�ı�ʾΪ����Զ��
	int selectedrpagenum = -1;
	
	const int RamPageNum = rCtl.rpagenum; //�ڴ�ҳ��
	int vPageInrPage[16] = { 0 }; //ÿ��ҳ���е�����ҳ��
	int WaitToFindvPagenum = RamPageNum; //�ȴ�Ѱ�ҵ�����ҳ��
	int vPageExpectOrder[16] = { 0 };//ÿ��ҳ�������������ҳ��������ֵĴ���
	int FarthestTime = 0; //ʹ��ҳ��������ҳ����Զ����

	
	

	for (int i = 0; i < RamPageNum; i++)
	{
		vPageInrPage[i] = rCtl.rtable[i].vpagenum;
	}

	int codeptr = exeorderptr;//��ǰָ��λ��
	int vpageptr = exeorderptr;//��ǰָ������ҳ
	//�ж���һ��ָ���Ƿ��ڵ�ǰҳ�У�����ǰҳ����Ϊ0����ʾ������Ҫʹ��
	


	while (true)
	{


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
	pCtl.ptable[vpnum].I = 1; //�ѵ����ڴ棬�ж�λ��1
	pCtl.ptable[vpnum].rampagenum = rnum;
	rCtl.rtable[rnum].I = 0; //ҳ���ѱ�ռ��
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
	if (pCtl.ptable[pnum].I == 0)
	{
		//����ҳ��δ�����ڴ�
		pagefault++;//ȱҳ����

		//ȱҳ����
		int rrpagenum = do_page_fault(pnum);
		
		//���ص��������ҳ���ڵ�ҳ���
		return rrpagenum;
	}
	else
	{
		//����ҳ�����ڴ���
		return pCtl.ptable[pnum].rampagenum; // ����ҳ���
	}
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

	for (int i = 0; i < exelen; i++,exeorderptr++)
	{
		//��ӡִ����ţ� ָ�������ַ�� ָ�������ַ��ָ��ֵ
		if (i % 15 == 0)
			printf("���\tvaddr\traddr\tvalue\n");
		//ִ����ţ� ָ���ַ�� ָ�������ָ��ֵ
		//printf("%d\t %d\t %s\t %d\n", i,exeorder[i],ordertype[exetype[i]].c_str(), process[exeorder[i]]);
		codeVaddr = exeorder[i];
		codeRaddr = vaddrToraddr(codeVaddr);
		

		if (0 <= codeRaddr && codeRaddr <= ramlen)
			printf("%d\t%d\t%d\t%d\n", i, codeVaddr, codeRaddr, ram[codeRaddr]); 
		else
		{
			printf("%d\t%d\t%d\n", i, codeVaddr, codeRaddr);
			printf("��ַԽ��\n");
		}
	}
	return 0;
}

int main()
{
	init();
	exe();
	return 0;
}