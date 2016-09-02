#include "common.h"


//���Ⱦɫ
int cleanRed()
{
	int i;
	for (i = 0; i < DotCount; i++)
	{
		p[DotNum[i]]->red = 0;
		p[DotNum[i]]->tempfromId = 0;
		p[DotNum[i]]->temptoId = 0;
		p[DotNum[i]]->entryWeight = 0;
		p[DotNum[i]]->exitWeight = 0;
	}
	return 1;
}

//ʹ�ö����򣬽�DotContainer[DotIndex]��DotContainer[DotIndex+count]����Ȩ����С�����������
void maxHeapify(int index)
{
	int largest = 0;
	int left = ((index << 1) + 1);
	int right = ((index << 1) + 2);
	if ((left <= heapSize) && (p[DotContainer[DotIndex+left]]->exitWeight > p[DotContainer[DotIndex+index]]->exitWeight))largest = left;
	else
	{
		largest = index;
	}
	if ((right <= heapSize) && (p[DotContainer[DotIndex + right]]->exitWeight > p[DotContainer[DotIndex + largest]]->exitWeight))largest = right;
	if (largest!=index)
	{
		int temp = DotContainer[DotIndex + index];
		DotContainer[DotIndex + index] = DotContainer[DotIndex + largest];
		DotContainer[DotIndex + largest] = temp;
		maxHeapify(largest);
	}
}

void heap_sort(int length)
{
	int i;
	length = length - 1;
	heapSize = length;
	for (i = (length >> 1); i >= 0; i--)
	{
		maxHeapify(i);
	}
	for ( i = DotIndex+length; i>=(DotIndex+1); i--)
	{
		int temp = DotContainer[DotIndex];
		DotContainer[DotIndex] = DotContainer[i];
		DotContainer[i] = temp;
		heapSize--;
		maxHeapify(0);
	}
}

//�������ʼ������������ҳ����в�����V�еĵ㣬ֱ������ʼ���ܹ��������ӵ�(����������ľ���)����ʼֵcount=1
//��Ҫע����ǣ�ÿһ�ε��ø÷�������Ҫ��ȫ�ֱ���DotIndex��0��ͬʱDotContainer[0]��ֵ��Ҫ��ֵΪ�յ�ID
int find_min_near_Vdot_path(int count)
{
	int n = DotIndex + count; //�����DotContainer�Ķ���λ��
	int flag = 0;
	//����ʹ�ô�������򣬰���Ȩ����С�����������
	heap_sort(count);
	int i, j;
	for (j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];		
		for (i = 0; i < p[dot]->count; i++)
		{
			//����Ҫ�жϸõ��Ƿ��Ѿ��߹�
			if (p[p[dot]->lines[i].DesId]->red != 3)
			{
				//����õ���V�㣬��ô���ǽ����ǣ����Ҳ�������һ�������ڵ㷶Χ��
				if (p[p[dot]->lines[i].DesId]->isV == 1)
				{
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//��¼�����ĵ�
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//����Ȩ��ֵ�ۼ�
					v[DotContainer[0]]->linkExit[v[DotContainer[0]]->exitCount].VId = p[dot]->lines[i].DesId;
					v[DotContainer[0]]->linkExit[v[DotContainer[0]]->exitCount].fromId = dot;
					v[DotContainer[0]]->linkExit[v[DotContainer[0]]->exitCount].weight = p[p[dot]->lines[i].DesId]->exitWeight;//����Ȩ��ֵ���浽�����ط�
					v[DotContainer[0]]->exitCount++; //������Ӧ����1
				}
				//����õ㲻��V�㣬��ô���ǽ��������һ�������Ľڵ㷶Χ
				else
				{
					p[p[dot]->lines[i].DesId]->tempfromId = dot;
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
					DotContainer[n] = p[dot]->lines[i].DesId;
					n++;
				}
				p[p[dot]->lines[i].DesId]->red = 3;
			}
			else
			{
				if (p[dot]->lines[i].DesId!=DotContainer[0])
				{
					if (p[p[dot]->lines[i].DesId]->isV == 1)
					{
						int h = 0;
						while (p[dot]->lines[i].DesId != v[DotContainer[0]]->linkExit[h].VId)
						{
							++h;
						}
						if (v[DotContainer[0]]->linkExit[h].weight > (p[dot]->exitWeight + p[dot]->lines[i].cost))
						{
							v[DotContainer[0]]->linkExit[h].weight = p[dot]->exitWeight + p[dot]->lines[i].cost;
							v[DotContainer[0]]->linkExit[h].fromId = dot;
						}
					}
					else
					{
						if (p[p[dot]->lines[i].DesId]->exitWeight>(p[dot]->exitWeight + p[dot]->lines[i].cost))
						{
							p[p[dot]->lines[i].DesId]->tempfromId = dot;
							p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
						}
					}
				}		
			}
		}
	}
	int newCount = n - (DotIndex + count); //�������һ�����ӵ�������
	if (newCount == 0)
	{
		return EXIT; //�Ѿ�û�����������ˣ�˵�����е������Ѿ��������
	}
	DotIndex = DotIndex + count;//����DotIndex��ֵ
	if (find_min_near_Vdot_path(newCount) == EXIT)
	{
		return EXIT;
	}
	return EXIT;
}

//�������ʼ������������ҳ����в�����P�еĵ㣬ֱ������ʼ���ܹ��������ӵ�(����������ľ���)����ʼֵcount=1
//��Ҫע����ǣ�ÿһ�ε��ø÷�������Ҫ��ȫ�ֱ���DotIndex��0��ͬʱDotContainer[0]��ֵ��Ҫ��ֵΪ�յ�ID
int find_min_near_Pdot_path(int count)
{
	int n = DotIndex + count; //�����DotContainer�Ķ���λ��
	int flag = 0;
	//����ʹ�ô�������򣬰���Ȩ����С�����������
	heap_sort(count);
	int i, j;
	for (j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (i = 0; i < p[dot]->count; i++)
		{

			//����Ҫ�жϸõ��Ƿ��Ѿ��߹�
			if (p[p[dot]->lines[i].DesId]->red != 3)
			{
				//����õ���V�㣬��ô���ǽ����ǣ����Ҳ�������һ�������ڵ㷶Χ��
				if (p[p[dot]->lines[i].DesId]->isP == 1)
				{
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//��¼�����ĵ�
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//����Ȩ��ֵ�ۼ�
					s[DotContainer[0]]->linkExit[s[DotContainer[0]]->exitCount].VId = p[dot]->lines[i].DesId;
					s[DotContainer[0]]->linkExit[s[DotContainer[0]]->exitCount].fromId = dot;
					s[DotContainer[0]]->linkExit[s[DotContainer[0]]->exitCount].weight = p[p[dot]->lines[i].DesId]->exitWeight;//����Ȩ��ֵ���浽�����ط�
					s[DotContainer[0]]->exitCount++; //������Ӧ����1
				}
				//����õ㲻��V�㣬��ô���ǽ��������һ�������Ľڵ㷶Χ
				else
				{
					p[p[dot]->lines[i].DesId]->tempfromId = dot;
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
					DotContainer[n] = p[dot]->lines[i].DesId;
					n++;
				}
				p[p[dot]->lines[i].DesId]->red = 3;

			}
			else
			{
				if (p[dot]->lines[i].DesId != DotContainer[0])
				{
					if (p[p[dot]->lines[i].DesId]->isP == 1)
					{
						int h = 0;
						while (p[dot]->lines[i].DesId != s[DotContainer[0]]->linkExit[h].VId)
						{
							++h;
						}
						if (s[DotContainer[0]]->linkExit[h].weight > (p[dot]->exitWeight + p[dot]->lines[i].cost))
						{
							s[DotContainer[0]]->linkExit[h].weight = p[dot]->exitWeight + p[dot]->lines[i].cost;
							s[DotContainer[0]]->linkExit[h].fromId = dot;
						}
					}
					else
					{
						if (p[p[dot]->lines[i].DesId]->exitWeight>(p[dot]->exitWeight + p[dot]->lines[i].cost))
						{
							p[p[dot]->lines[i].DesId]->tempfromId = dot;
							p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
						}
					}
				}
			}

		}

	}
	int newCount = n - (DotIndex + count); //�������һ�����ӵ�������
	if (newCount == 0)
	{
		return EXIT; //�Ѿ�û�����������ˣ�˵�����е������Ѿ��������
	}
	DotIndex = DotIndex + count;//����DotIndex��ֵ
	if (find_min_near_Pdot_path(newCount) == EXIT)
	{
		return EXIT;
	}
	return EXIT;
}

//������V���������������V���·����Ϣȫ������������
int get_VDot_path()
{
	int i;
	int j;
	//ð������,����Ȩ�ش�С����С��������
	for (i = 0; i < v[DotContainer[0]]->exitCount; i++)
	{
		for (j = 0; j < v[DotContainer[0]]->exitCount - i-1; j++)
		{
			if (v[DotContainer[0]]->linkExit[j].weight>v[DotContainer[0]]->linkExit[j+1].weight)
			{
				int  tempVId, tempWeight, tempFromId;
				tempVId = v[DotContainer[0]]->linkExit[j].VId;
				tempWeight = v[DotContainer[0]]->linkExit[j].weight;
				tempFromId = v[DotContainer[0]]->linkExit[j].fromId;
				v[DotContainer[0]]->linkExit[j].VId = v[DotContainer[0]]->linkExit[j + 1].VId;
				v[DotContainer[0]]->linkExit[j].weight = v[DotContainer[0]]->linkExit[j + 1].weight;
				v[DotContainer[0]]->linkExit[j].fromId = v[DotContainer[0]]->linkExit[j + 1].fromId;
				v[DotContainer[0]]->linkExit[j + 1].VId = tempVId;
				v[DotContainer[0]]->linkExit[j + 1].weight = tempWeight;
				v[DotContainer[0]]->linkExit[j + 1].fromId = tempFromId;
			}
		}
	}
	for ( i = 0; i < v[DotContainer[0]]->exitCount; i++)
	{
		int tempDot = v[DotContainer[0]]->linkExit[i].fromId;
		//v[v[DotContainer[0]]->linkExit[i].VId]->entryCount++;
		v[DotContainer[0]]->linkExit[i].line[0] = v[DotContainer[0]]->linkExit[i].fromId;
		v[DotContainer[0]]->linkExit[i].count = 1;
		j = 1;
		while (tempDot!=DotContainer[0])
		{
			tempDot = p[tempDot]->tempfromId;
			v[DotContainer[0]]->linkExit[i].line[j] = tempDot;
			j++;
		}
		v[DotContainer[0]]->linkExit[i].count = j;
	}
	return 0;
}

//������P���������������P���·����Ϣȫ������������
int get_PDot_path()
{
	int i;
	int j;
	//ð������,����Ȩ�ش�С����С��������
	for (i = 0; i < s[DotContainer[0]]->exitCount; i++)
	{
		for (j = 0; j < s[DotContainer[0]]->exitCount - i - 1; j++)
		{
			
			if (s[DotContainer[0]]->linkExit[j].weight>s[DotContainer[0]]->linkExit[j + 1].weight)
			{
				int  tempVId,tempWeight,tempFromId;
				tempVId = s[DotContainer[0]]->linkExit[j].VId;
				tempWeight = s[DotContainer[0]]->linkExit[j].weight;
				tempFromId = s[DotContainer[0]]->linkExit[j].fromId;
				s[DotContainer[0]]->linkExit[j].VId = s[DotContainer[0]]->linkExit[j + 1].VId;
				s[DotContainer[0]]->linkExit[j].weight = s[DotContainer[0]]->linkExit[j + 1].weight;
				s[DotContainer[0]]->linkExit[j].fromId = s[DotContainer[0]]->linkExit[j+1].fromId;
				s[DotContainer[0]]->linkExit[j + 1].VId = tempVId;
				s[DotContainer[0]]->linkExit[j + 1].weight = tempWeight;
				s[DotContainer[0]]->linkExit[j + 1].fromId = tempFromId;
			}
		}
	}
	for (i = 0; i < s[DotContainer[0]]->exitCount; i++)
	{
		int tempDot = s[DotContainer[0]]->linkExit[i].fromId;
		//v[v[DotContainer[0]]->linkExit[i].VId]->entryCount++;
		s[DotContainer[0]]->linkExit[i].line[0] = s[DotContainer[0]]->linkExit[i].fromId;
		s[DotContainer[0]]->linkExit[i].count = 1;
		j = 1;
		while (tempDot != DotContainer[0])
		{
			tempDot = p[tempDot]->tempfromId;
			s[DotContainer[0]]->linkExit[i].line[j] = tempDot;
			j++;
		}
		s[DotContainer[0]]->linkExit[i].count = j;
	}
	return 0;
}

//��ȡ���ڱؾ����Vͼ�����ͼ������ʼ�㡢�յ�
void get_VGraph()
{
	int i;
	for (i = 0; i < IncludeCount; i++)
	{
		DotContainer[0] = Include[i];
		DotIndex = 0;
		p[DotContainer[0]]->red = 3;
		find_min_near_Vdot_path(1);
		get_VDot_path();
		cleanRed();
	}
}

//��ȡ���ڱؾ����Pͼ�����ͼ������ʼ�㡢�յ�
void get_PGraph()
{
	int i;
	for (i = 0; i < IncludeCount_p; i++)
	{
		DotContainer[0] = Include_p[i];
		DotIndex = 0;
		p[DotContainer[0]]->red = 3;
		find_min_near_Pdot_path(1);
		get_PDot_path();
		cleanRed();
	}

}
/*
��һ������Ҫ�ǣ��ҳ����еĺ�ѡ�⣬��Щ��ѡ�ⲻһ����������ĿҪ�󣬺����ᶯ̬������Щ��ѡ��
1�����ȴ���ʼ�㿪ʼ���ҳ������С����һ��V�㣬������������
2������ʼ�������������V��������Ӧ��1��ͬʱ���������������V��ĳ���Ҳ��Ӧ��1��
3���Ժ������ڼ���V�������У�ֻҪ�ж����µ�V���Ƿ�����Ȼ����Ϊ0�ĵ㣬����У���˵�������Ѿ��޽�
4��ÿ�������ҵ�һ����ѡ�⣬���Ǿ����ϵ���������ѡ�⣬�������Ϊ���н⣬��������н��������
5��

*/


//��ȡ�����С����һ��V�㣬�����Ҫ����countFlag��־λ��
int get_min_entryCount(int Id)
{
	int tempCountNum = 0;
	int Index = NO;
	int deepCount;
	deepCount = 200 / IncludeCount;
	if (deepCount<5)
	{
		deepCount = 5;
	}
	if (v[Id]->contFlag >= deepCount)
	{
		return NO;
	}
	int i;
	for (i = 0; i < v[Id]->exitCount; i++)
	{
		if (v[v[Id]->linkExit[i].VId]->red == 0)
		{
			if (v[Id]->contFlag != tempCountNum)
			{
				tempCountNum++;
			}
			else
			{
				Index = i;
				break;
			}
		}
	}
	//����ҵ���һ��V��
	if (Index!=NO)
	{		
		v[v[Id]->linkExit[Index].VId]->red = 1;
		v[Id]->tempIndex = Index;//���������
		return v[Id]->linkExit[Index].VId;
	}
	//���û���ҵ�
	else
	{
		return NO;
	}

}

//��ȡ�����С����һ��V�㣬�����Ҫ����countFlag��־λ��
int get_min_p_entryCount(int Id)
{
	int tempCountNum = 0;
	int Index = NO;
	int deepCount;
	deepCount = 200 / IncludeCount_p;
	if (deepCount<5)
	{
		deepCount = 5;
	}
	if (s[Id]->contFlag >= deepCount)
	{
		return NO;
	}
	int i;
	for (i = 0; i < s[Id]->exitCount; i++)
	{
		if (s[s[Id]->linkExit[i].VId]->red == 0)
		{
			if (s[Id]->contFlag != tempCountNum)
			{		
				tempCountNum++;
			}
			else
			{
				Index = i;
				break;
			}
		}
	}
	//����ҵ���һ��V��
	if (Index != NO)
	{	
		s[s[Id]->linkExit[Index].VId]->red = 1;
		s[Id]->tempIndex = Index;//���������
		return s[Id]->linkExit[Index].VId;
	}
	//���û���ҵ�
	else
	{
		return NO;
	}

}

//���˽ڵ���
int retreat_num_of_Vdot(int count)
{
	int i;
	if (queueContainerIndex>count)
	{
		for (i = 0; i < count; i++)
		{
			queueContainerIndex--;
			if (queueContainer[queueContainerIndex]!=VDestinationID)
			{
				v[queueContainer[queueContainerIndex]]->red = 0;
			}		
			v[queueContainer[queueContainerIndex]]->contFlag = 0;
			queueContainer[queueContainerIndex] = 0;
		}
	}
	else
	{
		for (i = 0; i < queueContainerIndex-1; i++)
		{
			queueContainerIndex--;
			if (queueContainer[queueContainerIndex] != VDestinationID)
			{
				v[queueContainer[queueContainerIndex]]->red = 0;
			}
			v[queueContainer[queueContainerIndex]]->contFlag = 0;
			queueContainer[queueContainerIndex] = 0;
		}
	}
	return 0;	
}

//���˽ڵ���
int retreat_num_of_p_dot(int count)
{
	if (queueContainerIndex>count)
	{
		for (int i = 0; i < count; i++)
		{
			queueContainerIndex--;
			if (queueContainer[queueContainerIndex] != VDestinationID)
			{
				s[queueContainer[queueContainerIndex]]->red = 0;
			}
			s[queueContainer[queueContainerIndex]]->contFlag = 0;
			queueContainer[queueContainerIndex] = 0;
		}
	}
	else
	{
		for (int i = 0; i < queueContainerIndex-1; i++)
		{
			queueContainerIndex--;
			if (queueContainer[queueContainerIndex] != VDestinationID)
			{
				s[queueContainer[queueContainerIndex]]->red = 0;
			}
			s[queueContainer[queueContainerIndex]]->contFlag = 0;
			queueContainer[queueContainerIndex] = 0;
		}
	}
	return 0;

}

//����Ѱ�Һ�ѡ��
int find_the_candiate_queue()
{
	if (DOTLINES == NULL)
	{
		DOTLINES = (struct DotLines *)malloc(sizeof(struct DotLines));
		memset(DOTLINES, 0, sizeof(struct  DotLines));
	}
	queueContainer[0] = VSourseID;
	v[VSourseID]->red = 1;
	v[VDestinationID]->red = 1;
	queueContainerIndex++;
	int VId;
	int maxRetreat,minRetreat;
	maxRetreat = IncludeCount / 2;
	if (maxRetreat<3)
	{
		maxRetreat = 3;
	}
	minRetreat = IncludeCount / 7;
	if (minRetreat<1)
	{
		minRetreat = 1;
	}
	while (1)
	{
		if (queueContainerIndex != (IncludeCount - 1))
		{
			VId = get_min_entryCount(queueContainer[queueContainerIndex - 1]);
			if (VId == NO)
			{
				//int backStep;
				//queueContainerIndex = 50;
				//backStep = (queueContainerIndex / sqrt(IncludeCount)) + 1;
				if (queueContainerIndex<5)
				{
					retreat_num_of_Vdot(1);//����һ��V��
				}
				else
				{
					retreat_num_of_Vdot(minRetreat);//����һ��V��
				}		
				if (queueContainerIndex == 1)
				{
					return NO;
				}
				v[queueContainer[queueContainerIndex - 1]]->contFlag++;
			}
			else
			{
				queueContainer[queueContainerIndex] = VId;
				queueContainerIndex++;
			}
		}
		
		else
		{			
			//���ֻʣ����VDestination�ˣ���ô������յ��Ⱦɫ
			v[VDestinationID]->red = 0;
			VId = get_min_entryCount(queueContainer[queueContainerIndex - 1]);
			if (VId == NO)
			{
				retreat_num_of_Vdot(maxRetreat);//����2��V��
				if (queueContainerIndex == 1)
				{
					return NO;
				}
				v[queueContainer[queueContainerIndex - 1]]->contFlag++;
			}
			//˵���Ѿ��ҵ���һ����ѡ����
			else
			{
				queueContainer[queueContainerIndex] = VId;
				queueContainerIndex++;
				//�����ú�ѡ�⣬ʹ֮��Ϊ���������Ľ�
				if (get_the_correct_path(0) == SUCCESS)
				{
					//����ý�
					if (save_the_correct_path(0) == EXIT)return EXIT;
					DOTLINES->SuitIndex++;
					int rate;
					rate = DOTLINES->CandidateIndex / DOTLINES->SuitIndex;
					if (rate > 2)
					{
						rate = maxRetreat / 2;
						retreat_num_of_Vdot(rate);//����2��V��
					}
					else
					{
						retreat_num_of_Vdot(maxRetreat);//����2��V��
					}
					if (queueContainerIndex == 1)
					{
						return NO;
					}
					v[queueContainer[queueContainerIndex - 1]]->contFlag++;
					//����"p�Ƿ��Ѿ��н⣬����У��ͽ��ý���"p��Ƚϣ����Ƿ����ر�
				}
				else
				{
					DOTLINES->CandidateIndex++;
					retreat_num_of_Vdot(maxRetreat);//����4��V��
					v[queueContainer[queueContainerIndex - 1]]->contFlag++;
				}
			}
		}
	}
}


/*
������һ���֣���Ҫ������������
1����̬���ں�ѡ�⣬ʹ֮�ɺ�ѡ�⣬����н⡣
*/

//���ĺ������ú������ڵ��������ǽ�ĺ�ѡ��
int get_the_correct_path(int flag)
{
	//����Ҫ�����еĵ����Ⱦɫ����
	cleanRed();
	//h����ÿһ��V������Ķ��и���
	save_and_color_the_queue(flag);
	for (int j = 0; j < queueContainerIndex - 1; j++)
	{
		//color_the_path_where_red_is_zero(j);
		if (suit_the_candiate_queue(j, 0, 0, flag) == FAIL)return FAIL;
	}
	return SUCCESS;	
}


//��ĳһ�����еĲ�ͬV��·��Ⱦ�ϲ�ͬ��ɫ
int save_and_color_the_queue(int flag)
{
	if (flag==0)
	{
		int i;
		tempWeight = 0;
		for (i = 0; i<(queueContainerIndex - 1); i++)
		{
			int index = v[queueContainer[i]]->tempIndex;
			tempLine[i][0] = v[queueContainer[i]]->linkExit[index].weight;//save the weight of single Vdot_Line
			tempWeight += tempLine[i][0];  //save the whole line weight
			tempLine[i][1] = queueContainer[i + 1];  //be cautions this i should  plus 1
			p[tempLine[i][1]]->red = i + 1;
			int j;
			//�����countһ��Ҫ��һ����Ϊ���һ����������Լ�
			for (j = 0; j < v[queueContainer[i]]->linkExit[index].count - 1; j++)
			{
				tempLine[i][2 + j] = v[queueContainer[i]]->linkExit[index].line[j];
				p[tempLine[i][2 + j]]->red = i + 1;
			}
			tempLine[i][2 + j] = NO;
		}
		tempLineIndex = i;
	}
	else
	{
		int i;
		tempWeight = 0;
		for (i = 0; i<(queueContainerIndex - 1); i++)
		{
			int index = s[queueContainer[i]]->tempIndex;
			tempLine[i][0] = s[queueContainer[i]]->linkExit[index].weight;//save the weight of single Vdot_Line
			tempWeight += tempLine[i][0];  //save the whole line weight
			tempLine[i][1] = queueContainer[i + 1];  //be cautions this i should  plus 1
			p[tempLine[i][1]]->red = i + 1;
			int j;
			//�����countһ��Ҫ��һ����Ϊ���һ����������Լ�
			for (j = 0; j < s[queueContainer[i]]->linkExit[index].count - 1; j++)
			{
				tempLine[i][2 + j] = s[queueContainer[i]]->linkExit[index].line[j];
				p[tempLine[i][2 + j]]->red = i + 1;
			}
			tempLine[i][2 + j] = NO;
		}
		tempLineIndex = i;
	}
	return 0;
}


//�ú������ڽ�֮ǰ��·��Ҫ�ĵ㣬���Ǻ������·ռ�õĵ�����Ⱦ�Ϻ���������·����ɫ
int color_the_path_where_red_is_zero(int Index)
{
	int j = 1;
	while (tempLine[Index][j]!=NO)
	{
		if (p[tempLine[Index][j]]->red==0)
		{
			p[tempLine[Index][j]]->red = Index + 1;
		}
		j++;
	}
	return 0;
}

//�ú�������ȫͼ����
int find_path_from_all_dot(int count,int index)
{
	int flag = 0; //������¼�Լ��ķ���ֵ
	int flag2;
	int n = DotIndex + count; //�����DotContainer�Ķ���λ��
	for (int j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (int i = 0; i < p[dot]->count; i++)
		{
			
			if (p[p[dot]->lines[i].DesId]->red == 0 || p[p[dot]->lines[i].DesId]->red == RIGHT)
			{
				//����õ���V�㣬��ô���ǽ����ǣ����Ҳ�������һ�������ڵ㷶Χ��
				if (p[p[dot]->lines[i].DesId]->red == RIGHT)
				{
					//ֻҪ��CountIndex��CountNum���ʱ���������һ��
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//��¼�����ĵ�
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//����Ȩ��ֵ�ۼ�
					resultDotId = p[dot]->lines[i].DesId;
					return SUCCESS;

				}
				//����õ㲻��V�㣬��ô���ǽ��������һ�������Ľڵ㷶Χ
				else
				{
					//��ֹ�����Լ���������
					if (index==0)
					{
						flag2 = p[p[dot]->lines[i].DesId]->isV;
					}
					else
					{
						flag2 = p[p[dot]->lines[i].DesId]->isP;
					}
					if (flag2 == 0)
					{
						p[p[dot]->lines[i].DesId]->tempfromId = dot;
						p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
						DotContainer[n] = p[dot]->lines[i].DesId;
						p[p[dot]->lines[i].DesId]->red = OTHER;
						n++;						
					}
				}
			}
		}
	}
	int newCount = n - (DotIndex + count); //�������һ�����ӵ�������

	if (newCount == 0)
	{
		return NO; //�Ѿ�û�����������ˣ�˵�����е������Ѿ��������
	}
	DotIndex = DotIndex + count;//����DotIndex��ֵ
	flag = find_path_from_all_dot(newCount,index);
	if (flag == SUCCESS)
	{
		return SUCCESS;
	}
	//���û�����ţ��򷵻�No
	else 
	{
		return NO;
	}
}

//��ĳһ��V���·��Ⱦɫ
int color_the_one_Vdot(int Index,int flag,int Index2)
{
	int s = 1;
	while (tempLine[Index][s] != NO)
	{
		if (flag==0)
		{
			p[tempLine[Index][s]]->red = Index + 1;
		}
		else if (flag==1)
		{
			if (p[tempLine[Index][s]]->red == OTHER || p[tempLine[Index][s]]->red == Index + 1)
			{
				p[tempLine[Index][s]]->red = Index + 1;
			}
		}
		else
		{
			if (p[tempLine[Index][s]]->red == OTHER || p[tempLine[Index][s]]->red == Index + 1 || p[tempLine[Index][s]]->red==Index2+1)
			{
				p[tempLine[Index][s]]->red = Index + 1;
			}
		}
		s++;
	}
	return 0;
}

//�ú��������ڱ����µ���·֮ǰ��������е�Ⱦɫ��������û�б�������·ռ�еĵ�
int delet_color_queue(int Index)
{
	int s = 1;
	while (tempLine[Index][s] != NO)
	{
		if (p[tempLine[Index][s]]->red == (Index + 1))
		{
			p[tempLine[Index][s]]->red = 0;

		}
		s++;
	}
	return 0;
}

//�������RedΪ��OTHER���ı�־λ
int delete_Red_Is_other()
{
	for (int i = 0; i < DotCount; i++)
	{
		if (p[DotNum[i]]->red == OTHER)
		{
			p[DotNum[i]]->red = 0;
		}
		p[DotNum[i]]->exitWeight = 0;
		p[DotNum[i]]->tempfromId = 0;
	}
	return 0;
}

//���Ѿ����λcountFlag����·Ⱦɫ����ʾ����·�Ѿ�����ͼ��������һ���ˣ�����·�Ѿ����ܸı�
int color_the_back_path(int Index)
{
	int s;
	for (int i = 0; i < Index; i++)
	{
		s = 1;
		while (tempLine[i][s] != NO)
		{
			p[tempLine[i][s]]->red = Index + 1;
			s++;
		}		
	}
	return 0;
}

//Ⱦɫ���ݺ������ú���������ȫͼ����֮ǰ��Ӧ�ý����е�Ⱦɫ��������������Ϊ��ȫͼ������ʱ�򣬻Ὣ���еĵ������ɫ
int save_the_red(int flag)
{
	if (flag==1)
	{
		for (int i = 0; i < DotCount; i++)
		{
			backRed[DotNum[i]] = p[DotNum[i]]->red;
		}
	}
	else
	{
		for (int i = 0; i < DotCount; i++)
		{
			p[DotNum[i]]->red = backRed[DotNum[i]];
		}
	}
	return 1;
}

//�����㷨�����ڶ�̬����һ�������ǽ�ĺ�ѡ�⣬�������һ���ݹ���õĹ��̣�һ�����������������е�����������ʾ�Ƿ��ǵݹ����
int suit_the_candiate_queue(int Index,int indexFlag,int Index2,int flag2)
{
	int i = 1;
	int flag = 0;
	while (tempLine[Index][i] != NO)
	{
		if (p[tempLine[Index][i]]->red == OTHER || p[tempLine[Index][i]]->red == 0)
		{
			p[tempLine[Index][i]]->red = Index + 1;
		}
		if (p[tempLine[Index][i]]->red != (Index + 1))
		{
			int index = p[tempLine[Index][i]]->red - 1;
			//��ʾ����������
			DotIndex = 0;
			p[VSourseID]->red = 1;
			//p[VDestinationID]->red = 1;
			if (Index == 0)
			{
				DotContainer[0] = VSourseID;
				p[tempLine[Index][1]]->red = RIGHT;
			}
			else
			{
				DotContainer[0] = tempLine[Index-1][1];
				p[tempLine[Index][1]]->red = RIGHT;
			}
			delete_Red_Is_other();
			flag = find_the_useful_path_from_v_to_v(1, Index, flag2);
			p[tempLine[Index][1]]->red = Index + 1;
			if (flag == NO)
			{
				if (indexFlag == 0)
				{
					//���ȵ�һ�����Ǹ��Լ��ĵ�Ⱦ����ɫ
					p[tempLine[Index][i]]->red = Index + 1;
					color_the_one_Vdot(Index,1,0);
					//�ݹ���ù��õ��Ǹ���
					flag = suit_the_candiate_queue(index, 1,Index,flag2);
					if (flag == NO)
					{
						if (p[tempLine[Index][1]]->countFlag == 0)
						{
							
							//��ʼ������Χ�ĵ�����Ѳ�
							DotIndex = 0;
							//������Ҫ����Ⱦɫ����
							save_the_red(1);
							//���Ⱦɫ
							cleanRed();
							//����ʼ����յ�Ⱦɫ��
							//p[VDestinationID]->red = 1;
							p[VSourseID]->red = 1;
							DotContainer[0] = tempLine[Index - 1][1];
							p[tempLine[Index][1]]->red = RIGHT;
							//���Ѿ����й�ȫͼ�Ѳ�����·����Ⱦɫ����ֹ�䱻�������·ռ��
							color_the_back_path(Index);
							//����ȫͼ�Ѳ�
							flag = find_path_from_all_dot(1,flag2);
							//��������֮��Ӧ�ûָ���֮ǰ����ɫ

							if (flag != SUCCESS)
							{
								return FAIL;
							}
							save_the_red(0);
							//�ж��Ƿ������ɹ���������ɹ����ͷ�������·�ˣ�ֱ���˳���						
						}
						else
						{
							return FAIL;
						}

					}
					else
					{
						i++;
						continue;
					}
				}
				else
				{
					//����ݹ����û��������������Ҫ�Ѹ���·Ⱦ���Լ�����ɫ
					color_the_one_Vdot(Index,2,Index2);
					return NO;
				}
			}
			//����������������±������·��·��֮ǰ��������·��Ⱦɫ�������Ȼ���ʱ��󲿷ֵĵ㶼����ǳ�other�ˣ�����Ϊ�˷�ֹ��һ������Ҫ���һ��
			delet_color_queue(Index);
			//����Ȩ����Ϣ
			tempWeight -= tempLine[Index][0];
			tempLine[Index][0] = p[tempLine[Index][1]]->exitWeight;
			tempWeight += tempLine[Index][0];
			int evdot = p[tempLine[Index][1]]->tempfromId;
			int h = 2;
			while (evdot != DotContainer[0])
			{				
				tempLine[Index][h] = evdot;
				evdot = p[evdot]->tempfromId;
				h++;
			}
			tempLine[Index][h] = NO;
			//��·��������֮�����½�����·����ɫȾ���Լ�����ɫ
			color_the_one_Vdot(Index,0,0);
			return SUCCESS;
		}
		i++;
	}
	return FINISH;
}


//���ҳ�����V��֮�䣬�ڲ���������·���ĵ��ͬʱ���ҳ�������·
int find_the_useful_path_from_v_to_v(int count, int q,int index)
{
	int flag = 0; //������¼�Լ��ķ���ֵ
	int n = DotIndex + count; //�����DotContainer�Ķ���λ��
	int Index;
	for (int j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (int i = 0; i < p[dot]->count; i++)
		{
			//����Ҫ�жϸõ��Ƿ��Ѿ��߹�,���ߵĵ㣬ֻ�����Լ��ĵ㣬�����Ǳ����ΪRIGHT��û���߹��ĵ�
			if (p[p[dot]->lines[i].DesId]->red == 0 || p[p[dot]->lines[i].DesId]->red == RIGHT || p[p[dot]->lines[i].DesId]->red == (q + 1))
			{
				//����õ���V�㣬��ô���ǽ����ǣ����Ҳ�������һ�������ڵ㷶Χ��
				if (p[p[dot]->lines[i].DesId]->red == RIGHT)
				{
					//ֻҪ��CountIndex��CountNum���ʱ���������һ��
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//��¼�����ĵ�
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//����Ȩ��ֵ�ۼ�
					resultDotId = p[dot]->lines[i].DesId;
					return SUCCESS;

				}
				//����õ㲻��V�㣬��ô���ǽ��������һ�������Ľڵ㷶Χ
				else
				{
					//��ֹ�����Լ���������
					if (index==0)
					{
						Index = p[p[dot]->lines[i].DesId]->isV;
					}
					else
					{
						Index = p[p[dot]->lines[i].DesId]->isP;
					}
					if (Index == 0)
					{
						p[p[dot]->lines[i].DesId]->tempfromId = dot;
						p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
						DotContainer[n] = p[dot]->lines[i].DesId;
						n++;
						p[p[dot]->lines[i].DesId]->red = OTHER;
					}
				}
			}
		}
	}
	int newCount = n - (DotIndex + count); //�������һ�����ӵ�������

	if (newCount == 0)
	{
		return NO; //�Ѿ�û�����������ˣ�˵�����е������Ѿ��������
	}
	DotIndex = DotIndex + count;//����DotIndex��ֵ
	flag = find_the_useful_path_from_v_to_v(newCount, q,index);
	if (flag == SUCCESS)
	{
		return SUCCESS;
	}
	//���û�����ţ��򷵻�No
	else 
	{
		return NO;
	}
}

/*
������һ���������ڱ����Ѿ����������ģ���������ȷ�Ľ�
*/
int save_the_correct_path(int flag)
{
	if (DOTLINES==NULL)
	{
		DOTLINES = (struct DotLines *)malloc(sizeof(struct DotLines));
		memset(DOTLINES, 0, sizeof(struct  DotLines));
	}	
	//��ʾ�Ǵ�P��
	if (flag==0)
	{
		if (DOTLINES->Sindexflag<3000)
		{
			DOTLINES->SVlines[DOTLINES->Sindexflag].weight = tempWeight;
			PVSORT->V_sort[PVSORT->Vindex].index = DOTLINES->Sindexflag;
			PVSORT->V_sort[PVSORT->Vindex].weight = tempWeight;
			PVSORT->Vindex++;
			for (int i = 0; i < tempLineIndex; i++)
			{
				DOTLINES->SVlines[DOTLINES->Sindexflag].tlines[i][0] = tempLine[i][0];			
				int s = 1;
				int prev, next;
				next = tempLine[i][s];
				while (tempLine[i][s] != NO)
				{
					prev = tempLine[i][s];
					if (next!=prev)
					{
						for (int j = 0; j < p[prev]->count; j++)
						{
							if (p[prev]->lines[j].DesId==next)
							{
								p[prev]->lines[j].flag[p[prev]->lines[j].flagIndex] = DOTLINES->Sindexflag;
								p[prev]->lines[j].flagIndex++;
								break;
							}
						}
					}
					next = prev;
					DOTLINES->SVlines[DOTLINES->Sindexflag].tlines[i][s] = tempLine[i][s];
					s++;
				}
				DOTLINES->SVlines[DOTLINES->Sindexflag].tlines[i][s] = tempLine[i][s];
				if (i == 0)
				{
					prev = VSourseID;
				}
				else
				{
					prev = tempLine[i - 1][1];
				}
				for (int j = 0; j < p[prev]->count; j++)
				{
					if (p[prev]->lines[j].DesId == next)
					{
						p[prev]->lines[j].flag[p[prev]->lines[j].flagIndex] = DOTLINES->Sindexflag;
						p[prev]->lines[j].flagIndex++;
						break;
					}
				}
				DOTLINES->SVlines[DOTLINES->Sindexflag].index++;
			}
			DOTLINES->Sindexflag++;
		}
		else
		{
			return EXIT;
		}
	}
	else
	{
		if (DOTLINES->Cindexflag<1500)
		{
			DOTLINES->CVlines[DOTLINES->Cindexflag].weight = tempWeight;
			PVSORT->P_sort[PVSORT->Pindex].index = DOTLINES->Cindexflag;
			PVSORT->P_sort[PVSORT->Pindex].weight = tempWeight;
			PVSORT->Pindex++;
			for (int i = 0; i < tempLineIndex; i++)
			{
				DOTLINES->CVlines[DOTLINES->Cindexflag].tlines[i][0] = tempLine[i][0];
				int s = 1;
				while (tempLine[i][s] != NO)
				{
					DOTLINES->CVlines[DOTLINES->Cindexflag].tlines[i][s] = tempLine[i][s];
					s++;
				}
				DOTLINES->CVlines[DOTLINES->Cindexflag].tlines[i][s] = tempLine[i][s];
				DOTLINES->CVlines[DOTLINES->Cindexflag].index++;
			}
			DOTLINES->Cindexflag++;
		}
		else
		{
			//���ȶ���1000�����������Ȼ��			
			return EXIT;
		}
	}
	return FINISH;
}


//heap_sort
//ʹ�ö����򣬽�V·���Ľⰴ��Ȩ����С�����������
void PV_maxHeapify(int index, struct path_sort * sort)
{
	int largest = 0;
	int left = ((index << 1) + 1);
	int right = ((index << 1) + 2);
	if ((left <= heapSize) && (sort[left].weight > sort[index].weight))largest = left;
	else
	{
		largest = index;
	}
	if ((right <= heapSize) && (sort[right].weight > sort[largest].weight))largest = right;
	if (largest != index)
	{
		struct path_sort temp = sort[index];
		sort[index] = sort[largest];
		sort[largest] = temp;
		PV_maxHeapify(largest,sort);
	}
}

int PVheap_sort(int length,struct path_sort * sort)
{
	int i;
	length = length - 1;
	heapSize = length;
	for (i = (length >> 1); i >= 0; i--)
	{
		PV_maxHeapify(i, sort);
	}
	for (i = length; i >= 1; i--)
	{
		struct path_sort temp = sort[0];
		sort[0] = sort[i];
		sort[i] = temp;
		heapSize--;
		PV_maxHeapify(0, sort);
	}
	return 0;
}
/*
������һ����Ҫ��Ѱ��P�⣬ͬʱ��P���������Ѿ��ҳ�����1000����һһ���бȶԣ����Ƿ��ܹ��ҳ���С���غϱߵ���
1�����ȸ���1000��V����б�ǣ���һ��������ǰ������
2��ÿѰ��һ��P�⣬�������P�����������1000��V�����ƥ��
*/

//����Ѱ��P��ѡ��
int find_the_candiate_P_queue()
{
	DOTLINES->CandidateIndex = 0;
	DOTLINES->SuitIndex = 0;
	memset(queueContainer,0,sizeof(queueContainer));
	queueContainerIndex = 0;
	queueContainer[0] = VSourseID;
	s[VSourseID]->red = 1;
	s[VDestinationID]->red = 1;
	queueContainerIndex++;
	tempWeight = 0;
	int VId;
	int maxRetreat, minRetreat;
	maxRetreat = IncludeCount_p / 2;
	if (maxRetreat<3)
	{
		maxRetreat = 3;
	}
	minRetreat = IncludeCount_p / 7;
	if (minRetreat<1)
	{
		minRetreat = 1;
	}
	while (1)
	{
		if (queueContainerIndex != (IncludeCount_p - 1))
		{
			VId = get_min_p_entryCount(queueContainer[queueContainerIndex - 1]);
			if (VId == NO)
			{
				if (queueContainerIndex<5)
				{
					retreat_num_of_p_dot(1);//����һ��V��
				}
				else
				{
					retreat_num_of_p_dot(minRetreat);//����һ��V��
				}
				//retreat_num_of_p_dot(minRetreat);//����һ��V��

				if (queueContainerIndex == 1)
				{
					return NO;
				}
				s[queueContainer[queueContainerIndex - 1]]->contFlag++;
			}
			else
			{
				queueContainer[queueContainerIndex] = VId;
				queueContainerIndex++;
			}
		}		
		else
		{
			//���ֻʣ����VDestination�ˣ���ô������յ��Ⱦɫ
			s[VDestinationID]->red = 0;
			VId = get_min_p_entryCount(queueContainer[queueContainerIndex - 1]);
			if (VId == NO)
			{
				retreat_num_of_p_dot(maxRetreat);//����2��V��
				if (queueContainerIndex == 1)
				{
					return NO;
				}
				s[queueContainer[queueContainerIndex - 1]]->contFlag++;
			}
			//˵���Ѿ��ҵ���һ����ѡ����
			else
			{
				queueContainer[queueContainerIndex] = VId;
				queueContainerIndex++;
				//�����ú�ѡ�⣬ʹ֮��Ϊ���������Ľ�
				if (get_the_correct_path(1) == SUCCESS)
				{
					//����ý�					
					if (save_the_correct_path(1) == EXIT)return EXIT;
					DOTLINES->SuitIndex++;
					int rate;
					rate = DOTLINES->CandidateIndex / DOTLINES->SuitIndex;
					if (rate > 2)
					{
						rate = maxRetreat / 2;
						retreat_num_of_p_dot(rate);//����2��V��
					}
					else
					{
						retreat_num_of_p_dot(maxRetreat);//����2��V��
					}
					if (queueContainerIndex == 1)
					{
						return NO;
					}
					s[queueContainer[queueContainerIndex - 1]]->contFlag++;
					//����"p�Ƿ��Ѿ��н⣬����У��ͽ��ý���"p��Ƚϣ����Ƿ����ر�
				}
				else
				{
					DOTLINES->CandidateIndex++;
					retreat_num_of_p_dot(maxRetreat);//����4��V��
					if (queueContainerIndex == 1)
					{
						return NO;
					}
					s[queueContainer[queueContainerIndex - 1]]->contFlag++;
				}
			}
		}
	}
	return 0;
}


//�ú����������ҵ�һ��P��֮����ǰ���1000��V����в��ر߱Ƚ�
int compare_with_v_queues()
{
	if (SORT==NULL)
	{
		SORT = (struct Sort *)malloc(sizeof(struct Sort));
		memset(SORT, 0, sizeof(struct Sort));
	}	
	if (RESULT == NULL)
	{
		RESULT = (struct PVPath *)malloc(sizeof(struct PVPath));
		memset(RESULT, 0, sizeof(struct PVPath));
		RESULT->repeatCount = NO;
	}
	int i,j;
	int count1,count2;
	count1 = IncludeCount / 8;
	count2 = IncludeCount_p / 8;
	if (PVSORT->Pindex>100)
	{
		if (count1>count2)
		{
			PVSORT->Pindex = PVSORT->Pindex / count1;
		}
		else
		{
			if (count2 != 0)
			{
				PVSORT->Pindex = PVSORT->Pindex / count2;
			}
			else
			{
				PVSORT->Pindex = PVSORT->Pindex;
			}
		}
	}
	
	for (i = 0; i < PVSORT->Pindex; i++)
	{		
		int index = PVSORT->P_sort[i].index;
		if (RESULT->repeatCount!=0)
		{
			for (int h = 0; h < DOTLINES->Sindexflag; h++)
			{
				SORT->Result[h].index = h;
				SORT->Result[h].weight = DOTLINES->SVlines[h].weight;
			}
			get_the_result_of_repeat(index);
			threshold = (DOTLINES->Sindexflag / IncludeCount) * 4;
			sort_the_repeat_result();
			//��ȡ��С���غϱ���
			for (j = 0; j < threshold; j++)
			{			
				get_the_min_repeat(j, index,0);
			}
			//�ͷ�SORT����Դ
			memset(SORT, 0, sizeof(struct Sort));
		}
		else
		{
			int weight;
			int count = 0;
			weight = RESULT->wholeWeight - PVSORT->P_sort[i].weight;
			for (j = 0; j < PVSORT->Vindex; j++)
			{
				if (PVSORT->V_sort[j].weight<weight)
				{
					count++;
					if (count>=10)
					{
						break;
					}
					get_the_min_repeat(PVSORT->V_sort[j].index,index,1); //����V���������P�������					
				}
				else
				{
					break;
				}				
			}
			memset(SORT, 0, sizeof(struct Sort));			
		}		
	}
	return 0;
}


/*
ͬʱ��1000��V������غϱ߱Ƚ�
@param
index ��ʾV���������е�������
*/
int get_the_result_of_repeat(int index)
{
	int i, j;
	for (i = 0; i < DOTLINES->CVlines[index].index; i++)
	{
		int s = 1;
		int prev, next;
		next = DOTLINES->CVlines[index].tlines[i][s];
		while (DOTLINES->CVlines[index].tlines[i][s] != NO)
		{
			prev = DOTLINES->CVlines[index].tlines[i][s];
			if (next != prev)
			{
				for (j = 0; j < p[prev]->count; j++)
				{
					if (p[prev]->lines[j].DesId == next)
					{
						for (int h = 0; h < p[prev]->lines[j].flagIndex; h++)
						{
							SORT->Result[p[prev]->lines[j].flag[h]].count++;
						}
					}
				}
			}
			next = prev;
			s++;
		}
		if (i == 0)
		{
			prev = VSourseID;
		}
		else
		{
			prev = DOTLINES->CVlines[index].tlines[i - 1][1];
		}
		for (j = 0; j < p[prev]->count; j++)
		{
			if (p[prev]->lines[j].DesId == next)
			{
				for (int h = 0; h < p[prev]->lines[j].flagIndex; h++)
				{
					SORT->Result[p[prev]->lines[j].flag[h]].count++;
				}
			}
		}
	}
	return 0;
}







/*
 ��������غϱ�����������
 @param
 threshold ��ʾѡȡ�غϱ�Ϊǰthreshold�Ľ���е���
*/
int sort_the_repeat_result()
{
	//����Ƚ��٣�����ʵ�ʽ�ĸ���
	if (threshold == 0)
	{
		threshold = DOTLINES->Sindexflag;
	}
	int i,j;
	for (i = 0; i < threshold; i++)
	{
		for (j = DOTLINES->Sindexflag - 1; j > i; j--)
		{
			if (SORT->Result[j].count<SORT->Result[j - 1].count)
			{
				struct sort_result temp;
				temp = SORT->Result[j];
				SORT->Result[j] = SORT->Result[j - 1];
				SORT->Result[j - 1] = temp;
			}
			else if (SORT->Result[j].count == SORT->Result[j - 1].count)
			{
				if (SORT->Result[j].weight<SORT->Result[j - 1].weight)
				{
					struct sort_result temp;
					temp = SORT->Result[j];
					SORT->Result[j] = SORT->Result[j - 1];
					SORT->Result[j - 1] = temp;
				}
			}
			else
			{
				continue;
			}
		}

	}
	return 0;
}



/*
���غϱ���С�Ľ⿪ʼ����
*/
int get_the_min_repeat(int Index,int Index2,int Index3)
{
	
	int flag = 0;
	int weight;
	int red;
	
	if (Index3 == 0)
	{
		if (RESULT->repeatCount == 0)
		{
			weight = DOTLINES->CVlines[Index2].weight + DOTLINES->SVlines[SORT->Result[Index].index].weight;
			if ((weight-100) >= RESULT->wholeWeight)
			{
				return FAIL;
			}
		}
		cleanPVRed();
		red = color_the_v_path(SORT->Result[Index].index,0);
		color_the_p_path(Index2, red, Index,0);
	}
	else
	{
		cleanPVRed();
		red = color_the_v_path(Index,1);
		color_the_p_path(Index2, red, Index,1);
	}
	
	//���wholeWeight������û�д�����⣬�����Ƚ���һ������ȥ
	if (RESULT->wholeWeight == 0)
	{
		Save_the_two_path_result(SORT->Result[Index].count);
	}

	//����غϱ߲�Ϊ0�����𲽵���
	if (SORT->Result[Index].count != 0)
	{
		SORT->Result[Index].count = 0;
		for (int i = 0; i < RESULT->tempVPath.index; i++)
		{
			suit_the_two_p_path(i, Index);
		}
	}
	//�ж������֮ǰ�Ľ�Ҫ�ã����������
	if (SORT->Result[Index].count < RESULT->repeatCount)
	{
		Save_the_two_path_result(SORT->Result[Index].count);
	}
	else
	{
		weight = RESULT->tempVPath.weight + RESULT->tempPath.weight;
		if (SORT->Result[Index].count == RESULT->repeatCount && weight < RESULT->wholeWeight)
		{
			Save_the_two_path_result(SORT->Result[Index].count);
		}
	}
	return SUCCESS;

}


/*
���������·���Ľ�
@param
repeat ��ʾ�ý���ر߸���
*/
int Save_the_two_path_result( int repeat)
{
	int i;
	for (i = 0; i < RESULT->tempPath.index; i++)
	{
		int s = 1;
		RESULT->V_path.tlines[i][0] = RESULT->tempPath.tlines[i][0];
		while (RESULT->tempPath.tlines[i][s] != NO)
		{
			RESULT->V_path.tlines[i][s] = RESULT->tempPath.tlines[i][s];
			//printf("%d,", RESULT->tempPath.tlines[i][s]);
			s++;
		}
		RESULT->V_path.tlines[i][s] = RESULT->tempPath.tlines[i][s];
		//printf("\n");
	}
	RESULT->V_path.index = RESULT->tempPath.index;
	RESULT->V_path.weight = RESULT->tempPath.weight;
	//printf("P�⣺");
	//printf("\n");
	for (i = 0; i < RESULT->tempVPath.index; i++)
	{
		int s = 1;
		RESULT->P_path.tlines[i][0] = RESULT->tempVPath.tlines[i][0];
		while (RESULT->tempVPath.tlines[i][s] != NO)
		{
			RESULT->P_path.tlines[i][s] = RESULT->tempVPath.tlines[i][s];
			//printf("%d,", RESULT->tempVPath.tlines[i][s]);
			s++;
		}
		//printf("\n");
		RESULT->P_path.tlines[i][s] = RESULT->tempVPath.tlines[i][s];
	}
	RESULT->P_path.weight = RESULT->tempVPath.weight;
	RESULT->P_path.index = RESULT->tempVPath.index;

	RESULT->wholeWeight = RESULT->P_path.weight + RESULT->V_path.weight;
	RESULT->repeatCount = repeat;
	if (repeat==0)
	{
		redCount = NO;
	}
	return 0;
}



/*
��P·��Ϊ��ʯ���е���
*/
int suit_the_two_p_path(int Index,int Index2)
{
	int i = 1,j;
	int flag = 0;
	int flag2 = 0;
	int flag3;
	int next,prev;
	next = RESULT->tempVPath.tlines[Index][1];
	while (RESULT->tempVPath.tlines[Index][i] != NO)
	{
		prev = RESULT->tempVPath.tlines[Index][i];
		if (next!=prev)
		{
			for ( j = 0; j < p[prev]->count; j++)
			{
				if (p[prev]->lines[j].DesId==next)
				{
					if (p[prev]->lines[j].redV != 0)
					{
						//����������·�������һ�������ж�����һ��V�����ɫ
						flag3 = p[prev]->lines[j].redV - 1;
						flag ++;						
					}
					break;
				}
			}
		}
		next = prev;
		if (RESULT->tempVPath.tlines[Index][i+1] == NO)
		{
			if (Index==0)
			{
				prev = VSourseID;

			}
			else
			{
				prev = RESULT->tempVPath.tlines[Index-1][1];
			}
			for (j = 0; j < p[prev]->count; j++)
			{
				if (p[prev]->lines[j].DesId == next)
				{
					if (p[prev]->lines[j].redV != 0)
					{
						flag ++;
						//����������·�������һ�������ж�����һ��V�����ɫ
						flag3 = p[prev]->lines[j].redV - 1;						
					}
					break;

				}
			}

		}		
		if (flag != 0)
		{
			
			//��ʾ����������
			DotIndex = 0;
			p[VSourseID]->redP = 1;
			//p[VDestinationID]->red = 1;
			if (Index == 0)
			{
				DotContainer[0] = VSourseID;
				p[RESULT->tempVPath.tlines[Index][1]]->redP = RIGHT;
			}
			else
			{
				DotContainer[0] = RESULT->tempVPath.tlines[Index-1][1];
				p[RESULT->tempVPath.tlines[Index][1]]->redP = RIGHT;
			}
			delete_Red_PV_Is_other(0);
			//������Ҫע����ǣ�ѡȡ��
			flag2 = find_the_min_repeat_path_from_v_to_v(1, Index, 0);
			p[RESULT->tempVPath.tlines[Index][1]]->redP = Index + 1;
			if (flag2 == NO)
			{
				//���ȵ�һ�����Ǹ��Լ��ĵ�Ⱦ����ɫ
				p[RESULT->tempVPath.tlines[Index][i]]->redP = Index + 1;
				color_the_one_path_red(0, Index);
				//�ݹ���ù��õ��Ǹ���
				flag2 = suit_the_two_v_path(flag3);
				if (flag2 == NO)
				{
					//�����������¼�ظ��ߵĸ���
					
					SORT->Result[Index2].count += flag;
					i++;
					flag = 0;
					continue;

				}
				else
				{
					i++;
					continue;
				}
			}
			//����������������±������·��·��֮ǰ��������·��Ⱦɫ�������Ȼ���ʱ��󲿷ֵĵ㶼����ǳ�other�ˣ�����Ϊ�˷�ֹ��һ������Ҫ���һ��
			delete_one_path_PV_red(0, Index);
			//����Ȩ����Ϣ
			RESULT->tempVPath.weight -= RESULT->tempVPath.tlines[Index][0];
			RESULT->tempVPath.tlines[Index][0] = p[RESULT->tempVPath.tlines[Index][1]]->exitWeight;
			RESULT->tempVPath.weight += RESULT->tempVPath.tlines[Index][0];
			int evdot = p[RESULT->tempVPath.tlines[Index][1]]->tempfromId;
			int h = 2;
			while (evdot != DotContainer[0])
			{
				RESULT->tempVPath.tlines[Index][h] = evdot;
				evdot = p[evdot]->tempfromId;
				h++;
			}
			RESULT->tempVPath.tlines[Index][h] = NO;
			//��·��������֮�����½�����·����ɫȾ���Լ�����ɫ
			color_the_one_path_red(0, Index);
			return SUCCESS;
			
		}
		flag = 0;		
		i++;
	}
	return EXIT;
}

/*
��V·��Ϊ��ʯ���е�������������һ�������ĸ�������
*/
int suit_the_two_v_path(int Index)
{
	int i = 1,j;
	int flag = 0;
	int next, prev;
	next = RESULT->tempPath.tlines[Index][1];
	while (RESULT->tempPath.tlines[Index][i] != NO)
	{
		prev = RESULT->tempPath.tlines[Index][i];
		if (next != prev)
		{
			for (j = 0; j < p[prev]->count; j++)
			{
				if (p[prev]->lines[j].DesId == next)
				{
					if (p[prev]->lines[j].redV != 0)
					{
						//����������·�������һ�������ж�����һ��V�����ɫ
						//index = p[prev]->lines[j].redV - 1;
						flag = 1;
						break;
					}
				}
			}
		}
		next = prev;
		if (RESULT->tempPath.tlines[Index][i+1] == NO)
		{
			if (Index == 0)
			{
				prev = VSourseID;

			}
			else
			{
				prev = RESULT->tempPath.tlines[Index-1][1];
			}
			for (j = 0; j < p[prev]->count; j++)
			{
				if (p[prev]->lines[j].DesId == next)
				{
					if (p[prev]->lines[j].redV != 0)
					{
						flag = 1;
						//����������·�������һ�������ж�����һ��V�����ɫ
						//index = p[prev]->lines[j].redV - 1;
						break;
					}

				}
			}
		}
		if (flag!=0)
		{
			//��ʾ����������
			DotIndex = 0;
			p[VSourseID]->redV = 1;
			//p[VDestinationID]->red = 1;
			if (Index == 0)
			{
				DotContainer[0] = VSourseID;
				p[RESULT->tempPath.tlines[Index][1]]->redV = RIGHT;
			}
			else
			{
				DotContainer[0] = RESULT->tempPath.tlines[Index-1][1];
				p[RESULT->tempPath.tlines[Index][1]]->redV = RIGHT;
			}
			delete_Red_PV_Is_other(1);
			//������Ҫע����ǣ�ѡȡ��
			flag = find_the_min_repeat_path_from_v_to_v(1, Index, 1);
			p[RESULT->tempPath.tlines[Index][1]]->redV = Index + 1;
			if (flag == NO)
			{
				//����ݹ����û��������������Ҫ�Ѹ���·Ⱦ���Լ�����ɫ
				color_the_one_path_red(1, Index);
				return NO;
			}
			//����������������±������·��·��֮ǰ��������·��Ⱦɫ�������Ȼ���ʱ��󲿷ֵĵ㶼����ǳ�other�ˣ�����Ϊ�˷�ֹ��һ������Ҫ���һ��
			delete_one_path_PV_red(1, Index);
			//����Ȩ����Ϣ
			RESULT->tempPath.weight -= RESULT->tempPath.tlines[Index][0];
			RESULT->tempPath.tlines[Index][0] = p[RESULT->tempPath.tlines[Index][1]]->exitWeight;
			RESULT->tempPath.weight += RESULT->tempPath.tlines[Index][0];
			int evdot = p[RESULT->tempPath.tlines[Index][1]]->tempfromId;
			int h = 2;
			while (evdot != DotContainer[0])
			{
				RESULT->tempPath.tlines[Index][h] = evdot;
				evdot = p[evdot]->tempfromId;
				h++;
			}
			RESULT->tempPath.tlines[Index][h] = NO;
			//��·��������֮�����½�����·����ɫȾ���Լ�����ɫ
			color_the_one_path_red(1, Index);
			return SUCCESS;
		}
		flag = 0;
		i++;
	}
	return FINISH;
}


/*
���ҳ�����V��֮�䣬�ڲ���������·���ĵ��ͬʱ���ҳ�������·
@parm
IndexΪ0����ʾ����V·��
IndexΪ1����ʾ����P·��
*/
int find_the_min_repeat_path_from_v_to_v(int count, int q,int Index)
{
	int flag = 0; //������¼�Լ��ķ���ֵ
	int flag2;
	int n = DotIndex + count; //�����DotContainer�Ķ���λ��
	for (int j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (int i = 0; i < p[dot]->count; i++)
		{
			int temp;
			if (Index == 1)
			{
				flag2 = p[p[dot]->lines[i].DesId]->redV == 0 || p[p[dot]->lines[i].DesId]->redV == RIGHT || p[p[dot]->lines[i].DesId]->redV == (q + 1);
				temp = p[dot]->lines[i].redP;
			}
			else
			{			
				flag2 = p[p[dot]->lines[i].DesId]->redP == 0 || p[p[dot]->lines[i].DesId]->redP == RIGHT || p[p[dot]->lines[i].DesId]->redP == (q + 1);
				temp = p[dot]->lines[i].redV;
			}
			//����Ҫ�жϸõ��Ƿ��Ѿ��߹�,���ߵĵ㣬ֻ�����Լ��ĵ㣬�����Ǳ����ΪRIGHT��û���߹��ĵ�
			if (flag2)
			{				
				//��ʾû���غϱ�
				if (temp==0)
				{
					//����õ���V�㣬��ô���ǽ����ǣ����Ҳ�������һ�������ڵ㷶Χ��
					int temp2,temp3;
					if (Index==1)
					{
						temp2 = p[p[dot]->lines[i].DesId]->redV;
						temp3 = p[p[dot]->lines[i].DesId]->isV;
					}
					else
					{
						temp2 = p[p[dot]->lines[i].DesId]->redP;
						temp3 = p[p[dot]->lines[i].DesId]->isP;
					}
					if (temp2 == RIGHT)
					{
						//ֻҪ��CountIndex��CountNum���ʱ���������һ��
						p[p[dot]->lines[i].DesId]->tempfromId = dot;//��¼�����ĵ�
						p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//����Ȩ��ֵ�ۼ�
						resultDotId = p[dot]->lines[i].DesId;
						return SUCCESS;
					}
					//����õ㲻��V�㣬��ô���ǽ��������һ�������Ľڵ㷶Χ
					else
					{
						//��ֹ�����Լ���������
						if (temp3 == 0)
						{
							p[p[dot]->lines[i].DesId]->tempfromId = dot;
							p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;
							DotContainer[n] = p[dot]->lines[i].DesId;
							n++;
							if (Index==1)
							{
								p[p[dot]->lines[i].DesId]->redV = OTHER;
							}
							else
							{
								p[p[dot]->lines[i].DesId]->redP = OTHER;				
							}
						}
					}
				}			
			}
		}
	}
	int newCount = n - (DotIndex + count); //�������һ�����ӵ�������

	if (newCount == 0)
	{
		return NO; //�Ѿ�û�����������ˣ�˵�����е������Ѿ��������
	}
	DotIndex = DotIndex + count;//����DotIndex��ֵ
	flag = find_the_min_repeat_path_from_v_to_v(newCount,q,Index);
	if (flag == SUCCESS)
	{
		return SUCCESS;
	}
	//���û�����ţ��򷵻�No
	else 
	{
		return NO;
	}
}


/*
���dot���ϵ�PVȾɫ
*/
int cleanPVRed()
{
	for (int i = 0; i < DotCount; i++)
	{
		p[DotNum[i]]->redP = 0;
		p[DotNum[i]]->redV = 0;
		p[DotNum[i]]->tempfromId = 0;
		p[DotNum[i]]->temptoId = 0;
		p[DotNum[i]]->entryWeight = 0;
		p[DotNum[i]]->exitWeight = 0;
		for (int j = 0; j < p[DotNum[i]]->count; j++)
		{
			p[DotNum[i]]->lines[j].redP = 0;
			p[DotNum[i]]->lines[j].redV = 0;
		}
	}
	return 1;
}

/*
1����ĳһ��·��Ⱦɫ
*/
int color_the_one_path_red(int Index,int Index3)
{
	if (Index==1)
	{
		int s = 1;
		while (RESULT->tempPath.tlines[Index3][s] != NO)
		{
			p[RESULT->tempPath.tlines[Index3][s]]->redV = Index3 + 1;
			s++;
		}
	}
	else
	{
		int s = 1;
		while (RESULT->tempVPath.tlines[Index3][s] != NO)
		{
			p[RESULT->tempVPath.tlines[Index3][s]]->redP = Index3 + 1;
			s++;
		}
	}
	return 0;
}

/*
ɾ��ĳһ��·���ϵ�PVȾɫ
*/
int delete_one_path_PV_red(int flag,int Index)
{
	if (flag==1)
	{
		int s = 1;
		while (RESULT->tempPath.tlines[Index][s] != NO)
		{
			if (p[RESULT->tempPath.tlines[Index][s]]->redV == (Index + 1))
			{
				p[RESULT->tempPath.tlines[Index][s]]->redV = 0;
			}
			s++;
		}		
	}
	else
	{
		int s = 1;
		while (RESULT->tempVPath.tlines[Index][s] != NO)
		{
			if (p[RESULT->tempVPath.tlines[Index][s]]->redP == (Index + 1))
			{
				p[RESULT->tempVPath.tlines[Index][s]]->redP = 0;

			}
			s++;
		}
	}
	return 0;
}

//�������RedP��RedVΪ��OTHER���ı�־λ
int delete_Red_PV_Is_other(int Index)
{
	if (Index==1)
	{
		for (int i = 0; i < DotCount; i++)
		{
			if (p[DotNum[i]]->redV == OTHER)
			{
				p[DotNum[i]]->redV = 0;
			}
			p[DotNum[i]]->exitWeight = 0;
			p[DotNum[i]]->tempfromId = 0;
		}

	}
	else
	{
		for (int i = 0; i < DotCount; i++)
		{
			if (p[DotNum[i]]->redP == OTHER)
			{
				p[DotNum[i]]->redP = 0;
			}
			p[DotNum[i]]->exitWeight = 0;
			p[DotNum[i]]->tempfromId = 0;
		}
	}	
	return 0;
}
/*
�ֱ��P���V��Ⱦ�ϲ�ͬ����ɫ
������Ҫ���¸���һ��P���V�⣬����Ӱ��������
*/
int color_the_v_path(int Index,int flag)
{	
	int next, prev;
	int i, j;
	if (RESULT->repeatCount == 0)
	{
		redCount++;
	}
	if (redCount==0)
	{
		redCount = NO;
	}	
	for (i = 0; i < DOTLINES->SVlines[Index].index; i++)
	{
		next = DOTLINES->SVlines[Index].tlines[i][1];
		RESULT->tempPath.tlines[i][0] = DOTLINES->SVlines[Index].tlines[i][0];
		int s = 1;
		while (DOTLINES->SVlines[Index].tlines[i][s]!=NO)
		{
			prev = DOTLINES->SVlines[Index].tlines[i][s];
			if (next!=prev)
			{
				for (j = 0; j < p[prev]->count; j++)
				{
					if (p[prev]->lines[j].DesId == next)
					{
						p[prev]->lines[j].redV = i + 1;
						//�����ʱ�Ѿ��ҵ��غϱ�Ϊ0�Ľ⣬��ô���Ǿͼ���Ѱ�ҽ���
						if (flag==1)
						{
							p[prev]->lines[j].flagIndex = redCount; //��redCount��������־λ
						}
						break;
					}
				}
			}
			next = prev;
			RESULT->tempPath.tlines[i][s] = DOTLINES->SVlines[Index].tlines[i][s];
			p[RESULT->tempPath.tlines[i][s]]->redV = i + 1;
			s++;
		}
		RESULT->tempPath.tlines[i][s] = DOTLINES->SVlines[Index].tlines[i][s];
		if (i==0)
		{
			prev = VSourseID;
		}
		else
		{
			prev = DOTLINES->SVlines[Index].tlines[i-1][1];
		}
		if (next != prev)
		{
			for (j = 0; j < p[prev]->count; j++)
			{
				if (p[prev]->lines[j].DesId == next)
				{
					p[prev]->lines[j].redV = i + 1;
					if (flag == 1)
					{
						p[prev]->lines[j].flagIndex = redCount; //��redCount��������־λ
					}
					break;
				}
			}
		}
	}	
	RESULT->tempPath.weight = DOTLINES->SVlines[Index].weight;
	RESULT->tempPath.index = DOTLINES->SVlines[Index].index;
	return redCount;
}

/*

*/
int color_the_p_path(int Index, int red, int Sort_index,int flag)
{
	int next, prev;
	int i, j;
	for (i = 0; i < DOTLINES->CVlines[Index].index; i++)
	{
		RESULT->tempVPath.tlines[i][0] = DOTLINES->CVlines[Index].tlines[i][0];
		next = DOTLINES->CVlines[Index].tlines[i][1];
		int s = 1;
		while (DOTLINES->CVlines[Index].tlines[i][s]!= NO)
		{
			prev = DOTLINES->CVlines[Index].tlines[i][s];

			if (prev!=next)
			{
				for (j = 0; j < p[prev]->count; j++)
				{
					if (p[prev]->lines[j].DesId == next)
					{
						p[prev]->lines[j].redP = i + 1;
						if (flag == 1 && p[prev]->lines[j].flagIndex==red)
						{
							SORT->Result[Sort_index].count++;
						}
						break;
					}
				}
			}
			RESULT->tempVPath.tlines[i][s] = DOTLINES->CVlines[Index].tlines[i][s];
			p[RESULT->tempVPath.tlines[i][s]]->redP = i + 1;
			next = prev;
			s++;
		}
		RESULT->tempVPath.tlines[i][s] = DOTLINES->CVlines[Index].tlines[i][s];
		if (i == 0)
		{
			prev = VSourseID;
		}
		else
		{
			prev = DOTLINES->CVlines[Index].tlines[i-1][1];
		}
		if (next != prev)
		{
			for (j = 0; j < p[prev]->count; j++)
			{
				if (p[prev]->lines[j].DesId == next)
				{
					p[prev]->lines[j].redP = i + 1;
					if (flag == 1 && p[prev]->lines[j].flagIndex == red)
					{
						SORT->Result[Sort_index].count++;
					}
					break;
				}
			}
		}
	}
	RESULT->tempVPath.weight = DOTLINES->CVlines[Index].weight;
	RESULT->tempVPath.index = DOTLINES->CVlines[Index].index;
	return 0;
}

//���ȶ���ʼV�����Ⱦɫ
int core_algorithm()
{
	
	get_VGraph();
	get_PGraph();
	if (PVSORT == NULL)
	{
		PVSORT = (struct pv_sort *)malloc(sizeof(struct pv_sort));
		memset(PVSORT, 0, sizeof(struct pv_sort));
	}

	find_the_candiate_queue();
	PVheap_sort(DOTLINES->Sindexflag, PVSORT->V_sort);
	find_the_candiate_P_queue();
	PVheap_sort(DOTLINES->Cindexflag, PVSORT->P_sort);
	compare_with_v_queues();
	return 0;
}

