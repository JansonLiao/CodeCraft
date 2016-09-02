#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include <time.h>  
#include<string.h>
#include<math.h>

# define TOPO_URL "F:\\Job\\HUAWEI Code Craft 2016 复赛赛题包\\casetest3\\topo0.csv"
# define DEMAND_URL "F:\\Job\\HUAWEI Code Craft 2016 复赛赛题包\\casetest3\\demand0.csv"
# define EXIT 2001
# define FINISH 2002
# define SUCCESS 2003
# define NO 2004
# define CANDIADTE 2005
# define RIGHT 2006
# define OTHER 2007
# define FAIL 2008
# define PPATH 4000
# define VPATH 6000
# define RPATH 8000


struct Line
{
	int lineId;
	int DesId;
	int cost;
	int flag[4000];
	int flagIndex;
	int redP;//P解走过的边
	int redV;//V解走过的边
	int red;
};

struct Dot
{
	int Id;//该节点的id号
	struct Line lines[20];//用于存放出度的线
	int count;//表示该节点的出度
	int red;//该节点是否已经在队列中
	int redP;
	int redV;
	int isV; //表示该节点是属于V范围内的
	int isP; //表示该节点是属于P范围内的
	int tempfromId;//用于暂时存放其从哪一点来
	int temptoId;//用于暂时存放其到哪一点去
	int exitWeight;//用于记录从出发点到达该点的权重值
	int entryWeight;//用于记录从射入点延生到该点的权重值
	int countFlag;//用来标记，如果该点已经搜索过离它第二近的点，下一次就不搜索这一点了。后面该变量还用于该线路是否被全图搜索过
};

struct Link
{
	int VId;//其所关联的V点
	int weight;//到该点的权重值
	int fromId;//其父节点
	int count;//其所经过点的个数
	int line[200];//需要经过的点，这里最多经过四个点
};


struct DotV
{
	int ID;
	int exitCount;//V中对它的出射度
	struct Link linkExit[106];
	int contFlag; //用来标记选择指定的某一个DotV点
	int tempIndex;//用来指向下一个入度的索引
	int red;
};

struct SVdotline
{
	int tlines[102][500];//表示该起始V点共有多少条符合条件的路径
	int index;
	int weight;
	int red;//该线路是否已经处理过
	int flag;//表示该线路是正向循序，还是逆序的
	int Candiate;
};

struct DotLines
{
	struct SVdotline SVlines[4000];//保存P类型的解
	struct SVdotline CVlines[2000];//保存"P类型的解
	int Sindexflag;//用于记录该容器是否已经被完全存放过
	int Cindexflag;//用于记录候选容器中是否已经被完全放过
	int CandidateIndex;
	int SuitIndex;
	int SminWeight; //save the minWeight of the S Scontainer
	int CminWeight; // save the minWeight of the Ccontainer
};

struct path_sort
{
	int index;
	int weight;
};

struct pv_sort
{
	struct path_sort P_sort[3000];
	int Pindex;
	struct path_sort V_sort[4000];
	int Vindex;
};

struct sort_result
{
	int index; //该V解的索引
	int count; //p解与该V解的重合边数
	int weight; //该V解的权重值
};

struct Sort
{
	struct sort_result Result[4000];
};


struct PVPath
{
	struct SVdotline P_path;//save the p path
	struct SVdotline V_path;//save the v path
	int repeatCount;
	int wholeWeight;
	struct SVdotline tempPath;
	struct SVdotline tempVPath;
};

//全局变量
int heapSize; //堆排序
struct Dot *p[2000];//用于记录每一个dot结构体的地址
struct DotV *v[2000];//用于记录每一个dotV结构体的地址
struct DotV *s[2000];//用于记录第二个解dotV结构体的地址
struct SaveVdot* SAVEDOT;
struct DotLines * DOTLINES;//用于记录所有起始点可能的路径
struct Sort * SORT;
struct PVPath * RESULT;//save the result of the path
struct pv_sort * PVSORT;
int DotNum[3000];//用于记录存入的节点信息
int suitable_v[2000];//用于记录需要经过的顶点
int suitable_p[2000];
int suitableWeight_v;
int suitableWeight_p;
int suitableCount_v;
int suitableCount_p;
int lineresult_v[2000];
int lineresult_p[2000];


int Include[102];
int IncludeCount;//用于记录经过的点的个数
int Include_p[102];
int IncludeCount_p;


int DotCount;//记录dot的个数
int VSourseID, VDestinationID;//表示起点和终点
int resultDotId; //用于在找到一个适合的V点时，保存该点
int tempVcount;//用于记录在向外或者向里散射时，包含V的个数，用于判断是否无解情况
int tempPcount;//用于记录在向外或者向里散射时，包含P的个数，用于判断是否无解情况

int VdotIndex;//用于记录最佳队列处于哪一个Vdot
int VdotCountIndex;//用于记录最佳队列处于一个Vdot中的那一个队列


int DotContainer[2000];//用来存放已经染好色的DOT点
int DotIndex;//总是指向上面容器的顶值。

int tempLine[102][2000];//用于临时存放路径
int tempLineIndex;//索引
int tempWeight;


int queueContainer[102];//用来保存合适的V点排序
int queueContainerIndex;//上面二维数组的索引

int CountIndex;//设定取出离一个Vdot最近的第CountIndex个V点
int CountNum;//一个自增变量，用于追赶CountIndex变量


int backRed[2000];//在清除染色之前，用于备份染色

int Vcount; //用于限定搜索V解的个数

int redCount;//用于后面当repeatCount为0时，标记V解的边所用
int threshold;

//声明函数
int initDot();
int initPath();
int cleanRed();
int dotExitEntire(int count);
int dotEntryEntire(int count);
void maxHeapify(int index);
void heap_sort(int length);
int find_min_near_Vdot_path(int count);
int find_min_near_Pdot_path(int count);
int get_VDot_path();
int get_PDot_path();
void get_VGraph();
int minus_relative_entry_exit_count(int fromId, int desId, int flag);
int add_relative_entry_exit_count(int fromId, int desId, int flag);
int get_min_entryCount(int Id);
int get_min_p_entryCount(int Id);
int retreat_num_of_Vdot(int count);
int retreat_num_of_p_dot(int count);
int find_the_candiate_queue();
int get_the_correct_path(int flag);
int save_and_color_the_queue(int flag);
int color_the_path_where_red_is_zero(int Index);
int find_path_from_all_dot(int count, int index);
int color_the_one_Vdot(int Index, int flag, int Index2);
int delet_color_queue(int Index);
int delete_Red_Is_other();
int color_the_back_path(int Index);
int save_the_red(int flag);
int suit_the_candiate_queue(int Index, int indexFlag, int Index2, int flag2);
int find_the_useful_path_from_v_to_v(int count, int q, int index);
int save_the_correct_path(int flag);
int find_the_candiate_P_queue();
int compare_with_v_queues();
int get_the_min_repeat(int Index, int Index2, int Index3);
int Save_the_two_path_result(int repeat);
int suit_the_two_v_path(int Index);
int find_the_min_repeat_path_from_v_to_v(int count, int q, int Index);
int cleanPVRed();
int color_the_one_path_red(int Index, int Index3);
int delete_one_path_PV_red(int flag, int Index);
int delete_Red_PV_Is_other(int Index);
int color_the_p_path();
int core_algorithm();
void PV_maxHeapify(int index, struct path_sort * sort);
int PVheap_sort(int length, struct path_sort * sort);
int sort_the_repeat_result();
int get_the_result_of_repeat(int index);
int color_the_v_path(int Index, int flag);
int suit_the_two_p_path(int Index, int Index2);