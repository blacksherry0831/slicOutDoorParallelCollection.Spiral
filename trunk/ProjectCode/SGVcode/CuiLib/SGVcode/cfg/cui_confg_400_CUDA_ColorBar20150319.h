
#ifndef _CFG_H
#define  _CFG_H
#define OutDoor TRUE
/*-------------------------------------------------*/
#ifdef OutDoor 

#define  OUT_DOOR  TRUE
#define  IN_DOOR   FALSE

/////////////////////////////////////////
#define  OUT_DOOR_HUMAN FALSE

#define  OUT_DOOR_400_IMAGE_STABLE  TRUE
/////////////////////////////////////////

#endif 
/*-------------------------------------------------*/
#if OUT_DOOR_400_IMAGE_STABLE

#define  OUT_DOOR_SUPERPIXEL_COLOR_BAT TRUE

#endif
/*-------------------------------------------------*/
#if OUT_DOOR_HUMAN

#define  OUT_DOOR_SUPERPIXEL_COLOR_BAT FALSE

#define  ThetaML_Theta_Division	 (60)
#define  ThetaML_M_Division	 (2)
#define  ThetaML_L_Division	 (16)
#endif
/*-------------------------------------------------*/
#ifdef InDoor

#define  OUT_DOOR  FALSE
#define  IN_DOOR   TRUE

#endif 
/*-------------------------------------------------*/
/********LAB直方图维度划分***************************/
#define  Lab_L_Division	 (8)
#define  Lab_a_Division	 (16)
#define  Lab_b_Division	 (16)
/**************************************************/
/*-------------------------------------------------------------------*/
#define  HORIZONTAL_LINE_POS   (0.5)
//直方图B系数阈值
#define  Color_histogram_B_Threshold   (0.71)
//计算谱聚类的BHATTACHARYYA系数
#define  Spectral_Clustering_B_Threshold   (0.94)
#define  Spectral_Clustering_B_Wavelet_Threshold   (0.95)	
#define  Spectral_Clustering_Combine_Threshold (0.72)
/*-------------------------------------------------------------------*/
//迭代阈值
#define  Iteration__Threshold  (0.9)
#define  Iteration__Threshold_Color_SkyV  (0.9)
#define  Iteration__Threshold_Color_GND  (0.95)
#define  Iteration__Threshold_Vein_SkyV  (0.9)
#define  Iteration__Threshold_Vein_GND   (0.95)
//迭代完成最后组合阈值
#define  Iteration_Complete_Combine_Threshold (0)
/**************************************************/
#define  Sky_Energy_UP ((float)5.0)
#define  Sky_Energy_DOWN (3.5)
//#define  Sky_Energy_UP (7)
//#define  Sky_Energy_DOWN (4)
//小图7.7 大图7
//庐山 10
//大图19 ----小图7
#define  Sky_Lab_Distance (25)
/**************************************************/
#define  CUI_SAVE_LABLES  FALSE
#define  CUI_DRAW_SC_INDEX  FALSE
#define  CUI_SAVE_HARR_IMG  TRUE
/***************************************************
天空分类，能量+位置+色彩。
色彩距离D（lab）能量距离（HL LH HH）
62751 =	250	   25
48998 =	220
12%
****************************************************/
#define MAX_SP_NUM    (3000)
#define  MAXLINEDATA (2000) 
#define  AngLeDivided (180/4)
/*********************************************/
#define Unclassify (-2)
#define  Remove (-1)
#if 0
#define  Unknow (0)   
#endif
#define  Ground11  (-11)
#define  Ground12  (-12)
#define  Ground13  (-13)
#define  Ground (1)
#if 0
#define  NotSky   (1.5)
#endif
#define  Vertical (2)
#if 0
#define  NotGround (2.5)
#endif 
#define  Sky  (3)


/*********************************************/
#define Gray_Sky FALSE
#define   OUT_NOGROUND_IMG FALSE
/*********************************************/
#endif




