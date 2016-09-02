#include "common.h"


//清除染色
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

//使用堆排序，将DotContainer[DotIndex]到DotContainer[DotIndex+count]按照权重由小到大进行排序
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

//计算从起始点向外射出，找出所有不经过V中的点，直接与起始点能够发生连接的(并且是最近的距离)。初始值count=1
//需要注意的是，每一次调用该方法，需要将全局变量DotIndex置0，同时DotContainer[0]的值需要赋值为终点ID
int find_min_near_Vdot_path(int count)
{
	int n = DotIndex + count; //计算出DotContainer的顶端位置
	int flag = 0;
	//这里使用大根堆排序，按照权重由小到大进行排序
	heap_sort(count);
	int i, j;
	for (j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];		
		for (i = 0; i < p[dot]->count; i++)
		{
			//首先要判断该点是否已经走过
			if (p[p[dot]->lines[i].DesId]->red != 3)
			{
				//如果该点是V点，那么我们将其标记，并且不计入下一次搜索节点范围。
				if (p[p[dot]->lines[i].DesId]->isV == 1)
				{
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//记录其来的点
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//将其权重值累加
					v[DotContainer[0]]->linkExit[v[DotContainer[0]]->exitCount].VId = p[dot]->lines[i].DesId;
					v[DotContainer[0]]->linkExit[v[DotContainer[0]]->exitCount].fromId = dot;
					v[DotContainer[0]]->linkExit[v[DotContainer[0]]->exitCount].weight = p[p[dot]->lines[i].DesId]->exitWeight;//将其权重值保存到其他地方
					v[DotContainer[0]]->exitCount++; //出度相应增加1
				}
				//如果该点不是V点，那么我们将其计入下一次搜索的节点范围
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
	int newCount = n - (DotIndex + count); //计算出这一轮增加的数据量
	if (newCount == 0)
	{
		return EXIT; //已经没有数据增加了，说明所有的数据已经遍历完毕
	}
	DotIndex = DotIndex + count;//更新DotIndex的值
	if (find_min_near_Vdot_path(newCount) == EXIT)
	{
		return EXIT;
	}
	return EXIT;
}

//计算从起始点向外射出，找出所有不经过P中的点，直接与起始点能够发生连接的(并且是最近的距离)。初始值count=1
//需要注意的是，每一次调用该方法，需要将全局变量DotIndex置0，同时DotContainer[0]的值需要赋值为终点ID
int find_min_near_Pdot_path(int count)
{
	int n = DotIndex + count; //计算出DotContainer的顶端位置
	int flag = 0;
	//这里使用大根堆排序，按照权重由小到大进行排序
	heap_sort(count);
	int i, j;
	for (j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (i = 0; i < p[dot]->count; i++)
		{

			//首先要判断该点是否已经走过
			if (p[p[dot]->lines[i].DesId]->red != 3)
			{
				//如果该点是V点，那么我们将其标记，并且不计入下一次搜索节点范围。
				if (p[p[dot]->lines[i].DesId]->isP == 1)
				{
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//记录其来的点
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//将其权重值累加
					s[DotContainer[0]]->linkExit[s[DotContainer[0]]->exitCount].VId = p[dot]->lines[i].DesId;
					s[DotContainer[0]]->linkExit[s[DotContainer[0]]->exitCount].fromId = dot;
					s[DotContainer[0]]->linkExit[s[DotContainer[0]]->exitCount].weight = p[p[dot]->lines[i].DesId]->exitWeight;//将其权重值保存到其他地方
					s[DotContainer[0]]->exitCount++; //出度相应增加1
				}
				//如果该点不是V点，那么我们将其计入下一次搜索的节点范围
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
	int newCount = n - (DotIndex + count); //计算出这一轮增加的数据量
	if (newCount == 0)
	{
		return EXIT; //已经没有数据增加了，说明所有的数据已经遍历完毕
	}
	DotIndex = DotIndex + count;//更新DotIndex的值
	if (find_min_near_Pdot_path(newCount) == EXIT)
	{
		return EXIT;
	}
	return EXIT;
}

//将出该V点射出到其它所有V点的路径信息全部都保存起来
int get_VDot_path()
{
	int i;
	int j;
	//冒泡排序,按照权重大小，由小到大排列
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

//将出该P点射出到其它所有P点的路径信息全部都保存起来
int get_PDot_path()
{
	int i;
	int j;
	//冒泡排序,按照权重大小，由小到大排列
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

//获取属于必经点的V图，这个图包括起始点、终点
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

//获取属于必经点的P图，这个图包括起始点、终点
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
这一部分主要是，找出所有的候选解，这些候选解不一定都满足题目要求，后续会动态调整这些候选解
1、首先从起始点开始，找出入度最小的下一个V点，将其加入队列中
2、将起始点射出到其它的V点的入度相应减1，同时将射向这个被加入V点的出度也相应减1，
3、以后我们在加入V点额过程中，只要判断余下的V点是否含有入度或出度为0的点，如果有，则说明该线已经无解
4、每当我们找到一条候选解，我们就马上调整这条候选解，将其调整为可行解，加入进可行解的容器中
5、

*/


//获取入度最小的下一个V点，这个需要根据countFlag标志位来
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
	//如果找到下一个V点
	if (Index!=NO)
	{		
		v[v[Id]->linkExit[Index].VId]->red = 1;
		v[Id]->tempIndex = Index;//保存该索引
		return v[Id]->linkExit[Index].VId;
	}
	//如果没有找到
	else
	{
		return NO;
	}

}

//获取入度最小的下一个V点，这个需要根据countFlag标志位来
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
	//如果找到下一个V点
	if (Index != NO)
	{	
		s[s[Id]->linkExit[Index].VId]->red = 1;
		s[Id]->tempIndex = Index;//保存该索引
		return s[Id]->linkExit[Index].VId;
	}
	//如果没有找到
	else
	{
		return NO;
	}

}

//后退节点数
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

//后退节点数
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

//用于寻找候选解
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
					retreat_num_of_Vdot(1);//回退一个V点
				}
				else
				{
					retreat_num_of_Vdot(minRetreat);//回退一个V点
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
			//如果只剩最后的VDestination了，那么就清除终点的染色
			v[VDestinationID]->red = 0;
			VId = get_min_entryCount(queueContainer[queueContainerIndex - 1]);
			if (VId == NO)
			{
				retreat_num_of_Vdot(maxRetreat);//回退2个V点
				if (queueContainerIndex == 1)
				{
					return NO;
				}
				v[queueContainer[queueContainerIndex - 1]]->contFlag++;
			}
			//说明已经找到了一条候选解了
			else
			{
				queueContainer[queueContainerIndex] = VId;
				queueContainerIndex++;
				//调整该候选解，使之成为满足条件的解
				if (get_the_correct_path(0) == SUCCESS)
				{
					//保存该解
					if (save_the_correct_path(0) == EXIT)return EXIT;
					DOTLINES->SuitIndex++;
					int rate;
					rate = DOTLINES->CandidateIndex / DOTLINES->SuitIndex;
					if (rate > 2)
					{
						rate = maxRetreat / 2;
						retreat_num_of_Vdot(rate);//回退2个V点
					}
					else
					{
						retreat_num_of_Vdot(maxRetreat);//回退2个V点
					}
					if (queueContainerIndex == 1)
					{
						return NO;
					}
					v[queueContainer[queueContainerIndex - 1]]->contFlag++;
					//查找"p是否已经有解，如果有，就将该解与"p解比较，看是否有重边
				}
				else
				{
					DOTLINES->CandidateIndex++;
					retreat_num_of_Vdot(maxRetreat);//回退4个V点
					v[queueContainer[queueContainerIndex - 1]]->contFlag++;
				}
			}
		}
	}
}


/*
下面这一部分，主要是两个工作：
1、动态调节候选解，使之由候选解，变成有解。
*/

//核心函数，该函数用于调整可能是解的候选解
int get_the_correct_path(int flag)
{
	//首先要对所有的点进行染色清零
	cleanRed();
	//h代表每一个V点里面的队列个数
	save_and_color_the_queue(flag);
	for (int j = 0; j < queueContainerIndex - 1; j++)
	{
		//color_the_path_where_red_is_zero(j);
		if (suit_the_candiate_queue(j, 0, 0, flag) == FAIL)return FAIL;
	}
	return SUCCESS;	
}


//给某一个队列的不同V的路径染上不同颜色
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
			//这里的count一定要减一，因为最后一个点就是其自己
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
			//这里的count一定要减一，因为最后一个点就是其自己
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


//该函数用于将之前线路不要的点，但是后面的线路占用的点重新染上后面这条线路的颜色
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

//该函数用于全图搜索
int find_path_from_all_dot(int count,int index)
{
	int flag = 0; //用来记录自己的返回值
	int flag2;
	int n = DotIndex + count; //计算出DotContainer的顶端位置
	for (int j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (int i = 0; i < p[dot]->count; i++)
		{
			
			if (p[p[dot]->lines[i].DesId]->red == 0 || p[p[dot]->lines[i].DesId]->red == RIGHT)
			{
				//如果该点是V点，那么我们将其标记，并且不计入下一次搜索节点范围。
				if (p[p[dot]->lines[i].DesId]->red == RIGHT)
				{
					//只要当CountIndex与CountNum相等时，才输出这一点
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//记录其来的点
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//将其权重值累加
					resultDotId = p[dot]->lines[i].DesId;
					return SUCCESS;

				}
				//如果该点不是V点，那么我们将其计入下一次搜索的节点范围
				else
				{
					//防止出现自己被检索到
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
	int newCount = n - (DotIndex + count); //计算出这一轮增加的数据量

	if (newCount == 0)
	{
		return NO; //已经没有数据增加了，说明所有的数据已经遍历完毕
	}
	DotIndex = DotIndex + count;//更新DotIndex的值
	flag = find_path_from_all_dot(newCount,index);
	if (flag == SUCCESS)
	{
		return SUCCESS;
	}
	//如果没有找着，则返回No
	else 
	{
		return NO;
	}
}

//给某一个V点的路径染色
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

//该函数用于在保存新的线路之前，清除现有的染色，并且是没有被其他线路占有的点
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

//清除所有Red为“OTHER”的标志位
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

//给已经标记位countFlag的线路染色，表示该线路已经整个图都搜索过一次了，其线路已经不能改变
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

//染色备份函数，该函数用于在全图搜索之前，应该将现有的染色都保存起来，因为在全图搜索的时候，会将所有的点清除颜色
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

//核心算法，用于动态调整一个可能是解的候选解，这里会有一个递归调用的过程，一共有三个参数，其中第三个参数表示是否是递归调用
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
			//表示是正向序列
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
					//首先第一步还是给自己的点染上颜色
					p[tempLine[Index][i]]->red = Index + 1;
					color_the_one_Vdot(Index,1,0);
					//递归调用共用的那个点
					flag = suit_the_candiate_queue(index, 1,Index,flag2);
					if (flag == NO)
					{
						if (p[tempLine[Index][1]]->countFlag == 0)
						{
							
							//开始整个范围的点进行搜捕
							DotIndex = 0;
							//这里需要进行染色备份
							save_the_red(1);
							//清除染色
							cleanRed();
							//将起始点和终点染色好
							//p[VDestinationID]->red = 1;
							p[VSourseID]->red = 1;
							DotContainer[0] = tempLine[Index - 1][1];
							p[tempLine[Index][1]]->red = RIGHT;
							//将已经进行过全图搜捕的线路进行染色，防止其被后面的线路占用
							color_the_back_path(Index);
							//进行全图搜捕
							flag = find_path_from_all_dot(1,flag2);
							//搜索完了之后，应该恢复到之前的颜色

							if (flag != SUCCESS)
							{
								return FAIL;
							}
							save_the_red(0);
							//判断是否搜索成功，如果不成功，就放弃该线路了，直接退出了						
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
					//如果递归调用没有搜索到，则还是要把该线路染成自己的颜色
					color_the_one_Vdot(Index,2,Index2);
					return NO;
				}
			}
			//这个函数用于在重新保存该线路的路径之前，将旧线路的染色清除，虽然这个时候大部分的点都被标记成other了，但是为了防止万一，还是要清除一下
			delet_color_queue(Index);
			//更新权重信息
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
			//线路保存完了之后，重新将新线路的颜色染成自己的颜色
			color_the_one_Vdot(Index,0,0);
			return SUCCESS;
		}
		i++;
	}
	return FINISH;
}


//查找出两个V点之间，在不经过其它路径的点的同时，找出这条线路
int find_the_useful_path_from_v_to_v(int count, int q,int index)
{
	int flag = 0; //用来记录自己的返回值
	int n = DotIndex + count; //计算出DotContainer的顶端位置
	int Index;
	for (int j = DotIndex; j < DotIndex + count; j++)
	{
		int dot = DotContainer[j];
		for (int i = 0; i < p[dot]->count; i++)
		{
			//首先要判断该点是否已经走过,能走的点，只能是自己的点，或者是被标记为RIGHT和没有走过的点
			if (p[p[dot]->lines[i].DesId]->red == 0 || p[p[dot]->lines[i].DesId]->red == RIGHT || p[p[dot]->lines[i].DesId]->red == (q + 1))
			{
				//如果该点是V点，那么我们将其标记，并且不计入下一次搜索节点范围。
				if (p[p[dot]->lines[i].DesId]->red == RIGHT)
				{
					//只要当CountIndex与CountNum相等时，才输出这一点
					p[p[dot]->lines[i].DesId]->tempfromId = dot;//记录其来的点
					p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//将其权重值累加
					resultDotId = p[dot]->lines[i].DesId;
					return SUCCESS;

				}
				//如果该点不是V点，那么我们将其计入下一次搜索的节点范围
				else
				{
					//防止出现自己被检索到
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
	int newCount = n - (DotIndex + count); //计算出这一轮增加的数据量

	if (newCount == 0)
	{
		return NO; //已经没有数据增加了，说明所有的数据已经遍历完毕
	}
	DotIndex = DotIndex + count;//更新DotIndex的值
	flag = find_the_useful_path_from_v_to_v(newCount, q,index);
	if (flag == SUCCESS)
	{
		return SUCCESS;
	}
	//如果没有找着，则返回No
	else 
	{
		return NO;
	}
}

/*
下面这一部分是用于保存已经调整出来的，并且是正确的解
*/
int save_the_correct_path(int flag)
{
	if (DOTLINES==NULL)
	{
		DOTLINES = (struct DotLines *)malloc(sizeof(struct DotLines));
		memset(DOTLINES, 0, sizeof(struct  DotLines));
	}	
	//表示是存P解
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
			//首先对这1000条解进行排序，然后			
			return EXIT;
		}
	}
	return FINISH;
}


//heap_sort
//使用堆排序，将V路径的解按照权重由小到大进行排序
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
下面这一步主要是寻找P解，同时将P解与现在已经找出来的1000个解一一进行比对，看是否能够找出最小的重合边的数
1、首先给这1000条V解进行标记（这一步可以在前面做）
2、每寻找一个P解，就用这个P解和余下来的1000条V解进行匹配
*/

//用于寻找P候选解
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
					retreat_num_of_p_dot(1);//回退一个V点
				}
				else
				{
					retreat_num_of_p_dot(minRetreat);//回退一个V点
				}
				//retreat_num_of_p_dot(minRetreat);//回退一个V点

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
			//如果只剩最后的VDestination了，那么就清除终点的染色
			s[VDestinationID]->red = 0;
			VId = get_min_p_entryCount(queueContainer[queueContainerIndex - 1]);
			if (VId == NO)
			{
				retreat_num_of_p_dot(maxRetreat);//回退2个V点
				if (queueContainerIndex == 1)
				{
					return NO;
				}
				s[queueContainer[queueContainerIndex - 1]]->contFlag++;
			}
			//说明已经找到了一条候选解了
			else
			{
				queueContainer[queueContainerIndex] = VId;
				queueContainerIndex++;
				//调整该候选解，使之成为满足条件的解
				if (get_the_correct_path(1) == SUCCESS)
				{
					//保存该解					
					if (save_the_correct_path(1) == EXIT)return EXIT;
					DOTLINES->SuitIndex++;
					int rate;
					rate = DOTLINES->CandidateIndex / DOTLINES->SuitIndex;
					if (rate > 2)
					{
						rate = maxRetreat / 2;
						retreat_num_of_p_dot(rate);//回退2个V点
					}
					else
					{
						retreat_num_of_p_dot(maxRetreat);//回退2个V点
					}
					if (queueContainerIndex == 1)
					{
						return NO;
					}
					s[queueContainer[queueContainerIndex - 1]]->contFlag++;
					//查找"p是否已经有解，如果有，就将该解与"p解比较，看是否有重边
				}
				else
				{
					DOTLINES->CandidateIndex++;
					retreat_num_of_p_dot(maxRetreat);//回退4个V点
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


//该函数用于在找到一条P解之后，与前面的1000个V解进行不重边比较
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
			//获取最小的重合边数
			for (j = 0; j < threshold; j++)
			{			
				get_the_min_repeat(j, index,0);
			}
			//释放SORT的资源
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
					get_the_min_repeat(PVSORT->V_sort[j].index,index,1); //传递V解的索引和P解的索引					
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
同时与1000条V解进行重合边比较
@param
index 表示V解在容器中的索引号
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
 下面根据重合边数进行排序
 @param
 threshold 表示选取重合边为前threshold的解进行调整
*/
int sort_the_repeat_result()
{
	//当解比较少，则按照实际解的个数
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
从重合边最小的解开始调整
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
	
	//如果wholeWeight还从来没有储存过解，则首先将第一个解存进去
	if (RESULT->wholeWeight == 0)
	{
		Save_the_two_path_result(SORT->Result[Index].count);
	}

	//如果重合边不为0，则逐步调整
	if (SORT->Result[Index].count != 0)
	{
		SORT->Result[Index].count = 0;
		for (int i = 0; i < RESULT->tempVPath.index; i++)
		{
			suit_the_two_p_path(i, Index);
		}
	}
	//判断如果比之前的解要好，则存起来！
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
保存好两条路径的解
@param
repeat 表示该解的重边个数
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
	//printf("P解：");
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
以P路径为基石进行调整
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
						//如果这是这段路径的最后一个，则判断其上一个V点的颜色
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
						//如果这是这段路径的最后一个，则判断其上一个V点的颜色
						flag3 = p[prev]->lines[j].redV - 1;						
					}
					break;

				}
			}

		}		
		if (flag != 0)
		{
			
			//表示是正向序列
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
			//这里需要注意的是，选取的
			flag2 = find_the_min_repeat_path_from_v_to_v(1, Index, 0);
			p[RESULT->tempVPath.tlines[Index][1]]->redP = Index + 1;
			if (flag2 == NO)
			{
				//首先第一步还是给自己的点染上颜色
				p[RESULT->tempVPath.tlines[Index][i]]->redP = Index + 1;
				color_the_one_path_red(0, Index);
				//递归调用共用的那个点
				flag2 = suit_the_two_v_path(flag3);
				if (flag2 == NO)
				{
					//可以在这里记录重复边的个数
					
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
			//这个函数用于在重新保存该线路的路径之前，将旧线路的染色清除，虽然这个时候大部分的点都被标记成other了，但是为了防止万一，还是要清除一下
			delete_one_path_PV_red(0, Index);
			//更新权重信息
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
			//线路保存完了之后，重新将新线路的颜色染成自己的颜色
			color_the_one_path_red(0, Index);
			return SUCCESS;
			
		}
		flag = 0;		
		i++;
	}
	return EXIT;
}

/*
以V路径为基石进行调整，这里是上一个函数的辅助函数
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
						//如果这是这段路径的最后一个，则判断其上一个V点的颜色
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
						//如果这是这段路径的最后一个，则判断其上一个V点的颜色
						//index = p[prev]->lines[j].redV - 1;
						break;
					}

				}
			}
		}
		if (flag!=0)
		{
			//表示是正向序列
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
			//这里需要注意的是，选取的
			flag = find_the_min_repeat_path_from_v_to_v(1, Index, 1);
			p[RESULT->tempPath.tlines[Index][1]]->redV = Index + 1;
			if (flag == NO)
			{
				//如果递归调用没有搜索到，则还是要把该线路染成自己的颜色
				color_the_one_path_red(1, Index);
				return NO;
			}
			//这个函数用于在重新保存该线路的路径之前，将旧线路的染色清除，虽然这个时候大部分的点都被标记成other了，但是为了防止万一，还是要清除一下
			delete_one_path_PV_red(1, Index);
			//更新权重信息
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
			//线路保存完了之后，重新将新线路的颜色染成自己的颜色
			color_the_one_path_red(1, Index);
			return SUCCESS;
		}
		flag = 0;
		i++;
	}
	return FINISH;
}


/*
查找出两个V点之间，在不经过其它路径的点的同时，找出这条线路
@parm
Index为0，表示不走V路径
Index为1，表示不走P路径
*/
int find_the_min_repeat_path_from_v_to_v(int count, int q,int Index)
{
	int flag = 0; //用来记录自己的返回值
	int flag2;
	int n = DotIndex + count; //计算出DotContainer的顶端位置
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
			//首先要判断该点是否已经走过,能走的点，只能是自己的点，或者是被标记为RIGHT和没有走过的点
			if (flag2)
			{				
				//表示没有重合边
				if (temp==0)
				{
					//如果该点是V点，那么我们将其标记，并且不计入下一次搜索节点范围。
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
						//只要当CountIndex与CountNum相等时，才输出这一点
						p[p[dot]->lines[i].DesId]->tempfromId = dot;//记录其来的点
						p[p[dot]->lines[i].DesId]->exitWeight = p[dot]->exitWeight + p[dot]->lines[i].cost;//将其权重值累加
						resultDotId = p[dot]->lines[i].DesId;
						return SUCCESS;
					}
					//如果该点不是V点，那么我们将其计入下一次搜索的节点范围
					else
					{
						//防止出现自己被检索到
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
	int newCount = n - (DotIndex + count); //计算出这一轮增加的数据量

	if (newCount == 0)
	{
		return NO; //已经没有数据增加了，说明所有的数据已经遍历完毕
	}
	DotIndex = DotIndex + count;//更新DotIndex的值
	flag = find_the_min_repeat_path_from_v_to_v(newCount,q,Index);
	if (flag == SUCCESS)
	{
		return SUCCESS;
	}
	//如果没有找着，则返回No
	else 
	{
		return NO;
	}
}


/*
清除dot点上的PV染色
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
1、给某一个路径染色
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
删除某一条路径上的PV染色
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

//清除所有RedP或RedV为“OTHER”的标志位
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
分别给P解和V解染上不同的颜色
这里需要重新复制一份P解和V解，否则将影响后续结果
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
						//如果此时已经找到重合边为0的解，那么我们就加速寻找进程
						if (flag==1)
						{
							p[prev]->lines[j].flagIndex = redCount; //用redCount标记这个标志位
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
						p[prev]->lines[j].flagIndex = redCount; //用redCount标记这个标志位
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

//首先对起始V点进行染色
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

