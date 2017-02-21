#include "StdAfx.h"
#include "module_all_cui.h"
//#include "ManhattanHistogram.h"
/*-------------------------------------------------------------*/
/**
*由HistData构造Manhattan直方图
*@param  直方图数据 
*@param  直方图维度 
*
*/
/*-------------------------------------------------------------*/
ManhattanHistogram::ManhattanHistogram(HistData histdata,unsigned int Dim)
{
	this->HistDim=Dim;
	this->AllocaeMemory(Dim);
	memcpy(this->hist_all.data(),histdata.hist_all,sizeof(float)*Dim);
	memcpy(this->hist_sky.data(),histdata.hist_sky,sizeof(float)*Dim);
	memcpy(this->hist_ground.data(),histdata.hist_ground,sizeof(float)*Dim);
	memcpy(this->hist_vertical.data(),histdata.hist_vertical,sizeof(float)*Dim);
	memcpy(this->hist_Multiply.data(),histdata.hist_Multiply,sizeof(float)*Dim);
	memcpy(this->hist_all_count.data(),histdata.hist_all_count,sizeof(float)*Dim);
	
}
/*-------------------------------------------------------------*/
/**
*由HistData180构造Manhattan直方图
*@param  直方图数据 
*@param  直方图维度 
*
*/
/*-------------------------------------------------------------*/
ManhattanHistogram::ManhattanHistogram(HistData180 histdata,unsigned int Dim)
{
	this->HistDim=Dim;
	this->AllocaeMemory(Dim);
	memcpy(this->hist_all.data(),histdata.hist_all,sizeof(float)*Dim);
	memcpy(this->hist_sky.data(),histdata.hist_sky,sizeof(float)*Dim);
	memcpy(this->hist_ground.data(),histdata.hist_ground,sizeof(float)*Dim);
	memcpy(this->hist_vertical.data(),histdata.hist_vertical,sizeof(float)*Dim);
	memcpy(this->hist_Multiply.data(),histdata.hist_Multiply,sizeof(float)*Dim);
	memcpy(this->hist_all_count.data(),histdata.hist_all_count,sizeof(float)*Dim);
	memcpy(&this->histdata180,&histdata,sizeof(HistData180));
}
/*-------------------------------------------------------------*/
/**
*构造函数   
*@param  曼哈顿直方维度 
*/
/*-------------------------------------------------------------*/
ManhattanHistogram::ManhattanHistogram(unsigned int Dim)
{
	this->HistDim=Dim;
	this->AllocaeMemory(Dim);
}
/*-------------------------------------------------------------*/
/**
*析构
*
*/
/*-------------------------------------------------------------*/

ManhattanHistogram::~ManhattanHistogram(void)
{
	this->AllocaeMemory(0);
}
/*-------------------------------------------------------------*/
/**
*依据直方图长度为Manhattan直方图分配内存
*
*/
/*-------------------------------------------------------------*/
void ManhattanHistogram::AllocaeMemory(unsigned int Dim)
{
	this->HistDim=Dim;
	this->hist_all.resize(Dim);
	//this->hist_all.erase(this->hist_all.begin(),this->hist_all.end());
	memset(this->hist_all.data(),0,sizeof(float)*Dim);

	this->hist_sky.resize(Dim);
	//this->hist_sky.erase(this->hist_sky.begin(),this->hist_sky.end());
	memset(this->hist_sky.data(),0,sizeof(float)*Dim);

	this->hist_ground.resize(Dim);
	//this->hist_ground.erase(this->hist_ground.begin(),this->hist_ground.end());
	memset(this->hist_ground.data(),0,sizeof(float)*Dim);

	this->hist_vertical.resize(Dim);
	//this->hist_vertical.erase(this->hist_vertical.begin(),this->hist_vertical.end());
	memset(this->hist_vertical.data(),0,sizeof(float)*Dim);

	this->hist_Multiply.resize(Dim);
	//this->hist_Multiply.erase(this->hist_Multiply.begin(),this->hist_Multiply.end());
	memset(this->hist_Multiply.data(),0,sizeof(float)*Dim);

	this->hist_all_count.resize(Dim);
	//this->hist_all_count.erase(this->hist_all_count.begin(),this->hist_all_count.end());
	memset(this->hist_all_count.data(),0,sizeof(float)*Dim);
}
/*-------------------------------------------------------------*/
/**
*将180度 Manhattan直方图转换到5个维度的直方图
*
*/
/*-------------------------------------------------------------*/
void ManhattanHistogram::AdjustTo5Peak(void)
{
	//////////////////////////////////////////////
#if FALSE
	const float Peak3range[6]={0,44,84,96,136,180};
#endif

#if TRUE
	const float Peak3range[6]={0,4,84,96,176,180};
#endif	
	ASSERT(this->HistDim==180);
	vector<float> hist_all_t(this->HistDim);
	vector<float> hist_sky_t(this->HistDim);
	vector<float> hist_ground_t(this->HistDim);
	vector<float> hist_vertical_t(this->HistDim);
	vector<float> hist_Multiply_t(this->HistDim);
	vector<float> hist_all_count_t(this->HistDim);
	///////////////////////////////////////////////
	memcpy(hist_all_t.data(),this->hist_all.data(),sizeof(float)*this->HistDim);
	memcpy(hist_sky_t.data(),this->hist_sky.data(),sizeof(float)*this->HistDim);
	memcpy(hist_ground_t.data(),this->hist_ground.data(),sizeof(float)*this->HistDim);
	memcpy(hist_vertical_t.data(),this->hist_vertical.data(),sizeof(float)*this->HistDim);
	memcpy(hist_Multiply_t.data(),this->hist_Multiply.data(),sizeof(float)*this->HistDim);
	memcpy(hist_all_count.data(),this->hist_all_count.data(),sizeof(float)*this->HistDim);
	///////////////////////////////////////////////
	this->AllocaeMemory(5);
	///////////////////////////////////////////////	
	for (int i=0;i<180;i++){
#if TRUE
		if (i<=Peak3range[1]){
			this->hist_all[0]+=hist_all_t[i];
			this->hist_sky[0]+=hist_all_t[i];
			this->hist_ground[0]+=hist_all_t[i];
			this->hist_vertical[0]+=hist_all_t[i];
			this->hist_Multiply[0]+=hist_all_t[i];
			this->hist_all_count[0]+=hist_all_t[i];
		}

		if(i>Peak3range[1]&&i<=Peak3range[2]){
			this->hist_all[1]+=hist_all_t[i];
			this->hist_sky[1]+=hist_all_t[i];
			this->hist_ground[1]+=hist_all_t[i];
			this->hist_vertical[1]+=hist_all_t[i];
			this->hist_Multiply[1]+=hist_all_t[i];
			this->hist_all_count[1]+=hist_all_t[i];
		}

		if(i>Peak3range[2]&&i<=Peak3range[3]){
			this->hist_all[2]+=hist_all_t[i];
			this->hist_sky[2]+=hist_all_t[i];
			this->hist_ground[2]+=hist_all_t[i];
			this->hist_vertical[2]+=hist_all_t[i];
			this->hist_Multiply[2]+=hist_all_t[i];
			this->hist_all_count[2]+=hist_all_t[i];
		}

		if (i>Peak3range[3]&&i<Peak3range[4]){
			this->hist_all[3]+=hist_all_t[i];
			this->hist_sky[3]+=hist_all_t[i];
			this->hist_ground[3]+=hist_all_t[i];
			this->hist_vertical[3]+=hist_all_t[i];
			this->hist_Multiply[3]+=hist_all_t[i];
			this->hist_all_count[3]+=hist_all_t[i];
		}

		if (i>=Peak3range[4]){
			this->hist_all[4]+=hist_all_t[i];
			this->hist_sky[4]+=hist_all_t[i];
			this->hist_ground[4]+=hist_all_t[i];
			this->hist_vertical[4]+=hist_all_t[i];
			this->hist_Multiply[4]+=hist_all_t[i];
			this->hist_all_count[4]+=hist_all_t[i];
		}
#endif
	}
}
/*-------------------------------------------------------------*/
/**
*判断轮廓形态是否是矩形
*
*/
/*-------------------------------------------------------------*/
bool ManhattanHistogram::IsContourRectangle(void)
{
	float HorValue=0;
	float VerticalValue=0;
	float SumValue=0;
	
	if (this->HistDim==180){
#if TRUE
		
		float *histall=histdata180.hist_all;
		for (int i=0;i<this->HistDim;i++){
			if (i<=4||i>=176){
				HorValue+=histall[i];
			}
			if (i>=84&&i<=96){
				VerticalValue+=histall[i];
			}
			SumValue+=histall[i];
		}
#endif
	}else if(this->HistDim==5){
		
		float *histall=this->hist_all.data();
		for (int i=0;i<this->HistDim;i++){
			if (i==0||i==4){
				HorValue+=histall[i];
			}
			if (i==2){
				VerticalValue+=histall[i];
			}
			SumValue+=histall[i];
		}

	}else{
		ASSERT(0);
	}

	float Scale=(HorValue+VerticalValue)/SumValue;
	float Shape=VerticalValue/HorValue;
	if ((Scale>=0.8)
#if TRUE
		&&(Shape>=1.4)
#endif		
		){
		return true;
	}else{
		return false;
	}
	
}
/*-------------------------------------------------------------*/
/**
*垂直方向线长要大于一定值
*
*/
/*-------------------------------------------------------------*/
bool ManhattanHistogram::IsContourstrip(void)
{
	float VerticalValue=0;
	float SumValue=0;
	
	if (this->HistDim==180){
#if TRUE

		float *histall=histdata180.hist_all;
		for (int i=0;i<this->HistDim;i++){
			if (i>=84&&i<=96){
				VerticalValue+=histall[i];
			}
			SumValue+=histall[i];
		}
#endif
	}else if(this->HistDim==5){

		float *histall=this->hist_all.data();
		for (int i=0;i<this->HistDim;i++){
			if (i==2){
				VerticalValue+=histall[i];
			}
			SumValue+=histall[i];
		}

	}else{
		ASSERT(0);
	}
	float VerticalNotValue=SumValue-VerticalValue;
	if (VerticalNotValue<1E-6){
		VerticalNotValue=1E-6;
	}
	float Scale=VerticalValue/VerticalNotValue;
	/*-2014-05-08==1.9---------*/
	/*-2014-05-08==黄金分割点---------*/
	if (Scale>=1.618){
			return true;
	}else{
		return false;
	}
}
/*-------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------*/
