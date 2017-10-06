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
#define DEFLECTION	0.09	//�Ŷ�
#define DELTA_UP_DOWN	1.2		//�ϲ��̶������²��̶����Ĺ̶���ֵ
const double EPSILON = 1.00e-07;
#define IS_ZERO(x)  ((x<=EPSILON && x>= -EPSILON))




/*the metric is m*/
typedef struct pillar_param{

	int pillar_type;//1 ��ʾ����������2��ʾ����
	double pillar_h;//the height of pillar;
	double limit_range;//�޽�
	double delta_of_surface; //������������߲� ��ʽ�е�S��
	double slope; //б��
	double width_top;//֧���������
	double pillar_bottom;//֧���µ�
	double pillar_top;//֧���ϵ�
//	double component_size[5];//���������Ҫ���ϲ��̶����ϵ�����ߴ磬���ܳ������
}pillar_t;
typedef struct node_st{
	double weight;//�ڵ�����
	int is_hang_node;//�Ƿ�Ϊ���ҽڵ�
	double dist_to_front_node;//��ǰһ�ڵ���
	double dist_to_rear_node;//����һ�ڵ���
	double catenary_w_per_m;//������ÿ������
	double touch_line_w_per_m;//ÿ�׽Ӵ�������
	double horizon_catenary_length;//�ڵ��г��ȣ���ͼ��
	double upper_rope_length;//�ϲ��̶�������
	double lower_rope_length;//�²��̶�������
	double left_no_hang_dist;//�������ҽڵ㵽���ڵ���루����õ���
	double right_no_hang_dist;//�Ҳ�����ҽڵ㵽���ڵ���루����õ���
	//add by czh @ 2013.3.26
	double distance2pillar; //�ڵ㵽֧���ľ��룬Ϊ0��ʾ���ڵ㲻�Ǳ߽�ڵ�
	node_st(){
		distance2pillar = 0;
	}
	//end 
}node_t;

typedef struct input_param{
	double designed_stright_hang_size; //�û��������͵��ֱ���ӳ���
	pillar_t pillar_left;
	pillar_t pillar_right;//left pillar and right pillar
	struct{
		double ear_size;//�����������ߴ�
		double spring_size;//���ɲ������ߴ�
		double ear_ring_stick_size; //�����˳ߴ�
		double head_stick_size;//��ͷ��
	}component_size_t;
	struct{//�ɵ����
		double* distance; 
		int num_of_distance;
	}dist;
	struct{//real_pull_value = design_value - 4*over_value
		double* real_pull_value; //ʵ������ֵ(����õ����û���������)
		double design_value; //���ֵ
		double* over_value; //����
		int num_pull_value;//����ֵ����
	}pull_value;
	node_t* q_node;//�ڵ�����
	int node_num;//�ڵ�������������ҽڵ�������ҽڵ㣩
	struct {
		double cross_w_per_m;//���ÿ������
		double up_fasten_w_per_m;//�ϲ��̶���ÿ������
		double down_fasten_w_per_m;//�²��̶���ÿ������
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
	/*��ֱ����=���ҵ����
	  ,��λ������=���ҵ�+2
	  �����Ժ�г���=���ҵ�+2
	*/
	int is_get_draw_param;
	int hang_num;					  //���ҵ����
	double stright_hang_values[20];   //ֱ����
	double upper_position_ropes[20];  //ͼ�е���ƽ���ߵĸ��ζ�λ��
	double lower_position_ropes[20];  //��ƽ���ߵĸ��ζ�λ��
	double B4draw[20]; //���Ժ�г���
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

	void CalcLowestDistance(input_t* input);//����f1,f2
	void CalcRoadDistance(input_t* input);//����a1---an
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

