#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include <time.h>  
#include<string.h>
#include<math.h>

# define TOPO_URL "F:\\Job\\HUAWEI Code Craft 2016 ���������\\casetest3\\topo0.csv"
# define DEMAND_URL "F:\\Job\\HUAWEI Code Craft 2016 ���������\\casetest3\\demand0.csv"
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
	int redP;//P���߹��ı�
	int redV;//V���߹��ı�
	int red;
};

struct Dot
{
	int Id;//�ýڵ��id��
	struct Line lines[20];//���ڴ�ų��ȵ���
	int count;//��ʾ�ýڵ�ĳ���
	int red;//�ýڵ��Ƿ��Ѿ��ڶ�����
	int redP;
	int redV;
	int isV; //��ʾ�ýڵ�������V��Χ�ڵ�
	int isP; //��ʾ�ýڵ�������P��Χ�ڵ�
	int tempfromId;//������ʱ��������һ����
	int temptoId;//������ʱ����䵽��һ��ȥ
	int exitWeight;//���ڼ�¼�ӳ����㵽��õ��Ȩ��ֵ
	int entryWeight;//���ڼ�¼��������������õ��Ȩ��ֵ
	int countFlag;//������ǣ�����õ��Ѿ������������ڶ����ĵ㣬��һ�ξͲ�������һ���ˡ�����ñ��������ڸ���·�Ƿ�ȫͼ������
};

struct Link
{
	int VId;//����������V��
	int weight;//���õ��Ȩ��ֵ
	int fromId;//�丸�ڵ�
	int count;//����������ĸ���
	int line[200];//��Ҫ�����ĵ㣬������ྭ���ĸ���
};


struct DotV
{
	int ID;
	int exitCount;//V�ж����ĳ����
	struct Link linkExit[106];
	int contFlag; //�������ѡ��ָ����ĳһ��DotV��
	int tempIndex;//����ָ����һ����ȵ�����
	int red;
};

struct SVdotline
{
	int tlines[102][500];//��ʾ����ʼV�㹲�ж���������������·��
	int index;
	int weight;
	int red;//����·�Ƿ��Ѿ������
	int flag;//��ʾ����·������ѭ�򣬻��������
	int Candiate;
};

struct DotLines
{
	struct SVdotline SVlines[4000];//����P���͵Ľ�
	struct SVdotline CVlines[2000];//����"P���͵Ľ�
	int Sindexflag;//���ڼ�¼�������Ƿ��Ѿ�����ȫ��Ź�
	int Cindexflag;//���ڼ�¼��ѡ�������Ƿ��Ѿ�����ȫ�Ź�
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
	int index; //��V�������
	int count; //p�����V����غϱ���
	int weight; //��V���Ȩ��ֵ
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

//ȫ�ֱ���
int heapSize; //������
struct Dot *p[2000];//���ڼ�¼ÿһ��dot�ṹ��ĵ�ַ
struct DotV *v[2000];//���ڼ�¼ÿһ��dotV�ṹ��ĵ�ַ
struct DotV *s[2000];//���ڼ�¼�ڶ�����dotV�ṹ��ĵ�ַ
struct SaveVdot* SAVEDOT;
struct DotLines * DOTLINES;//���ڼ�¼������ʼ����ܵ�·��
struct Sort * SORT;
struct PVPath * RESULT;//save the result of the path
struct pv_sort * PVSORT;
int DotNum[3000];//���ڼ�¼����Ľڵ���Ϣ
int suitable_v[2000];//���ڼ�¼��Ҫ�����Ķ���
int suitable_p[2000];
int suitableWeight_v;
int suitableWeight_p;
int suitableCount_v;
int suitableCount_p;
int lineresult_v[2000];
int lineresult_p[2000];


int Include[102];
int IncludeCount;//���ڼ�¼�����ĵ�ĸ���
int Include_p[102];
int IncludeCount_p;


int DotCount;//��¼dot�ĸ���
int VSourseID, VDestinationID;//��ʾ�����յ�
int resultDotId; //�������ҵ�һ���ʺϵ�V��ʱ������õ�
int tempVcount;//���ڼ�¼�������������ɢ��ʱ������V�ĸ����������ж��Ƿ��޽����
int tempPcount;//���ڼ�¼�������������ɢ��ʱ������P�ĸ����������ж��Ƿ��޽����

int VdotIndex;//���ڼ�¼��Ѷ��д�����һ��Vdot
int VdotCountIndex;//���ڼ�¼��Ѷ��д���һ��Vdot�е���һ������


int DotContainer[2000];//��������Ѿ�Ⱦ��ɫ��DOT��
int DotIndex;//����ָ�����������Ķ�ֵ��

int tempLine[102][2000];//������ʱ���·��
int tempLineIndex;//����
int tempWeight;


int queueContainer[102];//����������ʵ�V������
int queueContainerIndex;//�����ά���������

int CountIndex;//�趨ȡ����һ��Vdot����ĵ�CountIndex��V��
int CountNum;//һ����������������׷��CountIndex����


int backRed[2000];//�����Ⱦɫ֮ǰ�����ڱ���Ⱦɫ

int Vcount; //�����޶�����V��ĸ���

int redCount;//���ں��浱repeatCountΪ0ʱ�����V��ı�����
int threshold;

//��������
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