#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SpSetProperty.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
SpSetProperty::SpSetProperty(void)
{
	this->cleardata();
	ImgWidth=0;
	ImgHeight=0;
	HorizontalLinePos=0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
SpSetProperty::~SpSetProperty()
{
	
}
/*----------------------------------------------------------------*/
/**
*���������ͷŷ�����ڴ�
*
*
*/
/*----------------------------------------------------------------*/
void SpSetProperty::Parse(int*ImgLabels,int slicNum,int width,int height,int HorizontalLinePos)
{
	TRACE_FUNC();
	this->ImgHeight=height;
	this->ImgWidth=width;
	this->HorizontalLinePos=HorizontalLinePos;
#if _MSC_VER &&_DEBUG
    LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#endif
	{
		    this->SpPropertySet.clear();
		    this->PointSet.clear();
			this->SpPropertySet.resize(slicNum);
			this->PointSet.resize(slicNum);
			SpPoint point;
			for(register int x=0;x<width;x++){
				for(register int y=0;y<height;y++){
					int spi=ImgLabels[y*width+x];
					ASSERT(spi>=0&&spi<=slicNum);
					this->PointSet[spi].SpIndex=spi;
							point.x=x;
							point.y=y;
					this->PointSet[spi].pointSet.push_back(point);
					ParsespBorderCategory(spi,x,y);
				}
			}
			for (register int spi=0;spi<slicNum;spi++){
				this->SpPropertySet[spi].SpIndex=this->PointSet[spi].SpIndex;
				this->SpPropertySet[spi].pointSetIdx.push_back(spi);
				this->SpPropertySet[spi].SpSize=this->PointSet[spi].pointSet.size();
			}
			
	}
#if _MSC_VER &&_DEBUG
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ͳ�Ƶ㼯ʱ��: %f���룩",dfTim);
	/*************************************************************/
#endif
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SpSetProperty::Combine2in1(int spi,int spj,bool CombineHarr)
{
#if 1
	int spMin=std::min(spi,spj);
	int spMax=std::max(spi,spj);
	int DesSp=this->SpPropertySet[spMin].SpIndex;
	this->SpPropertySet[spMax].SpIndex=DesSp;
	ASSERT(spi!=spj);	
	/*-------------------------------*/
	this->CombineBorderProperty(DesSp,spMax);
	this->combinePointSet(DesSp,spMax);
	this->CombineHarrProperty(DesSp,spMax,CombineHarr);
	/*-------------------------------*/
#endif
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
inline void SpSetProperty::combinePointSet(int DesSp,int SrcSp)
{
		 SpPropertySet[DesSp].pointSetIdx.insert(
		 SpPropertySet[DesSp].pointSetIdx.end(),
		 SpPropertySet[SrcSp].pointSetIdx.begin(),
	     SpPropertySet[SrcSp].pointSetIdx.end());
		 this->SpPropertySet[SrcSp].pointSetIdx.clear();
		 SpPropertySet[DesSp].SpSize+=SpPropertySet[SrcSp].SpSize;
		 SpPropertySet[SrcSp].SpSize=0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
inline void SpSetProperty::CombineBorderProperty(int DesSp,int SrcSp)
{
	 SpPropertySet[DesSp].BorderCategory|=SpPropertySet[SrcSp].BorderCategory;
	 SpPropertySet[SrcSp].BorderCategory=0;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
inline void SpSetProperty::CombineHarrProperty(int DesSp,int SrcSp,bool CombineHarr)
{
	if (CombineHarr==true){
		ASSERT(SpPropertySet[SrcSp].harr_energy.EnergyGrade==SpPropertySet[DesSp].harr_energy.EnergyGrade);
	}
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SpSetProperty::RemoveEmptySet()
{
#if 1
for(vector<SpProperty>::iterator it = SpPropertySet.begin(); it != SpPropertySet.end();){
		if( it->pointSetIdx.size()==0){
			it = SpPropertySet.erase( it);
		}
		else
			it++;
	}
#endif
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SpSetProperty::AdjustSpSet(void)
{
#if 1
	this->RemoveEmptySet();
	int i=0;
	for(vector<SpProperty>::iterator it = SpPropertySet.begin(); it != SpPropertySet.end();it++){
		it->SpIndex=i++;
	}

#endif	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
inline void SpSetProperty::ParsespBorderCategory(int spi,int width_x,int height_y)
{
	if (width_x==0){
		SpPropertySet[spi].BorderCategory|=spBorderCategoryLeft;
	}
	if (width_x==ImgWidth-1){
		SpPropertySet[spi].BorderCategory|=spBorderCategoryRight;
	}

	if (height_y==0){
		SpPropertySet[spi].BorderCategory|=spBorderCategoryTop;
//		SpPropertySet[spi].Category_Force|=SKY_Force;
	}
	
	if (height_y==ImgHeight-1){
		SpPropertySet[spi].BorderCategory|=spBorderCategoryBottom;
	//	SpPropertySet[spi].Category_Force|=GROUND_Force;
	}
	
	if (height_y==HorizontalLinePos){
		SpPropertySet[spi].BorderCategory|=spBorderCategoryCrossHorz;
	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SpSetProperty::cleardata()
{
	{
		int old_size=this->SpPropertySet.size();
		this->SpPropertySet.resize(0);
		this->SpPropertySet.resize(old_size);
	}	
	{
		int old_size=this->PointSet.size();
		this->PointSet.resize(0);
		this->PointSet.resize(old_size);
	}	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

