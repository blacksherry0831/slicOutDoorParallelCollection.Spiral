#pragma once

/*---------------------------------------------------------------------*/
/**
*�ָ������ϵ����ͽ���
*
*��ľ������-����Manhattan������ʵ��
*/
/*---------------------------------------------------------------------*/
class SplitTreeandBuilding
{
private:
	void AllocMem(void);
	void ReleaseMem(void);
	ImageMemData* pMD;/**<ͼ�����ݼ�������м����*/
public:
	SVGcount* svgCount;/**<ÿһ��ͼ��ķ���*/
	SplitTreeandBuilding(ImageMemData* pMD_t);

	SplitTreeandBuilding(
		int* ImgLables_SVG,
		int* ImgLables,
		ImageMemData* pMD_t);

	~SplitTreeandBuilding(void);
public:

	void SlpitTreeBuilding(void);
	bool IsTree(int sp);
	bool IsBuilding(int sp);
	void ReBuildingImgLab(void);
	
	void CalculateSVGImgLables(void);
};

