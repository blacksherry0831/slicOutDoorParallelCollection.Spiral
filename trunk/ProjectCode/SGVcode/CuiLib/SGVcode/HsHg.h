#pragma once
#define  UseVerticalTop FALSE
#define  UseVerticalSpTopAvg TRUE
/*-------------------------------------------------------------------*/
/**
*�������ڵ���ָ�λ�á��컨��ָ�λ��
*/
/*-------------------------------------------------------------------*/
class HgHsLinePos
{
public:
	HgHsLinePos(){
		this->PgOffset=0;
		this->PsOffset=0;
		this->SkyGndCategoryChange=false;
	}
	HgHsLinePos(float PsOffset,
		        float PgOffset,
	            bool SkyGndCategoryChange){
		this->PgOffset=PgOffset;
		this->PsOffset=PsOffset;
		this->SkyGndCategoryChange=SkyGndCategoryChange;
	}
public:
	float PsOffset;/**<�컨��ָ�λ��*/
	float PgOffset;/**<����ָ�λ��*/
	bool  SkyGndCategoryChange;/**<�ָ���λ�øı�ʱ�������ؿ��Ƿ���ͼ�鷢���仯*/
};
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
class HsHg
{
private:
	ImageMemData* pMD;/**<�����õĹ����������м��ڴ�*/
	float CameraHeight;/**<����ͷ�߶�*/
	float CameraOffAngle;/**<����ͷ����ƫ��*/
	float RoomLength;/**<������ӳ���*/
	float RoomHeight;/**<������Ӹ߶�*/
	float ImgFocus;/**<ͼ�񽹾࣬���ؼ�*/
	float ImgHeight;/**<ͼ��߶�*/
	float RoomHeight2Top;/**<������������ľ���*/
	float PgOffset_Last;/**<�ϴε���ֽ���λ��*/
	float PsOffset_Last;/**<�ϴ��컨��ֽ���λ��*/
	std::shared_ptr<INT32>  Matrix_Category_Lable_SkyBoder;/**<������շֽ��ߴ���ͼ����*/
	std::shared_ptr<INT32>  Matrix_Category_Lable_GndBoder;/**<���ڵ���ֽ��ߴ���ͼ����*/
	vector<HgHsLinePos>  HgHsLine;/**<��¼���α߽��߱仯�Ľ��*/
public:
	HsHg(ImageMemData* pMD_t);
	~HsHg(void);
public:
	
	float GetHs_InDoor_Geometry(float DeflectionAngle);
	float GetHg_InDoor_Geometry(float DeflectionAngle);	
	float GetHs_ByIteration(void);
	float GetHg_ByIteration(void);
	void  Get_HsHg_ByIteration(void);
	void  Get_HsHg_ByIterationNoGeometric(void);
	void  Get_HsHg_By_NBig_FSmall_5m4d(void);
	bool  IsConvergence(void);
	bool  IsConvergence_5m4d(void);
	bool  IsBorderLineConvergence_5m4d(void);
	bool  IsBoderCategoryChange_5m4d(void);
	void RecordBoderCategory_5m4d(void);
	void CheackBoderCategory_5m4d(void);
	void  Get_HsHg_By_SV_VG(void);
private:
	float GetHs_InDoor(float DeflectionAngle);
	float GetHg_InDoor(float DeflectionAngle);

	void  ForceGround_InDoor_DFS(void);
	void  ForceSky_InDoor_DFS(void);

	void  DFS_4Ground2Up(int spi);

	void  InitDFSLinkConnection_Ground(void);
	void  InitDFSLinkConnection_Sky(void);
	void  DFS_4Sky2Down(int spi);
	void  StatisticsTrapezoidHgHs_5m4d(vector<float>& SkyY,vector<float>& GndY);

	float GetHs_Nogeometry(void);
	float GetHg_Nogeometry(void);
};
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/

