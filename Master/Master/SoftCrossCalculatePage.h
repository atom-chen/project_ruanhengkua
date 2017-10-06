/**********************************************************************
* ���ߣ�hezhr
* ʱ�䣺2012-09-23
* �������������
**********************************************************************/

#ifndef _SOFT_CROSS_CALCULATE_
#define _SOFT_CROSS_CALCULATE_

#include "DB/SegInputDBManager.h"
#include "DB/SegParamDBManager.h"
#include "DB/SegInFormatter.h"
#include "DB/SegOutDBManager.h"
#include "DB/SegOutFormatter.h"
#include "SoftCrossCompute.h"
#include "UltimateGrid/GridCtrl.h"
#include <string>
#include <set>


class SoftCrossCalculatePage
{
public:
	SoftCrossCalculatePage();
	~SoftCrossCalculatePage();

public:
	/*
	��	�ܣ���ʼ����ҳ
	��	����grid - ���ӽ��棻segment_id - �ֶ�id��show - �Ƿ���ʾ
	����ֵ��void
	*/
	void init(GridCtrl* grid, DWORD segment_id = -1, BOOL show = TRUE);

	/*
	��	�ܣ���ʼ�༭
	��	����void
	����ֵ��void
	*/
	void startEdit(void);

	/*
	��	�ܣ������༭
	��	����grid - ���ӽ��棻param - ���Ӳ���
	����ֵ��void
	*/
	void endEdit(GridCtrl* grid, long param);
	/*
	��	�ܣ������ͼ���õ���ͼ����
	��	����pDrawParam - �õ��Ļ�ͼ����
	����ֵ��TRUE - ��ʾ��óɹ���FALSE - ʧ��	
	*/
	BOOL calcAndGetDrawParam(draw_param_st* pDrawParam);

	static bool firstShowMessageBox;

private:
	/*
	��	�ܣ��������ҳ
	��	����void
	����ֵ��void
	*/
	void clear(void);

	/*
	��	�ܣ���ʼ�����
	*/
	void initMaterialIdSet(const SoftCrossSegParam& segment_parameter);

	/*
	��	�ܣ���ʼ�������������
	*/
	void initMaterialGrid(GridCtrl* grid);

	/*
	��	�ܣ���ʼ���ڵ��������
	*/
	void initNodeGrid(GridCtrl* grid);

	/*
	��	�ܣ���ʼ�������������
	*/
	void initInputGrid(GridCtrl* grid);

	/*
	��	�ܣ���ʼ������Ĳ���
	*/
	bool initCalculateParameter(void);

	/*
	��	�ܣ����㵥Ԫ������������ݲ���
	*/
	bool initCalculateInput(void);

	/*
	��	�ܣ���֤�Ƿ�������Ҫ����ĵ�Ԫ��������
	*/
	bool checkInputFinish();

	/*
	��	�ܣ�������
	*/
	void saveResult(void);

	/*
	��	�ܣ�������ʾ���
	*/
	void updateResult(GridCtrl* grid);

	/*
	��	�ܣ�����֧���༭
	*/
	void endEditBrace(GridCtrl* grid, long param);

	/*
	��	�ܣ������ɵ����༭
	*/
	void endEditStationTrackDistance(GridCtrl* grid, long param);

	/*
	��	�ܣ������ɵ����ֵ�༭
	*/
	int endEditStationTrackDesign(GridCtrl* grid, long param);

	/*
	��	�ܣ������ɵ�����ֵ�༭
	*/
	int endEditStationOverstep(GridCtrl* grid, long param);

	/*
	��	�ܣ�����ɵ�ʵ������ֵ
	*/
	void endEditStationReality(GridCtrl* grid, size_t index);

	/*
	��	�ܣ������༭
	*/
	bool calculateEndEdit(GridCtrl* grid);
	

	void showMessageBox(void);

private:
	CSoftCrossCompute *mCalculate;				// ������
	bool mIsParameterInit;						// ���õĲ����Ƿ��Ѿ�����ʼ��
	bool mIsCalcFinish;							// �Ƿ��Ѿ��õ�������
	DWORD mSegmentId;							// �ֶ�id
	DWORD mEditSegmentId;						// ��ǰ�༭�ķֶ�id,��ֹ������ʱֱ���л��ֶε��µ���ֵ���浽���л��ķֶ���
	std::string mSegmentTitle;					// �ֶα���
	SoftCrossSegParam mSegmentParameter;		// �ֶβ���
	SoftCrossSegInput mSegmentInput;			// �ֶ�����
	SoftCrossSegOut mSegmentOut;				// �ֶ����
	std::set<DWORD> mMaterialIdSet;				// ����id����
	input_t mInput;								// ����
	output_t mOutput;							// ���
};


#endif	// _SOFT_CROSS_CALCULATE_


