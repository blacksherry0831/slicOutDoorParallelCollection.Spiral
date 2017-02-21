#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SceneDetermine.h"
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
B3TemplatePeak SceneDetermine:: B3PeaksIndoor[BuildTemplates];
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
float SceneDetermine:: B3TemplatesInDoor[BuildTemplates][AngLeDivided180];
/*----------------------------------------------------------------*/
/**
*构造函数\n
*初始化曼哈顿峰值模型
*
*/
/*----------------------------------------------------------------*/
SceneDetermine::SceneDetermine(void)
{
#if 1
const B3TemplatePeak peak[BuildTemplates]={{0,AngLeDivided/4.0-1,AngLeDivided/2.0-1,AngLeDivided/2.0+1,AngLeDivided*3/4.0+1,AngLeDivided-1}};
#else
const B3TemplatePeak peak[BuildTemplates]={{0,AngLeDivided/4.0-1,AngLeDivided*3/8.0,AngLeDivided*5/8,AngLeDivided*3/4.0+1,AngLeDivided-1}};
#endif
///////////////////////////////////////////////////////////////////
  for (int pi=0;pi<BuildTemplates;pi++){
	  memcpy(&B3Peaks[pi],&peak[pi],sizeof(B3TemplatePeak));
  }
  this->Init3PeakTemplate();
///////////////////////////////////////////////////////////////////
}
/*----------------------------------------------------------------*/
/**
*构造函数\n
*初始化曼哈顿峰值模型
*
*@param MemData_t   运算用到的中间变量
*
*/
/*----------------------------------------------------------------*/
SceneDetermine::SceneDetermine(ImageMemData* MemData_t){
	////////////////////
	this->pMD=MemData_t;
	////////////////////
#if 1
	const B3TemplatePeak peak[BuildTemplates]={{0,AngLeDivided/4.0-1,AngLeDivided/2.0-1,AngLeDivided/2.0+1,AngLeDivided*3/4.0+1,AngLeDivided-1}};
#else
	const B3TemplatePeak peak[BuildTemplates]={{0,AngLeDivided/4.0-1,AngLeDivided*3/8.0,AngLeDivided*5/8,AngLeDivided*3/4.0+1,AngLeDivided-1}};
#endif
	for (int pi=0;pi<BuildTemplates;pi++){
		memcpy(&B3Peaks[pi],&peak[pi],sizeof(B3TemplatePeak));
	}
	this->Init3PeakTemplate();
}
/*----------------------------------------------------------------*/
/**
*析构函数
*
*
*/
/*----------------------------------------------------------------*/
SceneDetermine::~SceneDetermine(void)
{

}
/*----------------------------------------------------------------*/
/**
*根据直线的直方图决定本图片的场景
*
*@param  hist_data 直方图数据
*
*/
/*----------------------------------------------------------------*/

BuildingType SceneDetermine::DetermineScene(HistData hist_data)
{
#if 0
  if ((true==IsGndUp2HorizontalLine(pMD->p_ImgLables_SVG))&&
       (true==this->IsManhattanPeak(0.85,hist_data))){

			  if (true==IsPeakValueInMiddle(hist_data.hist_all)){
					return BT_bigbuildingVP;
			  }else if (true==IsPeakValueInHorizontal(hist_data.hist_all)){
				    return BT_bigbuindingFront;
			  }else{
				    return BT_unknow;
			  }
			

	  }else if((false==this->IsManhattanPeak(0.85,hist_data))
		  &&(true==IsGndUp2HorizontalLine(pMD->p_ImgLables_SVG))){
			//树木
			  return BT_trees;
	  }else{
		   //无需分割
		      return BT_unknow;

	  }	
#endif 
#if 0
	  if ((true==this->IsManhattanPeak(0.85,hist_data))){

			  if (true==IsPeakValueInMiddle(hist_data.hist_all)){
				  return BT_bigbuildingVP;
			  }else if (true==IsPeakValueInHorizontal(hist_data.hist_all)){
				  return BT_bigbuindingFront;
			  }else{
				  return BT_unknow;
			  }


	  }else if((false==this->IsManhattanPeak(0.85,hist_data))){
			  //无需分割
			  return BT_unknow;

	  }	
#endif
#if 0
if ((true==this->IsManhattanPeak(0.85,hist_data))){

		  if (true==IsPeakValueInMiddle(hist_data.hist_all)){
			  return BT_bigbuildingVP;
		  }else{
			  return BT_bigbuindingFront;
		  }


	  }else if((false==this->IsManhattanPeak(0.85,hist_data))){
		  //无需分割
		  return BT_unknow;

	  }	
#endif
#if 0
if (this->IsBigBuildingVP(hist_data)){
	return BT_bigbuildingVP;
}else if (this->IsBigBuildingFront(hist_data)){
	return BT_bigbuindingFront;
}else{
	return BT_unknow;
}
#endif
#if TRUE
/*---------------------*/
/**
*放弃水平线分割，有灭点找灭点
*/
/*-----------------------*/
if (this->IsBigBuildingVP(hist_data)){
	this->bt=BT_bigbuildingVP;
	return BT_bigbuildingVP;
}else{
	this->bt=BT_unknow;
	return BT_unknow;
}
#endif
	  
}
/*----------------------------------------------------------------*/
/**
*初始化3个峰值的模板
*
*
*
*/
/*----------------------------------------------------------------*/
void SceneDetermine::Init3PeakTemplate(void)
{
	memset(B3Templates,0,sizeof(B3Templates));
	for (int pi=0;pi<BuildTemplates;pi++){
		
			 for (int ti=B3Peaks[pi].Lpeak[0];ti<=B3Peaks[pi].Lpeak[1];ti++){
				   B3Templates[pi][ti]=1;
			 }
			 for (int ti=B3Peaks[pi].Mpeak[0];ti<=B3Peaks[pi].Mpeak[1];ti++){
				   B3Templates[pi][ti]=1;
			 }
			 for (int ti=B3Peaks[pi].Rpeak[0];ti<=B3Peaks[pi].Rpeak[1];ti++){
				   B3Templates[pi][ti]=1;
			 }
	
	}
}
/*----------------------------------------------------------------*/
/**
*计算符合曼哈顿方向的能量
*
*@param hist 直线的直方图
*@return 无意义
*/
/*----------------------------------------------------------------*/

double SceneDetermine::CalculatePos(float *hist)
{
	memset(pos,0,sizeof(pos));
	for (int pi=0;pi<BuildTemplates;pi++){
		for (int ti=B3Peaks[pi].Lpeak[0];ti<=B3Peaks[pi].Lpeak[1];ti++){
			pos[pi].Lpeak+=hist[ti];
		}
		for (int ti=B3Peaks[pi].Mpeak[0];ti<=B3Peaks[pi].Mpeak[1];ti++){
			pos[pi].Mpeak+=hist[ti];
		}
		for (int ti=B3Peaks[pi].Rpeak[0];ti<=B3Peaks[pi].Rpeak[1];ti++){
			pos[pi].Rpeak+=hist[ti];
		}
		pos[pi].Sumpeak=pos[pi].Lpeak+pos[pi].Mpeak+pos[pi].Rpeak;
	}
	return 0;

}
/*----------------------------------------------------------------*/
/**
*计算在曼哈顿方向线之外的能量
*
*@param hist 直线的直方图
*@return 无意义
*/
/*----------------------------------------------------------------*/
double SceneDetermine::CalculateNeg(float *hist)
{
	
	memset(neg,0,sizeof(neg));
	for (int pi=0;pi<BuildTemplates;pi++){
		
		for (int ti=0;ti<AngLeDivided/2-1;ti++){
			if (B3Templates[pi][ti]==0){
				neg[pi].Lpeak+=hist[ti];
			}		
		}
		for (int ti=AngLeDivided/2+1;ti<AngLeDivided;ti++){
			if (B3Templates[pi][ti]==0){
				neg[pi].Rpeak+=hist[ti];
			}	
		}
		neg[pi].Sumpeak=neg[pi].Lpeak+neg[pi].Rpeak;
		
	}
	return 0;
}
/*----------------------------------------------------------------*/
/**
*判断直方图数据是否符合曼哈顿方向
*
*@param pos_weight  直方图在曼哈顿方向上所占的比重
*@param  hist_data  直方图数据
*@retval true 符合
*@retval false 不符合
*/
/*----------------------------------------------------------------*/


bool SceneDetermine::IsManhattanPeak(double pos_weight,HistData hist_data)
{
	
	this->CalculateNeg(hist_data.hist_all);
	this->CalculatePos(hist_data.hist_all);
	for (int pi=0;pi<BuildTemplates;pi++){

		double pos_weight_t=pos[pi].Sumpeak/(pos[pi].Sumpeak+neg[pi].Sumpeak);
		this->ManhattanEnergy_ALL=pos_weight_t;
         if (pos_weight_t>pos_weight){
			 return true;
         }else{
		     return false;
		 }

	}

	
}
/*----------------------------------------------------------------*/
/**
*判断地面的超像素块，是否超出了视平线
*
*@param cui_ImgLables_SVG 天空、立面、地面的标定信息
*@retval true 是
*@retval false 否
*/
/*----------------------------------------------------------------*/

bool SceneDetermine::IsGndUp2HorizontalLine(INT32* cui_ImgLables_SVG)
{
	unsigned int count_up=0;
	 for (int x=0;x<pMD->ImgWidth;x++){
		 for (int y=0;y<pMD->ImgHeight;y++){			 
				if ((y<pMD->Seg_HorizontalLinePos)&&(Ground==cui_ImgLables_SVG[y*pMD->ImgWidth+x])){
				 //超出地面
					count_up++;
			   }
			 }
			  
		 }
	 /*****************************************************/
	 if (count_up>=pMD->ImgWidth*2){
		return true;
	 }else{
	    return false;
	 }
	
}
/*----------------------------------------------------------------*/
/**
*导入上次计算用到的地面、立面、天空的标定数据
*
*@param cui_ImgLables_SVG地面、立面、天空的标定矩阵
*
*/
/*----------------------------------------------------------------*/

void SceneDetermine::SetSVGLabel(INT32* cui_ImgLables_SVG)
{
	memcpy(pMD->p_ImgLables_SVG,cui_ImgLables_SVG,sizeof(UINT32)*pMD->ImgWidth*pMD->ImgHeight);
}
#if 0

bool SceneDetermine::IsPeakValueInMiddle(float *hist)
{
	float MaxValue,Energy_M,Energy_ALL,Energy_percent;
	int max_sort[3];
	cui_GeneralImgProcess::GetMaxValueIndex(hist,AngLeDivided,max_sort,sizeof(max_sort)/sizeof(int));
	Energy_ALL=cui_GeneralImgProcess::GetSumValue(hist,AngLeDivided);
	Energy_M=cui_GeneralImgProcess::GetSumValue(hist+AngLeDivided/3,AngLeDivided/3);
	Energy_percent=Energy_M/Energy_ALL;
	if (
#if 0
		(max_sort[0]>=AngLeDivided/2-1)&&
		(max_sort[0]<=AngLeDivided/2+1)
#else
		//只要最大值--不再边界
		(max_sort[0]>=AngLeDivided/8*1)&&
		(max_sort[0]<=AngLeDivided/8*7)
#endif
		
		
		
		){
			/****/
			//直线分割
			return true; 

	}else{
		    return false;
	}
}

#endif
/*----------------------------------------------------------------*/
/**
*判断直方图的峰值是否在垂直方向附近
*
*@param hist 直方图数据
*@return 是否在垂直方向位置
*-  true   在垂直方向附近
*-  false  不在垂直方向附近
*/
/*----------------------------------------------------------------*/
bool SceneDetermine::IsPeakValueInMiddleArea(HistData histdata)
{


	float MaxValue,Energy_M,Energy_ALL,Energy_percent;
	int max_sort[3];
	float *hist=histdata.hist_all;
	cui_GeneralImgProcess::GetMaxValueIndex(hist,AngLeDivided,max_sort,sizeof(max_sort)/sizeof(int));
	Energy_ALL=cui_GeneralImgProcess::GetSumValue(hist,AngLeDivided);
	Energy_M=cui_GeneralImgProcess::GetSumValue(hist+AngLeDivided/3,AngLeDivided/3);
	Energy_percent=Energy_M/Energy_ALL;

	bool IsPeakInMiddle=false;
	for (int i=B3Peaks[0].Mpeak[0];i<B3Peaks[0].Mpeak[1];i++){

		if ((hist[i]/hist[max_sort[0]])>=0.8)
		{
		   IsPeakInMiddle=true;
		}

	}

  /***********************************************/
	if (IsPeakInMiddle==true){
		// this->IsMaxValueMiddle=TRUE;
		 return true;
	}else{
		// this->IsMaxValueMiddle=FALSE;
		 return false;
	}
}
/*----------------------------------------------------------------*/
/**
*判断直方图的峰值是否在水平方向附近
*
*@param hist 直方图数据
*@return 是否在水平线位置
*-  true   在水平直线附近
*-  false  不在水平直线附近
*/
/*----------------------------------------------------------------*/

bool SceneDetermine::IsPeakValueInHorizontal(float *hist){
	float MaxValue,Energy_M,Energy_ALL,Energy_percent;
	int max_sort[3];
	cui_GeneralImgProcess::GetMaxValueIndex(hist,AngLeDivided,max_sort,sizeof(max_sort)/sizeof(int));
	Energy_ALL=cui_GeneralImgProcess::GetSumValue(hist,AngLeDivided);
	Energy_M=cui_GeneralImgProcess::GetSumValue(hist+AngLeDivided/3,AngLeDivided/3);
	Energy_percent=Energy_M/Energy_ALL;
	if (((max_sort[0]>=B3Peaks[0].Lpeak[0])&&(max_sort[0]<=B3Peaks[0].Lpeak[1]))
		||((max_sort[0]>=B3Peaks[0].Rpeak[0])&&(max_sort[0]<=B3Peaks[0].Rpeak[1]))){
			/****/
			//直线分割
			return true; 

	}else{
		    return false;
	}
}
/*----------------------------------------------------------------*/
/**
*判断建筑是否是大建筑，稍后会寻找灭点分割地面
*
*@param hist_data 直方图数据
*@return 判断结果
*-  true 是此类建筑
*-  false 非此类建筑
*/
/*----------------------------------------------------------------*/
bool SceneDetermine::IsBigBuildingVP(HistData hist_data)
{
	float MaxValue,Energy_M,Energy_ALL,Energy_percent;
	int max_sort[3];
	float *hist=hist_data.hist_all;
	cui_GeneralImgProcess::GetMaxValueIndex(hist,AngLeDivided,max_sort,sizeof(max_sort)/sizeof(int));
	Energy_ALL=cui_GeneralImgProcess::GetSumValue(hist,AngLeDivided);
	Energy_M=cui_GeneralImgProcess::GetSumValue(hist+AngLeDivided/3,AngLeDivided/3);
	Energy_percent=Energy_M/Energy_ALL;
#if TRUE
		this->IsManhattanPeak(0.85,hist_data);
		if (//最大值在中间附近
			(max_sort[0]>=AngLeDivided/2-1)&&
			(max_sort[0]<=AngLeDivided/2+1)&&
			(this->IsPeakValueInMiddleArea(hist_data))){
				 this->IsMaxValueMiddle=TRUE;
		}else{
			     this->IsMaxValueMiddle=FALSE;
		}

#endif
	if (
#if 1
		//最大值在中间附近
		(max_sort[0]>=AngLeDivided/2-1)&&
		(max_sort[0]<=AngLeDivided/2+1)&&
		(this->IsManhattanPeak(0.85,hist_data)&&
		(this->IsPeakValueInMiddleArea(hist_data))
		)
#endif	
		){
			/*----------------------------------------------------------------*/
			int up_count=0;
			const int hor[4]={0,1,AngLeDivided-2,AngLeDivided-1};
			for (int i=0;i<4;i++){
				if (hist[i]/hist[max_sort[0]]>0.7){
					up_count++;
				}else{

				}

			}
		/*----------------------------------------------------------------*/
			if (up_count==0){
				return true;
				//水平范围，没有最大值
			}else{
				//水平范围有最大值
			    return false; 
			}

			/*----------------------------------------------------------------*/


	}else{
		return false;
	}
		/*----------------------------------------------------------------*/
}
/*----------------------------------------------------------------*/
/**
*判断建筑是否是比较靠近的大建筑，稍后会用一条水平线切割
*
*@param hist_data 直方图数据
*@return 判断结果
*-  true 是此类建筑
*-  false 非此类建筑
*/
/*----------------------------------------------------------------*/
bool SceneDetermine::IsBigBuildingFront(HistData hist_data)
{
	float MaxValue,Energy_M,Energy_ALL,Energy_percent;
	int max_sort[3];
	float *hist=hist_data.hist_all;
	cui_GeneralImgProcess::GetMaxValueIndex(hist,AngLeDivided,max_sort,sizeof(max_sort)/sizeof(int));
	Energy_ALL=cui_GeneralImgProcess::GetSumValue(hist,AngLeDivided);
	Energy_M=cui_GeneralImgProcess::GetSumValue(hist+AngLeDivided/3,AngLeDivided/3);
	Energy_percent=Energy_M/Energy_ALL;
	if (
		this->IsManhattanPeak(0.85,hist_data)
		){
			/*----------------------------------------------------------------*/
			int up_count=0;
			const int hor[4]={0,1,AngLeDivided-2,AngLeDivided-1};
			for (int i=0;i<4;i++){
				if (hist[i]/hist[max_sort[0]]>0.7){
					up_count++;
				}else{

				}

			}
			/*----------------------------------------------------------------*/
			if (up_count==0){
				//水平范围，没有最大值
				return false; 
				
			}else{
				//水平范围有最大值
				return true;
			}

			/*----------------------------------------------------------------*/


	}else{
		return false;
	}
	/*----------------------------------------------------------------*/
}
/*----------------------------------------------------------------*/
/**
*计算曼哈顿特性
*
*/
/*----------------------------------------------------------------*/
bool SceneDetermine::IsHistRegularShapes(HistData180 data)
{
	float *histall=data.hist_all;
	int histsize=AngLeDivided180;
	
	const B3TemplatePeak peak[1]={{0,histsize/4.0-1,histsize/2.0-1,histsize/2.0+1,histsize*3/4.0+1,histsize-1}};
	SceneDetermine::Init3PeakTemplateInDoor(&peak[0],&B3TemplatesInDoor[0][0],histsize);
	
	Pos3Peak posIn[1];
	Neg3Peak negIn[1];

		SceneDetermine::CalculateNeg(&B3TemplatesInDoor[0][0],&negIn[0],data.hist_all,histsize);
		SceneDetermine::CalculatePos(&peak[0],&posIn[0],data.hist_all,histsize);

		for (int pi=0;pi<BuildTemplates;pi++){

			double pos_weight_t=posIn[pi].Sumpeak/(posIn[pi].Sumpeak+negIn[pi].Sumpeak);
			double pos_weight=0.8;
			if (pos_weight_t>pos_weight){
				return true;
			}else{
				return false;
			}

		}

	return true;

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
double SceneDetermine::CalculateNeg(
	float *B3Templates,
	Neg3Peak* neg ,
	float *hist,
	int Dim)
{
	memset(neg,0,sizeof(Neg3Peak));
	for (int pi=0;pi<BuildTemplates;pi++){

		for (int ti=0;ti<Dim/2-1;ti++){
			if (B3Templates[pi*Dim+ti]==0){
				neg[pi].Lpeak+=hist[ti];
			}		
		}
		for (int ti=Dim/2+1;ti<Dim;ti++){
			if (B3Templates[pi*Dim+ti]==0){
				neg[pi].Rpeak+=hist[ti];
			}	
		}
		neg[pi].Sumpeak=neg[pi].Lpeak+neg[pi].Rpeak;

	}
	return 0;
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
double SceneDetermine::CalculatePos(
	const B3TemplatePeak* B3Peaks,
	Pos3Peak* pos,
	float *hist,
	int Dim)
{
	
	memset(pos,0,sizeof(Pos3Peak));
	for (int pi=0;pi<BuildTemplates;pi++){
		for (int ti=B3Peaks[pi].Lpeak[0];ti<=B3Peaks[pi].Lpeak[1];ti++){
			pos[pi].Lpeak+=hist[ti];
		}
		for (int ti=B3Peaks[pi].Mpeak[0];ti<=B3Peaks[pi].Mpeak[1];ti++){
			pos[pi].Mpeak+=hist[ti];
		}
		for (int ti=B3Peaks[pi].Rpeak[0];ti<=B3Peaks[pi].Rpeak[1];ti++){
			pos[pi].Rpeak+=hist[ti];
		}
		pos[pi].Sumpeak=pos[pi].Lpeak+pos[pi].Mpeak+pos[pi].Rpeak;
	}
	return 0;

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
void SceneDetermine::Init3PeakTemplateInDoor(
	const B3TemplatePeak* B3Peaks,
	float*B3Templates,
	int Dim)
{
#if 1
	memset(B3Templates,0,sizeof(B3Templates));
	for (int pi=0;pi<BuildTemplates;pi++){

		for (int ti=B3Peaks[pi].Lpeak[0];ti<=B3Peaks[pi].Lpeak[1];ti++){
			B3Templates[pi*Dim+ti]=1;
		}

		for (int ti=B3Peaks[pi].Mpeak[0];ti<=B3Peaks[pi].Mpeak[1];ti++){
			B3Templates[pi*Dim+ti]=1;
		}

		for (int ti=B3Peaks[pi].Rpeak[0];ti<=B3Peaks[pi].Rpeak[1];ti++){
			B3Templates[pi*Dim+ti]=1;
		}

	}
#endif

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
bool SceneDetermine::IsBlockTree(HistData180 histdata,int Dim)
{
#if 0
return !(IsHistRegularShapes(histdata));
#endif
return !IsVerticalEnergyFocus(histdata,AngLeDivided180);
	
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
bool SceneDetermine::IsBlockBuilding(HistData180 histdata,int Dim)
{
#if 0
return IsHistRegularShapes(histdata);
#endif
	return IsVerticalEnergyFocus(histdata,AngLeDivided180);
}
/*----------------------------------------------------------------------------------------*/
/**
*能量是否集中垂直方向
*/
/*----------------------------------------------------------------------------------------*/
bool SceneDetermine::IsVerticalEnergyFocus(HistData180 hist,int Dim)
{
	double sum=0;
	for (int ai=0;ai<Dim;ai++){
		sum+=hist.hist_all[ai];
	}
	double sum_ver=0;
	for (int vi=Dim/8.0*3;vi<Dim/8.0*5;vi++){
		sum_ver+=hist.hist_all[vi];
	}
	double Vscale=sum_ver/sum;
	if (Vscale>=0.5){
		return true;
	}else{
		return false;
	}

}
/*----------------------------------------------------------------------------------------*/
/**
*直方图是否是矩形
*@param  hist 直方图 
*@param  Dim 直方图维度 
*/
/*----------------------------------------------------------------------------------------*/
bool SceneDetermine::IsSpRectangle(HistData180 hist,int Dim)
{
	double sum=0;
	ASSERT(Dim==180);
	for (int ai=0;ai<Dim;ai++){
		sum+=hist.hist_all[ai];
	}
	double sum_ver=0;
	for (int vi=90-3;vi<90+3;vi++){
		sum_ver+=hist.hist_all[vi];
	}
	double sum_hor=0;
	for (int ai=0;ai<Dim;ai++){
		if ((ai<=3)
			||(ai>=180-3)){
			sum_hor+=hist.hist_all[ai];
		}

	}

	double vhscal=(sum_hor+sum_ver)/sum;
	if (vhscal>=0.8){
		return true;
	}else{
		return false;
	}

}
/*----------------------------------------------------------------------------------------*/
/**
*保存建筑信息
*@param path 路径  
*@param filename 文件名 
*/
/*----------------------------------------------------------------------------------------*/
void SceneDetermine::SaveBulidingTypeInfo(string path,string filename)
{
	/********************************************************/
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else

	string nameandextension =filename;
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif
	char data_t[1024];
	ofstream outfile;
	outfile.open(finalpath.c_str(),ios::app);
	{
		if (outfile.is_open()){
			{
						outfile<<pMD->FileReadFullPath<<" ";
						outfile<<ManhattanEnergy_ALL<<" ";
						outfile<<IsMaxValueMiddle<<" ";
						if (this->bt==BT_unknow)
						{
							outfile<<1<<" ";
						}else if (this->bt==BT_bigbuildingVP){
							outfile<<0<<" ";
						}
					}
		}else{
			ASSERT(0);
		}
		
		outfile<<endl;
	} 
	outfile.close();
}
/*----------------------------------------------------------------------------------------*/
/**
*判断直方图是否具有曼哈顿特性
*@param hist  
*/
/*----------------------------------------------------------------------------------------*/
bool SceneDetermine::IsManhattan(ManhattanHistogram hist)
{
	if (hist.HistDim==5){
	float* hist_all=hist.hist_all.data();
		float sum=cui_GeneralImgProcess::GetSumValue(hist_all,hist.HistDim);
		float manhattan3Peak=hist_all[0]+hist_all[2]+hist_all[4];
		float weightScale=manhattan3Peak/sum;
		if (weightScale>=0.8){
			return true;
		}else{
			return false;
		}
	}else{
		ASSERT(0);
	}
	
	
}
/*-----------------------------------------------------------------------------------------*/