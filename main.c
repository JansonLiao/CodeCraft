
#include "common.h"
#include<time.h>

int main()
{
	clock_t start, finish;
	start = clock();
	int flag;

	//录入数据
	flag = initDot();
	if (flag == 0)
	{
		printf("录入有向图失败！\n");
	}
	flag = initPath();
	if (flag == 0)
	{
		printf("录入有向图失败！\n");
	}
	core_algorithm();
	int j = 0;
	if (RESULT->wholeWeight != 0)
	{
		printf("totalWeight：%d  repeatCount：%d\n", RESULT->wholeWeight, RESULT->repeatCount);
		printf("the first path：\n");
		int i = 1;
		suitable_v[0] = VSourseID;
		for (int q = 0; q < RESULT->V_path.index; q++)
		{
			int s = 1;
			while (RESULT->V_path.tlines[q][s] != NO)
			{
				++s;
			}
			--s;
			while (s > 0)
			{
				suitable_v[i] = RESULT->V_path.tlines[q][s];
				i++;
				s--;
			}
		}
		int weight=0;
		int weight1 = 0;
		for (j=0; j < (i - 1); j++)
		{
			int h = 0;
			if (p[suitable_v[j]]->isV == 1)
			{
				weight1 = 0;
			}
			for (; h < p[suitable_v[j]]->count; h++)
			{
				if (p[suitable_v[j]]->lines[h].DesId == suitable_v[j + 1])
				{
					lineresult_v[j] = p[suitable_v[j]]->lines[h].lineId;
					weight += p[suitable_v[j]]->lines[h].cost;
					weight1 += p[suitable_v[j]]->lines[h].cost;
					break;
				}

			}
		}

		for (int z = 0; z < j; z++)
		{
			printf("%d|", lineresult_v[z]);
		}
		printf("\n");
		printf("\n");
		weight = 0;
		printf("the second path：\n");
		i = 1;
		j = 0;
		suitable_p[0] = VSourseID;
		for (int q = 0; q < RESULT->P_path.index; q++)
		{
			int s = 1;
			while (RESULT->P_path.tlines[q][s] != NO)
			{
				++s;
			}
			--s;
			while (s > 0)
			{
				suitable_p[i] = RESULT->P_path.tlines[q][s];
				i++;
				s--;
			}
		}
		for (j=0; j < (i - 1); j++)
		{
			int h = 0;
			for (; h < p[suitable_p[j]]->count; h++)
			{
				if (p[suitable_p[j]]->lines[h].DesId == suitable_p[j + 1])
				{
					lineresult_p[j] = p[suitable_p[j]]->lines[h].lineId;
					weight += p[suitable_p[j]]->lines[h].cost;
					weight1 += p[suitable_p[j]]->lines[h].cost;
					break;
				}

			}
		}
		for (int z = 0; z < j; z++)
		{
			printf("%d|", lineresult_p[z]);
		}

	}
	printf("\n");
	finish = clock();
	printf("总时长：%d ms", finish - start);
	printf("\n");
	system("pause");

}





