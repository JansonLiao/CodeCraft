
#include "common.h"
#include "malloc.h"


//录入有向图函数
int initDot()
{
	int LinkID, SourseID, DestinationID, Cost;
	FILE *fp_CVS;
	errno_t err;
	err = fopen_s(&fp_CVS, TOPO_URL, "r");
	if (err != 0)
	{
		printf("can't open the path.csv file\n");
		return 0;
	}
	DotCount = 0;
	while (fscanf_s(fp_CVS, "%d,%d,%d,%d\n", &LinkID, &SourseID, &DestinationID, &Cost) != EOF)
	{
		if (p[DestinationID] == NULL)
		{
			//用于产生一条新的Dot节点
			DotNum[DotCount] = DestinationID;
			DotCount++;
			p[DestinationID] = (struct Dot *)malloc(sizeof(struct Dot));
			memset(p[DestinationID], 0, sizeof(struct Dot));
			p[DestinationID]->Id = DestinationID;
		}
		if (p[SourseID] == NULL)
		{
			//用于产生一条新的Dot节点
			DotNum[DotCount] = SourseID;
			DotCount++; //节点数量值增加1
			p[SourseID] = (struct Dot *)malloc(sizeof(struct Dot));
			memset(p[SourseID], 0, sizeof(struct Dot));
			p[SourseID]->Id = SourseID;
		}

		//用于记录出度
		p[SourseID]->lines[p[SourseID]->count].lineId = LinkID;
		p[SourseID]->lines[p[SourseID]->count].DesId = DestinationID;
		p[SourseID]->lines[p[SourseID]->count].cost = Cost;
		p[SourseID]->lines[p[SourseID]->count].red = NO;
		p[SourseID]->count++;
	}
	return 1;
}

//初始化条件信息
int initPath()
{
	
	FILE *fd = NULL;
	errno_t err;
	err = fopen_s(&fd, DEMAND_URL, "r");
	if (err != 0)
	{
		printf("can't open the path.csv file\n");
		return 0;
	}
	int flag; 
	int flag2;
	int value;
	char line[4000];
	char *prev, *end;
	IncludeCount = 2;
	IncludeCount_p = 2;;
	for (int i = 0; i < 2; i++)
	{
		flag2 = 0;
		fgets(line, 4000, fd);
		prev = line;
		for (int j = 0; j < 3; j++)
		{
			end = strchr(prev, ',');
			*end = '\0';
			value = atoi(prev);
			if (j==1)
			{
				VSourseID = value;
			}
			if (j==2)
			{
				VDestinationID = value;
			}
			prev = end + 1;
			if (*prev == 'N')
			{
				flag2 = 1;
			}
		}
		if (flag2==1)
		{
			continue;
		}
		while (1)
		{
			flag = 0;
			end = strchr(prev, '|');
			if (end==NULL)
			{
				flag = 1;
			}
			if (flag==0)
			{
				*end = '\0';			
			}			
			value = atoi(prev);
			if (i==0)
			{
				v[value] = (struct DotV *)malloc(sizeof(struct DotV));
				v[value]->ID = value;
				v[value]->exitCount = 0;
				v[value]->contFlag = 0;
				v[value]->red = 0;
				p[value]->isV = 1;
				Include[IncludeCount] = value;
				IncludeCount++;
			}
			else
			{
				s[value] = (struct DotV *)malloc(sizeof(struct DotV));
				s[value]->ID = value;
				s[value]->exitCount = 0;
				s[value]->contFlag = 0;
				s[value]->red = 0;
				p[value]->isP = 1;
				Include_p[IncludeCount_p] = value;
				IncludeCount_p++;
			}
			if (flag == 0)prev = end + 1;
			else break;
		}
		
	}
	v[VSourseID] = (struct DotV *)malloc(sizeof(struct DotV));
	v[VSourseID]->ID = VSourseID;

	v[VSourseID]->exitCount = 0;
	v[VSourseID]->contFlag = 0;
	v[VSourseID]->red = 0;
	Include[0] = VSourseID;
	p[VSourseID]->isV = 1;
	v[VDestinationID] = (struct DotV *)malloc(sizeof(struct DotV));
	v[VDestinationID]->ID = VDestinationID;
	v[VDestinationID]->exitCount = 0;
	v[VDestinationID]->contFlag = 0;
	v[VDestinationID]->red = 0;
	Include[1] = VDestinationID;
	p[VDestinationID]->isV = 1;	

	s[VSourseID] = (struct DotV *)malloc(sizeof(struct DotV));
	s[VSourseID]->ID = VSourseID;
	s[VSourseID]->exitCount = 0;
	s[VSourseID]->contFlag = 0;
	s[VSourseID]->red = 0;
	Include_p[0] = VSourseID;
	p[VSourseID]->isP = 1;
	s[VDestinationID] = (struct DotV *)malloc(sizeof(struct DotV));
	s[VDestinationID]->ID = VDestinationID;
	s[VDestinationID]->exitCount = 0;
	s[VDestinationID]->contFlag = 0;
	s[VDestinationID]->red = 0;
	Include_p[1] = VDestinationID;
	p[VDestinationID]->isP = 1;		
	return 1;
	

}



