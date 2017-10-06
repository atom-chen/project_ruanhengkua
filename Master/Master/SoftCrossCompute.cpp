#include "StdAfx.h"
#include "SoftCrossCompute.h"
#include <math.h>

CSoftCrossCompute::CSoftCrossCompute(int hang_num)
:m_p_Q(NULL)
,m_p_a(NULL)
,m_p_k(NULL)
,m_p_B(NULL)
,m_p_pull_value(NULL)

{
	m_hang_num = hang_num;
	m_mask_num = 0;
	m_lowest_pt_idx = 0;
	//if(hang_num > 19){
	//	printf("hang number must be less than 19!\n");
	//	return ;
	//}
	if(m_hang_num <= 4)
		m_c_min = 0.4;
	else if(m_hang_num > 4 && m_hang_num <=6)
		m_c_min = 0.6;
	else if(m_hang_num > 6 && m_hang_num <= 8)
		m_c_min = 0.8;
	int i = 0;
	m_p_a = (double*)new double[hang_num+1];
	if(!m_p_a){
		printf("malloc error!\n");
		return;
	}
	m_p_Q = new double[hang_num];

	if(!m_p_Q ){
		printf("malloc error!\n");
		delete []m_p_a;
		return;
	}
	m_p_k = new double[hang_num+1];
	if(!m_p_k){
		printf("malloc error!\n");
		delete []m_p_a;
		delete []m_p_Q;
		return;
	}
	m_p_B = new double[hang_num+1];
	if(!m_p_B){
		printf("malloc error!\n");
		delete []m_p_a;
		delete []m_p_Q;
		delete []m_p_k;
		return;
	}
	m_p_pull_value = new double[hang_num];
	if(!m_p_pull_value){
		printf("malloc error!\n");
		delete []m_p_a;
		delete []m_p_Q;
		delete []m_p_k;
		delete []m_p_B;
		return;
	}
	m_draw.lowest_pt_idx = 0;
	for(i = 0; i < 20 ; i++){
		m_draw.B4draw[i] = m_draw.lower_position_ropes[i] = m_draw.stright_hang_values[i] = m_draw.upper_position_ropes[i] = 0;	
	}
	for(i = 0; i< hang_num; i++){
		m_p_a[i] = m_p_Q[i] = m_p_k[i] = m_p_B[i] = 0;
	}
		m_p_a[hang_num] = m_p_k[hang_num] = 0;
		m_T = m_Y = 0;
		m_f[0] = m_f[1] = m_F[0] = m_F[1] = 0;
		m_M[0] = m_M[1] = 0;
		m_draw.is_get_draw_param = 0;
		m_draw.hang_num = m_hang_num;
}

CSoftCrossCompute::~CSoftCrossCompute(void)
{
	if(m_p_a)
		delete []m_p_a;
	if(m_p_Q)
		delete []m_p_Q;
	if(m_p_k)
		delete []m_p_k;
	if(m_p_pull_value)
		delete []m_p_pull_value;
	if(m_p_B)
		delete []m_p_B;
}
void format_input_value(input_t* input)
{//ͳһ��λ�����������
	int i = 0;
	if(input->component_size_t.ear_ring_stick_size > MAX_COMP_SIZE)
		input->component_size_t.ear_ring_stick_size /= 1000;
	if(input->component_size_t.ear_size > MAX_COMP_SIZE)
		input->component_size_t.ear_size /=1000;
	if(input->component_size_t.head_stick_size > MAX_COMP_SIZE)
		input->component_size_t.head_stick_size /= 1000;
	if(input->component_size_t.spring_size > MAX_COMP_SIZE)
		input->component_size_t.spring_size /= 1000;
	for(i = 0; i< input->dist.num_of_distance; i++){
		if(input->dist.distance[i] > MAX_TRACK_DIST)
			input->dist.distance[i] /= 1000;
	}
	if(input->pillar_left.delta_of_surface > MAX_PILLAR_PARAM_VAL)
		input->pillar_left.delta_of_surface /=1000;
	if(input->pillar_left.limit_range > MAX_PILLAR_PARAM_VAL)
		input->pillar_left.delta_of_surface /= 1000;
	if(input->pillar_left.pillar_bottom > MAX_PILLAR_PARAM_VAL)
		input->pillar_left.pillar_bottom /= 1000;
	if(input->pillar_left.pillar_h > MAX_PILLAR_PARAM_VAL)
		input->pillar_left.pillar_h /= 1000;
	if(input->pillar_left.pillar_top > MAX_PILLAR_PARAM_VAL)
		input->pillar_left.pillar_top /= 1000;
	if(input->pillar_left.width_top > MAX_PILLAR_PARAM_VAL)
		input->pillar_left.width_top /= 1000;

	if(input->pillar_right.delta_of_surface > MAX_PILLAR_PARAM_VAL)
		input->pillar_right.delta_of_surface /=1000;
	if(input->pillar_right.limit_range > MAX_PILLAR_PARAM_VAL)
		input->pillar_right.delta_of_surface /= 1000;
	if(input->pillar_right.pillar_bottom > MAX_PILLAR_PARAM_VAL)
		input->pillar_right.pillar_bottom /= 1000;
	if(input->pillar_right.pillar_h > MAX_PILLAR_PARAM_VAL)
		input->pillar_right.pillar_h /= 1000;
	if(input->pillar_right.pillar_top > MAX_PILLAR_PARAM_VAL)
		input->pillar_right.pillar_top /= 1000;
	if(input->pillar_right.width_top > MAX_PILLAR_PARAM_VAL)
		input->pillar_right.width_top /= 1000;

	for(i = 0; i <  input->pull_value.num_pull_value; i++){
	
		if(input->pull_value.over_value[i] > 2)
			input->pull_value.over_value[i] /= 1000;
	}
}

void CSoftCrossCompute::CalcDrawParam( input_t* input,output_t* output)
{
	int i = 0;
	int j = 0;
	double designed_stright_size = input->designed_stright_hang_size;
	if(designed_stright_size > 100)
		designed_stright_size /= 1000;
	m_draw.lowest_pt_idx = output->lowest_pt_idx;
	m_draw.stright_hang_values[output->lowest_pt_idx] = designed_stright_size;
/*==============================����ֱ���ӳ���========================================*/
	for(i = output->lowest_pt_idx - 1; i>=0 ; i--){
		m_draw.stright_hang_values[i] = m_draw.stright_hang_values[i+1] + output->k[i+1];
	}
	for(i=output->lowest_pt_idx+1; i< m_hang_num; i++){
		m_draw.stright_hang_values[i] = m_draw.stright_hang_values[i-1] + output->k[i];
	}
/*============================�����ϲ���λ������===================================*/
	m_draw.upper_position_ropes[0] = input->pillar_left.limit_range - HS_VALUE * input->pillar_left.slope - input->q_node[0].upper_rope_length 
		- DEFLECTION - input->pull_value.real_pull_value[0];
	for(i = 1 ; i < output->lowest_pt_idx; i++)
		m_draw.upper_position_ropes[i] = output->a[i];
	m_draw.upper_position_ropes[output->lowest_pt_idx] = input->dist.distance[output->lowest_pt_idx]/2 
		+ input->pull_value.real_pull_value[output->lowest_pt_idx] - (input->q_node[output->lowest_pt_idx + 1].upper_rope_length)/2;
	m_draw.upper_position_ropes[output->lowest_pt_idx+1] = input->dist.distance[output->lowest_pt_idx]/2 
		- input->pull_value.real_pull_value[output->lowest_pt_idx + 1] - (input->q_node[output->lowest_pt_idx + 1].upper_rope_length)/2;
	for(i = output->lowest_pt_idx + 2 ; i <= m_hang_num ; i++)
		m_draw.upper_position_ropes[i] = output->a[i];

	m_draw.upper_position_ropes[m_hang_num+1] = input->pillar_right.limit_range - HS_VALUE * input->pillar_right.slope 
		- input->q_node[input->node_num - 1].upper_rope_length + input->pull_value.real_pull_value[m_hang_num - 1];
/*======================================================================================================================
	//�����²���λ������,�²���λ�� = �ϲ���λ������+/-��λ������,���ڵ������ֵ����һ�ڵ�����ֵͬ������ͬ*/
	double fasten_com = DELTA_UP_DOWN;
	if(input->pull_value.real_pull_value[0] < 0 )
		fasten_com = DELTA_UP_DOWN;
	else
		fasten_com = -DELTA_UP_DOWN;
	m_draw.lower_position_ropes[0] = m_draw.upper_position_ropes[0] + fasten_com;
	for(i = 1,j=1; i< m_hang_num; i++)
	{
		if(i == output->lowest_pt_idx){
			j+=2;
			continue;
		}
		//������ߵ�����ֵ��ͬһ������Ļ������ϲ���λ������
		if(input->pull_value.real_pull_value[i-1] * input->pull_value.real_pull_value[i] > 0){
			m_draw.lower_position_ropes[j] = m_draw.upper_position_ropes[j];
		}
		//���һ�߷���һ����
		if(input->pull_value.real_pull_value[i-1] < 0 && input->pull_value.real_pull_value[i] > 0)
		{
			m_draw.lower_position_ropes[j] = m_draw.upper_position_ropes[j] - DELTA_UP_DOWN;
		}
		if(input->pull_value.real_pull_value[i-1] > 0 && input->pull_value.real_pull_value[i] < 0)
		{
			m_draw.lower_position_ropes[j] = m_draw.upper_position_ropes[j] + DELTA_UP_DOWN;
		}
		j++;
		if(i == m_hang_num - 1){
			if(input->pull_value.real_pull_value[i] < 0 )
				m_draw.lower_position_ropes[j] = m_draw.upper_position_ropes[j] - DELTA_UP_DOWN;
			else
				m_draw.lower_position_ropes[j] = m_draw.upper_position_ropes[j] + DELTA_UP_DOWN;
		}
	}
	/*��͵㵥������*/
	i = output->lowest_pt_idx;
	if(input->pull_value.real_pull_value[i - 1] * input->pull_value.real_pull_value[i] < 0 )
	{
		m_draw.lower_position_ropes[i] = m_draw.upper_position_ropes[i] - DELTA_UP_DOWN;
		m_draw.lower_position_ropes[i+1] = m_draw.upper_position_ropes[i+1] - DELTA_UP_DOWN;
	}else{
		m_draw.lower_position_ropes[i] = m_draw.upper_position_ropes[i];
		m_draw.lower_position_ropes[i+1] = m_draw.upper_position_ropes[i+1];
	}
/*======================================================================================*/
	/*������Ժ�г���,���� =  ���ҵ���� + 2*/
	for(i = 0,j=0; i <= m_hang_num; i++)
	{
		if(i == 0){
			m_draw.B4draw[j] = output->B[i] - input->q_node[i].horizon_catenary_length;
		}else if(i == output->lowest_pt_idx){
			m_draw.B4draw[j] = (output->B[i] - input->q_node[i+1].horizon_catenary_length)/2;
			m_draw.B4draw[j+1] = m_draw.B4draw[j];
			j++;
		}else if(i == m_hang_num){
			m_draw.B4draw[j] = output->B[i] - input->q_node[input->node_num - 1].horizon_catenary_length;
		}else{
			m_draw.B4draw[j] = output->B[i];
		}
		j++;
	}
}
int CSoftCrossCompute::GetDrawParm(draw_param_st* draw_param)
{
	memcpy(draw_param,&m_draw,sizeof(draw_param_st));
	draw_param->is_get_draw_param = 1;
	return true;
}
int CSoftCrossCompute::calc_soft_cross(input_t* input,output_t* output)
{
	input->pull_value.real_pull_value = m_p_pull_value;
	int i = 0;
	format_input_value(input);
	for(i = 0; i< input->pull_value.num_pull_value; i++)
		m_p_pull_value[i] = input->pull_value.design_value - 4*(input->pull_value.over_value[i]);
	CalcRoadDistance(input);
	CalcLowestDistance(input);
	CalcQ(input);
	CalcLowestPoint();
	memcpy(output->a,m_p_a,sizeof(double)*(m_hang_num+1));
	memcpy(output->Q,m_p_Q,sizeof(double)*m_hang_num);
	memcpy(output->k,m_p_k,sizeof(double)*(m_hang_num+1));
	memcpy(output->B,m_p_B,sizeof(double)*(m_hang_num+1));
	memcpy(output->F,m_F,sizeof(double)*2);
	memcpy(output->f,m_f,sizeof(double)*2);
	memcpy(output->L,m_L,sizeof(double)*2);
	memcpy(output->M,m_M,sizeof(double)*2);
	output->T = m_T;
	output->Y = m_Y;
	output->lowest_pt_idx = m_lowest_pt_idx;
	return 0;
}

void CSoftCrossCompute::CalcRoadDistance(input_t* input)
{

	pillar_t* pillar_left = &(input->pillar_left);
	pillar_t* pillar_right = &(input->pillar_right);
	if(!input->dist.distance)
		return ;
	int idx = 0;
	if(pillar_left->slope > 1)
		pillar_left->slope = pillar_left->slope/1000;
	if(pillar_right->slope > 1)
		pillar_right->slope = pillar_right->slope/1000;
	
	pillar_t* pillar;
	double pull_value;
	for(idx = 0; idx<= m_hang_num; idx++){
		if(idx == 0 || idx == m_hang_num){//����a1��an
			if(idx == 0){
				pillar = pillar_left;
				pull_value = input->pull_value.real_pull_value[idx];
			}
			else{
				pillar = pillar_right;
				pull_value = - input->pull_value.real_pull_value[idx - 1];
			}
			/**
			֧�����ضԵ�1���绯�ɵ����޽磨���洦��-б��*��֧���ܳ�+���׸߲-֧��������ȣ���ֱ��·�ࣩ
			  -70mm*��֧���µ�-֧���ϵ�-֧���ܳ�*б�ʣ�/֧���ܳ� - ����ֵ
			*/
			m_p_a[idx] = pillar->limit_range - pillar->slope*(pillar->pillar_h+pillar->delta_of_surface) - 
			pillar->width_top - 0.07 * (pillar->pillar_bottom - pillar->pillar_top - pillar->pillar_h*pillar->slope)/pillar->pillar_h -
			pull_value - 0.08;
		}else{
			/*��idx���绯�ɵ�����idx+1���绯�ɵ�����+��idx�����ҵ�����ֵ-��idx+1�����ҵ�����ֵ*/
			m_p_a[idx] = input->dist.distance[idx] + input->pull_value.real_pull_value[idx-1] - input->pull_value.real_pull_value[idx];
		}
	}
}
/*���� f1,f2  */
void CSoftCrossCompute::CalcLowestDistance(input_t* input)
{
	m_c_min = 0.4;
	pillar_t* pillar_left = &(input->pillar_left);
	pillar_t* pillar_right = &(input->pillar_right);
	/*if(pillar_left->pillar_type == STEEL_PILLAR){
	*/	
	m_f[0] = pillar_left->pillar_h+ pillar_left->delta_of_surface - m_c_min - HS_VALUE - 0.07;
	m_f[1] = pillar_right->pillar_h+ pillar_right->delta_of_surface - m_c_min - HS_VALUE - 0.07;
	/*}
	if(pillar_left->pillar_type == CONCRETE_PILLAR){
		m_f[0] = 10.8 - HS_VALUE - m_c_min + pillar_left->delta_of_surface;
		m_f[1] = 10.8 - HS_VALUE - m_c_min + pillar_right->delta_of_surface;
	}*/
}

void calc_node_dist(input_t* input)
{
	node_t* node = input->q_node;
	double comp = 0;
	int i = 0; 
	int j = 0;
	int num = input->node_num;
	for(i = 0; i < num; i++)
	{
		node[i].left_no_hang_dist = 0;
		node[i].right_no_hang_dist = 0;
		if(node[i].is_hang_node){//���ҽڵ�

			if(i == 0){
				printf("error!first node must be a pillar\n");
				break;
			}
			//modify by czh @2013.3.26
			if(i == 1){//������ҽڵ���֧��
				comp = node[i].distance2pillar + 0.08 + 7.56* input->pillar_left.slope;
				comp = input->dist.distance[j]/2 - comp;
			}
			if(i == num-2){
				comp = node[i].distance2pillar + 0.08 + 7.56* input->pillar_right.slope;
				comp = input->dist.distance[j+1]/2 + 0.4 - comp;
			}
			//end of modify
			if(!node[i-1].is_hang_node){//ǰһ���ڵ��Ƿ����ҽڵ�
				if(i == num - 1)
					comp = 0;
				node[i].left_no_hang_dist = input->dist.distance[j]/2 - input->pull_value.real_pull_value[j] + comp;
			}
			if(!node[i+1].is_hang_node){//��һ�ڵ��Ƿ����ҽڵ�
				if(i == 1)
					comp = 0;
				node[i].right_no_hang_dist = input->dist.distance[j+1]/2 + input->pull_value.real_pull_value[j] + comp;
			}
		
			j++;
			comp = 0;
		}
		//�����Ƿ����ҽڵ�
	}//end of for
}
/*from here 20120410*/
void CSoftCrossCompute::CalcQ(input_t* input)
{


	int i = 0;
	int j = 0;
	double left_not_hang_w = 0;
	double right_not_hang_w = 0;
	node_t*	node = input->q_node;
	
	calc_node_dist(input);
	/*
	q1= 
    ����ڵ�����+��a1+a2��/2*��ÿ�׺������+ÿ���ϲ��̶�������+ÿ���²��̶������أ�+
   ����ǰһ���ҵ���+����һ���ҵ��ࣩ/2*��������ÿ������+�Ӵ���ÿ�����أ�+
    �������ҽڵ�����*��a1-�������ҽڵ�����λ�������ڵ�ˮƽ���룩/a1+
	�Ҳ�����ҽڵ�����*��a2-�Ҳ�����ҽڵ�����λ�������ڵ�ˮƽ���룩/a2
	*/
	for(i = 0; i< input->node_num; i++)
	{
		if(!node[i].is_hang_node)//�����ҽڵ㣬û��Qֵ
			continue;
		left_not_hang_w = 0;
		right_not_hang_w = 0;		
		
		if( i != 0 && !node[i-1].is_hang_node){//��������ҽڵ�
			left_not_hang_w = node[i-1].weight;
		}
		if(i!= input->node_num - 1 && !node[i+1].is_hang_node){//�ұ������ҽڵ�
			right_not_hang_w = node[i+1].weight;
		}
		//����Qֵ
		m_p_Q[j] = node[i].weight + ((m_p_a[j] + m_p_a[j+1])/2)*(input->weight_pt.cross_w_per_m + input->weight_pt.up_fasten_w_per_m + 
			input->weight_pt.down_fasten_w_per_m) +
			(node[i].dist_to_front_node + node[i].dist_to_rear_node)/2 *(node[i].catenary_w_per_m + node[i].touch_line_w_per_m) +
			left_not_hang_w * (m_p_a[j] - node[i].left_no_hang_dist)/m_p_a[j] + right_not_hang_w * (m_p_a[j+1] - node[i].right_no_hang_dist)/m_p_a[j+1];
		j++;
	}
	m_mask_num |= 0x2;//�����Q�ļ��㣬Q��Ҫ�õ�ax��ֵ��

}
double CSoftCrossCompute::ArraySum(double* arr,int startpos,int n)
{
	int i = 0;
	double res = 0;
	for(i = startpos; i< startpos+n; i++)
		res += arr[i];
	return res;
}
int CSoftCrossCompute::CalcLowestPoint(void)
{

	if(!(m_mask_num&0x3))
	{
		printf("should be calculate Q and a value first!\n");
		return -1;
	}
	int i = 0;
	double left = 0;
	double total = ArraySum(m_p_Q,0,m_hang_num);
	double a_sum = 0;
	double q_sum = 0;
    //����Ϊ0
    m_F[0] = m_F[1] = 0;
    m_M[0] = 0;
    m_M[1] = 0;

	/*
	F2 = (Q1*a1+Q2*(a1+a2)+Q3*(a1+a2+a3)+Q4*(a1+a2+a3+a4)+...+Qn*(a1+a2+a3+a4+...+an))/(a1+a2+a3+a4+...+a(n+1))*10
	F1 = (Q1+...+Qn)*10 - F2
	*/
	for(i = 0; i<m_hang_num; i++)
		m_F[1] += m_p_Q[i]*(ArraySum(m_p_a,0,i+1));
	m_F[1] = (m_F[1]/(ArraySum(m_p_a,0,m_hang_num+1)))*10;
	m_F[0] = ArraySum(m_p_Q,0,m_hang_num)*10 - m_F[1];
	double F_tmp = m_F[0]/10;
	for(i = 0; i< m_hang_num - 2; i++){//mark here!1!!!!
		left = ArraySum(m_p_Q,0,i+1);
		if( F_tmp  > total - left ){
			m_lowest_pt_idx = i+1;
			break;
		}
	}//end of for
	i = 0;
	/*the lowest point is m_lowest_pt_idx*/
	if(m_lowest_pt_idx >= 0){
		/*
			L1 = a1+...+ax;
			L2 = ax+1 +... +an
		*/
		m_L[0] = ArraySum(m_p_a,0,m_lowest_pt_idx+1);
		m_L[1] = ArraySum(m_p_a,0,m_hang_num+1) - m_L[0];
		/*
			M1 = (Q1*a1+...+Qx*(a1+...ax-1))*10
			M2 = (Qx*(ax+1+...+an)+...+Qn-1*an)*10
		*/

		for(i = 0; i< m_lowest_pt_idx; i++){
			m_M[0] += m_p_Q[i]*ArraySum(m_p_a,0,i+1);
		}
		m_M[0] *= 10;
	
		for(i = m_hang_num -1; i >= m_lowest_pt_idx; i--)
		{
			a_sum = ArraySum(m_p_a,i+1,m_hang_num - i);
			m_M[1] += m_p_Q[i] * a_sum;
		}
		m_M[1] *= 10;
		/*T = (L1*M2+M1*L2)/(L1*f2+L2*f1)*/
		m_T = (m_L[0]*m_M[1] + m_L[1] * m_M[0])/(m_L[0]*m_f[1] + m_L[1]*m_f[0]);
		/*Y = (f1*M2-f2*M1)/(L1*f2+L2*f1)*/
		m_Y = (m_f[0]*m_M[1] - m_f[1]*m_M[0])/(m_L[0]*m_f[1]+m_L[1]*m_f[0]);
		
		for(i = 0; i<=m_hang_num; i++){
			if(i < m_lowest_pt_idx){
				q_sum = ArraySum(m_p_Q,i,m_lowest_pt_idx - i);
				m_p_k[i] = m_p_a[i]*(q_sum*10+m_Y)/m_T;
			}
			if(i == m_lowest_pt_idx){
				m_p_k[i] = m_p_a[i]*m_Y/m_T;
			}
			if(i > m_lowest_pt_idx){
				m_p_k[i] = m_p_a[i] * (ArraySum(m_p_Q,m_lowest_pt_idx,i-m_lowest_pt_idx)*10 - m_Y)/m_T;
			}
			m_p_B[i] = sqrt(m_p_a[i]*m_p_a[i] + m_p_k[i]*m_p_k[i]);
		}//end of for
	}else{
		printf("get lowest point error!\n");
	}//end of if
    return 0;
}