#pragma once

/*---------------------------------------------------------------------*/
/**
*分割立面上的树和建筑
*
*树木，建筑-采用Manhattan方向线实现
*/
/*---------------------------------------------------------------------*/
class SplitTreeandBuilding
{
private:
	void AllocMem(void);
	void ReleaseMem(void);
	ImageMemData* pMD;/**<图像数据及计算的中间变量*/
public:
	SVGcount* svgCount;/**<每一个图块的分类*/
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

