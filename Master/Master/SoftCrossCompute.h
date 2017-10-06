#pragma once
#include <memory.h>

//#include "GridCtrl_src\GridCtrl.h"

#define HS_VALUE	7.56
#define CONCRETE_PILLAR	0
#define STEEL_PILLAR	1
#define HANG_PT_CROSS_DISTANCE	35
#define MAX_COMP_SIZE	5
#define MAX_TRACK_DIST	20
#define MAX_PILLAR_PARAM_VAL 30
#define DEFLECTION	0.09	//扰度
#define DELTA_UP_DOWN	1.2		//上部固定绳与下部固定绳的固定差值
const double EPSILON = 1.00e-07;
#define IS_ZERO(x)  ((x<=EPSILON && x>= -EPSILON))




/*the metric is m*/
typedef struct pillar_param{

	int pillar_type;//1 表示混凝土柱，2表示钢柱
	double pillar_h;//the height of pillar;
	double limit_range;//限界
	double delta_of_surface; //基础面与柱面高差 公式中的S上
	double slope; //斜率
	double width_top;//支柱顶部宽度
	double pillar_bottom;//支柱下底
	double pillar_top;//支柱上底
//	double component_size[5];//零件链，主要是上部固定绳上的零件尺寸，不能超过五个
}pillar_t;
typedef struct node_st{
	double weight;//节点自重
	int is_hang_node;//是否为悬挂节点
	double dist_to_front_node;//至前一节点跨距
	double dist_to_rear_node;//至后一节点跨距
	double catenary_w_per_m;//承力索每米自重
	double touch_line_w_per_m;//每米接触线重量
	double horizon_catenary_length;//节点横承长度，画图用
	double upper_rope_length;//上部固定绳长度
	double lower_rope_length;//下部固定绳长度
	double left_no_hang_dist;//左侧非悬挂节点到本节点距离（计算得到）
	double right_no_hang_dist;//右侧非悬挂节点到本节点距离（计算得到）
	//add by czh @ 2013.3.26
	double distance2pillar; //节点到支柱的距离，为0表示本节点不是边界节点
	node_st(){
		distance2pillar = 0;
	}
	//end 
}node_t;

typedef struct input_param{
	double designed_stright_hang_size; //用户输入的最低点的直吊子长度
	pillar_t pillar_left;
	pillar_t pillar_right;//left pillar and right pillar
	struct{
		double ear_size;//耳朵连接器尺寸
		double spring_size;//弹簧补偿器尺寸
		double ear_ring_stick_size; //耳环杆尺寸
		double head_stick_size;//杵头杆
	}component_size_t;
	struct{//股道间距
		double* distance; 
		int num_of_distance;
	}dist;
	struct{//real_pull_value = design_value - 4*over_value
		double* real_pull_value; //实际拉出值(计算得到，用户无需输入)
		double design_value; //设计值
		double* over_value; //超高
		int num_pull_value;//拉出值个数
	}pull_value;
	node_t* q_node;//节点数组
	int node_num;//节点个数（包括悬挂节点与非悬挂节点）
	struct {
		double cross_w_per_m;//横承每米自重
		double up_fasten_w_per_m;//上部固定绳每米自重
		double down_fasten_w_per_m;//下部固定绳每米自重
	}weight_pt;
    input_param()
    {
        dist.num_of_distance = 0;
        dist.distance = NULL;
        pull_value.real_pull_value = NULL;
        pull_value.over_value = NULL;
        pull_value.num_pull_value = 0;
        q_node = NULL;
    }
}input_t;

typedef struct output_st{
	int lowest_pt_idx;
	double f[2];
	double a[20];
	double Q[20];
	double F[2];
	double L[2];
	double M[2];
	double T;
	double Y;
	double k[20];
	double B[20];
}output_t;

typedef struct draw_param_st{
	int lowest_pt_idx;
	/*，直吊子=悬挂点个数
	  ,定位绳数量=悬挂点+2
	  ，绝对横承长度=悬挂点+2
	*/
	int is_get_draw_param;
	int hang_num;					  //悬挂点个数
	double stright_hang_values[20];   //直吊子
	double upper_position_ropes[20];  //图中的上平行线的各段定位绳
	double lower_position_ropes[20];  //下平行线的各段定位绳
	double B4draw[20]; //绝对横承长度
}draw_param_t;

class CSoftCrossCompute
{
public:
	CSoftCrossCompute(int hang_num);/*Need to get the number of hang point*/
	~CSoftCrossCompute(void);
	int GetDrawParm(draw_param_t* draw_p);
	int calc_soft_cross( input_t* input, output_t* output);
	void CalcDrawParam(input_t* input,output_t* output);
private:

	void CalcLowestDistance(input_t* input);//计算f1,f2
	void CalcRoadDistance(input_t* input);//计算a1---an
	void CalcQ(input_t* input);
	int CalcLowestPoint(void);//get the lowest point.
	
	double ArraySum(double* arr,int startpos, int n);

	int m_hang_num;
	unsigned int m_mask_num;//0x1 means get the a1...an,0x11 means get Q value
	int m_lowest_pt_idx;
	double *m_p_pull_value;
	double m_f[2];
	double* m_p_a;
	double* m_p_Q;
	double m_F[2];
	double m_L[2];
	double m_M[2];
	double m_T;
	double m_Y;
	double* m_p_k;
	double* m_p_B;
	double m_c_min;
	draw_param_t m_draw;
	double q_left_distance[20];
	double q_right_distance[20];
};

