// SLIC.cpp: implementation of the SLIC class.
//
// Copyright (C) Radhakrishna Achanta 2012
// All rights reserved
// Email: firstname.lastname@epfl.ch
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "module_all_cui.h"

//#include "SLIC.h"
//#include "cui_GeneralImgProcess.h"

//using namespace std;
//#define UINT32_MAX 0xffffffff

/*----------------------------------------------------------------------------------------------------------------*/
/** 
*构造函数
*完成变量初始化
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC::SLIC()
{
	this->InitParameter();
}
/*----------------------------------------------------------------------------------------------------------------*/
/** 
*构造函数...
*导入图像数据，完成变量初始化
*@param pMD_t 图像数据及暂存的中间结果结果
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC::SLIC(ImageMemData* pMD_t)
{
	this->pMD=pMD_t;
	this->InitParameter();
	this->CuiGetImageData();
}
/*----------------------------------------------------------------------------------------------------------------*/
/** 
*构造函数...
*导入图像数据，完成变量初始化
*@param pMD_t 图像数据及暂存的中间结果结果
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC::SLIC(ImageData* pMD_t)
{
	TRACE_FUNC();
	this->pIMD=pMD_t;
	this->InitParameter();
	this->CuiGetImageDataPIMD();
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*初始化成员变量
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::InitParameter(void)
{
	
	TRACE_FUNC();
	m_lvecvec = NULL;
	m_avecvec = NULL;
	m_bvecvec = NULL;
	CuiImgData=NULL;
	Cui_Matrix_E=NULL;
	Cui_Matrix_D=NULL;
	Cui_Matrix_W=NULL;
	Cui_Matrix_W_Vein=NULL;
	Cui_Matrix_L=NULL;
	Cui_MatrixEigenVector_L=NULL;
	CUi_MatrixEigenValue_L=NULL;
	Cui_Matrix_Category_Lable=NULL;
	Cui_Matrix_Category_Simple=NULL;
	CuiImgLables=NULL;
	/////////////////
	CuiCvImage=NULL;
	Cui_ARGB_Image=NULL;
	Cui_lab_hsv_Image=NULL;
	h_l_plane=NULL;
	s_a_plane=NULL;
	v_b_plane=NULL;
	////////////////
#if 1
	alpha=0.1;
	betta=0.6;
	gama=1-alpha-betta;
	fai=1;
#endif
	////////////////
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*析构函数，检查并释放内存
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC::~SLIC()
{

	if (CuiImgData) delete []CuiImgData;
	if (CuiImgLables) delete[]CuiImgLables;

	if (Cui_Matrix_E) delete[]Cui_Matrix_E;
	if (Cui_Matrix_D) delete[]Cui_Matrix_D;
	if (Cui_Matrix_W) delete[]Cui_Matrix_W;
	if (Cui_Matrix_L) delete[]Cui_Matrix_L;
	if (Cui_MatrixEigenVector_L) delete[]Cui_MatrixEigenVector_L;
	if (CUi_MatrixEigenValue_L)  delete[]CUi_MatrixEigenValue_L;
	if (Cui_Matrix_Category_Simple)  delete[]Cui_Matrix_Category_Simple;
	if (Cui_Matrix_Category_Lable)  delete[]Cui_Matrix_Category_Lable;

/**********************************************************/
	if (CuiCvImage)  cvReleaseImage(&CuiCvImage);
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	if (Cui_lab_hsv_Image)  cvReleaseImage(&Cui_lab_hsv_Image);
	if (h_l_plane)  cvReleaseImage(&h_l_plane);
	if (s_a_plane)  cvReleaseImage(&s_a_plane);
	if (v_b_plane)  cvReleaseImage(&v_b_plane);
/********************************************/

	if(m_lvecvec)
	{
		for( int d = 0; d < m_depth; d++ ) delete [] m_lvecvec[d];
		delete [] m_lvecvec;
	}
	if(m_avecvec)
	{
		for( int d = 0; d < m_depth; d++ ) delete [] m_avecvec[d];
		delete [] m_avecvec;
	}
	if(m_bvecvec)
	{
		for( int d = 0; d < m_depth; d++ ) delete [] m_bvecvec[d];
		delete [] m_bvecvec;
	}


	delete[] Cui_Matrix_W_Vein;
	
}


//==============================================================================
///	RGB2XYZ
///
/// sRGB (D65 illuninant assumption) to XYZ conversion
//==============================================================================
//void SLIC::RGB2XYZ(
//	const int&		sR,
//	const int&		sG,
//	const int&		sB,
//	double&			X,
//	double&			Y,
//	double&			Z)
//{
//	double R = sR/255.0;
//	double G = sG/255.0;
//	double B = sB/255.0;
//
//	double r, g, b;
//
//	if(R <= 0.04045)	r = R/12.92;
//	else				r = pow((R+0.055)/1.055,2.4);
//	if(G <= 0.04045)	g = G/12.92;
//	else				g = pow((G+0.055)/1.055,2.4);
//	if(B <= 0.04045)	b = B/12.92;
//	else				b = pow((B+0.055)/1.055,2.4);
//
//	X = r*0.4124564 + g*0.3575761 + b*0.1804375;
//	Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
//	Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
//}

//===========================================================================
///	RGB2LAB
//===========================================================================
//void SLIC::RGB2LAB(const int& sR, const int& sG, const int& sB, double& lval, double& aval, double& bval)
//{
//	//------------------------
//	// sRGB to XYZ conversion
//	//------------------------
//	double X, Y, Z;
//	RGB2XYZ(sR, sG, sB, X, Y, Z);
//
//	//------------------------
//	// XYZ to LAB conversion
//	//------------------------
//	double epsilon = 0.008856;	//actual CIE standard
//	double kappa   = 903.3;		//actual CIE standard
//
//	double Xr = 0.950456;	//reference white
//	double Yr = 1.0;		//reference white
//	double Zr = 1.088754;	//reference white
//
//	double xr = X/Xr;
//	double yr = Y/Yr;
//	double zr = Z/Zr;
//
//	double fx, fy, fz;
//	if(xr > epsilon)	fx = pow(xr, 1.0/3.0);
//	else				fx = (kappa*xr + 16.0)/116.0;
//	if(yr > epsilon)	fy = pow(yr, 1.0/3.0);
//	else				fy = (kappa*yr + 16.0)/116.0;
//	if(zr > epsilon)	fz = pow(zr, 1.0/3.0);
//	else				fz = (kappa*zr + 16.0)/116.0;
//
//	lval = 116.0*fy-16.0;
//	aval = 500.0*(fx-fy);
//	bval = 200.0*(fy-fz);
//}
string SLIC::cuiGetCurrentTime(void)
{
	
std::string cui_t;

#ifdef _WIN32 || _WIN64
#if _WIN32 || _WIN64
#if _MSC_VER

time_t curtime = time(0); 
struct tm tim;
localtime_s(&tim, &curtime);

char title[128];
static int s_num = 0;
sprintf(title, "%d_%02d_%02d_%02d_%02d_%02d", tim.tm_year+1900, tim.tm_mon+1,
	tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);

	/*SYSTEMTIME systime;
	cui_t="win32__win64";*/
cui_t.append(title);
#endif


#ifdef Use_CString&&_MSC_VER
#if Use_CString	&&_MSC_VER
  CString strName,Time;
  GetSystemTime(&systime);
#if 1
  Time.Format(_T("%u_%u_%u_%u_%u_%u_%u"),
	  systime.wYear, 
	  systime.wMonth, 
	  systime.wDay,
	  systime.wHour, 
	  systime.wMinute, 
	  systime.wSecond, 
	  systime.wMilliseconds);
#else
  Time.Format(_T("%u:%u:%u__%uMs"),
	  systime.wHour, 
	  systime.wMinute, 
	  systime.wSecond, 
	  systime.wMilliseconds);
#endif
 cui_t=cui_GeneralImgProcess::ConvertCS2string(Time);	
#endif
#endif


#endif
 

#endif
/**********************************/
#ifdef linux
#if linux 
  cui_t="linux_time";
#endif
#endif
/**********************************/
	return cui_t;
}




//===========================================================================
///	DoRGBtoLABConversion
///
///	For whole image: overlaoded floating point version
//===========================================================================
void SLIC::DoRGBtoLABConversion(
	const unsigned int*&		ubuff,
	double*&					lvec,
	double*&					avec,
	double*&					bvec)
{

	ImageData::DoRGBtoLABConversion(ubuff,lvec,avec,bvec,m_width,m_height);

	//int sz = m_width*m_height;
	//lvec = new double[sz];
	//avec = new double[sz];
	//bvec = new double[sz];
	///*r:0--255*/
	///*g:0--255*/
	///*b:0--255*/
	//for( int j = 0; j < sz; j++ )
	//{
	//	int r = (ubuff[j] >> 16) & 0xFF;
	//	int g = (ubuff[j] >>  8) & 0xFF;
	//	int b = (ubuff[j]      ) & 0xFF;
	//	//////////////////////////////////
	//	  // a  r      g      b
	//	//////////////////////////////////
	//	RGB2LAB( r, g, b, lvec[j], avec[j], bvec[j] );
	//	/*assert();*/
	//}
}

//===========================================================================
///	DoRGBtoLABConversion
///
/// For whole volume
//===========================================================================
void SLIC::DoRGBtoLABConversion(
	unsigned int**&		ubuff,
	double**&					lvec,
	double**&					avec,
	double**&					bvec)
{
	int sz = m_width*m_height;
	for( int d = 0; d < m_depth; d++ )
	{
		for( int j = 0; j < sz; j++ )
		{
			int r = (ubuff[d][j] >> 16) & 0xFF;
			int g = (ubuff[d][j] >>  8) & 0xFF;
			int b = (ubuff[d][j]      ) & 0xFF;

			ImageData::RGB2LAB( r, g, b, lvec[d][j], avec[d][j], bvec[d][j] );
		}
	}
}

//=================================================================================
/// DrawContoursAroundSegments
///
/// Internal contour drawing option exists. One only needs to comment the if
/// statement inside the loop that looks at neighbourhood.
//=================================================================================



//===========================================================================
///	PerturbSeeds
//===========================================================================
void SLIC::PerturbSeeds(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
    const vector<double>&       edges)
{
	TRACE_FUNC();
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	
	int numseeds = kseedsl.size();

	for( int n = 0; n < numseeds; n++ )
	{
		int ox = kseedsx[n];//original x
		int oy = kseedsy[n];//original y
		int oind = oy*m_width + ox;

		int storeind = oind;
		for( int i = 0; i < 8; i++ )
		{
			int nx = ox+dx8[i];//new x
			int ny = oy+dy8[i];//new y

			if( nx >= 0 && nx < m_width && ny >= 0 && ny < m_height)
			{
				int nind = ny*m_width + nx;
				if( edges[nind] < edges[storeind])
				{
					storeind = nind;
				}
			}
		}
		if(storeind != oind)
		{
			kseedsx[n] = storeind%m_width;
			kseedsy[n] = storeind/m_width;
			kseedsl[n] = m_lvec[storeind];
			kseedsa[n] = m_avec[storeind];
			kseedsb[n] = m_bvec[storeind];
		}
	}
}


//===========================================================================
///	GetLABXYSeeds_ForGivenStepSize
///
/// The k seed values are taken as uniform spatial pixel samples.
//===========================================================================
void SLIC::GetLABXYSeeds_ForGivenStepSize(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
    const int&					STEP,
    const bool&					perturbseeds,
    const vector<double>&       edgemag)
{
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
    const bool hexgrid = false;
	int numseeds(0);
	int n(0);

	//int xstrips = m_width/STEP;
	//int ystrips = m_height/STEP;
	int xstrips = (0.5+double(m_width)/double(STEP));
	int ystrips = (0.5+double(m_height)/double(STEP));

    int xerr = m_width  - STEP*xstrips;if(xerr < 0){xstrips--;xerr = m_width - STEP*xstrips;}
    int yerr = m_height - STEP*ystrips;if(yerr < 0){ystrips--;yerr = m_height- STEP*ystrips;}

	double xerrperstrip = double(xerr)/double(xstrips);
	double yerrperstrip = double(yerr)/double(ystrips);

	int xoff = STEP/2;
	int yoff = STEP/2;
	//-------------------------
	numseeds = xstrips*ystrips;
	//-------------------------
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for( int y = 0; y < ystrips; y++ )
	{
		int ye = y*yerrperstrip;
		for( int x = 0; x < xstrips; x++ )
		{
			int xe = x*xerrperstrip;
            int seedx = (x*STEP+xoff+xe);
            if(hexgrid){ seedx = x*STEP+(xoff<<(y&0x1))+xe; seedx = min(m_width-1,seedx); }//for hex grid sampling
            int seedy = (y*STEP+yoff+ye);
            int i = seedy*m_width + seedx;
			
			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
            kseedsx[n] = seedx;
            kseedsy[n] = seedy;
			n++;
		}
	}

#if 0
	if(perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
#endif
}
void SLIC::DetectLabEdges(
	const double*				lvec,
	const double*				avec,
	const double*				bvec,
	const int&					width,
	const int&					height,
	vector<double>&				edges)
{
	int sz = width*height;

	edges.resize(sz,0);
	for( int j = 1; j < height-1; j++ )
	{
		for( int k = 1; k < width-1; k++ )
		{
			int i = j*width+k;

			double dx = (lvec[i-1]-lvec[i+1])*(lvec[i-1]-lvec[i+1]) +
				(avec[i-1]-avec[i+1])*(avec[i-1]-avec[i+1]) +
				(bvec[i-1]-bvec[i+1])*(bvec[i-1]-bvec[i+1]);

			double dy = (lvec[i-width]-lvec[i+width])*(lvec[i-width]-lvec[i+width]) +
				(avec[i-width]-avec[i+width])*(avec[i-width]-avec[i+width]) +
				(bvec[i-width]-bvec[i+width])*(bvec[i-width]-bvec[i+width]);

			//edges[i] = fabs(dx) + fabs(dy);
			edges[i] = dx*dx + dy*dy;
		}
	}
}
//===========================================================================
///	GetKValues_LABXYZ
///
/// The k seed values are taken as uniform spatial pixel samples.
//===========================================================================
void SLIC::GetKValues_LABXYZ(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	vector<double>&				kseedsz,
        const int&				STEP)
{
    const bool hexgrid = false;
	int numseeds(0);
	int n(0);

	int xstrips = (0.5+double(m_width)/double(STEP));
	int ystrips = (0.5+double(m_height)/double(STEP));
	int zstrips = (0.5+double(m_depth)/double(STEP));

    int xerr = m_width  - STEP*xstrips;if(xerr < 0){xstrips--;xerr = m_width - STEP*xstrips;}
    int yerr = m_height - STEP*ystrips;if(yerr < 0){ystrips--;yerr = m_height- STEP*ystrips;}
    int zerr = m_depth  - STEP*zstrips;if(zerr < 0){zstrips--;zerr = m_depth - STEP*zstrips;}

	double xerrperstrip = double(xerr)/double(xstrips);
	double yerrperstrip = double(yerr)/double(ystrips);
	double zerrperstrip = double(zerr)/double(zstrips);

	int xoff = STEP/2;
	int yoff = STEP/2;
	int zoff = STEP/2;
	//-------------------------
	numseeds = xstrips*ystrips*zstrips;
	//-------------------------
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);
	kseedsz.resize(numseeds);

	for( int z = 0; z < zstrips; z++ )
	{
		int ze = z*zerrperstrip;
		int d = (z*STEP+zoff+ze);
		for( int y = 0; y < ystrips; y++ )
		{
			int ye = y*yerrperstrip;
			for( int x = 0; x < xstrips; x++ )
			{
				int xe = x*xerrperstrip;
				int i = (y*STEP+yoff+ye)*m_width + (x*STEP+xoff+xe);
				
				kseedsl[n] = m_lvecvec[d][i];
				kseedsa[n] = m_avecvec[d][i];
				kseedsb[n] = m_bvecvec[d][i];
				kseedsx[n] = (x*STEP+xoff+xe);
				kseedsy[n] = (y*STEP+yoff+ye);
				kseedsz[n] = d;
				n++;
			}
		}
	}
}

//===========================================================================
///	PerformSuperpixelSLIC
///
///	Performs k mean segmentation. It is fast because it looks locally, not
/// over the entire image.
//===========================================================================
void SLIC::PerformSuperpixelSLIC(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
        int*&					klabels,
        const int&				STEP,
        const vector<double>&                   edgemag,
	const double&				M)
{
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	int sz = m_width*m_height;
	const int numk = kseedsl.size();
	//----------------
	int offset = STEP;
        //if(STEP < 8) offset = STEP*1.5;//to prevent a crash due to a very small step size
	//----------------
	
	vector<double> clustersize(numk, 0);
	vector<double> inv(numk, 0);//to store 1/clustersize[k] values

	vector<double> sigmal(numk, 0);
	vector<double> sigmaa(numk, 0);
	vector<double> sigmab(numk, 0);
	vector<double> sigmax(numk, 0);
	vector<double> sigmay(numk, 0);
	vector<double> distvec(sz, DBL_MAX);

	double invwt = 1.0/((STEP/M)*(STEP/M));

	int x1, y1, x2, y2;
	double l, a, b;
	double dist;
	double distxy;
	for( int itr = 0; itr < 10; itr++ )
	{
		distvec.assign(sz, DBL_MAX);
		for( int n = 0; n < numk; n++ )
		{
                        y1 = max(0.0,			kseedsy[n]-offset);
                        y2 = min((double)m_height,	kseedsy[n]+offset);
                        x1 = max(0.0,			kseedsx[n]-offset);
                        x2 = min((double)m_width,	kseedsx[n]+offset);


			for( int y = y1; y < y2; y++ )
			{
				for( int x = x1; x < x2; x++ )
				{
					int i = y*m_width + x;

					l = m_lvec[i];
					a = m_avec[i];
					b = m_bvec[i];

					dist =			(l - kseedsl[n])*(l - kseedsl[n]) +
									(a - kseedsa[n])*(a - kseedsa[n]) +
									(b - kseedsb[n])*(b - kseedsb[n]);

					distxy =		(x - kseedsx[n])*(x - kseedsx[n]) +
									(y - kseedsy[n])*(y - kseedsy[n]);
					
					//------------------------------------------------------------------------
					dist += distxy*invwt;//dist = sqrt(dist) + sqrt(distxy*invwt);//this is more exact
					//------------------------------------------------------------------------
					if( dist < distvec[i] )
					{
						distvec[i] = dist;
						klabels[i]  = n;
					}
				}
			}
		}
		//-----------------------------------------------------------------
		// Recalculate the centroid and store in the seed values
		//-----------------------------------------------------------------
		//instead of reassigning memory on each iteration, just reset.
	
		sigmal.assign(numk, 0);
		sigmaa.assign(numk, 0);
		sigmab.assign(numk, 0);
		sigmax.assign(numk, 0);
		sigmay.assign(numk, 0);
		clustersize.assign(numk, 0);
		//------------------------------------
		//edgesum.assign(numk, 0);
		//------------------------------------

		{int ind(0);
		for( int r = 0; r < m_height; r++ )
		{
			for( int c = 0; c < m_width; c++ )
			{
				sigmal[klabels[ind]] += m_lvec[ind];
				sigmaa[klabels[ind]] += m_avec[ind];
				sigmab[klabels[ind]] += m_bvec[ind];
				sigmax[klabels[ind]] += c;
				sigmay[klabels[ind]] += r;
				//------------------------------------
				//edgesum[klabels[ind]] += edgemag[ind];
				//------------------------------------
				clustersize[klabels[ind]] += 1.0;
				ind++;
			}
		}}

		{for( int k = 0; k < numk; k++ )
		{
			if( clustersize[k] <= 0 ) clustersize[k] = 1;
			inv[k] = 1.0/clustersize[k];//computing inverse now to multiply, than divide later
		}}
		
		{for( int k = 0; k < numk; k++ )
		{
			kseedsl[k] = sigmal[k]*inv[k];
			kseedsa[k] = sigmaa[k]*inv[k];
			kseedsb[k] = sigmab[k]*inv[k];
			kseedsx[k] = sigmax[k]*inv[k];
			kseedsy[k] = sigmay[k]*inv[k];
			//------------------------------------
			//edgesum[k] *= inv[k];
			//------------------------------------
		}}
	}
}

//===========================================================================
///	PerformSupervoxelSLIC
///
///	Performs k mean segmentation. It is fast because it searches locally, not
/// over the entire image.
//===========================================================================
void SLIC::PerformSupervoxelSLIC(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	vector<double>&				kseedsz,
        int**&					klabels,
        const int&				STEP,
	const double&				compactness)
{
	int sz = m_width*m_height;
	const int numk = kseedsl.size();
        //int numitr(0);

	//----------------
	int offset = STEP;
        //if(STEP < 8) offset = STEP*1.5;//to prevent a crash due to a very small step size
	//----------------

	vector<double> clustersize(numk, 0);
	vector<double> inv(numk, 0);//to store 1/clustersize[k] values

	vector<double> sigmal(numk, 0);
	vector<double> sigmaa(numk, 0);
	vector<double> sigmab(numk, 0);
	vector<double> sigmax(numk, 0);
	vector<double> sigmay(numk, 0);
	vector<double> sigmaz(numk, 0);

	vector< double > initdouble(sz, DBL_MAX);
	vector< vector<double> > distvec(m_depth, initdouble);
	//vector<double> distvec(sz, DBL_MAX);

	double invwt = 1.0/((STEP/compactness)*(STEP/compactness));//compactness = 20.0 is usually good.

	int x1, y1, x2, y2, z1, z2;
	double l, a, b;
	double dist;
	double distxyz;
	for( int itr = 0; itr < 5; itr++ )
	{
		distvec.assign(m_depth, initdouble);
		for( int n = 0; n < numk; n++ )
		{
                        y1 = max(0.0,			kseedsy[n]-offset);
                        y2 = min((double)m_height,	kseedsy[n]+offset);
                        x1 = max(0.0,			kseedsx[n]-offset);
                        x2 = min((double)m_width,	kseedsx[n]+offset);
                        z1 = max(0.0,			kseedsz[n]-offset);
                        z2 = min((double)m_depth,	kseedsz[n]+offset);


			for( int z = z1; z < z2; z++ )
			{
				for( int y = y1; y < y2; y++ )
				{
					for( int x = x1; x < x2; x++ )
					{
						int i = y*m_width + x;

						l = m_lvecvec[z][i];
						a = m_avecvec[z][i];
						b = m_bvecvec[z][i];

						dist =			(l - kseedsl[n])*(l - kseedsl[n]) +
										(a - kseedsa[n])*(a - kseedsa[n]) +
										(b - kseedsb[n])*(b - kseedsb[n]);

						distxyz =		(x - kseedsx[n])*(x - kseedsx[n]) +
										(y - kseedsy[n])*(y - kseedsy[n]) +
										(z - kseedsz[n])*(z - kseedsz[n]);
						//------------------------------------------------------------------------
						dist += distxyz*invwt;
						//------------------------------------------------------------------------
						if( dist < distvec[z][i] )
						{
							distvec[z][i] = dist;
							klabels[z][i]  = n;
						}
					}
				}
			}
		}
		//-----------------------------------------------------------------
		// Recalculate the centroid and store in the seed values
		//-----------------------------------------------------------------
		//instead of reassigning memory on each iteration, just reset.
	
		sigmal.assign(numk, 0);
		sigmaa.assign(numk, 0);
		sigmab.assign(numk, 0);
		sigmax.assign(numk, 0);
		sigmay.assign(numk, 0);
		sigmaz.assign(numk, 0);
		clustersize.assign(numk, 0);

		for( int d = 0; d < m_depth; d++  )
		{
			int ind(0);
			for( int r = 0; r < m_height; r++ )
			{
				for( int c = 0; c < m_width; c++ )
				{
					sigmal[klabels[d][ind]] += m_lvecvec[d][ind];
					sigmaa[klabels[d][ind]] += m_avecvec[d][ind];
					sigmab[klabels[d][ind]] += m_bvecvec[d][ind];
					sigmax[klabels[d][ind]] += c;
					sigmay[klabels[d][ind]] += r;
					sigmaz[klabels[d][ind]] += d;

					clustersize[klabels[d][ind]] += 1.0;
					ind++;
				}
			}
		}

		{for( int k = 0; k < numk; k++ )
		{
			if( clustersize[k] <= 0 ) clustersize[k] = 1;
			inv[k] = 1.0/clustersize[k];//computing inverse now to multiply, than divide later
		}}
		
		{for( int k = 0; k < numk; k++ )
		{
			kseedsl[k] = sigmal[k]*inv[k];
			kseedsa[k] = sigmaa[k]*inv[k];
			kseedsb[k] = sigmab[k]*inv[k];
			kseedsx[k] = sigmax[k]*inv[k];
			kseedsy[k] = sigmay[k]*inv[k];
			kseedsz[k] = sigmaz[k]*inv[k];
		}}
	}
}

/*----------------------------------------------------------------------------------------------------------------*/
/**
*SaveSuperpixelLabels
*Save labels in raster scan order.
*保存超像素的labels(调试程序时使用)
*@param labels labels图块矩阵
*@param width  图像宽度
*@param height 图像高度
*@param filename 文件名
*@param path  保存路径
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::SaveSuperpixelLabels(
	 INT32*&					labels,
	const int&					width,
	const int&					height,
	const string&				filename,
	const string&				path) 
{ 
#if CUI_SAVE_LABLES 
#ifdef WINDOWS
        char fname[256];
        char extn[256];
        _splitpath(filename.c_str(), NULL, NULL, fname, extn);
        string temp = fname;
        string finalpath = path + temp + string(".dat");
#else
        string nameandextension = filename+cuiGetCurrentTime();
        size_t pos = filename.find_last_of("/");
        if(pos != string::npos)//if a slash is found, then take the filename with extension
        {
            nameandextension = filename.substr(pos+1);
        }
        string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
        string finalpath = path+newname;
#endif

        int sz = width*height;
	ofstream outfile;
	  outfile.open(finalpath.c_str(),ios::out);

	
	/* int **cui_label;*/
	INT cui_label_value;
	char data_t[10];
	/*cui_label=&labels;*/

	 for( int i = 0; i <height; i++ ){
		for( int j = 0; j <width; j++ ){
			cui_label_value=labels[i*width+j];
			sprintf(data_t,"%3d ",cui_label_value);

			outfile<<data_t;
		}
		outfile<<endl;
	}

	
	

	outfile.close(); 
#endif
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*将wstring类型的窄字符串，装换为string的宽字符
*@param  wideString 宽字符
*@param  窄字符串
*/
/*----------------------------------------------------------------------------------------------------------------*/
string SLIC::Wide2Narrow(const wstring&		wideString)
{
#if _MSC_VER
int m_codepage = ::_getmbcp();

	int result = ::WideCharToMultiByte( 
		m_codepage,  // Code page
		0,		// Default
		wideString.c_str(), // WCS buffer
		-1,		// Assume null terminated str, calclate length auto
		0,      // Buffer to receive MBCS string
		0,		// Length of MB buffer ( 0 -> return length required )
		0,		// lpdefaultChar
		0		// lpUsedDefaultChar
		);
	assert(result);
	char *test = new char[result+1]; 
	result = ::WideCharToMultiByte( 
		m_codepage,  // Code page
		0,		// Default
		wideString.c_str(), // WCS buffer
		-1,		// Assume null terminated str, calclate length auto
		test,   // Buffer to receive MBCS string
		result,	// Length of MB buffer ( 0 -> return length required )
		0,		// lpdefaultChar
		0		// lpUsedDefaultChar
		);

	std::string temp(test);
	delete []test;

	return temp;
#endif

#if linux||__linux||__linux__||__GNUC__
	ASSERT(0);
	std::string temp("linux_not_support");
    return  temp;
#endif

	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*将string类型的窄字符串，装换为wstring的宽字符
*@param  narrowString 窄字符
*@return  宽字符串
*/
/*----------------------------------------------------------------------------------------------------------------*/
wstring SLIC::Narrow2Wide(const std::string& narrowString)
{
#if _MSC_VER
int m_codepage = _getmbcp();

	int numChars =
		::MultiByteToWideChar( m_codepage, 
		MB_PRECOMPOSED, 
		narrowString.c_str(), 
		-1, 
		0, 
		0
		);
	assert(numChars);
	//	TRACE("Number of characters in the string is %d", numChars);

	wchar_t* test = new wchar_t[numChars+1];
	numChars =
		::MultiByteToWideChar( m_codepage, 
		MB_PRECOMPOSED, 
		narrowString.c_str(), 
		-1, 
		test, 
		numChars
		);

	std::wstring temp(test);
	delete []test;

	return temp;
#endif
#if linux||__linux||__linux__||__GNUC__
	ASSERT(0);
	std::wstring temp(L"linux_not_support");
	return  temp;
#endif
	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*建立连接矩阵E  \n
*从Lables中寻找并保存相邻矩阵E
*@param  SelfNighbour 自身不计算相邻（默认值为0）
*@param  filename  保存相邻矩阵的文件名
*@param  path   相邻矩阵E保存的路径
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveNighbour_E_matrix(
	const UINT32&                  SelfNighbour,
	const string&				filename,
	const string&				path)
{
#if 0
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	int mainindex(0);int cind(0);

	if (Cui_Matrix_E){
		delete []Cui_Matrix_E;
	}
	Cui_Matrix_E=new UINT32[pMD->slic_current_num*pMD->slic_current_num];
	memset(Cui_Matrix_E,0,pMD->slic_current_num*pMD->slic_current_num*sizeof(UINT32));

#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else
	string nameandextension = filename;
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif
	   for (register int ii=0;ii<pMD->slic_current_num;ii++) {
		     Cui_Matrix_E[ii*pMD->slic_current_num+ii]=SelfNighbour;
	   }

	for(register  int j = 0; j <CuiHeight; j++ ){
		for(register  int k = 0; k <CuiWidth; k++ ){
			int np(0);
			for(register  int i = 0; i < 8; i++ ){
				int x = k + dx8[i];
				int y = j + dy8[i];

				if( (x >= 0 && x < CuiWidth) && (y >= 0 && y <CuiHeight) ){
					int index = y*CuiWidth + x;

					//if( false == istaken[index] )//comment this to obtain internal contours
					{

						if( CuiImgLables[mainindex] != CuiImgLables[index] ) {
							//index与mainindex超像素图块具有连接关系
							UINT32  matrix_x=CuiImgLables[index];
							UINT32  matrix_y=CuiImgLables[mainindex];
							//统计连接个数
							Cui_Matrix_E[matrix_x*pMD->slic_current_num+matrix_y]=++Cui_Matrix_E[matrix_y*pMD->slic_current_num+matrix_x];
							  if (Cui_Matrix_E[matrix_x*pMD->slic_current_num+matrix_y]>999*2){
								  Cui_Matrix_E[matrix_x*pMD->slic_current_num+matrix_y]=Cui_Matrix_E[matrix_y*pMD->slic_current_num+matrix_x]=999*2;
							  }
						}
					}
				}
			}
			///////////////////////////////////////////////////
			////////////////////////////////////////////////////
			 mainindex++;
		}
	}
	/************************************************************/
	for(register  int i = 0; i <pMD->slic_current_num; i++ ){
		for(register  int j = 0; j <pMD->slic_current_num; j++ ){	
			//修正连接个数		
			Cui_Matrix_E[i*pMD->slic_current_num+j]=(Cui_Matrix_E[i*pMD->slic_current_num+j]+4)/6;
		}
		
	}
	{
	 	char data_t[1024];
	ofstream outfile;
	outfile.open(finalpath.c_str(),ios::out);
	for(register  int i = 0; i <pMD->slic_current_num; i++ ){
		for(register  int j = 0; j <pMD->slic_current_num; j++ ){
			UINT32 cui_label_value=Cui_Matrix_E[i*pMD->slic_current_num+j];
			sprintf(data_t,"%4d",cui_label_value);
			outfile<<data_t;
		}
		outfile<<endl;
	} 
	outfile.close();
	}

   /*************************************************************/
#if 0
	{
			double  threshold;		//计算超像素面积
			  threshold=4*sqrtl(CuiWidth*CuiHeight/pMD->slic_current_num);	//周长
			  threshold=threshold/8; //8分之一周长

			for( int i = 0; i <pMD->slic_current_num; i++ ){
					for( int j = 0; j <pMD->slic_current_num; j++ ){

						if (Cui_Matrix_E[i*pMD->slic_current_num+j]<threshold){
							Cui_Matrix_E[i*pMD->slic_current_num+j]=0;     
						}else{
							Cui_Matrix_E[i*pMD->slic_current_num+j]=1;    
						}			
					}

			}
	}
#else
	{
		double  threshold;		//计算超像素面积
		threshold=4*sqrtl(CuiWidth*CuiHeight/pMD->slic_current_num);	//周长
		threshold=threshold/8; //8分之一周长

		for(register  int i = 0; i <pMD->slic_current_num; i++ ){
			for(register  int j = 0; j <pMD->slic_current_num; j++ ){

				if (Cui_Matrix_E[i*pMD->slic_current_num+j]>0){
					//相邻则置为1 
					Cui_Matrix_E[i*pMD->slic_current_num+j]=1;     
				}else{
					//不相邻则置为0
					Cui_Matrix_E[i*pMD->slic_current_num+j]=0;    
				}			
			}

		}
	}
#endif
	{
		char data_t[1024];
	    ofstream outfile;
	    outfile.open("Matrix_E.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){
			for( int j = 0; j <pMD->slic_current_num; j++ ){
				UINT32 cui_label_value=Cui_Matrix_E[i*pMD->slic_current_num+j];
				sprintf(data_t,"%4d",cui_label_value);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
	
	/*************************************************************/
#endif
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif
	{
		if (Cui_Matrix_E){
			delete []Cui_Matrix_E;
		}
		Cui_Matrix_E=new UINT32[pMD->slic_current_num*pMD->slic_current_num];
		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(CuiImgLables,pMD->slic_current_num,Cui_Matrix_E,pMD->ImgWidth,pMD->ImgHeight,filename,path);
	}
#if _MSC_VER
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒

#endif
	
}

//===========================================================================
///	SaveSupervoxelLabels
///
///	Save labels in raster scan order.
//===========================================================================
void SLIC::SaveSupervoxelLabels(
	const int**&				labels,
	const int&					width,
	const int&					height,
	const int&					depth,
	const string&				filename,
	const string&				path) 
{
#ifdef WINDOWS
        char fname[256];
        char extn[256];
        _splitpath(filename.c_str(), NULL, NULL, fname, extn);
        string temp = fname;
        string finalpath = path + temp + string(".dat");
#else
        string nameandextension = filename;
        size_t pos = filename.find_last_of("/");
        if(pos != string::npos)//if a slash is found, then take the filename with extension
        {
            nameandextension = filename.substr(pos+1);
        }
        string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
        string finalpath = path+newname;
#endif

        int sz = width*height;
	ofstream outfile;
	outfile.open(finalpath.c_str(), ios::binary);
	for( int d = 0; d < depth; d++ )
	{
		for( int i = 0; i < sz; i++ )
		{
			outfile.write((const char*)&labels[d][i], sizeof(int));
		}
	}
	outfile.close();
}

//===========================================================================
///	EnforceLabelConnectivity
///
///		1. finding an adjacent label for each new component at the start
///		2. if a certain component is too small, assigning the previously found
///		    adjacent label to this component, and not incrementing the label.
//===========================================================================
void SLIC::EnforceLabelConnectivity(
	const int*					labels,//input labels that need to be corrected to remove stray labels
	const int					width,
	const int					height,
	int*&						nlabels,//new labels
	int&						numlabels,//the number of labels changes in the end if segments are removed
	const int&					K) //the number of superpixels desired by the user
{
//	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
//	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	const int dx4[4] = {-1,  0,  1,  0};
	const int dy4[4] = { 0, -1,  0,  1};

	const int sz = width*height;
	const int SUPSZ = sz/K;
	//nlabels.resize(sz, -1);
	for(register int i = 0; i < sz; i++ ) nlabels[i] = -1;
	int label(0);
	int* xvec = new int[sz];
	int* yvec = new int[sz];
	int oindex(0);
	int adjlabel(0);//adjacent label
	for(register int j = 0; j < height; j++ )
	{
		for(register int k = 0; k < width; k++ )
		{
			if( 0 > nlabels[oindex] )
			{
				nlabels[oindex] = label;
				//--------------------
				// Start a new segment
				//--------------------
				xvec[0] = k;
				yvec[0] = j;
				//-------------------------------------------------------
				// Quickly find an adjacent label for use later if needed
				//-------------------------------------------------------
				{for(register int n = 0; n < 4; n++ )
				{
					int x = xvec[0] + dx4[n];
					int y = yvec[0] + dy4[n];
					if( (x >= 0 && x < width) && (y >= 0 && y < height) )
					{
						int nindex = y*width + x;
						if(nlabels[nindex] >= 0) adjlabel = nlabels[nindex];
					}
				}}

				int count(1);
				for(register int c = 0; c < count; c++ )
				{
					for(register int n = 0; n < 4; n++ )
					{
						int x = xvec[c] + dx4[n];
						int y = yvec[c] + dy4[n];

						if( (x >= 0 && x < width) && (y >= 0 && y < height) )
						{
							int nindex = y*width + x;

							if( 0 > nlabels[nindex] && labels[oindex] == labels[nindex] )
							{
								xvec[count] = x;
								yvec[count] = y;
								nlabels[nindex] = label;
								count++;
							}
						}

					}
				}
				
#if TRUE
				//-------------------------------------------------------
				// If segment size is less then a limit, assign an
				// adjacent label found before, and decrement label count.
				//-------------------------------------------------------
				
				//图块尺寸小于超像素平均尺寸的二分之一
				if(count <= SUPSZ >> 2)
				{
					for(register int c = 0; c < count; c++ )
					{
						int ind = yvec[c]*width+xvec[c];
						nlabels[ind] = adjlabel;
					}
					label--;
				}
#endif

				label++;
			}
			oindex++;
		}
	}
	numlabels = label;

	if(xvec) delete [] xvec;
	if(yvec) delete [] yvec;
}


//===========================================================================
///	RelabelStraySupervoxels
//===========================================================================
void SLIC::EnforceSupervoxelLabelConnectivity(
	int**&						labels,//input - previous labels, output - new labels
	const int&					width,
	const int&					height,
	const int&					depth,
	int&						numlabels,
	const int&					STEP)
{
	const int dx10[10] = {-1,  0,  1,  0, -1,  1,  1, -1,  0, 0};
	const int dy10[10] = { 0, -1,  0,  1, -1, -1,  1,  1,  0, 0};
	const int dz10[10] = { 0,  0,  0,  0,  0,  0,  0,  0, -1, 1};

	int sz = width*height;
	const int SUPSZ = STEP*STEP*STEP;

	int adjlabel(0);//adjacent label
        int* xvec = new int[SUPSZ*10];//a large enough size
        int* yvec = new int[SUPSZ*10];//a large enough size
        int* zvec = new int[SUPSZ*10];//a large enough size
	//------------------
	// memory allocation
	//------------------
	int** nlabels = new int*[depth];
	{for( int d = 0; d < depth; d++ )
	{
		nlabels[d] = new int[sz];
		for( int i = 0; i < sz; i++ ) nlabels[d][i] = -1;
	}}
	//------------------
	// labeling
	//------------------
	int lab(0);
	{for( int d = 0; d < depth; d++ )
	{
		int i(0);
		for( int h = 0; h < height; h++ )
		{
			for( int w = 0; w < width; w++ )
			{
				if(nlabels[d][i] < 0)
				{
					nlabels[d][i] = lab;
					//-------------------------------------------------------
					// Quickly find an adjacent label for use later if needed
					//-------------------------------------------------------
					{for( int n = 0; n < 10; n++ )
					{
						int x = w + dx10[n];
						int y = h + dy10[n];
						int z = d + dz10[n];
						if( (x >= 0 && x < width) && (y >= 0 && y < height) && (z >= 0 && z < depth) )
						{
							int nindex = y*width + x;
							if(nlabels[z][nindex] >= 0)
							{
								adjlabel = nlabels[z][nindex];
							}
						}
					}}
					
					xvec[0] = w; yvec[0] = h; zvec[0] = d;
					int count(1);
					for( int c = 0; c < count; c++ )
					{
						for( int n = 0; n < 10; n++ )
						{
							int x = xvec[c] + dx10[n];
							int y = yvec[c] + dy10[n];
							int z = zvec[c] + dz10[n];

							if( (x >= 0 && x < width) && (y >= 0 && y < height) && (z >= 0 && z < depth))
							{
								int nindex = y*width + x;

								if( 0 > nlabels[z][nindex] && labels[d][i] == labels[z][nindex] )
								{
									xvec[count] = x;
									yvec[count] = y;
									zvec[count] = z;
									nlabels[z][nindex] = lab;
									count++;
								}
							}

						}
					}
					//-------------------------------------------------------
					// If segment size is less then a limit, assign an
					// adjacent label found before, and decrement label count.
					//-------------------------------------------------------
					if(count <= (SUPSZ >> 2))//this threshold can be changed according to needs
					{
						for( int c = 0; c < count; c++ )
						{
							int ind = yvec[c]*width+xvec[c];
							nlabels[zvec[c]][ind] = adjlabel;
						}
						lab--;
					}
					//--------------------------------------------------------
					lab++;
				}
				i++;
			}
		}
	}}
	//------------------
	// mem de-allocation
	//------------------
	{for( int d = 0; d < depth; d++ )
	{
		for( int i = 0; i < sz; i++ ) labels[d][i] = nlabels[d][i];
	}}
	{for( int d = 0; d < depth; d++ )
	{
		delete [] nlabels[d];
	}}
	delete [] nlabels;
	//------------------
	if(xvec) delete [] xvec;
	if(yvec) delete [] yvec;
	if(zvec) delete [] zvec;
	//------------------
	numlabels = lab;
	//------------------
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*迭代执行超像素算法\n
*十次迭代+去除孤立区域
*
*@param             ubuff     图像数据
*@param				width     图像宽度
*@param				height    图像高度
*@param				klabels   标定信息
*@param				numlabels 图像个数
*@param				superpixelsize  超像素块的平均大小
*@param             compactness    密度因素
*
*
*<pre>
*===========================================================================
*	DoSuperpixelSegmentation_ForGivenSuperpixelSize
*
* The input parameter ubuff conains RGB values in a 32-bit unsigned integers
* as follows:
*
* [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]
*
*        Nothing              R                 G                  B
*
* The RGB values are accessed from (and packed into) the unsigned integers
* using bitwise operators as can be seen in the function DoRGBtoLABConversion().
*
* compactness value depends on the input pixels values. For instance, if
* the input is greyscale with values ranging from 0-100, then a compactness
* value of 20.0 would give good results. A greater value will make the
* superpixels more compact while a smaller value would make them more uneven.
*
* The labels can be saved if needed using SaveSuperpixelLabels()
*===========================================================================
*</pre>
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::DoSuperpixelSegmentation_ForGivenSuperpixelSize(
    const unsigned int*         ubuff,
	const int					width,
	const int					height,
	INT32*&						klabels,
	int&						numlabels,
    const int&					superpixelsize,
    const double&               compactness)
{
#if 0
double*  m_lvec;
	double*  m_avec;
	double*  m_bvec;
    //------------------------------------------------
    const int STEP = sqrt(double(superpixelsize))+0.5;
    //------------------------------------------------
	//--------------------------------------------------
	m_width  = width;
	m_height = height;
	int sz = m_width*m_height;
	//klabels.resize( sz, -1 );
	//--------------------------------------------------
	klabels = new INT32[sz];
	for( int s = 0; s < sz; s++ ) 
		klabels[s] = -1;
    //--------------------------------------------------
    if(1)//LAB, the default option
    {
        DoRGBtoLABConversion(ubuff, m_lvec, m_avec, m_bvec);
    }
    else//RGB
    {
        m_lvec = new double[sz]; m_avec = new double[sz]; m_bvec = new double[sz];
        for( int i = 0; i < sz; i++ )
        {
                m_lvec[i] = ubuff[i] >> 16 & 0xff;
                m_avec[i] = ubuff[i] >>  8 & 0xff;
                m_bvec[i] = ubuff[i]       & 0xff;
        }
    }
	//--------------------------------------------------
    bool perturbseeds(false);//perturb seeds is not absolutely necessary, one can set this flag to false
	vector<double> edgemag(0);
	if(perturbseeds) DetectLabEdges(m_lvec, m_avec, m_bvec, m_width, m_height, edgemag);
#if 0
	GetLABXYSeeds_ForGivenStepSize_Rectangle(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy,m_K, perturbseeds, edgemag);
#endif
#if 1
	GetLABXYSeeds_ForGivenStepSize(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, STEP, perturbseeds, edgemag);
#endif

#if 1
	PerformSuperpixelSLIC(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, klabels, STEP, edgemag,compactness);
#endif
#if 0
	PerformSuperpixelSLIC_CUINEW(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, klabels, STEP, edgemag,compactness);
#endif	
#if 0
	PerformSuperpixelSLIC_cuda(alpha,betta,gama,fai,
								kseedsl.size(),
								m_width,m_height,
								m_lvec,m_avec,m_bvec,
								kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),klabels,
								STEP,nullptr,compactness);
//	FillImgWithSeeds(kseedsl,kseedsa,kseedsb,kseedsx,kseedsy);
#endif
	numlabels = kseedsl.size();

#if 1
	int* nlabels = new int[sz];
	EnforceLabelConnectivity(klabels, m_width, m_height, nlabels, numlabels, double(sz)/double(STEP*STEP));
		
	{
		for(int i = 0; i < sz; i++ )
		klabels[i] = nlabels[i];
	}
	if(nlabels) delete [] nlabels;
#endif
#endif
	
	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxy(
	const unsigned int*         ubuff,
	const int					width,
	const int					height,
	INT32*&						klabels,
	int&						numlabels,
	const int&					superpixelsize)
{
	//------------------------------------------------
	const int STEP = sqrt(double(superpixelsize))+0.5;
	//------------------------------------------------
	//--------------------------------------------------
	m_width  = width;
	m_height = height;
	int sz = m_width*m_height;
	//klabels.resize( sz, -1 );
	//--------------------------------------------------
	
	for( int s = 0; s < sz; s++ ) 
		klabels[s] = -1;
	//--------------------------------------------------

	if(1)//LAB, the default option
	{
		
	}
	//--------------------------------------------------
	bool perturbseeds(false);//perturb seeds is not absolutely necessary, one can set this flag to false
	vector<double> edgemag(0);
#if 0
if(perturbseeds)
		DetectLabEdges(m_lvec, m_avec, m_bvec, m_width, m_height, edgemag);
#endif
	
#if TRUE
    pIMD->GetThetaMLXYSeeds_ForGivenStepSize_Rectangle(perturbseeds,edgemag);
#endif	
#if 1
	PerformSuperpixelSLIC_ThetaMLXY_cuda(
		alpha,betta,gama,fai,
		 numlabels,
		 pIMD->ImgWidth,   
		 pIMD->ImgHeight,
		 pIMD->sita_n,
		 pIMD->m_n,
		 pIMD->L_n,
		 pIMD->X_n,
		 pIMD->Y_n,
		 pIMD->kseedsTheta.data(),
		 pIMD->kseedsM.data(),
		 pIMD->kseedsL.data(),
		 pIMD->kseedsX.data(),
		 pIMD->kseedsY.data(),
		 klabels,
		 STEP);
	/*PerformSuperpixelSLIC_cuda(
		alpha,betta,gama,fai,
		kseedsl.size(),
		m_width,m_height,
		m_lvec,m_avec,m_bvec,
		kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),klabels,
		STEP,nullptr,compactness);*/
#endif
	
#if 1
	int* nlabels = new int[sz];
	EnforceLabelConnectivity(klabels, m_width, m_height, nlabels, numlabels, double(sz)/double(STEP*STEP));

	{
		for(int i = 0; i < sz; i++ )
			klabels[i] = nlabels[i];
	}
	if(nlabels) delete [] nlabels;
#endif
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxyIncompletion(
	const unsigned int*         ubuff,
	const int					width,
	const int					height,
	INT32*&						klabels,
	int&						numlabels,
	const int&					superpixelsize)
{
	TRACE_FUNC();
	//------------------------------------------------
	const int STEP = sqrt(double(superpixelsize))+0.5;
	//------------------------------------------------
	//--------------------------------------------------
	m_width  = width;
	m_height = height;
	int sz = m_width*m_height;
	//--------------------------------------------------
	bool perturbseeds(false);//perturb seeds is not absolutely necessary, one can set this flag to false
	vector<double> edgemag(0);
	/*if(perturbseeds) DetectLabEdges(m_lvec, m_avec, m_bvec, m_width, m_height, edgemag);*/
#if TRUE
	GetLABXYSeeds_ForGivenStepSize_Rectangle2(
		pIMD->kseedsl,
		pIMD->kseedsa,
		pIMD->kseedsb, 
		pIMD->kseedsx, 
		pIMD->kseedsy,
		numlabels, 
		perturbseeds,
		edgemag,
		pIMD->m_lvec,
		pIMD->m_avec,
		pIMD->m_bvec);
#endif

#if 1
	PerformSuperpixelSLIC_cuda(
		alpha,betta,gama,fai,
		numlabels,
		m_width,m_height,
		pIMD->m_lvec,
		pIMD->m_avec,
		pIMD->m_bvec,
		pIMD->kseedsl.data(),
		pIMD->kseedsa.data(),
		pIMD->kseedsb.data(), 
		pIMD->kseedsx.data(), 
		pIMD->kseedsy.data(),
		klabels,
		STEP,
		nullptr,
		0);
	//	FillImgWithSeeds(kseedsl,kseedsa,kseedsb,kseedsx,kseedsy);
#endif
	

#if 1
	int* nlabels = new int[sz];
	EnforceLabelConnectivity(
		klabels,
		m_width, 
		m_height, 
		nlabels, 
		numlabels,
		double(sz)/double(STEP*STEP));

	{
		for(register int i = 0; i < sz; i++ )
			klabels[i] = nlabels[i];
	}
	if(nlabels) delete [] nlabels;
#endif
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*执行超像素运算
*
*@param     [in]       ubuff         图像数据
*@param				   width         图像宽度
*@param				   height        图块高度
*@param		[out]	   klabels       图像标定信息
*@param		[in,out]   numlabels     图块（超像素）个数
*@param		[in,out]   K             未使用
*@param                compactness   密度因素（未使用）
*
*<pre>
*===========================================================================
*	DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels
*
* The input parameter ubuff conains RGB values in a 32-bit unsigned integers
* as follows:
*
* [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]
*
*        Nothing              R                 G                  B
*
* The RGB values are accessed from (and packed into) the unsigned integers
* using bitwise operators as can be seen in the function DoRGBtoLABConversion().
*
* compactness value depends on the input pixels values. For instance, if
* the input is greyscale with values ranging from 0-100, then a compactness
* value of 20.0 would give good results. A greater value will make the
* superpixels more compact while a smaller value would make them more uneven.
*
* The labels can be saved if needed using SaveSuperpixelLabels()
*===========================================================================
*</pre>
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy(
    const UINT32*               ubuff,
	const int					width,
	const int					height,
	int*&						klabels,
	int&						numlabels)
{
	TRACE_FUNC();
    const int superpixelsize = 0.5+double(width*height)/double(numlabels);
#if 0
 DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxy(
	 ubuff,
	 width,
	 height,
	 klabels,
	 numlabels,
	 superpixelsize);
#endif
#if 1
DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxyIncompletion(
	ubuff,
	width,height,
	klabels,
	numlabels,
	superpixelsize);
#endif
	
   
}
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(
	const UINT32*               ubuff,
	const int					width,
	const int					height,
	int*&						klabels,
	int&						numlabels,
	const int&					K,//required number of superpixels
	const double&               compactness)//weight given to spatial distance
{
	const int superpixelsize = 0.5+double(width*height)/double(K);
	m_K=K;
	DoSuperpixelSegmentation_ForGivenSuperpixelSize(ubuff,width,height,klabels,numlabels,superpixelsize,compactness);
}
//===========================================================================
//	DoSupervoxelSegmentation
//
// There is option to save the labels if needed.
//
// The input parameter ubuffvec holds all the video frames. It is a
// 2-dimensional array. The first dimension is depth and the second dimension
// is pixel location in a frame. For example, to access a pixel in the 3rd
// frame (i.e. depth index 2), in the 4th row (i.e. height index 3) on the
// 37th column (i.e. width index 36), you would write:
//
// unsigned int the_pixel_i_want = ubuffvec[2][3*width + 36]
//
// In addition, here is how the RGB values are contained in a 32-bit unsigned
// integer:
//
// [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]  [1 1 1 1 1 1 1 1]
//
//        Nothing              R                 G                  B
//
// The RGB values are accessed from (and packed into) the unsigned integers
// using bitwise operators as can be seen in the function DoRGBtoLABConversion().
//
// compactness value depends on the input pixels values. For instance, if
// the input is greyscale with values ranging from 0-100, then a compactness
// value of 20.0 would give good results. A greater value will make the
// supervoxels more compact while a smaller value would make them more uneven.
//===========================================================================
void SLIC::DoSupervoxelSegmentation(
	unsigned int**&				ubuffvec,
	const int&					width,
	const int&					height,
	const int&					depth,
	int**&						klabels,
	int&						numlabels,
    const int&					supervoxelsize,
    const double&               compactness)
{
    //---------------------------------------------------------
    const int STEP = 0.5 + pow(double(supervoxelsize),1.0/3.0);
    //---------------------------------------------------------
	vector<double> kseedsl(0);
	vector<double> kseedsa(0);
	vector<double> kseedsb(0);
	vector<double> kseedsx(0);
	vector<double> kseedsy(0);
	vector<double> kseedsz(0);

	//--------------------------------------------------
	m_width  = width;
	m_height = height;
	m_depth  = depth;
	int sz = m_width*m_height;
	
	//--------------------------------------------------
        //klabels = new int*[depth];
	m_lvecvec = new double*[depth];
	m_avecvec = new double*[depth];
	m_bvecvec = new double*[depth];
	for( int d = 0; d < depth; d++ )
	{
                //klabels[d] = new int[sz];
		m_lvecvec[d] = new double[sz];
		m_avecvec[d] = new double[sz];
		m_bvecvec[d] = new double[sz];
		for( int s = 0; s < sz; s++ )
		{
			klabels[d][s] = -1;
		}
	}
	
	DoRGBtoLABConversion(ubuffvec, m_lvecvec, m_avecvec, m_bvecvec);

	GetKValues_LABXYZ(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, kseedsz, STEP);

	PerformSupervoxelSLIC(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, kseedsz, klabels, STEP, compactness);

	EnforceSupervoxelLabelConnectivity(klabels, width, height, depth, numlabels, STEP);
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的路径获取图像数据
*设置文件的保存位置
*@param filename 文件路径
*@param  filesavepath 文件保存位置
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiGetImageData(string filename,string filesavepath)
{
/**********************************************/
	if (CuiImgData) delete []CuiImgData;
//	if (CuiImgLables) delete[]CuiImgLables;

	if (Cui_Matrix_E) delete[]Cui_Matrix_E;
	if (Cui_Matrix_D) delete[]Cui_Matrix_D;
	if (Cui_Matrix_W) delete[]Cui_Matrix_W;
	if (Cui_Matrix_L) delete[]Cui_Matrix_L;
	if (Cui_MatrixEigenVector_L) delete[]Cui_MatrixEigenVector_L;
	if (CUi_MatrixEigenValue_L)  delete[]CUi_MatrixEigenValue_L;
	if (Cui_Matrix_Category_Simple)  delete[]Cui_Matrix_Category_Simple;
	if (Cui_Matrix_Category_Lable)  delete[]Cui_Matrix_Category_Lable;

	/**********************************************************/
	if (CuiCvImage)  cvReleaseImage(&CuiCvImage);
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	if (Cui_lab_hsv_Image)  cvReleaseImage(&Cui_lab_hsv_Image);
	if (h_l_plane)  cvReleaseImage(&h_l_plane);
	if (s_a_plane)  cvReleaseImage(&s_a_plane);
	if (v_b_plane)  cvReleaseImage(&v_b_plane);
	 //////////////////////////////////////////////////
	/* pMD->FileWritePath=filesavepath;
	 pMD->FileReadFullPath=filename;*/
	this->FileReadFullPath=filename;
	this->FileWritePath=filesavepath;
#if TRUE
	 //resize 图片尺寸到规范大小
	 {
		 CuiCvImage=cvLoadImage(filename.c_str()); 
		 IplImage* CuiCvImage_new=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,CuiCvImage->nChannels);
		 cvResize(CuiCvImage,CuiCvImage_new);
		 cvReleaseImage(&CuiCvImage);
		 CuiCvImage=cvCloneImage(CuiCvImage_new);
		 cvReleaseImage(&CuiCvImage_new);
	 }
	
#endif
	

	/*********************************************/	
	Cui_ARGB_Image=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,4);   
    cvCvtColor(CuiCvImage,Cui_ARGB_Image,CV_BGR2BGRA );
	CuiWidth=Cui_ARGB_Image->width;
	CuiHeight=Cui_ARGB_Image->height;
	CuiImgData= new UINT32[CuiWidth*CuiHeight];
	memcpy(CuiImgData,Cui_ARGB_Image->imageData,sizeof(UINT)*CuiWidth*CuiHeight);
	/*********************************************/
	Cui_lab_hsv_Image=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,3);	
	h_l_plane=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,1);
	s_a_plane=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,1);
	v_b_plane=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,1);	
	cvCvtColor(CuiCvImage,Cui_lab_hsv_Image,CV_BGR2Lab);
	cvCvtPixToPlane(Cui_lab_hsv_Image,h_l_plane,s_a_plane,v_b_plane,NULL);
	/*On output 0≤L≤100, -127≤a≤127, -127≤b≤127 
		The values are then converted to the destination data type: 
	8-bit images: 
	L <- L*255/100, a <- a + 128, b <- b + 128 
		16-bit images are currently not supported 
		32-bit images: 
	L, a, b are left as is 	 */
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的路径获取图像数据
*设置文件的保存位置
*@param filename 文件路径
*@param  filesavepath 文件保存位置
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiGetImageData(void)
{
#if 1
	/**********************************************/
	if (CuiImgData) delete []CuiImgData;
	if (CuiImgLables) delete[]CuiImgLables;
	if (Cui_Matrix_E) delete[]Cui_Matrix_E;
	if (Cui_Matrix_D) delete[]Cui_Matrix_D;
	if (Cui_Matrix_W) delete[]Cui_Matrix_W;
	if (Cui_Matrix_L) delete[]Cui_Matrix_L;
	if (Cui_MatrixEigenVector_L) delete[]Cui_MatrixEigenVector_L;
	if (CUi_MatrixEigenValue_L)  delete[]CUi_MatrixEigenValue_L;
	if (Cui_Matrix_Category_Simple)  delete[]Cui_Matrix_Category_Simple;
	if (Cui_Matrix_Category_Lable)  delete[]Cui_Matrix_Category_Lable;
	/**********************************************************/
	if (CuiCvImage)  cvReleaseImage(&CuiCvImage);
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	if (Cui_lab_hsv_Image)  cvReleaseImage(&Cui_lab_hsv_Image);
	if (h_l_plane)  cvReleaseImage(&h_l_plane);
	if (s_a_plane)  cvReleaseImage(&s_a_plane);
	if (v_b_plane)  cvReleaseImage(&v_b_plane);	
	this->FileReadFullPath=pMD->FileReadFullPath;
	this->FileWritePath=pMD->FileWritePath;
#endif
#if 0
	this->CuiGetImageData(pMD->FileReadFullPath,pMD->FileWritePath);
#endif
	
#if TRUE
	 {
		 CuiCvImage=cvLoadImage(pMD->FileReadFullPath.c_str()); 
		 IplImage* CuiCvImage_new=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,CuiCvImage->nChannels);
		 cvResize(CuiCvImage,CuiCvImage_new);
		 cvReleaseImage(&CuiCvImage);
		 CuiCvImage=cvCloneImage(CuiCvImage_new);
		 cvReleaseImage(&CuiCvImage_new);
	 }	
#endif
	/*********************************************/	
	Cui_ARGB_Image=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,4);   
    cvCvtColor(CuiCvImage,Cui_ARGB_Image,CV_BGR2BGRA );
	CuiWidth=Cui_ARGB_Image->width;
	CuiHeight=Cui_ARGB_Image->height;
	CuiImgData= new UINT32[CuiWidth*CuiHeight];
	memcpy(CuiImgData,Cui_ARGB_Image->imageData,sizeof(UINT)*CuiWidth*CuiHeight);
	/*********************************************/
	Cui_lab_hsv_Image=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,3);	
	h_l_plane=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,1);
	s_a_plane=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,1);
	v_b_plane=cvCreateImage(cvGetSize(CuiCvImage),IPL_DEPTH_8U,1);	
	cvCvtColor(CuiCvImage,Cui_lab_hsv_Image,CV_BGR2Lab);
	cvCvtPixToPlane(Cui_lab_hsv_Image,h_l_plane,s_a_plane,v_b_plane,NULL);
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的路径获取图像数据
*设置文件的保存位置
*@param filename 文件路径
*@param  filesavepath 文件保存位置
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiGetImageDataPIMD(void)
{
	TRACE_FUNC();
#if 1
	/**********************************************/
	if (CuiImgData) delete []CuiImgData;
	if (CuiImgLables) delete[]CuiImgLables;
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	/**********************************************************/
	this->FileReadFullPath=pIMD->FileReadFullPath;
	this->FileWritePath=pIMD->FileWritePath;
#endif

#if TRUE
		
	
	Cui_ARGB_Image=cvCloneImage(pIMD->srcCv_ImgBGRA);
	
			
#endif
	/*********************************************/	
	
	CuiWidth=pIMD->ImgWidth;
	CuiHeight=pIMD->ImgHeight;

	CuiImgData= new UINT32[CuiWidth*CuiHeight];
	memcpy(CuiImgData,pIMD->src_ImgBGRA,sizeof(UINT)*CuiWidth*CuiHeight);
	/*********************************************/
	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*输入超像素分割参数完成超像素分割
*
*@param K 期望分割的超像素个数
*@param compactness 密度因子
*@param savelable 是否保存Lable图像矩阵（默认为false）
*
*@note 对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像；\n
*采用Achanta R提出的简单线性迭代聚类方法即SLIC (Simple Linear Iterative Clustering)，\n
*该方法以像素的CIELAB色彩空间的L、a、b值及像素的x，y轴坐标构建5维空间，并定义了规范化\n
*的距离测量方法Ds，具体定义如下：\n
*@image html image004.png 
*@image html image005.png 
*@image html image006.png 
*@image html image007.png 
* 其中：
*@image html image008.png 
*为聚类的中心；
*@image html image009.png 
*为图像像素点的5维空间坐标；N为图像的像素数；K为期望获得的超像素的数目；S为超像素中心栅格间距；\n
*Ds为色彩lab距离dlab和dxy基于S的规范化距离；m为可控的超像素密度因子。\n
*time==3.7292521742394231s 2014年12月22日
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels( int& K, double&  compactness,int savelable)
{
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif

#if 1
{
			ASSERT(pMD->ImgHeight==this->CuiHeight);
			ASSERT(pMD->ImgWidth==this->CuiWidth);
			int sz = CuiWidth*CuiHeight;
			//---------------------------------------------------------
			if(K < 20 || K > sz/4) K = sz/200;//i.e the default size of the superpixel is 200 pixels
			if(compactness < 1.0 || compactness > 80.0) compactness = 20.0;
			//---------------------------------------------------------		
			if (CuiImgLables){
				delete [] CuiImgLables;
				CuiImgLables=NULL;
			}
			//CuiImgLables= new int[CuiWidth*CuiHeight];
			DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(CuiImgData,CuiWidth,CuiHeight,CuiImgLables,pMD->slic_current_num, K, compactness);
}
#endif
			if (savelable){
					this->SaveSuperpixelLabels(CuiImgLables,CuiWidth,CuiHeight);
			}
			{
				std::stringstream ss;
				std::string str_add;
				ss<<"_Num_";
				ss<<K;
				ss<<"_Factor_" ;
				ss<<compactness;
#if TRUE
				ss<<"_Fai_";
				ss<<alpha*10;
#endif
				ss>>str_add;
				cui_GeneralImgProcess::CuiSaveImgWithContours(CuiImgData,CuiImgLables,CuiWidth,CuiHeight,FileReadFullPath,FileWritePath,str_add);
			}
#if _MSC_VER
	        QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;//获得中止值
			dfMinus = (double)(QPart2-QPart1);
			dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
			TRACE("\n 超像素计算时间: %f（秒）",dfTim);
			/*************************************************************/
#endif
		
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*输入超像素分割参数完成超像素分割
*
*@param K 期望分割的超像素个数
*@param savelable 是否保存Lable图像矩阵（默认为false）
*
*@note 对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像；\n
*
**/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy(int savelable)
{
	TRACE_FUNC();
#if _MSC_VER&&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	/*double compactness=8;*/
#endif
	{
		ASSERT(pIMD->ImgHeight==this->CuiHeight);
		ASSERT(pIMD->ImgWidth==this->CuiWidth);
		int sz = CuiWidth*CuiHeight;
		//---------------------------------------------------------
		//if(K < 20 || K > sz/4) K = sz/200;//i.e the default size of the superpixel is 200 pixels
		
	/*	if(compactness < 1.0 || compactness > 80.0) compactness = 20.0;*/
		//---------------------------------------------------------		
		
		//CuiImgLables= new int[CuiWidth*CuiHeight];
		DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy(
			CuiImgData,
			CuiWidth,CuiHeight,
			pIMD->src_ImgLabels,
			pIMD->slic_current_num);

	}
#if _DEBUG
	if (savelable){
		this->SaveSuperpixelLabels(pIMD->src_ImgLabels,CuiWidth,CuiHeight);
	}
#endif


	std::string str_add;
#if _DEBUG
	{
		std::stringstream ss;
		ss<<"_Num_";
		ss<<pIMD->slic_expect_num;
		ss<<"_Factor_" ;
		ss<<1;
		ss<<"_Fai_";
		ss<<alpha*10;
		ss>>str_add;		
	}	
#endif
#if	SaveContours2Disk
	pIMD->SaveImgWithContours(str_add);
#endif
	/////////////////////////////////////////////////////
#if _MSC_VER&&_DEBUG
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 超像素计算时间: %f（秒）",dfTim);
#endif	
	/*************************************************************/
	pIMD->initMemData(pIMD->src_ImgLabels);
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据相邻矩阵计算超像素块之间的相似度计算相似矩阵W
*并保存到硬盘
*@param filename 要保存的文件名
*@param path    要保存的文件路径
*
*@note 将图像的色彩空间转换到CIELab空间，并将L通道的取值范围划分为8级等分，\n
*a通道的取值范围划分为16等分，将b 通道的取值范围划分成16个等级，其中将L通道\n
*取值范围划分为8级的目的是降低色彩亮度变化对权值的扰动，每个超像素在8×16×16=2048\n
*维度的空间计算值直方图\n
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveSimilar_W_matrix2(const string& filename,const string&	path)
{
	TRACE_FUNC();
#if OUT_DOOR
	assert(pIMD->slic_current_num<MAX_SP_NUM);
#endif
	/****************************************************************/
	CvHistogram *hist[MAX_SP_NUM ];
	vector<unsigned char> hist_h_value[MAX_SP_NUM ];
	vector<unsigned char> hist_s_value[MAX_SP_NUM ];
	vector<unsigned char> hist_v_value[MAX_SP_NUM ];
	IplImage  *hist_h_matrix=NULL;
	IplImage  *hist_s_matrix=NULL;
	IplImage  *hist_v_matrix=NULL;
	/****************************************************************/
	{
		if (Cui_Matrix_W){
			delete []Cui_Matrix_W;
			Cui_Matrix_W=NULL;
		}
		Cui_Matrix_W=new double[pIMD->slic_current_num*pIMD->slic_current_num];
		memset(Cui_Matrix_W,0,pIMD->slic_current_num*pIMD->slic_current_num*sizeof(double));
	}
	/****************************************************************/

	for (register int spi=0;spi<pMD->slic_current_num;spi++){
		{
			int	hist_size[3]={Lab_L_Division,Lab_a_Division,Lab_b_Division};
			float h_l_ranges[]={0,256};
			float s_a_range[]={0,256};
			float v_b_range[]={0,256}; //亮度
			float *ranges[]={h_l_ranges,s_a_range,v_b_range};
			hist[spi]=cvCreateHist(3,hist_size,CV_HIST_ARRAY,ranges);

		}  
	}
	/****************************************************************/
	for (register int li=0;li<CuiHeight;li++) {
		for (register int lj=0;lj<CuiWidth;lj++){
			int sp=CuiImgLables[li*CuiWidth+lj];
				hist_h_value[sp].push_back((unsigned char)cvGetReal2D(h_l_plane,li,lj));
				hist_s_value[sp].push_back((unsigned char)cvGetReal2D(s_a_plane,li,lj));
				hist_v_value[sp].push_back((unsigned char)cvGetReal2D(v_b_plane,li,lj));

		}
	}
	for (register int sp=0;sp<pMD->slic_current_num;sp++){
		hist_h_matrix=cvCreateImage(cvSize(hist_h_value[sp].size(),1),IPL_DEPTH_8U,1);
		hist_s_matrix=cvCreateImage(cvSize(hist_s_value[sp].size(),1),IPL_DEPTH_8U,1);
		hist_v_matrix=cvCreateImage(cvSize(hist_v_value[sp].size(),1),IPL_DEPTH_8U,1);
		IplImage *hist_matrix[]={hist_h_matrix,hist_s_matrix,hist_v_matrix};
		memcpy(hist_h_matrix->imageData,hist_h_value[sp].data(),hist_h_value[sp].size()*sizeof(unsigned char));
		memcpy(hist_s_matrix->imageData,hist_s_value[sp].data(),hist_s_value[sp].size()*sizeof(unsigned char));
		memcpy(hist_v_matrix->imageData,hist_v_value[sp].data(),hist_v_value[sp].size()*sizeof(unsigned char));
		cvCalcHist(hist_matrix,hist[sp]);
		cvNormalizeHist(hist[sp],1.0);
		cvReleaseImage(&hist_h_matrix);
		cvReleaseImage(&hist_s_matrix);
		cvReleaseImage(&hist_v_matrix);
	}
	/****************************************************************/
	for(register int i = 0; i <pMD->slic_current_num; i++ ){
		for(register int j = i+1; j <pMD->slic_current_num; j++ ){

			if (0!=(UINT32)Cui_Matrix_E[i*pMD->slic_current_num+j]){	
				//i ,j超像素相邻				
				double B_distance=cvCompareHist(hist[i],hist[j],CV_COMP_BHATTACHARYYA);
				B_distance=1-powl(B_distance,2);
				B_distance=(B_distance<Color_histogram_B_Threshold)?0:B_distance;
				Cui_Matrix_W[j*pMD->slic_current_num+i]=Cui_Matrix_W[i*pMD->slic_current_num+j]=B_distance;
			}
		}
	}
	//////////////////////////////////////////////////////////////
	for (register int spi=0;spi<pMD->slic_current_num;spi++){
		cvReleaseHist(&hist[spi]);
	}
	/********************************************************/
#if _DEBUG
	cui_GeneralImgProcess::SaveMatrix_W(path,filename,pMD->slic_current_num,Cui_Matrix_W);
#endif
	/********************************************************/
}

/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据相邻矩阵计算超像素块之间的相似度计算相似矩阵W
*并保存到硬盘
*@param filename 要保存的文件名
*@param path    要保存的文件路径
*
*@note 将图像的色彩空间转换到CIELab空间，并将L通道的取值范围划分为8级等分，\n
*a通道的取值范围划分为16等分，将b 通道的取值范围划分成16个等级，其中将L通道\n
*取值范围划分为8级的目的是降低色彩亮度变化对权值的扰动，每个超像素在8×16×16=2048\n
*维度的空间计算值直方图\n
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveSimilar_W_matrix2_2016_09_26(const string& filename,const string&	path)
{
	TimeCountStart();
	TRACE_FUNC();
#if OUT_DOOR
	assert(pIMD->slic_current_num<MAX_SP_NUM);
#endif
	/****************************************************************/
	CvHistogram *hist[MAX_SP_NUM ];
	vector<unsigned char> hist_Theta_value[MAX_SP_NUM ];
	vector<unsigned char> hist_M_value[MAX_SP_NUM ];
	vector<unsigned char> hist_L_value[MAX_SP_NUM ];
	IplImage  *hist_Theta_matrix=NULL;
	IplImage  *hist_M_matrix=NULL;
	IplImage  *hist_L_matrix=NULL;
	/****************************************************************/
	{
		if (Cui_Matrix_W){
			delete []Cui_Matrix_W;
			Cui_Matrix_W=NULL;
		}
		Cui_Matrix_W=new double[pIMD->slic_current_num*pIMD->slic_current_num];
		memset(Cui_Matrix_W,0,pIMD->slic_current_num*pIMD->slic_current_num*sizeof(double));
	}
	/****************************************************************/

	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		{
#if OUT_DOOR_HUMAN
			int	hist_size[3]={ThetaML_Theta_Division,ThetaML_M_Division,ThetaML_L_Division};
			float Theta_ranges[]={0,256};
			float M_range[]={0,256};
			float L_range[]={0,256}; //亮度
			float *ranges[]={Theta_ranges,M_range,L_range};
			hist[spi]=cvCreateHist(3,hist_size,CV_HIST_ARRAY,ranges);
#else
			assert(FALSE);
#endif


		}  
	}
	/****************************************************************/
	for (register int li=0;li<pIMD->ImgHeight;li++) {
		for (register int lj=0;lj<pIMD->ImgWidth;lj++){
			int sp=pIMD->src_ImgLabels[li*pIMD->ImgWidth+lj];
#if OUT_DOOR_HUMAN
			int index_t=li*pIMD->ImgWidth+lj;
			
			{
				hist_Theta_value[sp].push_back((unsigned char) (pIMD->sita_n[index_t]*256));
			}

			{
					unsigned char m_t=(unsigned char) (pIMD->m_n[index_t]*256);
					m_t=(m_t>3)?255:0;
					hist_M_value[sp].push_back(m_t);
			}

			{
					hist_L_value[sp].push_back((unsigned char) (pIMD->L_n[index_t]*256));	
			}


#endif
		}
	}
	for (register int sp=0;sp<pIMD->slic_current_num;sp++){
		hist_Theta_matrix=cvCreateImage(cvSize(hist_Theta_value[sp].size(),1),IPL_DEPTH_8U,1);
		hist_M_matrix=cvCreateImage(cvSize(hist_M_value[sp].size(),1),IPL_DEPTH_8U,1);
		hist_L_matrix=cvCreateImage(cvSize(hist_L_value[sp].size(),1),IPL_DEPTH_8U,1);
		IplImage *hist_matrix[]={hist_Theta_matrix,hist_M_matrix,hist_L_matrix};
		memcpy(hist_Theta_matrix->imageData,hist_Theta_value[sp].data(),hist_Theta_value[sp].size()*sizeof(unsigned char));
		memcpy(hist_M_matrix->imageData,hist_M_value[sp].data(),hist_M_value[sp].size()*sizeof(unsigned char));
		memcpy(hist_L_matrix->imageData,hist_L_value[sp].data(),hist_L_value[sp].size()*sizeof(unsigned char));
		cvCalcHist(hist_matrix,hist[sp]);
		cvNormalizeHist(hist[sp],1.0);
		cvReleaseImage(&hist_Theta_matrix);
		cvReleaseImage(&hist_M_matrix);
		cvReleaseImage(&hist_L_matrix);
	}
	/****************************************************************/
	for(register int i = 0; i <pIMD->slic_current_num; i++ ){
		for(register int j = i+1; j <pIMD->slic_current_num; j++ ){

			if (0!=(UINT32)pIMD->Matrix_E[i*pIMD->slic_current_num+j]){	
				//i ,j超像素相邻				
				double B_distance=cvCompareHist(hist[i],hist[j],CV_COMP_BHATTACHARYYA);
				B_distance=1-powl(B_distance,2);
				B_distance=(B_distance<Color_histogram_B_Threshold)?0:B_distance;//<0.71 直接置0

				Cui_Matrix_W[j*pIMD->slic_current_num+i]=Cui_Matrix_W[i*pIMD->slic_current_num+j]=B_distance;
			}else{
				Cui_Matrix_W[j*pIMD->slic_current_num+i]=Cui_Matrix_W[i*pIMD->slic_current_num+j]=0;//不相邻，直接置0
			}
		}
	}
	//////////////////////////////////////////////////////////////
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		cvReleaseHist(&hist[spi]);
	}
	/********************************************************/
#if _DEBUG
	cui_GeneralImgProcess::SaveMatrix_W(path,filename,pIMD->slic_current_num,Cui_Matrix_W);
#endif
	/********************************************************/
	TimeCountStop("W matrix Cost Time : ");
}

/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据相似矩阵W 计算D（度矩阵）
*并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveDgeree_D_matrix(void)
{
	 if (Cui_Matrix_D){
		 delete []Cui_Matrix_D;
		 Cui_Matrix_D=NULL;
	 }
	 Cui_Matrix_D=new double[pMD->slic_current_num*pMD->slic_current_num];
	 memset(Cui_Matrix_D,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	 /**************************************/ 		
		   for (register int wi=0;wi<pMD->slic_current_num;wi++){
			   double sum=0;
			   for (register int wj=0;wj<pMD->slic_current_num;wj++){ 
				  	sum+=Cui_Matrix_W[wi*pMD->slic_current_num+wj];
			   }
			   Cui_Matrix_D[wi*pMD->slic_current_num+wi]=sum;
		   }	
		  
	 /***************************************/
#if _DEBUG
		   if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
			   char data_t[1024];
			   ofstream outfile;
			   outfile.open("Matrix_D.data",ios::out);
			   for(register int i = 0; i <pMD->slic_current_num; i++ ){
				   for(register int j = 0; j <pMD->slic_current_num; j++ ){
					   double value_t=Cui_Matrix_D[i*pMD->slic_current_num+j];
					   sprintf(data_t," %0.2e ",value_t);
					   outfile<<data_t;
				   }
				   outfile<<endl;
			   } 
			   outfile.close();
		   }
#endif
	/*****************************************/
		   CvMat D_Matrix_t;
		   cvInitMatHeader(&D_Matrix_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,  Cui_Matrix_D);
		   cvInvert(&D_Matrix_t,&D_Matrix_t,CV_SVD);
		   cvPow(&D_Matrix_t,&D_Matrix_t,0.5);
	 /******************************************/
#if _DEBUG
		   if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
			   char data_t[1024];
			   ofstream outfile;
			   outfile.open("Matrix_D-sqrt.data",ios::out);
			   for( int i = 0; i <pMD->slic_current_num; i++ ){
				   for( int j = 0; j <pMD->slic_current_num; j++ ){
					   double value_t=Cui_Matrix_D[i*pMD->slic_current_num+j];
					   sprintf(data_t," %0.2e ",value_t);
					   outfile<<data_t;
				   }
				   outfile<<endl;
			   } 
			   outfile.close();
		   }
#endif

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据相似矩阵W 计算D（度矩阵）
*并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveDgeree_D_matrix2016_09_26(void)
{
	if (Cui_Matrix_D){
		delete []Cui_Matrix_D;
		Cui_Matrix_D=NULL;
	}
	Cui_Matrix_D=new double[pIMD->slic_current_num*pIMD->slic_current_num];
	memset(Cui_Matrix_D,0,sizeof(double)*pIMD->slic_current_num*pIMD->slic_current_num);
	/**************************************/ 		
	for (register int wi=0;wi<pIMD->slic_current_num;wi++){
		double sum=0;
		for (register int wj=0;wj<pIMD->slic_current_num;wj++){ 
			sum+=Cui_Matrix_W[wi*pIMD->slic_current_num+wj];
		}
		Cui_Matrix_D[wi*pIMD->slic_current_num+wi]=sum;
	}	

	/***************************************/
#if _DEBUG
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_D.data",ios::out);
		for(register int i = 0; i <pIMD->slic_current_num; i++ ){
			for(register int j = 0; j <pIMD->slic_current_num; j++ ){
				double value_t=Cui_Matrix_D[i*pIMD->slic_current_num+j];
				sprintf(data_t," %0.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif
	/*****************************************/
	CvMat D_Matrix_t;
	cvInitMatHeader(&D_Matrix_t,pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1,  Cui_Matrix_D);
	cvInvert(&D_Matrix_t,&D_Matrix_t,CV_SVD);
	cvPow(&D_Matrix_t,&D_Matrix_t,0.5);
	/******************************************/
#if _DEBUG
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_D-sqrt.data",ios::out);
		for( int i = 0; i <pIMD->slic_current_num; i++ ){
			for( int j = 0; j <pIMD->slic_current_num; j++ ){
				double value_t=Cui_Matrix_D[i*pIMD->slic_current_num+j];
				sprintf(data_t," %0.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*采用Normalized-cut准则来计算规范化的Laplacian（拉普拉斯)矩阵L \n
*L_sym=I-D^((-1)⁄2) WD^((-1)⁄2)  \n
* 并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveLaplace_L_matrix(void)
{

  //////////////////////////////////////////////////////////////
   if (Cui_Matrix_L){
	  delete []Cui_Matrix_L;
	  Cui_Matrix_L=NULL;
  }
  Cui_Matrix_L=new double[pMD->slic_current_num*pMD->slic_current_num];
  memset(Cui_Matrix_L,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
   //////////////////////////////////////////////////////////////
  CvMat D_t,W_t,L_t;
  cvInitMatHeader(&D_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_D);
  cvInitMatHeader(&W_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_W);
  cvInitMatHeader(&L_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_L); 
#if 0	 
  cvSub(&D_t,&W_t,&L_t); 
#else
   CvMat *I_t,*cui_mat_t;
   cui_mat_t=cvCreateMat(pMD->slic_current_num,pMD->slic_current_num,CV_64FC1);
   I_t=cvCreateMat(pMD->slic_current_num,pMD->slic_current_num,CV_64FC1);
   cvSetIdentity(I_t);
   cvmMul(&D_t,&W_t,cui_mat_t);
   cvmMul(cui_mat_t,&D_t,&L_t);
   cvSub(I_t,&L_t,&L_t); 
   cvReleaseMat(&I_t);
   cvReleaseMat(&cui_mat_t);
#endif
/*************************/
#if _DEBUG
   if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
	   char data_t[1024];
	   ofstream outfile;
	   outfile.open("Matrix_L.data",ios::out);
	   for( int i = 0; i <pMD->slic_current_num; i++ ){
		   for( int j = 0; j <pMD->slic_current_num; j++ ){
			   double value_t=Cui_Matrix_L[i*pMD->slic_current_num+j];
			   sprintf(data_t," %0.2e ",value_t);
			   outfile<<data_t;
		   }
		   outfile<<endl;
	   } 
	   outfile.close();
   }
#endif


}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*采用Normalized-cut准则来计算规范化的Laplacian（拉普拉斯)矩阵L \n
*L_sym=I-D^((-1)⁄2) WD^((-1)⁄2)  \n
* 并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
/*void SLIC::CuiFindSaveLaplace_L_matrix_2016_09_26(void)
{

	//////////////////////////////////////////////////////////////
	if (Cui_Matrix_L){
		delete []Cui_Matrix_L;
		Cui_Matrix_L=NULL;
	}
	Cui_Matrix_L=new double[pIMD->slic_current_num*pIMD->slic_current_num];
	memset(Cui_Matrix_L,0,sizeof(double)*pMD->slic_current_num*pMD->slic_current_num);
	//////////////////////////////////////////////////////////////
	CvMat D_t,W_t,L_t;
	cvInitMatHeader(&D_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_D);
	cvInitMatHeader(&W_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_W);
	cvInitMatHeader(&L_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_L); 
#if 0	 
	cvSub(&D_t,&W_t,&L_t); 
#else
	CvMat *I_t,*cui_mat_t;
	cui_mat_t=cvCreateMat(pMD->slic_current_num,pMD->slic_current_num,CV_64FC1);
	I_t=cvCreateMat(pMD->slic_current_num,pMD->slic_current_num,CV_64FC1);
	cvSetIdentity(I_t);
	cvmMul(&D_t,&W_t,cui_mat_t);
	cvmMul(cui_mat_t,&D_t,&L_t);
	cvSub(I_t,&L_t,&L_t); 
	cvReleaseMat(&I_t);
	cvReleaseMat(&cui_mat_t);
#endif
	/*************************/
/*#if _DEBUG
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_L.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){
			for( int j = 0; j <pMD->slic_current_num; j++ ){
				double value_t=Cui_Matrix_L[i*pMD->slic_current_num+j];
				sprintf(data_t," %0.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif*/


//}*/
/*----------------------------------------------------------------------------------------------------------------*/
/**
*采用Normalized-cut准则来计算规范化的Laplacian（拉普拉斯)矩阵L \n
*L_sym=I-D^((-1)⁄2) WD^((-1)⁄2)  \n
* 并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSaveLaplace_L_matrix_2016_09_26(void)
{
	if (Cui_Matrix_L){
		delete []Cui_Matrix_L;
		Cui_Matrix_L=NULL;
	}
	Cui_Matrix_L=new double[pIMD->slic_current_num*pIMD->slic_current_num];
	memset(Cui_Matrix_L,0,sizeof(double)*pIMD->slic_current_num*pIMD->slic_current_num);
	//////////////////////////////////////////////////////////////
	CvMat D_t,W_t,L_t;
	cvInitMatHeader(&D_t,pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1,Cui_Matrix_D);
	cvInitMatHeader(&W_t,pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1,Cui_Matrix_W);
	cvInitMatHeader(&L_t,pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1,Cui_Matrix_L); 
#if 0	 
	cvSub(&D_t,&W_t,&L_t); 
#else
	CvMat *I_t,*cui_mat_t;
	cui_mat_t=cvCreateMat(pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1);
	I_t=cvCreateMat(pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1);
	cvSetIdentity(I_t);
	cvmMul(&D_t,&W_t,cui_mat_t);
	cvmMul(cui_mat_t,&D_t,&L_t);
	cvSub(I_t,&L_t,&L_t); 
	cvReleaseMat(&I_t);
	cvReleaseMat(&cui_mat_t);
#endif
	/*************************/
#if _DEBUG
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_L.data",ios::out);
		for( int i = 0; i <pIMD->slic_current_num; i++ ){
			for( int j = 0; j <pIMD->slic_current_num; j++ ){
				double value_t=Cui_Matrix_L[i*pIMD->slic_current_num+j];
				sprintf(data_t," %0.2e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif


}
/*----------------------------------------------------------------------------------------------------------------*/
/**
* 利用公式计算L（拉普拉斯）矩阵的特征值、征向量 \n
* 并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSave_L_Eigenvalue(void)
{
	/////////////特征向量///////////////////////////////////////////////////////////////////
	if (Cui_MatrixEigenVector_L){
		delete []Cui_MatrixEigenVector_L;
		Cui_MatrixEigenVector_L=NULL;
	}
	Cui_MatrixEigenVector_L=new float[pMD->slic_current_num*pMD->slic_current_num];
	memset(Cui_MatrixEigenVector_L,0,sizeof(float)*pMD->slic_current_num*pMD->slic_current_num);
	/////////////特征值///////////////////////////////////////////////////////////////////
	if (CUi_MatrixEigenValue_L){
		delete []CUi_MatrixEigenValue_L;
		CUi_MatrixEigenValue_L=NULL;
	}
	CUi_MatrixEigenValue_L=new double[pMD->slic_current_num];
	memset(CUi_MatrixEigenValue_L,0,sizeof(double)*pMD->slic_current_num);
	////////////////////////////////////////////////////////////////////////////////
	CvMat E_vector_t,E_value_t,L_t;
	cvInitMatHeader(&E_vector_t,pMD->slic_current_num,pMD->slic_current_num,CV_32FC1,Cui_MatrixEigenVector_L);
	cvInitMatHeader(&E_value_t,pMD->slic_current_num,1,CV_64FC1,CUi_MatrixEigenValue_L);
	cvInitMatHeader(&L_t,pMD->slic_current_num,pMD->slic_current_num,CV_64FC1,Cui_Matrix_L); 
	cvEigenVV(&L_t,&E_vector_t,&E_value_t);//C是A的特征值(降序排列)，而B则是A的特征向量(每行)
	cvInvert(&E_vector_t,&E_vector_t);
	/******************************************/
#if 0
	cui_GeneralImgProcess::SaveMatrix_Float("","Matrix_L_Vector.data",pMD->slic_current_num,Cui_MatrixEigenVector_L);
	cui_GeneralImgProcess::SaveMatrix_W("","Matrix_L_Value.data",pMD->slic_current_num,CUi_MatrixEigenValue_L);
#endif
	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
* 利用公式计算L（拉普拉斯）矩阵的特征值、征向量 \n
* 并保存矩阵到硬盘
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC::CuiFindSave_L_Eigenvalue_2016_09_26(void)
{
	TRACE_FUNC();
#if _MSC_VER&&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	TRACE("\n开始求解矩阵特征值、特征向量");
#endif
	TimeCountStart();
	/////////////特征向量///////////////////////////////////////////////////////////////////
	if (Cui_MatrixEigenVector_L){
		delete []Cui_MatrixEigenVector_L;
		Cui_MatrixEigenVector_L=NULL;
	}
	Cui_MatrixEigenVector_L=new float[pIMD->slic_current_num*pIMD->slic_current_num];
	memset(Cui_MatrixEigenVector_L,0,sizeof(float)*pIMD->slic_current_num*pIMD->slic_current_num);
	/////////////特征值///////////////////////////////////////////////////////////////////
	if (CUi_MatrixEigenValue_L){
		delete []CUi_MatrixEigenValue_L;
		CUi_MatrixEigenValue_L=NULL;
	}
	CUi_MatrixEigenValue_L=new double[pIMD->slic_current_num];
	memset(CUi_MatrixEigenValue_L,0,sizeof(double)*pIMD->slic_current_num);
	////////////////////////////////////////////////////////////////////////////////
	CvMat E_vector_t,E_value_t,L_t;
	cvInitMatHeader(&E_vector_t,pIMD->slic_current_num,pIMD->slic_current_num,CV_32FC1,Cui_MatrixEigenVector_L);
	cvInitMatHeader(&E_value_t,pIMD->slic_current_num,1,CV_64FC1,CUi_MatrixEigenValue_L);
	cvInitMatHeader(&L_t,pIMD->slic_current_num,pIMD->slic_current_num,CV_64FC1,Cui_Matrix_L); 
#if 0
	cvEigenVV(&L_t,&E_vector_t,&E_value_t);//C是A的特征值(降序排列)，而B则是A的特征向量(每行)
#else
	cvEigenVV(&L_t,&E_vector_t,&E_value_t,DBL_EPSILON);//C是A的特征值(降序排列)，而B则是A的特征向量(每行)
#endif
	//cvEigenVV(&L_t,&E_vector_t,&E_value_t,DBL_EPSILON);//C是A的特征值(降序排列)，而B则是A的特征向量(每行)
	cvInvert(&E_vector_t,&E_vector_t);
	/******************************************/
	/*
	
	拉普拉斯矩阵=对称半正定矩阵

	特征向量特征值
	void cvEigenVV(CvArr* mat, CvArr* evects, CvArr* evals, double eps=0);//计算对称矩阵的特征值和特征向量，evects输出特征向量，evals输出特征值，eps雅可比方法停止参数
	要求三个矩阵都是浮点类型，10×10以下该方法有效，20×20以上的矩阵不能计算出结果，为节约计算量，eps通常设为DBL_EPSILON(10^-15)
	
	
	如果给定的矩阵是对称正定矩阵，那么考虑使用cvSVD();

	*/

#if _DEBUG
	cui_GeneralImgProcess::SaveMatrix_W("","Matrix_L_Laplace.matrix",pIMD->slic_current_num,Cui_Matrix_L);//矩阵
	cui_GeneralImgProcess::SaveMatrix_Float("","Matrix_L_Eigen_Vector.matrix",pIMD->slic_current_num,Cui_MatrixEigenVector_L);//矩阵
	cui_GeneralImgProcess::SaveVector_Double("","Matrix_L_Eigen_Value.matrix",pIMD->slic_current_num,CUi_MatrixEigenValue_L);//特征值
#endif
	TimeCountStop("解特征值、特征向量：");
#if _MSC_VER&&_DEBUG
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	
	TRACE("\n解特征值、特征向量： %f (s)",dfTim);
#endif
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*对前K个特征向量组成的特征值用Kmean聚类
*@param EigenvectorNum 特征向量的个数K==EigenvectorNum 
*@param ClusterNum  （未使用的局部变量）
*@param Threshold  两个图块之间的B式系数达到此阈值就合并，否则不合并
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::Cui_Kmean_Cluster(UINT EigenvectorNum, UINT ClusterNum){
	
/**********前K个特征值********************************/
	if (Cui_Matrix_Category_Simple){
		delete []Cui_Matrix_Category_Simple;
		Cui_Matrix_Category_Simple=NULL;
		
	}
	Cui_Matrix_Category_Simple=new float[pMD->slic_current_num*EigenvectorNum];
	memset(Cui_Matrix_Category_Simple,0,sizeof(float)*pMD->slic_current_num*EigenvectorNum);

	for (register int i=0;i<pMD->slic_current_num;i++){  //row
		for (register UINT j=0;j<EigenvectorNum;j++){	//col
			//N*K						   //N*N
			Cui_Matrix_Category_Simple[EigenvectorNum*i+j]=Cui_MatrixEigenVector_L[pMD->slic_current_num*i+j+pMD->slic_current_num-1-EigenvectorNum];

		}
	}
	
#if 1
	{
		char data_t[1024];
		memset(data_t,0,sizeof(data_t));
		ofstream outfile;
		outfile.open("K_MeanSample.dat",ios::out);
		for (int i=0;i<pMD->slic_current_num;i++){
			for (UINT j=0;j<EigenvectorNum;j++){
				//N*K						   //N*N
				
				double value_t_t=Cui_Matrix_Category_Simple[EigenvectorNum*i+j];
				sprintf(data_t,"%5.2f ",value_t_t);
				outfile<<data_t;
				
			}
			outfile<<endl;
		} 

		outfile.close();
	}
#endif
/**************************************************/


#if 0
	for( int i = 0; i <EigenvalueNum; i++ ){
		CvMat normalize_t;
		cvInitMatHeader(&normalize_t,1,pMD->slic_current_num,CV_32FC1,&K_mean_sample[i*pMD->slic_current_num]);
		cvNormalize(&normalize_t,&normalize_t,1,0,CV_L2);	//a,b无用	
	} 
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("K_mean_sample_Normalize.data",ios::out);
		for( int i = 0; i <EigenvalueNum; i++ ){
			for( int j = 0; j <pMD->slic_current_num; j++ ){
				double value_t=K_mean_sample[i*pMD->slic_current_num+j];
				sprintf(data_t," %5.2f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif	
	
	///////////// K均值Lable/////////////////////////////////////////////
	if (Cui_Matrix_Category_Lable) {
		delete[]Cui_Matrix_Category_Lable;
		Cui_Matrix_Category_Lable=NULL;
	
	}
	 	Cui_Matrix_Category_Lable=new INT32[pMD->slic_current_num]; 
		memset(Cui_Matrix_Category_Lable,0,sizeof(int)*pMD->slic_current_num-1);

	try{
		CvMat K_Sample_t,K_Mean_lable_t;
		cvInitMatHeader(&K_Sample_t,pMD->slic_current_num,EigenvectorNum,CV_32FC1,Cui_Matrix_Category_Simple);
	    cvInitMatHeader(&K_Mean_lable_t,pMD->slic_current_num,1,CV_32SC1,Cui_Matrix_Category_Lable);
		cvKMeans2(&K_Sample_t,ClusterNum,&K_Mean_lable_t,cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,10,0.01));
	}
	catch (cv::Exception* cve){
		string msg_t=cve->msg;
		
	}
#if 1
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("K_mean_Lable.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){
			
				INT32 value_t=Cui_Matrix_Category_Lable[i];
				sprintf(data_t,"%3d ",value_t);
				outfile<<data_t;
				outfile<<endl;
			}

	}
#endif	
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*对前K个特征向量组成的特征值进行Bhattacharyya聚类
*@param EigenvectorNum 特征向量的个数K==EigenvectorNum 
*@param ClusterNum  （未使用的局部变量）
*@param Threshold  两个图块之间的B式系数达到此阈值就合并，否则不合并
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::Cui_B_Cluster(UINT EigenvectorNum, UINT ClusterNum,double Threshold){
	
	EigenvectorNum=EigenvectorNum>2?EigenvectorNum:2;
	Threshold=Threshold>0.5?Threshold:0.5;
	/**********前K个特征值********************************/
	if (Cui_Matrix_Category_Simple){
		delete []Cui_Matrix_Category_Simple;
		Cui_Matrix_Category_Simple=NULL;

	}
	Cui_Matrix_Category_Simple=new float[pMD->slic_current_num*EigenvectorNum];
	memset(Cui_Matrix_Category_Simple,0,sizeof(float)*pMD->slic_current_num*EigenvectorNum);

	for (int i=0;i<pMD->slic_current_num;i++){  //row
		for (UINT j=0;j<EigenvectorNum;j++){	//col
			//N*K						   //N*N
			Cui_Matrix_Category_Simple[EigenvectorNum*i+j]=Cui_MatrixEigenVector_L[pMD->slic_current_num*i+j+pMD->slic_current_num-1-EigenvectorNum];

		}
	}

#if 1
	{
		char data_t[1024];
		memset(data_t,0,sizeof(data_t));
		ofstream outfile;
		outfile.open("B_Sample.dat",ios::out);
		for (int i=0;i<pMD->slic_current_num;i++){
			for (UINT j=0;j<EigenvectorNum;j++){
				//N*K						   //N*N

				double value_t_t=Cui_Matrix_Category_Simple[EigenvectorNum*i+j];
				sprintf(data_t,"%5.2f ",value_t_t);
				outfile<<data_t;

			}
			outfile<<endl;
		} 

		outfile.close();
	}
#endif
  for( int i = 0; i <pMD->slic_current_num; i++ ){
		CvMat normalize_t;
		cvInitMatHeader(&normalize_t,1,EigenvectorNum,CV_32FC1,&Cui_Matrix_Category_Simple[i*EigenvectorNum]);
		cvNormalize(&normalize_t,&normalize_t,1,0,CV_L1);	//a,b无用	
	} 
#if 1
	
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("B_sample_Normalize.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){
			for( UINT j = 0; j <EigenvectorNum; j++ ){
				double value_t=Cui_Matrix_Category_Simple[i*EigenvectorNum+j];
				sprintf(data_t," %5.2f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif	
	/************清空Lable****************************************/
	if (Cui_Matrix_Category_Lable) {
		delete[]Cui_Matrix_Category_Lable;
		Cui_Matrix_Category_Lable=NULL;

	}
	Cui_Matrix_Category_Lable=new INT32[pMD->slic_current_num]; 
	memset(Cui_Matrix_Category_Lable,0xffffffff,sizeof(int)*pMD->slic_current_num-1);
  /****************************************************************/
	double  B_like_t=0;
	INT32  Cui_lable_t[MAX_SP_NUM ];
	memset(Cui_lable_t,-1,MAX_SP_NUM *sizeof(INT32));
	for (int i=0;i<pMD->slic_current_num;i++){
		for (int j=i+1;j<pMD->slic_current_num;j++){
			/****判断超像素i,j相似度*category=i行j行相似度**/
			if (Cui_Matrix_W[i*pMD->slic_current_num+j]>0){
						B_like_t=0;        	
						for (UINT col=0;col<EigenvectorNum;col++){
							 //计算前K个特征值相似度
							  B_like_t+=sqrtl(fabsl( Cui_Matrix_Category_Simple[EigenvectorNum*i+col]*Cui_Matrix_Category_Simple[EigenvectorNum*j+col])); 	  
						}
						if (Cui_Matrix_Category_Lable[i]==0xffffffff){
							Cui_Matrix_Category_Lable[i]=i;
						}
						 if (B_like_t>Threshold){
				            //相似度大于阈值，归属同一类
							 double wenli_like=cui_GeneralImgProcess::CalculateSpSimilar(i,j,pMD->p_SpProperty);
							 if(wenli_like>0.95){
									 Cui_Matrix_Category_Lable[j]=Cui_Matrix_Category_Lable[i];
							 }
							
						 }
						 memcpy(Cui_lable_t,Cui_Matrix_Category_Lable,sizeof(INT32)*pMD->slic_current_num);       	   	
			 
						/*********************************************/
		}
		}
	}

#if 1
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("B_Lable.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){

			INT32 value_t=Cui_Matrix_Category_Lable[i];
			sprintf(data_t,"%3d ",value_t);
			outfile<<data_t;
			outfile<<endl;
		}

	}
#endif	
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*对前K个特征向量组成的特征值进行Bhattacharyya聚类
*@param EigenvectorNum 特征向量的个数K==EigenvectorNum 
*@param ClusterNum  （未使用的局部变量）
*@param Threshold  两个图块之间的B式系数达到此阈值就合并，否则不合并
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::Cui_B_Cluster_2016_09_27(UINT EigenvectorNum, UINT ClusterNum,double Threshold){

	EigenvectorNum=EigenvectorNum>2?EigenvectorNum:2;
	Threshold=Threshold>0.5?Threshold:0.5;
	/**********前K个特征值********************************/
	if (Cui_Matrix_Category_Simple){
		delete []Cui_Matrix_Category_Simple;
		Cui_Matrix_Category_Simple=NULL;

	}
	Cui_Matrix_Category_Simple=new float[pIMD->slic_current_num*EigenvectorNum];
	memset(Cui_Matrix_Category_Simple,0,sizeof(float)*pIMD->slic_current_num*EigenvectorNum);

	for (int i=0;i<pIMD->slic_current_num;i++){  //row
		for (UINT j=0;j<EigenvectorNum;j++){	//col
			//N*K   //N*N
			Cui_Matrix_Category_Simple[EigenvectorNum*i+j]=Cui_MatrixEigenVector_L[pIMD->slic_current_num*i+j+pIMD->slic_current_num-1-EigenvectorNum];

		}
	}

#if _DEBUG
	if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		memset(data_t,0,sizeof(data_t));
		ofstream outfile;
		outfile.open("B_Sample.dat",ios::out);
		for (int i=0;i<pIMD->slic_current_num;i++){
			for (UINT j=0;j<EigenvectorNum;j++){
				//N*K						   //N*N

				double value_t_t=Cui_Matrix_Category_Simple[EigenvectorNum*i+j];
				sprintf(data_t,"%e ",value_t_t);
				outfile<<data_t;
			}
			outfile<<endl;
		}
		outfile.close();
	}
#endif
	for( int i = 0; i <pIMD->slic_current_num; i++ ){
		CvMat normalize_t;
		cvInitMatHeader(&normalize_t,1,EigenvectorNum,CV_32FC1,&Cui_Matrix_Category_Simple[i*EigenvectorNum]);
		cvNormalize(&normalize_t,&normalize_t,1,0,CV_L1);	//a,b无用	
	} 
#if _DEBUG

	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("B_sample_Normalize.data",ios::out);
		for( int i = 0; i <pIMD->slic_current_num; i++ ){
			for( UINT j = 0; j <EigenvectorNum; j++ ){
				double value_t=Cui_Matrix_Category_Simple[i*EigenvectorNum+j];
				sprintf(data_t," %e ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif	
	/************清空Lable****************************************/
	if (Cui_Matrix_Category_Lable) {
		delete[]Cui_Matrix_Category_Lable;
		Cui_Matrix_Category_Lable=NULL;

	}
	Cui_Matrix_Category_Lable=new INT32[pIMD->slic_current_num]; 
	memset(Cui_Matrix_Category_Lable,0xffffffff,sizeof(int)*pIMD->slic_current_num-1);
	/****************************************************************/
	double  B_like_t=0;
#if _DEBUG

	INT32  Cui_lable_t[MAX_SP_NUM ];
	memset(Cui_lable_t,-1,MAX_SP_NUM *sizeof(INT32));

#endif
	for (int i=0;i<pIMD->slic_current_num;i++){
		for (int j=i+1;j<pIMD->slic_current_num;j++){
			/****判断超像素i,j相似度*category=i行j行相似度**/
			if (Cui_Matrix_W[i*pIMD->slic_current_num+j]>0){
				B_like_t=0;        	
				for (UINT col=0;col<EigenvectorNum;col++){
					//计算前K个特征值相似度
					B_like_t+=sqrtl(fabsl( Cui_Matrix_Category_Simple[EigenvectorNum*i+col]*Cui_Matrix_Category_Simple[EigenvectorNum*j+col])); 	  
				}
				if (Cui_Matrix_Category_Lable[i]==0xffffffff){
					Cui_Matrix_Category_Lable[i]=i;
				}
				if (B_like_t>Threshold){
					//相似度大于阈值，归属同一类
					double wenli_like=cui_GeneralImgProcess::CalculateSpSimilar(i,j,pIMD->p_SpProperty);
					if(wenli_like>Spectral_Clustering_B_Wavelet_Threshold){
						Cui_Matrix_Category_Lable[j]=Cui_Matrix_Category_Lable[i];
					}

				}
#if _DEBUG
				memcpy(Cui_lable_t,Cui_Matrix_Category_Lable,sizeof(INT32)*pIMD->slic_current_num);     
#endif
				/*********************************************/
			}
		}
	}

#if _DEBUG
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("B_Lable.data",ios::out);
		for( int i = 0; i <pIMD->slic_current_num; i++ ){

			INT32 value_t=Cui_Matrix_Category_Lable[i];
			sprintf(data_t,"%3d ",value_t);
			outfile<<data_t;
			outfile<<endl;
		}

	}
#endif	
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*丢弃的聚类方法
*算法中未使用
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::Cui_Min_Cluster(void)
{
	const   UINT EigenvectorNum=1;
	
	if (Cui_Matrix_Category_Simple){
		delete []Cui_Matrix_Category_Simple;
		Cui_Matrix_Category_Simple=NULL;

	}
	Cui_Matrix_Category_Simple=new float[pMD->slic_current_num];
	memset(Cui_Matrix_Category_Simple,0,sizeof(float)*pMD->slic_current_num);

	for (int i=0;i<pMD->slic_current_num;i++){  //row
		
			Cui_Matrix_Category_Simple[i]=Cui_MatrixEigenVector_L[pMD->slic_current_num*i+pMD->slic_current_num-1];

	}

#if _DEBUG
	{
		char data_t[1024];
		memset(data_t,0,sizeof(data_t));
		ofstream outfile;
		outfile.open("Mim_Sample.dat",ios::out);
		for (int i=0;i<pMD->slic_current_num;i++){
			for (int j=0;j<EigenvectorNum;j++){
				//N*K						   //N*N

				double value_t_t=Cui_Matrix_Category_Simple[EigenvectorNum*i+j];
				sprintf(data_t,"%5.2f ",value_t_t);
				outfile<<data_t;

			}
			outfile<<endl;
		} 

		outfile.close();
	}
#endif
#if 0
	for( int i = 0; i <pMD->slic_current_num; i++ ){
		CvMat normalize_t;
		cvInitMatHeader(&normalize_t,1,EigenvectorNum,CV_32FC1,&Cui_Matrix_Category_Simple[i*EigenvectorNum]);
		cvNormalize(&normalize_t,&normalize_t,1,0,CV_L2);	//a,b无用	
	} 
#endif
	
#if 0

	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Min_sample_Normalize.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){
			for( int j = 0; j <EigenvectorNum; j++ ){
				double value_t=Cui_Matrix_Category_Simple[i*EigenvectorNum+j];
				sprintf(data_t," %5.2f ",value_t);
				outfile<<data_t;
			}
			outfile<<endl;
		} 
		outfile.close();
	}
#endif	
	/************清空Lable****************************************/
	if (Cui_Matrix_Category_Lable) {
		delete[]Cui_Matrix_Category_Lable;
		Cui_Matrix_Category_Lable=NULL;

	}
	Cui_Matrix_Category_Lable=new INT32[pMD->slic_current_num]; 
	memset(Cui_Matrix_Category_Lable,0xffffffff,sizeof(int)*pMD->slic_current_num-1);
	/****************************************************************/
	double  B_like_t=0;
	for (int i=0;i<pMD->slic_current_num;i++){		
		  
		for (int j=i+1;j<pMD->slic_current_num;j++){
			/****判断超像素i,j相似度*category=i行j行相似度**/  
				if (Cui_Matrix_W[i*pMD->slic_current_num+j]>0){
						double spi_value=fabsl(Cui_Matrix_Category_Simple[i]);
						double spj_value=fabsl(Cui_Matrix_Category_Simple[j]);
						if (max(spi_value,spj_value)){
							B_like_t=fabsl(spi_value-spj_value)/max(spi_value,spj_value);
						}else{
							B_like_t=0; 	
						} 

						if (Cui_Matrix_Category_Lable[i]==0xffffffff){
								Cui_Matrix_Category_Lable[i]=i;
						}	

						if (B_like_t<0.5){
						
							Cui_Matrix_Category_Lable[j]=Cui_Matrix_Category_Lable[i];
			           }

				}

			/*********************************************/
		}
	}

#if _DEBUG
	if(cui_GeneralImgProcess::SAVE_DEBUG_2DISK){
		char data_t[1024];
		ofstream outfile;
		outfile.open("Min_mean_Lable.data",ios::out);
		for( int i = 0; i <pMD->slic_current_num; i++ ){

			INT32 value_t=Cui_Matrix_Category_Lable[i];
			sprintf(data_t,"%3d ",value_t);
			outfile<<data_t;
			outfile<<endl;
		}

	}
#endif	
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*查找超像素块之间最大的相似度数值
*迭代过程中使用
*/
/*------------------------------------------------------------------------------------------------------------------*/
double SLIC::Cui_Find_MaxSimilar(void)
{
	double Max_Similar=0;
#if OUT_DOOR_SUPERPIXEL_COLOR_BAT
	for (register int spi=0;spi<pMD->slic_current_num;spi++){
		for (register int spj=spi+1;spj<pMD->slic_current_num;spj++){
			if ( Cui_Matrix_W[spi*pMD->slic_current_num+spj]>=Max_Similar){
				Max_Similar=Cui_Matrix_W[spi*pMD->slic_current_num+spj];

			}
		}
	}
#endif

#if OUT_DOOR_HUMAN
	for (register int spi=0;spi<pIMD->slic_current_num;spi++){
		for (register int spj=spi+1;spj<pIMD->slic_current_num;spj++){
			if ( Cui_Matrix_W[spi*pIMD->slic_current_num+spj]>=Max_Similar){
				Max_Similar=Cui_Matrix_W[spi*pIMD->slic_current_num+spj];

			}
		}

	}
#endif

	
	return Max_Similar;
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*采用K均值完成谱聚类算法
*@param EigenvectorNumPercent (默认值百分之十)
*@param ClusterPercent 未使用的局部变量
*@param Threshold （未使用的阈值）参数由全局宏进行传递
*/
/*------------------------------------------------------------------------------------------------------------------*/
bool SLIC::Cui_Spectral_Clustering_KM(UINT EigenvectorNum, UINT ClusterNum,double Similar_Threshold){
	this->CuiFindSaveNighbour_E_matrix();//得到相邻矩阵
	this->CuiFindSaveSimilar_W_matrix2(); //相似矩阵
	this->CuiFindSaveDgeree_D_matrix();
	this->CuiFindSaveLaplace_L_matrix();
	this->CuiFindSave_L_Eigenvalue();
	this->Cui_Kmean_Cluster(EigenvectorNum,ClusterNum);
	/*************普及类描绘轮廓************************************/
	   int *CuiImgLables_t=NULL;
	   CuiImgLables_t=new int[CuiWidth*CuiHeight];
	   memcpy_s(CuiImgLables_t,sizeof(int)*CuiWidth*CuiHeight,CuiImgLables,sizeof(int)*CuiWidth*CuiHeight);
	   ////////////////////////////////
	   for (int spi=0;spi<pMD->slic_current_num;spi++){
		   for (int spj=spi+1;spj<pMD->slic_current_num;spj++){
					if (Cui_Matrix_Category_Lable[spi]==Cui_Matrix_Category_Lable[spj]){
						//超像素i,j 属于同一类；
						if (Cui_Matrix_W[spi*pMD->slic_current_num+spj]>Similar_Threshold){
							  //相似度大于0.9；
							 /*************FindSetValue***************************/
							  int SetValue=0;
							   for (int mi=0;mi<CuiHeight;mi++){
							       for (int mj=0;mj<CuiWidth;mj++){
									  if (CuiImgLables[mi*CuiWidth+mj]==spi){
									      SetValue=CuiImgLables_t[mi*CuiWidth+mj];
									  }
							       }
							   }
							   /**************SetValue*************************/
							   for (int mi=0;mi<CuiHeight;mi++){
								   for (int mj=0;mj<CuiWidth;mj++){
									   if (CuiImgLables[mi*CuiWidth+mj]==spi
										   ||CuiImgLables[mi*CuiWidth+mj]==spj){
										   
										   CuiImgLables_t[mi*CuiWidth+mj]=SetValue;
										   
									   }
								   }
							   }
							  /****************************************/
						}
					}
		   }
	   } 
	   /***************************************************************/
		  // 标准化lable
	   vector<int> cui_t;
	   cui_t.push_back(0);
	   for (int mi=0;mi<CuiHeight;mi++){
		   for (int mj=0;mj<CuiWidth;mj++){			  

				if (cui_t.at(cui_t.size()-1)<CuiImgLables_t[mi*CuiWidth+mj]){
					cui_t.push_back( CuiImgLables_t[mi*CuiWidth+mj]);	
				}
				

		   }
	   }
		for (unsigned int i=0;i<cui_t.size();i++){
			
			for (int mi=0;mi<CuiHeight;mi++){
				for (int mj=0;mj<CuiWidth;mj++){			  

					if (cui_t.at(i)==CuiImgLables_t[mi*CuiWidth+mj]){
						 CuiImgLables_t[mi*CuiWidth+mj]=i;
					}


				}
			}
		}
		pMD->slic_current_num=cui_t.size();
	   /***************************************************************/
	   
	   memcpy_s(CuiImgLables,sizeof(int)*CuiWidth*CuiHeight,CuiImgLables_t,sizeof(int)*CuiWidth*CuiHeight);
	   delete []CuiImgLables_t;	 
	   this->SaveSuperpixelLabels(CuiImgLables,CuiWidth,CuiHeight);
	   cui_GeneralImgProcess::CuiSaveImgWithContours(CuiImgData,CuiImgLables,CuiWidth,CuiHeight,pMD->FileReadFullPath,pMD->FileWritePath,"");
	/**************************************************************/
	   double Max_Similar=0;
	   for (int spi=0;spi<pMD->slic_current_num;spi++){
		   for (int spj=spi+1;spj<pMD->slic_current_num;spj++){
			 	 if ( Cui_Matrix_W[spi*pMD->slic_current_num+spj]>Max_Similar){
					 Max_Similar=Cui_Matrix_W[spi*pMD->slic_current_num+spj];
					
			 	 }
		   }
	   }
	   if (Max_Similar<Similar_Threshold||pMD->slic_current_num==1){
		   return false;
	   }else{
		   return true;
	   }
	   /***********************************************************/
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*采用Bhattacharyya系数完成谱聚类算法
*@param EigenvectorNumPercent (默认值百分之十)
*@param ClusterPercent 未使用的局部变量
*@param Threshold （未使用的阈值）参数由全局宏进行传递
*@note 色彩空间谱聚类方法
*/
/*------------------------------------------------------------------------------------------------------------------*/
bool SLIC::Cui_Spectral_Clustering_B(
	double EigenvectorNumPercent, 
	double ClusterPercent,
	double Threshold)
{
#if 1
	UINT EigenvectorNum=pMD->slic_current_num*EigenvectorNumPercent;
	UINT ClusterNum=pMD->slic_current_num*ClusterPercent;
	double	W_Threshold;
	//10%作为图像聚类特征向量的维度(EigenvectorNumPercent=0.1)
	this->CuiFindSaveNighbour_E_matrix();//得到相邻矩阵905ms
	this->CuiFindSaveSimilar_W_matrix2();//5.9s
	this->CuiFindSaveDgeree_D_matrix();//5.1s
	this->CuiFindSaveLaplace_L_matrix();//3.6s	
	/////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER&&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif
	{
			this->CuiFindSave_L_Eigenvalue();//41s
	}
#if _MSC_VER&&_DEBUG
    QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	double dftims=dfMinus/1000;	
	/////////////////////////////////////////////////////////////////////////////
#endif	
	
	if (0){
		this->Cui_Kmean_Cluster(EigenvectorNum,ClusterNum);
		W_Threshold=0.707;
	}else if (1){
#if OUT_DOOR
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num);
		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(pMD->slic_current_num,pMD->p_SpProperty,pMD->Src_ImgData,CuiImgLables,pMD->ImgWidth,pMD->ImgHeight);

		this->Cui_B_Cluster(EigenvectorNum,ClusterNum,Threshold); 
		cui_GeneralImgProcess::Cui_Combination_ImgLabs2(CuiImgData,CuiImgLables,Cui_Matrix_Category_Lable,Cui_Matrix_W,pMD->slic_current_num,CuiWidth,CuiHeight,
			Spectral_Clustering_Combine_Threshold,pMD);
		///////////////////////////////////////////////////
		  W_Threshold=Iteration__Threshold;
#endif
#if IN_DOOR
		  ASSERT(sizeof(int)==sizeof(INT32));
		  cui_GeneralImgProcess::CalculateAllSpPropertyRange(CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num);
		  cui_GeneralImgProcess::CalculateAllSpBlockEnergy(pMD->slic_current_num,pMD->p_SpProperty,pMD->Src_ImgData,CuiImgLables,pMD->ImgWidth,pMD->ImgHeight);

		  this->Cui_B_Cluster(EigenvectorNum,ClusterNum,Threshold); 
		  cui_GeneralImgProcess::Cui_Combination_ImgLabs2(CuiImgData,CuiImgLables,Cui_Matrix_Category_Lable,Cui_Matrix_W,pMD->slic_current_num,CuiWidth,CuiHeight,
			  Iteration__Threshold,pMD);
		  ///////////////////////////////////////////////////
		  W_Threshold=Iteration__Threshold;
#endif
		
	}else{
	/*	this->Cui_Min_Cluster();
#if OUT_DOOR
cui_GeneralImgProcess::Cui_Combination_ImgLabs2(CuiImgData,CuiImgLables,Cui_Matrix_Category_Lable,Cui_Matrix_W,pMD->slic_current_num,CuiWidth,CuiHeight,
			Spectral_Clustering_Combine_Threshold,pMD);
#endif
#if IN_DOOR
		 cui_GeneralImgProcess::Cui_Combination_ImgLabs2(CuiImgData,CuiImgLables,Cui_Matrix_Category_Lable,Cui_Matrix_W,pMD->slic_current_num,CuiWidth,CuiHeight,
				Spectral_Clustering_Combine_Threshold,pMD);
#endif		*/
		return false;
		//W_Threshold=0;
	}	
	/***********************************************************/
			  do{ 
						    double T_Similar=0;
							this->CuiFindSaveNighbour_E_matrix();//得到相邻矩阵0.9s
							this->CuiFindSaveSimilar_W_matrix2();//5.9s
#ifdef InDoor
#if ((IN_DOOR)&&(Iteration__Threshold_Vein_SkyV>0)&&(Iteration__Threshold_Vein_GND>0))
							cui_GeneralImgProcess::CalculateAllSpPropertyRange(CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num);
							cui_GeneralImgProcess::CalculateAllSpBlockEnergy(pMD->slic_current_num,pMD->p_SpProperty,pMD->Src_ImgData,CuiImgLables,pMD->ImgWidth,pMD->ImgHeight);
							cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num,pMD->Seg_HorizontalLinePos);
							
							cui_GeneralImgProcess::CuiSetNighbour_W_Vein_matrix(pMD->Matrix_W_Vein,pMD->slic_current_num,pMD->p_SpProperty,pMD);//纹理相似阵
							cui_GeneralImgProcess::AdjustNighbour_W(Cui_Matrix_W,pMD->Matrix_W_Vein,Cui_Matrix_W,pMD->slic_current_num,Iteration__Threshold_Vein_SkyV,Iteration__Threshold_Vein_GND,Iteration__Threshold_Color_SkyV,Iteration__Threshold_Color_GND,pMD->p_SpProperty);
#endif
#endif

#if OUT_DOOR
							cui_GeneralImgProcess::CalculateAllSpPropertyRange(CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num);//78ms
							cui_GeneralImgProcess::CalculateAllSpBlockEnergy(pMD->slic_current_num,pMD->p_SpProperty,pMD->Src_ImgData,CuiImgLables,pMD->ImgWidth,pMD->ImgHeight);//1s

							cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,pMD->p_SpProperty,pMD->slic_current_num,pMD->Seg_HorizontalLinePos);//76ms
							    cui_GeneralImgProcess::CuiSetNighbour_W_Vein_matrix(pMD->Matrix_W_Vein,pMD->slic_current_num,pMD->p_SpProperty,pMD);//纹理相似阵
							    cui_GeneralImgProcess::AdjustNighbour_W(Cui_Matrix_W,pMD->Matrix_W_Vein,Cui_Matrix_W,pMD->slic_current_num,
								Iteration__Threshold_Vein_SkyV,Iteration__Threshold_Vein_GND,Iteration__Threshold_Color_SkyV,Iteration__Threshold_Color_GND,pMD->p_SpProperty);											
#endif
							memset(Cui_Matrix_Category_Lable,0,sizeof(INT32)*pMD->slic_current_num);
							T_Similar=this->Cui_Find_MaxSimilar();
							if (T_Similar>Iteration__Threshold){				
								cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
									CuiImgData,CuiImgLables,
									Cui_Matrix_Category_Lable,
									Cui_Matrix_W,pMD->slic_current_num,
									CuiWidth,CuiHeight,
									Iteration__Threshold,pMD);//1.4s									
							}else{	
#if 0
	 							//	this->Cui_Combination_ImgLabs2(0.71);	//组合时依然使用相似度
									//s*****c int i=0;
									//if (0==++i%2){
									//	break;
									//}
#else
								if (Iteration_Complete_Combine_Threshold){
									cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
										CuiImgData,
										CuiImgLables,
										Cui_Matrix_Category_Lable,
										Cui_Matrix_W,
										pIMD->slic_current_num,
										pIMD->ImgWidth,
										pIMD->ImgHeight,
										Spectral_Clustering_Combine_Threshold,
										pIMD);
									break;
								}else{
									break;
								}
#endif
							}
		
			}while(1);


		this->Cui_SurroundClassification();

#if _DEBUG&&OUT_DOOR_SUPERPIXEL_COLOR_BAT
		pMD->SaveColorSpectralClusteringNum();
#endif
	return false;
#endif
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*采用Bhattacharyya系数完成谱聚类算法
*@param EigenvectorNumPercent (默认值百分之十)
*@param ClusterPercent 未使用的局部变量
*@param Threshold （未使用的阈值）参数由全局宏进行传递
*@note 色彩空间谱聚类方法
*/
/*------------------------------------------------------------------------------------------------------------------*/
bool SLIC::Cui_Spectral_Clustering_B_2016_09_26(
	double EigenvectorNumPercent, 
	double ClusterPercent,
	double Threshold)
{
	TRACE_FUNC();
#if 1
	if(Cui_Matrix_W_Vein) {
		delete[] Cui_Matrix_W_Vein;
	}
	Cui_Matrix_W_Vein=new double[pIMD->slic_current_num*pIMD->slic_current_num];
#endif
#if 1
	UINT EigenvectorNum=pIMD->slic_current_num*EigenvectorNumPercent;
	UINT ClusterNum=pIMD->slic_current_num*ClusterPercent;
	double	W_Threshold;
	//10%作为图像聚类特征向量的维度(EigenvectorNumPercent=0.1)
	pIMD->GetMatrixE();//this->CuiFindSaveNighbour_E_matrix();//得到相邻矩阵905ms
	this->CuiFindSaveSimilar_W_matrix2_2016_09_26();//5.9s
	this->CuiFindSaveDgeree_D_matrix2016_09_26();//5.1s
	this->CuiFindSaveLaplace_L_matrix_2016_09_26();//3.6s	
	/////////////////////////////////////////////////////////////////////////////////
	{
		this->CuiFindSave_L_Eigenvalue_2016_09_26();//41s
	}
	if (0){
		
	}else if (1){
	/*--颜色&&纹理&&谱聚类--*/
#if OUT_DOOR
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight,
			pIMD->p_SpProperty,
			pIMD->slic_current_num);

		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
			pIMD->slic_current_num,
			pIMD->p_SpProperty,
			pIMD->src_ImgBGRA,
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight);

		/*--谱聚类+纹理聚类 【这个算法】--小波分析输出聚类结果--*/
		this->Cui_B_Cluster_2016_09_27(EigenvectorNum,ClusterNum,Threshold); 

		//组合聚类结果

		pIMD->CombineLabelsByCategory_and_WMatrix(
			Cui_Matrix_Category_Lable,
			Cui_Matrix_W,
			Spectral_Clustering_Combine_Threshold);
		///////////////////////////////////////////////////
		W_Threshold=Iteration__Threshold;
#endif
	}else{		
		return false;
	}	
	/*************************颜色直方图迭代**********************************/
	do{ 
		double T_Similar=0;
		pIMD->GetMatrixE();//this->CuiFindSaveNighbour_E_matrix();//得到相邻矩阵0.9s
		this->CuiFindSaveSimilar_W_matrix2_2016_09_26();//颜色直方图-------5.9s
		/*小波分析*/
#if OUT_DOOR&&0
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight,
			pIMD->p_SpProperty,
			pIMD->slic_current_num);//78ms

		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
			pIMD->slic_current_num,
			pIMD->p_SpProperty,
			pIMD->src_ImgBGRA,
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight);//1s

		cui_GeneralImgProcess::CalculateAllSpPropertyPostitonByHLine(
			pIMD->src_ImgLabels,
			pIMD->ImgWidth,
			pIMD->ImgHeight,
			pIMD->p_SpProperty,
			pIMD->slic_current_num,
			pIMD->Seg_HorizontalLinePos);//76ms

		cui_GeneralImgProcess::CuiSetNighbour_W_Vein_matrix(
			Cui_Matrix_W_Vein,
			pIMD->slic_current_num,
			pIMD->p_SpProperty,
			NULL);//纹理相似阵

		cui_GeneralImgProcess::AdjustNighbour_W(
			Cui_Matrix_W,
			Cui_Matrix_W_Vein,
			Cui_Matrix_W,
			pIMD->slic_current_num,
			Iteration__Threshold_Vein_SkyV,
			Iteration__Threshold_Vein_GND,
			Iteration__Threshold_Color_SkyV,
			Iteration__Threshold_Color_GND,
			pIMD->p_SpProperty);											
#endif		
		T_Similar=this->Cui_Find_MaxSimilar();
		if (T_Similar>0.9){
			pIMD->CombineLabelsByWMatrix(Cui_Matrix_W,0.9);//1.4s									
		}else{	
			break;
		}
	}while(1);
#if 0
	this->Cui_SurroundClassification();
#endif

//#if _DEBUG
//	pIMD->SaveColorSpectralClusteringNum();
//#endif

	return false;
#endif
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*对完全包围的超像素图块进行全包围聚类；\n
*算法在谱聚类完成之后调用\n
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::Cui_SurroundClassification(void){
#if 0
	INT32*	category;
	category=new INT32[pMD->slic_current_num];
	cui_GeneralImgProcess::SurroundClassification(CuiImgLables,pMD->slic_current_num,CuiWidth,CuiHeight,category);
	cui_GeneralImgProcess::Cui_Combination_ImgLabs2(CuiImgData,CuiImgLables,category,NULL,pMD->slic_current_num,CuiWidth,CuiHeight,0,FileReadFullPath,FileWritePath);
	delete[] category;
#endif
#if OUT_DOOR_SUPERPIXEL_COLOR_BAT
   cui_GeneralImgProcess::InSideClusteringByopencv(
	   CuiImgData,
	   CuiImgLables,
	   pMD->slic_current_num,
	   CuiWidth,
	   CuiHeight,
	   pMD->FileReadFullPath,
	   pMD->FileWritePath);
#endif
#if OUT_DOOR_HUMAN
   pIMD->SurroundClassification();
#endif
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*导出超像素的标定图
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::ExportMemData(void)
{
	this->pMD->ImportMemData(pMD->slic_current_num,CuiImgLables);

}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*循环迭代，计算超像素
*
*@param		[out]	kseedsl 种子的L分量
*@param		[out]	kseedsa 种子的A分量
*@param		[out]	kseedsb 种子的B分量
*@param		[out]	kseedsx 种子的X坐标分量
*@param		[out]	kseedsy 种子的Y坐标分量
*@param		[in]	klabels labes矩阵
*@param			STEP     超像素步长
*@param		    edgemag  未使用
*@param			M        未使用
*
*
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC::PerformSuperpixelSLIC_CUINEW(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	int*&						klabels,
	const int&					STEP,
	const vector<double>&		edgemag,
	const double&				M)
{
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	clock_t start, finish;
	    start = clock();
	{
#if TRUE
	int sz = m_width*m_height;
	const int numk = kseedsl.size();
	//----------------
	int offset = STEP;
	//if(STEP < 8) offset = STEP*1.5;//to prevent a crash due to a very small step size
	//----------------

	vector<double> clustersize(numk, 0);
	vector<double> inv(numk, 0);//to store 1/clustersize[k] values

	vector<double> sigmal(numk, 0);
	vector<double> sigmaa(numk, 0);
	vector<double> sigmab(numk, 0);
	vector<double> sigmax(numk, 0);
	vector<double> sigmay(numk, 0);
	vector<double> distvec(sz, DBL_MAX);

	double invwt = 1.0/((STEP/M)*(STEP/M));

	int x1, y1, x2, y2;
	double l, a, b;
	double dist;
	double distxy;
	for( int itr = 0; itr < 10; itr++ )
	{
#if _MSC_VER
/**/
		LARGE_INTEGER litmp;
		LONGLONG QPart1,QPart2;
		double dfMinus, dfFreq, dfTim;
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
#endif
	{
				distvec.assign(sz, DBL_MAX);		
	}
#if _MSC_VER
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2-QPart1);
		dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
		dfTim*=1000;
		/**/
#endif
	
		
		for( int n = 0; n < numk; n++ )
		{
			y1 = max(0.0,			kseedsy[n]-offset);
			y2 = min((double)m_height,	kseedsy[n]+offset);
			x1 = max(0.0,			kseedsx[n]-offset);
			x2 = min((double)m_width,	kseedsx[n]+offset);
#if TRUE
			for( int y = y1; y < y2; y++ )
			{
				for( int x = x1; x < x2; x++ )
				{
					int i = y*m_width + x;

					l = m_lvec[i];
					a = m_avec[i];
					b = m_bvec[i];
#if FALSE
					dist =			(l - kseedsl[n])*(l - kseedsl[n]) +
						(a - kseedsa[n])*(a - kseedsa[n]) +
						(b - kseedsb[n])*(b - kseedsb[n]);
					//if (itr>5)
					{
						distxy =		(x - kseedsx[n])*(x - kseedsx[n]) +
							(y - kseedsy[n])*(y - kseedsy[n]);
					}
					//else
					//{
					//	distxy =0;
					//}
					//------------------------------------------------------------------------
					dist += distxy*invwt;//dist = sqrt(dist) + sqrt(distxy*invwt);//this is more exact
					//------------------------------------------------------------------------
#endif
#if TRUE
					double sita_n, m_n, L_n, X_n, Y_n;
					double Ksita_n, Km_n, KL_n, KX_n, KY_n;
					ConvertLab2oml(l,a,b,x,y,sita_n,m_n,L_n,X_n,Y_n);
					ConvertLab2oml(kseedsl[n],kseedsa[n],kseedsb[n],kseedsx[n],kseedsy[n],Ksita_n,Km_n,KL_n,KX_n,KY_n);
					dist=CalculateNewDistance(sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
#endif
					if( dist < distvec[i] )
					{
						distvec[i] = dist;
						klabels[i]  = n;
					}
				}
			}
#endif
		}
		//-----------------------------------------------------------------
		// Recalculate the centroid and store in the seed values
		//-----------------------------------------------------------------
		//instead of reassigning memory on each iteration, just reset.

		sigmal.assign(numk, 0);
		sigmaa.assign(numk, 0);
		sigmab.assign(numk, 0);
		sigmax.assign(numk, 0);
		sigmay.assign(numk, 0);
		clustersize.assign(numk, 0);
		//------------------------------------
		//edgesum.assign(numk, 0);
		//------------------------------------
		
		{
			int ind(0);
			for( int r = 0; r < m_height; r++ )
			{
				for( int c = 0; c < m_width; c++ )
				{
					sigmal[klabels[ind]] += m_lvec[ind];
					sigmaa[klabels[ind]] += m_avec[ind];
					sigmab[klabels[ind]] += m_bvec[ind];
					sigmax[klabels[ind]] += c;
					sigmay[klabels[ind]] += r;
					//------------------------------------
					//edgesum[klabels[ind]] += edgemag[ind];
					//------------------------------------
					clustersize[klabels[ind]] += 1.0;
					ind++;
				}
			}
		}

		{
			for( int k = 0; k < numk; k++ )
			{
				if( clustersize[k] <= 0 ) clustersize[k] = 1;
				inv[k] = 1.0/clustersize[k];//computing inverse now to multiply, than divide later
			}
		}

		{
			for( int k = 0; k < numk; k++ )
			{
				kseedsl[k] = sigmal[k]*inv[k];
				kseedsa[k] = sigmaa[k]*inv[k];
				kseedsb[k] = sigmab[k]*inv[k];
				kseedsx[k] = sigmax[k]*inv[k];
				kseedsy[k] = sigmay[k]*inv[k];
				//------------------------------------
				//edgesum[k] *= inv[k];
				//------------------------------------
			}
		}
		this->showDistance(distvec);
	}
#endif
	}
	
	finish = clock();
	double duration_ms = (double)(finish - start)*1000 / CLOCKS_PER_SEC;
	double duration_s = duration_ms/1000;
	double duration_min =duration_s/60;
	printf( "gpu time is %f ms/n", duration_ms );

}
/*---------------------------------------------------------------------------------*/
/**
*转换LAB色彩空间成角度、模长、亮度表示法
*
*@param m_width 图像宽度
*@param m_height 图像高度
*@param L 像素点的L分量
*@param A 像素点的A分量
*@param B 像素点的B分量
*@param X 像素点的X坐标
*@param Y 像素点的Y坐标
*@param sita_n 像素点的角度（归一化到[0,1]）
*@param m_n    像素点的模长（归一化到[0,1]）
*@param L_n    像素点的亮度（归一化到[0,1]）
*@param X_n    像素点的X坐标（归一化到[0,1]）
*@param Y_n    像素点的Y坐标（归一化到[0,1]）
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::ConvertLab2oml(
	double L,
	double A,
	double B,
	double X,
	double Y,
	double& sita_n,
	double& m_n,
	double& L_n,
	double& X_n,
	double& Y_n)
{
	assert(L>=0&&L<=100+1);
	assert(A>=-128&&A<=128+1);
	assert(B>=-128&&B<=128+1);
	assert(X>=0&&X<=m_width+1);
	assert(Y>=0&&Y<=m_height+1);
#if 1
	///////////////////////////////////////
	sita_n=atan2(A,B);
	sita_n=sita_n/(2*3.1415927)+0.5;
	///////////////////////////////////////
	m_n=sqrt(A*A+B*B);
	m_n=m_n/(128*1.415);
	///////////////////////////////////////
	L_n=L/100;
	assert(L_n>=0&&L_n<=1+1E-1);
	///////////////////////////////////////
	X_n=X/m_width;
	assert(X_n>=0&&X_n<=1+1E-1);
	Y_n=Y/m_height;
	assert(Y_n>=0&&Y_n<=1+1E-1);
#endif
	assert(sita_n>=0-0.1&&sita_n<=1+0.1);
	assert(m_n>=0-0.1&&m_n<=1+0.1);
	assert(L_n>=0-0.1&&L_n<=1+0.1);
	assert(X_n>=0-0.1&&X_n<=1+0.1);
	assert(Y_n>=0-0.1&&Y_n<=1+0.1);
}
/*---------------------------------------------------------------------------------*/
/**
*
*计算两个像素点的距离
*@image html image027.png "色彩公式"
*@image html image029.png "距离公式"
*@image html image031.png "距离综合公式"
*
*
*@param  sita_n0   像素0的角度
*@param  m_n0      像素0的模长
*@param  L_n0      像素0的亮度
*@param  X_n0      像素0的等价X坐标
*@param  Y_n0      像素0的等价Y坐标
*@param  sita_n1   像素1的角度
*@param  m_n1      像素1的模长
*@param  L_n1      像素1的亮度
*@param  X_n1      像素1的等价X坐标
*@param  Y_n1      像素1的等价Y坐标
*@return 
*
*/
/*---------------------------------------------------------------------------------*/
double  SLIC::CalculateNewDistance(
	double sita_n0,
	double m_n0,
	double L_n0,
	double X_n0,
	double Y_n0,
	double sita_n1,
	double m_n1,
	double L_n1,
	double X_n1,
	double Y_n1)
{
	/*alpha=0.6;
	betta=0.3;
	gama=1-alpha-betta;*/
	/*float fai=100;*/
	double dst=alpha*fabs(sita_n1-sita_n0)+betta*fabs(m_n1-m_n0)+gama*fabs(L_n1-L_n0);
	double dst_xy=fai*sqrt(powl(X_n1-X_n0,2)+powl(Y_n1-Y_n0,2));
	dst+=dst_xy;
	return dst;
};
/*---------------------------------------------------------------------------------*/
/**
*获取a、b最大公约数
*@param  a  
*@param  b
*@return  最大公约数
*
*/
/*---------------------------------------------------------------------------------*/
unsigned int SLIC::GetGCD(unsigned int a, unsigned int b)
{
	unsigned int t=0;
	if(a>b){
		t=b;
		b=a;
		a=t;
	}

	while(b%a)
	{
		t=b;
		b=a;
		a=t%a;
	}
	return a;
}

/*---------------------------------------------------------------------------------*/
/**
*显示距离图像
*聚类矩阵的长宽与图像等同
*@param clustersize 记录最小距离的矩阵
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::showDistance(vector<double> clustersize)
{
	float max=0;
	for (unsigned int i=0;i<clustersize.size();i++){
		if (clustersize[i]>max){
			max=clustersize[i];
		}
	}
	UINT32* depth=new UINT32[m_width*m_height];
	for (unsigned int i=0;i<clustersize.size();i++){
		/*depth[i]=((UINT32)(255-1.0*clustersize[i]/max*255));*/
		depth[i]=clustersize[i]*4;
	}
	IplImage img_header;
	IplImage *img=cvInitImageHeader(&img_header,cvSize(m_width,m_height),IPL_DEPTH_8U,4);
	img_header.imageData=(char*)depth;
	///////////////////////////////////////////////////////////////
	cvSaveImage("depth.jpg",img);
	///////////////////////////////////////////////////////////////
	delete[]depth;
}
/*---------------------------------------------------------------------------------*/
/**
*提取初始条件的种子，种子为自然数的平方数，以矩阵方式均匀分布种子
*
*@param 	[iout]	kseedsl 种子L分量
*@param 	[out]	kseedsa 种子A分量
*@param 	[out]	kseedsb 种子B分量
*@param 	[out]	kseedsx 种子X轴分量
*@param 	[out]	kseedsy 种子Y轴分量
*@param 	[in,out]	K       期望的种子数
*@param 	[in,out]	perturbseeds 未使用
*@param 	[in,out]    edgemag 未使用
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::GetLABXYSeeds_ForGivenStepSize_Rectangle(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	const int&					K,
	const bool&					perturbseeds,
	const vector<double>&		edgemag)
{
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	float xStep;
	float yStep;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);
#if FALSE
	int max_wh=(m_width>m_height)?m_width:m_height; 
	int min_wh=(m_width<m_height)?m_width:m_height;
	float scale_wh=1.0*min_wh/max_wh;
	unsigned int wh_gcd=GetGCD(m_width,m_height);
	float scale_3_2=1.0*2/3;
	if (scale_wh==scale_3_2){
		int K_new=(K/12+0.5)*12;
		if (m_width>m_height){
			xStep=sqrtl(m_height*m_width*4/(12*3));
			yStep=3.0/4.0*xStep;
		}else{
			yStep=sqrtl(m_height*m_width*4/(12*3));
			xStep=3.0/4.0*yStep;
		}
	}else{
		ASSERT(FALSE);
	}
#else
	unsigned int wh_gcd=GetGCD(m_width,m_height);
	int width_min=m_width/wh_gcd;
	int height_min=m_height/wh_gcd;
	int block_min=width_min*height_min;
	double  wh_scale=1.0*m_width/m_height;
	{
		/*int K_new=((int)(K/block_min+0.5))*block_min;*/
		yStep=sqrtl(m_height*m_height/K);
		int K_new=(m_height*m_height)/(yStep*yStep);
		{
			/*yStep=sqrtl(1.0*m_height*m_width/(K_new*wh_scale));*/
			xStep=wh_scale*yStep;
		}
	}
#endif	

	float xstrips_test = (double(m_width)/double(xStep));
	float ystrips_test = (double(m_height)/double(yStep));
	int xstrips = (double(m_width)/double(xStep));
	int ystrips = (double(m_height)/double(yStep));
	ASSERT(xstrips==ystrips);
#if TRUE
	int xerr = m_width  - xStep*xstrips;
	//if(xerr < 0){xstrips--;xerr = m_width - STEP*xstrips;}
	int yerr = m_height - yStep*ystrips;
	//if(yerr < 0){ystrips--;yerr = m_height- STEP*ystrips;}
#endif
	double xerrperstrip = double(xerr)/double(xstrips);
	double yerrperstrip = double(yerr)/double(ystrips);

	int xoff = xStep/2;
	int yoff = yStep/2;
	//-------------------------
	numseeds = xstrips*ystrips;
	//------------------------- 
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for( int y = 0; y < ystrips; y++ )
	{
		int ye = y*yerrperstrip;
		for( int x = 0; x < xstrips; x++ )
		{
			int xe = x*xerrperstrip;
			int seedx = (x*xStep+xoff+xe);
			if(hexgrid){ seedx = x*xStep+(xoff<<(y&0x1))+xe; seedx = min(m_width-1,seedx); }//for hex grid sampling
			int seedy = (y*yStep+yoff+ye);
			int i = seedy*m_width + seedx;

			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
			ASSERT(0);
			kseedsx[n] = seedx;
			kseedsy[n] = seedy;
			n++;
		}
	}


	if(perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
}
/*---------------------------------------------------------------------------------*/
/**
*提取初始条件的种子，种子为自然数的平方数，以矩阵方式均匀分布种子
*
*@param 	[iout]	kseedsl 种子L分量
*@param 	[out]	kseedsa 种子A分量
*@param 	[out]	kseedsb 种子B分量
*@param 	[out]	kseedsx 种子X轴分量
*@param 	[out]	kseedsy 种子Y轴分量
*@param 	[in,out]	K       期望的种子数
*@param 	[in,out]	perturbseeds 未使用
*@param 	[in,out]    edgemag 未使用
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::GetLABXYSeeds_ForGivenStepSize_Rectangle2(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	int&					    NumLabels,
	const bool&					perturbseeds,
	const vector<double>&		edgemag,
	double*  m_lvec,
	double*  m_avec,
	double*  m_bvec)
{	
	TRACE_FUNC();
	float xStep;
	float yStep;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);
	int K=NumLabels;
#if FALSE
	int max_wh=(m_width>m_height)?m_width:m_height; 
	int min_wh=(m_width<m_height)?m_width:m_height;
	float scale_wh=1.0*min_wh/max_wh;
	unsigned int wh_gcd=GetGCD(m_width,m_height);
	float scale_3_2=1.0*2/3;
	if (scale_wh==scale_3_2){
		int K_new=(K/12+0.5)*12;
		if (m_width>m_height){
			xStep=sqrtl(m_height*m_width*4/(12*3));
			yStep=3.0/4.0*xStep;
		}else{
			yStep=sqrtl(m_height*m_width*4/(12*3));
			xStep=3.0/4.0*yStep;
		}
	}else{
		ASSERT(FALSE);
	}
#else
	unsigned int wh_gcd=GetGCD(m_width,m_height);
	int width_min=m_width/wh_gcd;
	int height_min=m_height/wh_gcd;
	int block_min=width_min*height_min;
	double  wh_scale=1.0*m_width/m_height;
	{
		/*int K_new=((int)(K/block_min+0.5))*block_min;*/
		yStep=sqrtl(m_height*m_height/K);
		int K_new=(m_height*m_height)/(yStep*yStep);
		{
			/*yStep=sqrtl(1.0*m_height*m_width/(K_new*wh_scale));*/
			xStep=wh_scale*yStep;
		}
	}
#endif	

	float xstrips_test = (double(m_width)/double(xStep));
	float ystrips_test = (double(m_height)/double(yStep));
	int xstrips = (double(m_width)/double(xStep));
	int ystrips = (double(m_height)/double(yStep));
	ASSERT(xstrips==ystrips);
#if TRUE
	int xerr = m_width  - xStep*xstrips;
	//if(xerr < 0){xstrips--;xerr = m_width - STEP*xstrips;}
	int yerr = m_height - yStep*ystrips;
	//if(yerr < 0){ystrips--;yerr = m_height- STEP*ystrips;}
#endif
	double xerrperstrip = double(xerr)/double(xstrips);
	double yerrperstrip = double(yerr)/double(ystrips);

	int xoff = xStep/2;
	int yoff = yStep/2;
	//-------------------------
	NumLabels=numseeds = xstrips*ystrips;
	//------------------------- 
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for(register int y = 0; y < ystrips; y++ )
	{
		int ye = y*yerrperstrip;
		for(register int x = 0; x < xstrips; x++ )
		{
			int xe = x*xerrperstrip;
			int seedx = (x*xStep+xoff+xe);
			if(hexgrid){ seedx = x*xStep+(xoff<<(y&0x1))+xe; seedx = min(m_width-1,seedx); }//for hex grid sampling
			int seedy = (y*yStep+yoff+ye);
			int i = seedy*m_width + seedx;

			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
			//ASSERT(0);
			kseedsx[n] = seedx;
			kseedsy[n] = seedy;
			n++;
		}
	}


	if(perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
}
/*---------------------------------------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::GetLABXYSeeds_Spiral(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	int&					    NumLabels,
	const bool&					perturbseeds,
	const vector<double>&		edgemag,
	double*  m_lvec,
	double*  m_avec,
	double*  m_bvec)
{	
	TRACE_FUNC();
	float xStep;
	float yStep;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);
	int K=NumLabels;
	
	unsigned int wh_gcd=GetGCD(m_width,m_height);
	int width_min=m_width/wh_gcd;
	int height_min=m_height/wh_gcd;
	int block_min=width_min*height_min;
	double  wh_scale=1.0*m_width/m_height;
	{
		/*int K_new=((int)(K/block_min+0.5))*block_min;*/
		yStep=sqrtl(m_height*m_height/K);
		int K_new=(m_height*m_height)/(yStep*yStep);
		{
			/*yStep=sqrtl(1.0*m_height*m_width/(K_new*wh_scale));*/
			xStep=wh_scale*yStep;
		}
	}

	float xstrips_test = (double(m_width)/double(xStep));
	float ystrips_test = (double(m_height)/double(yStep));
	int xstrips = (double(m_width)/double(xStep));
	int ystrips = (double(m_height)/double(yStep));
	ASSERT(xstrips==ystrips);
#if TRUE
	int xerr = m_width  - xStep*xstrips;
	//if(xerr < 0){xstrips--;xerr = m_width - STEP*xstrips;}
	int yerr = m_height - yStep*ystrips;
	//if(yerr < 0){ystrips--;yerr = m_height- STEP*ystrips;}
#endif
	double xerrperstrip = double(xerr)/double(xstrips);
	double yerrperstrip = double(yerr)/double(ystrips);

	int xoff = xStep/2;
	int yoff = yStep/2;
	//-------------------------
	NumLabels=numseeds = xstrips*ystrips;
	//------------------------- 
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for(register int y = 0; y < ystrips; y++ )
	{
		int ye = y*yerrperstrip;
		for(register int x = 0; x < xstrips; x++ )
		{
			int xe = x*xerrperstrip;
			int seedx = (x*xStep+xoff+xe);
			if(hexgrid){ seedx = x*xStep+(xoff<<(y&0x1))+xe; seedx = min(m_width-1,seedx); }//for hex grid sampling
			int seedy = (y*yStep+yoff+ye);
			int i = seedy*m_width + seedx;

			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
			//ASSERT(0);
			kseedsx[n] = seedx;
			kseedsy[n] = seedy;
			n++;
		}
	}


	if(perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
}
/*---------------------------------------------------------------------------------*/
/**
*利用提取的种子的LABXY综合特性填充图像
*
*@param [in] kseedsl  L分量
*@param [in] kseedsa  A分量
*@param [in] kseedsb  B分量
*@param [in] kseedsx  X坐标分量
*@param [in] kseedsy  Y坐标分量
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::FillImgWithSeeds(
		vector<double> kseedsl,
		vector<double> kseedsa,
		vector<double> kseedsb,
		vector<double> kseedsx,
		vector<double> kseedsy)
{
	int HistDimSPLAB=180;
#if 1
int vectorSize=kseedsa.size();
	{
	int negcount;
	//opencv L, a, b最后的取值变为了 0 - 255
	//origin L 0-100  a -127--128   b-127----128
	IplImage* img=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,3);
	for (int xi=0;xi<pMD->ImgWidth;xi++){
		for (int yj=0;yj<pMD->ImgHeight;yj++){
			int seedIdx=CuiImgLables[yj*pMD->ImgWidth+xi];
			assert(seedIdx>=0&&seedIdx<vectorSize);
			double L=kseedsl[seedIdx]*2.55;
			double A=kseedsa[seedIdx]+127;
			double B=kseedsb[seedIdx]+127;
			CvScalar v_lab =cvScalar(L,A,B);
			cvSet2D(img,yj,xi,v_lab);
		}
	}
	cvCvtColor(img,img,CV_Lab2BGR);
	FileNameSplit fns;
	string filesaveimg_t="SpSeeds.jpg";
#ifdef Use_CString&&_MSC_VER
#if Use_CString&&_MSC_VER
    fns.Parse(CString(FileReadFullPath.c_str()));
	filesaveimg_t=pMD->FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+"SpSeeds.jpg";
#endif
#endif
#ifdef linux
	filesaveimg_t="linuxSpSeeds.jpg";
#endif
	cvSaveImage(filesaveimg_t.c_str(),img);
	cvReleaseImage(&img);
	}
	
#else  
int negcount=0;
int vectorSize=kseedsa.size();
for (int xi=0;xi<pMD->ImgWidth;xi++){
	for (int yj=0;yj<pMD->ImgHeight;yj++){
		int seedIdx=CuiImgLables[yj*pMD->ImgWidth+xi];
		assert(seedIdx>=-1&&seedIdx<vectorSize);
		if (seedIdx==-1){
			negcount++;
		}
	}
}
#endif
#if 0
/////////////////////////////////////////////////////////////////
char data_t[1024];
ofstream outfile;
outfile.open("OriginalSeedLABData.txt",ios::out|ios::app);

if (outfile.is_open()){
	outfile<<FileReadFullPath<<"   ";
		for (int i=0;i<vectorSize;i++){
			double A=kseedsa[i];//-127--128
			double B=kseedsb[i];//-127--128
			double angle=atan2(A,B);//-PI---PI
			double angule_360=(angle+M_PI)*180/M_PI;
			assert(angule_360>=-0.1&&angule_360<=360.1);
			//////////////			
			outfile<<angule_360<<"  ";
			///////////////////			
		}
		outfile<<endl;
	
}else{
	assert(0);
}
outfile.close();	
//////////////////////////////////////////////////////////////////
#endif
#if 1
/////////////////////////////////////////////////////////////////
  double* hist_L=new double[HistDimSPLAB];//0-100
  double* hist_AB=new double[HistDimSPLAB];//-127--128
  double* hist_Count=new double[HistDimSPLAB];//count
  double* hist_AB_length=new double[HistDimSPLAB];//128*1.414
  memset(hist_L,0,sizeof(double)*HistDimSPLAB);
  memset(hist_AB,0,sizeof(double)*HistDimSPLAB);
  memset(hist_Count,0,sizeof(double)*HistDimSPLAB);
  memset(hist_AB_length,0,sizeof(double)*HistDimSPLAB);
	for (int i=0;i<vectorSize;i++){
		double L=kseedsl[i];//0-100
		double A=kseedsa[i];//-127--128
		double B=kseedsb[i];//-127--128
		double AB_length=sqrt(A*A+B*B);
		double angle=atan2(A,B);//-PI---PI
		double angule_360=(angle+M_PI)*180/M_PI;
		assert(angule_360>=-0.1&&angule_360<=360.1);
		int angle_index=cvFloor(angule_360/360*HistDimSPLAB);
		//////////////////////////////////////////
		hist_Count[angle_index]+=1;
		hist_L[angle_index]+=L;
		hist_AB[angle_index]+=1;
		hist_AB_length[angle_index]+=AB_length;
		//////////////////////////////////////////
	}
//////////////////////////////////////////
	for (int j=0;j<HistDimSPLAB;j++){
		hist_L[j]=hist_L[j]/hist_Count[j];
		hist_AB_length[j]=hist_AB_length[j]/hist_Count[j];
	}
	double AB_count_max=cui_GeneralImgProcess::GetMaxValue(&hist_AB[0],HistDimSPLAB);
//////////////////////////////////////////////////////////////////
	int bin_w=1800/HistDimSPLAB;
	int h_bins=HistDimSPLAB;
	int height = 240;
	int scale = 2;  
	int width =h_bins*bin_w;
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvRectangle( hist_img, cvPoint(0,0),cvPoint(width,height),cvScalar(255,255/2,255/2),-1, 8, 0 );
/////////////////////////////////////////////////////////////////////////////////////
	for(int h = 0; h <h_bins; h++){
		/** 获得直方图中的统计次数，计算显示在图像中的高度 */
		float bin_val =hist_AB[h];
		int intensity =cvRound(bin_val*height/AB_count_max);
		intensity=intensity<0?0:intensity;
		/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
		float sita=M_PI*(2*(h+0.5)/HistDimSPLAB-1);
		CvScalar color=cvScalar(hist_L[h]*2.55,hist_AB_length[h]*sin(sita)+127,hist_AB_length[h]*cos(sita)+127);
		cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,height - intensity),cvScalar(0,255/2,255/2),2);
		cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,height - intensity+1),color,-1, 8, 0 );
	}	
	for(int h = 0; h <h_bins; h++){	
		int cut_num=8;
		int cut_step=h_bins/cut_num;
		if (h%cut_step==0){
			cvLine(hist_img, cvPoint((h+0.5)*bin_w,height),cvPoint((h+0.5)*bin_w,0),cvScalar(180,128,128+70),1);
		}		
	}
/////////////////////////////////////////////////////////////////////////////////////
	cvCvtColor(hist_img,hist_img,CV_Lab2BGR);	
	FileNameSplit fns;
#if Use_CString&&_MSC_VER
    fns.Parse(CString(FileReadFullPath.c_str()));
	string filesaveimg=pMD->FileWritePath+FileNameSplit::ConvertCS2string(fns.filename)+"Histogram.jpg";
	cvSaveImage( filesaveimg.c_str(), hist_img );//在"H-S Histogtam"窗口中显示图像
	cvReleaseImage(&hist_img);
#endif
	delete[] hist_L;//0-100
	delete[] hist_AB;//-127--128
	delete[] hist_Count;//count
	delete[] hist_AB_length;//128*1.414
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*根据直方图的统计结果每个单条直方图进行聚类
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::Clustering_ByHistogramOne(void)
{
#if 0
	int old_slic_num;
	int new_slic_num;
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	INT32 *Matrix_Category_Lable=new INT32[pMD->slic_current_num];
	double* Matrix_W=new double[pMD->slic_current_num*pMD->slic_current_num];
	do{
		old_slic_num=pMD->slic_current_num;
		int HistDimSPLAB=180;
		kseedsl.resize(pMD->slic_current_num);
		kseedsa.resize(pMD->slic_current_num);
		kseedsb.resize(pMD->slic_current_num);
		kseedsx.resize(pMD->slic_current_num);
		kseedsy.resize(pMD->slic_current_num);
		//重新获取种子
		GetSeedsLabxy_cuda(pMD->slic_current_num,
			m_width,m_height,
			m_lvec,m_avec,m_bvec,
			kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),CuiImgLables);
		//获取连接矩阵
		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
			CuiImgLables,
			pMD->slic_current_num,
			pMD->Matrix_E.get(),
			pMD->ImgWidth,pMD->ImgHeight,
			"Compute_matrix.matrix","");
		//	绘制直方图
		//获得直方图数据
		KseedsHistData seeddata(HistDimSPLAB);

		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata,
			CuiImgLables,
			pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,
			pMD->FileWritePath);

		HistogramOne2Matrix_Category_Lable(Matrix_Category_Lable,seeddata);	

		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata,
			CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,pMD->FileWritePath);

		UINT32* Matrix_E=pMD->Matrix_E.get();
		for (int i=0;i<pMD->slic_current_num;i++){
			for (int j=0;j<pMD->slic_current_num;j++){
				Matrix_W[i+j*pMD->slic_current_num]=Matrix_E[i+j*pMD->slic_current_num];
			}
		}

		cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			CuiImgData,CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth,pMD->ImgHeight,
			0.9,pMD);
		new_slic_num=pMD->slic_current_num;
	}while (old_slic_num!=new_slic_num);

	//////////////////////////////////////
	delete []Matrix_Category_Lable;
	delete []Matrix_W;
#endif
	
}
/*---------------------------------------------------------------------------------*/
/**
*将不同颜色角度统计到N维直方图中\n
*每一维直方图单独聚类，分别完成彩色聚类和灰度聚类
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC:: Clustering_ByHistogramOneColorGray(void)
{
#if 0
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	/*---------------------------------------------------*/
	int old_slic_num;
	int new_slic_num;
	int HistDimSPLABColor=180;
	int HistDimSPLgray=16;
	INT32 *Matrix_Category_Lable=new INT32[pMD->slic_current_num];
	double* Matrix_W=new double[pMD->slic_current_num*pMD->slic_current_num];
	do{
		old_slic_num=pMD->slic_current_num;
		kseedsl.clear();
		kseedsa.clear();
		kseedsb.clear();
		kseedsx.clear();
		kseedsy.clear();
		kseedsl.resize(pMD->slic_current_num);
		kseedsa.resize(pMD->slic_current_num);
		kseedsb.resize(pMD->slic_current_num);
		kseedsx.resize(pMD->slic_current_num);
		kseedsy.resize(pMD->slic_current_num);
		//重新获取种子
		GetSeedsLabxy_cuda(pMD->slic_current_num,
			m_width,m_height,
			m_lvec,m_avec,m_bvec,
			kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),CuiImgLables);
		//获取连接矩阵
		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
			CuiImgLables,
			pMD->slic_current_num,
			pMD->Matrix_E.get(),
			pMD->ImgWidth,pMD->ImgHeight,
			"Compute_matrix.matrix","");
		//	绘制直方图
		//获得直方图数据
		KseedsHistColorGray seeddata;
		seeddata.ColorHist.resize(HistDimSPLABColor);
		seeddata.GrayHist.resize(HistDimSPLgray);
		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata,
			CuiImgLables,
			pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,
			pMD->FileWritePath);
		/**/
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata.ColorHist,
			CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,pMD->FileWritePath);
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata.GrayHist,
			CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,pMD->FileWritePath);
#if 1
		HistogramOne2Matrix_Category_Lable(Matrix_Category_Lable,seeddata);	
#endif
		UINT32* Matrix_E=pMD->Matrix_E.get();
		for (int i=0;i<pMD->slic_current_num;i++){
			for (int j=0;j<pMD->slic_current_num;j++){
				Matrix_W[i+j*pMD->slic_current_num]=Matrix_E[i+j*pMD->slic_current_num];
			}
		}

#if		UseTextureInColorWithCombine
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			CuiImgLables,
			pMD->ImgWidth,pMD->ImgHeight,
			pMD->p_SpProperty,
			pMD->slic_current_num);
		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
			pMD->slic_current_num,
			pMD->p_SpProperty,
			pMD->Src_ImgData,
			CuiImgLables,
			pMD->ImgWidth,pMD->ImgHeight);
		cui_GeneralImgProcess::Cui_Combination_ImgLabsWithTextureInColor(
			CuiImgData,CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth,pMD->ImgHeight,
			0.9,pMD);
#else
		cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			CuiImgData,CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth,pMD->ImgHeight,
			0.9,pMD);
#endif

		new_slic_num=pMD->slic_current_num;
	}while (old_slic_num!=new_slic_num);

	//////////////////////////////////////
	delete []Matrix_Category_Lable;
	delete []Matrix_W;
	/*---------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 单条聚类: %f（秒）",dfTim);
#endif
	
}
/*---------------------------------------------------------------------------------*/
/**
*
*@param [out]  Matrix_Category_Lable  超像素分类信息
*@param [in] seeddata    彩色直方图与灰度直方图数据
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistData& seeddata)
{
#if 1
	int HistDimSPLAB=seeddata.histDimSPLAB;
	INT32 Matrix_CL_TEST[1024];
	for(int i=0;i<pMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
	for (int j=0;j<HistDimSPLAB;j++){
		int SamSP=seeddata.hist_AB_detail[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=seeddata.hist_AB_detail[j].at(0);
				int changeIdx=seeddata.hist_AB_detail[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*@param [out]  Matrix_Category_Lable  超像素分类信息
*@param [in] seeddata    彩色直方图与灰度直方图数据
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable,KseedsHistColorGray& seeddata)
{

	int HistDimSPLABColor=seeddata.ColorHist.histDimSPLAB;
	int HistDimSPLGray=seeddata.GrayHist.histDimSPLAB;
	INT32 Matrix_CL_TEST[1024];
	for(int i=0;i<pMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
#if 1	
	for (int j=0;j<HistDimSPLABColor;j++){
		int SamSP=seeddata.ColorHist.hist_AB_detail[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=seeddata.ColorHist.hist_AB_detail[j].at(0);
				int changeIdx=seeddata.ColorHist.hist_AB_detail[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif	
#if 1
	for (int j=0;j<HistDimSPLGray;j++){
		int SamSP=seeddata.GrayHist.hist_AB_detail[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=seeddata.GrayHist.hist_AB_detail[j].at(0);
				int changeIdx=seeddata.GrayHist.hist_AB_detail[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*直方图范围聚类
*@deprecated  未使用
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::HistogramRange2Matrix_Category_Lable(
	INT32 *Matrix_Category_Lable,
	INT32 *Matrix_Category_SP_USED,
	KseedsHistData& seeddata,
	int ColorangleSpan)
{
	int HistDimSPLAB=seeddata.histDimSPLAB;
	int PieceNum=9;
	int cut_step=HistDimSPLAB/PieceNum;
	int range=ColorangleSpan/2;
	int stepLR=1.0*range/360*HistDimSPLAB;
	vector<HistRangeData> histrangedata_t(PieceNum);
	int sort_t[2];
	/*-----获取9个分组的最大值---------------------------------------------------------------------*/
	for(int h = 0; h <HistDimSPLAB; h++){			
		if (h%cut_step==0){
			int rangeIdx=h/cut_step;
			histrangedata_t[rangeIdx].start=&(seeddata.hist_AB_Count[h]);
			histrangedata_t[rangeIdx].len=cut_step;
			histrangedata_t[rangeIdx].angle_up=h*2;
			histrangedata_t[rangeIdx].angle_down=(h+cut_step)*2-1;
			histrangedata_t[rangeIdx].StepLR=stepLR;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[rangeIdx].start,cut_step,sort_t,2);
			if (histrangedata_t[rangeIdx].start[sort_t[0]]==0){
				histrangedata_t[rangeIdx].maxHistIdx=-1;
				histrangedata_t[rangeIdx].maxHistLeft=0;
				histrangedata_t[rangeIdx].maxHistRight=0;
				histrangedata_t[rangeIdx].maxHistCenter=0;
			}else{
				histrangedata_t[rangeIdx].maxHistIdx=sort_t[0]+h;
				histrangedata_t[rangeIdx].maxHistLeft=&(histrangedata_t[rangeIdx].start[sort_t[0]-stepLR]);
				histrangedata_t[rangeIdx].maxHistRight=&(histrangedata_t[rangeIdx].start[sort_t[0]+stepLR]);
				histrangedata_t[rangeIdx].maxHistCenter=&(histrangedata_t[rangeIdx].start[sort_t[0]]);
			}
		}		
	}
	/*-----对获取9个分组的最大值进行校正-------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[i].maxHistLeft,Dim,sort_t,sizeof(sort_t)/sizeof(int));
			if (histrangedata_t[i].maxHistCenter!=&histrangedata_t[i].maxHistLeft[sort_t[0]]){
				histrangedata_t[i].maxHistIdx=-1;
				histrangedata_t[i].maxHistLeft=0;
				histrangedata_t[i].maxHistRight=0;
				histrangedata_t[i].maxHistCenter=0;
			}
		}	
	}
	/*---------------记录参与本次聚类的直方图-------------------------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx!=-1){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			for (int j=SameClassDown;j<=SameClassTop;j++){
				seeddata.hist_AB_CountRemove[j]=-1;
			}			
		}	
	}
	/*----------------------------------------------------------------------------*/
#if 0
	/*-----对获取9个分组的范围检查-------------------------------------------*/
	for (int i=0;i<PieceNum-1;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			unsigned int range_1Right=(unsigned int)histrangedata_t[i].maxHistRight;
			unsigned int range_2Left=(unsigned int)histrangedata_t[i+1].maxHistLeft;
			if (range_1Right>range_2Left){
				unsigned int index1=histrangedata_t[i].maxHistIdx;
				unsigned int index2=histrangedata_t[i+1].maxHistIdx;
				double value_1=seeddata.hist_AB_Count[index1];
				double value_2=seeddata.hist_AB_Count[index2];
				if (value_1<value_2){
					histrangedata_t[i].maxHistIdx=-1;
					histrangedata_t[i].maxHistLeft=0;
					histrangedata_t[i].maxHistRight=0;
					histrangedata_t[i].maxHistCenter=0;
				}else{
					histrangedata_t[i+1].maxHistIdx=-1;
					histrangedata_t[i+1].maxHistLeft=0;
					histrangedata_t[i+1].maxHistRight=0;
					histrangedata_t[i+1].maxHistCenter=0;
				}
			}
		}	
	}
#endif
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		CuiImgLables,
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
#if 1
	vector<vector<int>> SameClass(PieceNum);
	SameClass.resize(PieceNum);
	/*-----对获取9个分组进行填充------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx>0){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			for (int j=SameClassDown;j<=SameClassTop;j++){
				int SamSP=seeddata.hist_AB_detail[j].size();
				for (int k=0;k<SamSP;k++){					
					double SameIdx_t=seeddata.hist_AB_detail[j].at(k);
					SameClass[i].push_back(SameIdx_t);
				}
			}
		}
	}	
	/*-----填充LAB矩阵-------------------------------------------------------------*/
	/*------------------------------------------------------*/
	INT32 Matrix_CL_TEST[1024];
	for(int i=0;i<pMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
	/*------------------------------------------------------*/
	for (int j=0;j<PieceNum;j++){
		int SamSP=SameClass[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=SameClass[j].at(0);
				int changeIdx=SameClass[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}

	for (int i=0;i<pMD->slic_current_num;i++){
		spGuid SPguid=pMD->p_SpProperty[i].SPGuid;
		if (Matrix_Category_SP_USED[SPguid.GUIDX+pMD->ImgWidth*SPguid.GUIDY]==-1){
			Matrix_Category_Lable[i]=i;
			Matrix_CL_TEST[i]=i;
		}
	}
	/*------------------------------------------------------*/
	//for (int i=0;i<pMD->slic_current_num;i++){
	//	if (Matrix_Category_Lable_USED[i]==-1){
	//		Matrix_Category_Lable[i]=i;
	//	}
	//	
	//}
#endif
	/*-----------------------------------------------------------------------------*/
#if 1
	/*--------------标记参与聚类的超像素-------------------------------------------*/
	for (int j=0;j<PieceNum;j++){
		int SamSP=SameClass[j].size();
		for (int i=0;i<SamSP;i++){
			int SPIdx=SameClass[j].at(i);
			spGuid SPguid=pMD->p_SpProperty[SPIdx].SPGuid;
			Matrix_Category_SP_USED[SPguid.GUIDX+pMD->ImgWidth*SPguid.GUIDY]=-1;

		}
	}
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*根据直方图统计数据填充SameClassLink数组
*
*@param [] Matrix_Category_Lable  矩阵
*@param [] seeddata       直方图统计数据
*@param [] SameClassLink 属于同一个类的超像素块投入到同一个vector
*@param  ColorangleSpan  颜色跨度
*
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::HistogramRange2Matrix_Category_Lable_SameLink(
	INT32 *Matrix_Category_Lable,
	KseedsHistData& seeddata,
	vector<vector<int>>& SameClassLink,
	int ColorangleSpan)
{
	int HistDimSPLAB=seeddata.histDimSPLAB;
	int PieceNum=9;
	int cut_step=HistDimSPLAB/PieceNum;
	int range=ColorangleSpan/2;
	int stepLR=1.0*range/360*HistDimSPLAB;
	vector<HistRangeData> histrangedata_t(PieceNum);
	int sort_t[2];
	/*-----获取9个分组的最大值---------------------------------------------------------------------*/
	for(int h = 0; h <HistDimSPLAB; h++){			
		if (h%cut_step==0){
			int rangeIdx=h/cut_step;
			histrangedata_t[rangeIdx].start=&(seeddata.hist_AB_Count[h]);
			histrangedata_t[rangeIdx].len=cut_step;
			histrangedata_t[rangeIdx].angle_up=h*2;
			histrangedata_t[rangeIdx].angle_down=(h+cut_step)*2-1;
			histrangedata_t[rangeIdx].StepLR=stepLR;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[rangeIdx].start,cut_step,sort_t,2);
			if (histrangedata_t[rangeIdx].start[sort_t[0]]==0){
				histrangedata_t[rangeIdx].maxHistIdx=-1;
				histrangedata_t[rangeIdx].maxHistLeft=0;
				histrangedata_t[rangeIdx].maxHistRight=0;
				histrangedata_t[rangeIdx].maxHistCenter=0;
			}else{
				histrangedata_t[rangeIdx].maxHistIdx=sort_t[0]+h;
				histrangedata_t[rangeIdx].maxHistLeft=&(histrangedata_t[rangeIdx].start[sort_t[0]-stepLR]);
				histrangedata_t[rangeIdx].maxHistRight=&(histrangedata_t[rangeIdx].start[sort_t[0]+stepLR]);
				histrangedata_t[rangeIdx].maxHistCenter=&(histrangedata_t[rangeIdx].start[sort_t[0]]);
				///////////////////////////////////////////////////////////////////////////////////
				histrangedata_t[rangeIdx].maxHistLeft=(histrangedata_t[rangeIdx].maxHistLeft<&seeddata.hist_AB_Count[0])?&seeddata.hist_AB_Count[0]:histrangedata_t[rangeIdx].maxHistLeft;
				histrangedata_t[rangeIdx].maxHistRight=(histrangedata_t[rangeIdx].maxHistRight>&seeddata.hist_AB_Count[seeddata.histDimSPLAB-1])?&seeddata.hist_AB_Count[seeddata.histDimSPLAB-1]:histrangedata_t[rangeIdx].maxHistRight;
				///////////////////////////////////////////////////////////////////////////////////
			}
		}		
	}
	/*-----对获取9个分组的最大值进行校正-------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			Dim+=1;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[i].maxHistLeft,Dim,sort_t,sizeof(sort_t)/sizeof(int));
			if (histrangedata_t[i].maxHistCenter!=&histrangedata_t[i].maxHistLeft[sort_t[0]]){
				histrangedata_t[i].maxHistIdx=-1;
				histrangedata_t[i].maxHistLeft=0;
				histrangedata_t[i].maxHistRight=0;
				histrangedata_t[i].maxHistCenter=0;
			}
		}	
	}
	/*---------------记录参与本次聚类的直方图-------------------------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx!=-1){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			for (int j=SameClassDown;j<=SameClassTop;j++){
				seeddata.hist_AB_CountRemove[j]=-1;
			}			
		}	
	}
	/*----------------------------------------------------------------------------*/
#if 0
/*-----对获取9个分组的范围检查-------------------------------------------*/
	for (int i=0;i<PieceNum-1;i++){
		unsigned int Dim=(unsigned int)histrangedata_t[i].maxHistRight-(unsigned int)histrangedata_t[i].maxHistLeft;
		Dim=Dim/sizeof(double);
		if (Dim!=0){
			unsigned int range_1Right=(unsigned int)histrangedata_t[i].maxHistRight;
			unsigned int range_2Left=(unsigned int)histrangedata_t[i+1].maxHistLeft;
			if (range_1Right>range_2Left){
				unsigned int index1=histrangedata_t[i].maxHistIdx;
				unsigned int index2=histrangedata_t[i+1].maxHistIdx;
				double value_1=seeddata.hist_AB_Count[index1];
				double value_2=seeddata.hist_AB_Count[index2];
				if (value_1<value_2){
					histrangedata_t[i].maxHistIdx=-1;
					histrangedata_t[i].maxHistLeft=0;
					histrangedata_t[i].maxHistRight=0;
					histrangedata_t[i].maxHistCenter=0;
				}else{
					histrangedata_t[i+1].maxHistIdx=-1;
					histrangedata_t[i+1].maxHistLeft=0;
					histrangedata_t[i+1].maxHistRight=0;
					histrangedata_t[i+1].maxHistCenter=0;
				}
			}
		}	
	}
#endif
#if 1	
	/*-----对获取9个分组进行填充------------------------------------------*/
	for (int i=0;i<PieceNum;i++){
		if (histrangedata_t[i].maxHistIdx>0){
			int SameClassTop=histrangedata_t[i].maxHistIdx+histrangedata_t[i].StepLR;
			int SameClassDown=histrangedata_t[i].maxHistIdx-histrangedata_t[i].StepLR;
			if (SameClassDown<0){
				SameClassDown=0;
			}
			if (SameClassTop>HistDimSPLAB-1){
				SameClassTop=HistDimSPLAB-1;
			}
			/////////////////////////////////////////////
			SameClassLink.push_back(vector<int>());
			int SameClassLinkIdx=SameClassLink.size()-1;
			//////////////////////////////////////////////
			for (int j=SameClassDown;j<=SameClassTop;j++){
				int SamSP=seeddata.hist_AB_detail[j].size();
				for (int k=0;k<SamSP;k++){					
					int SameIdx_t=seeddata.hist_AB_detail[j].at(k);
					SameClassLink[SameClassLinkIdx].push_back(SameIdx_t);
				}
			}
		}
	}	
#endif
#if 1
	/*-----填充Labels矩阵-------------------------------------------------------------*/
	INT32 Matrix_CL_TEST[2048];
	///初始化Category
	for(int i=0;i<pMD->slic_current_num;i++){
		Matrix_Category_Lable[i]=i;
		Matrix_CL_TEST[i]=i;
	}
	/*--修正Category--------------------------------------------*/
	for (int j=0;j<SameClassLink.size();j++){
		int SamSP=SameClassLink[j].size();
		for (int i=0;i<SamSP;i++){
			if (SamSP>=2&&i<SamSP-1){
				int orgIdx=SameClassLink[j].at(0);
				int changeIdx=SameClassLink[j].at(i+1);
				Matrix_Category_Lable[changeIdx]=orgIdx;
				Matrix_CL_TEST[changeIdx]=orgIdx;
			}
		}
	}
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*
*按照一定的颜色跨度在在N维色彩直方图上聚类\n
*最大值
*@param ColorangleSpan 颜色跨度
*/
/*---------------------------------------------------------------------------------*/
void SLIC::Clustering_ByHistogramMaxHist(int ColorangleSpan)
{
#if 0
int old_slic_num;
	int new_slic_num;
	
	double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;

	INT32 *Matrix_Category_Lable=new INT32[pMD->slic_current_num];
	INT32 *Matrix_Category_SP_USED=new INT32[pMD->ImgWidth*pMD->ImgHeight];
	memset(Matrix_Category_SP_USED,0,sizeof(INT32)*pMD->ImgWidth*pMD->ImgHeight);
//	INT32 Matrix_CL_TEST[1024];
	double* Matrix_W=new double[pMD->slic_current_num*pMD->slic_current_num];
	int HistDimSPLAB=180;
	KseedsHistData seeddata(HistDimSPLAB);
	do{	
		old_slic_num=pMD->slic_current_num;		
#if 1
		do {
#if 1
			kseedsl.resize(pMD->slic_current_num);
			kseedsa.resize(pMD->slic_current_num);
			kseedsb.resize(pMD->slic_current_num);
			kseedsx.resize(pMD->slic_current_num);
			kseedsy.resize(pMD->slic_current_num);
			///重新获取种子
			GetSeedsLabxy_cuda(
				pMD->slic_current_num,
				m_width,m_height,
				m_lvec,m_avec,m_bvec,
				kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),
				CuiImgLables);
			///获取连接矩阵
			cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
				CuiImgLables,
				pMD->slic_current_num,
				pMD->Matrix_E.get(),
				pMD->ImgWidth,pMD->ImgHeight,
				"Compute_matrix.matrix","");
			///	绘制直方图
			///获得直方图数据
			cui_GeneralImgProcess::Get_Kseeds_Histogram(
				kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
				seeddata,
				CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
				pMD->FileReadFullPath,pMD->FileWritePath);
	
			seeddata.removeUsedHist();

			this->HistogramRange2Matrix_Category_Lable(Matrix_Category_Lable,Matrix_Category_SP_USED,seeddata,ColorangleSpan);

			cui_GeneralImgProcess::Draw_Kseeds_Histogram(
				kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
				seeddata,
				CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
				pMD->FileReadFullPath,pMD->FileWritePath);

			UINT32* Matrix_E=pMD->Matrix_E.get();
			for (int i=0;i<pMD->slic_current_num;i++){
				for (int j=0;j<pMD->slic_current_num;j++){
					Matrix_W[i+j*pMD->slic_current_num]=Matrix_E[i+j*pMD->slic_current_num];
				}
			}

			cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
				CuiImgData,CuiImgLables,
				Matrix_Category_Lable,
				Matrix_W,
				pMD->slic_current_num,
				pMD->ImgWidth,pMD->ImgHeight,
				0.9,pMD);
			
#endif
		} while (0);
		new_slic_num=pMD->slic_current_num;
#endif	
	}while (old_slic_num!=new_slic_num);
	//////////////////////////////////////
	delete []Matrix_Category_Lable;
	delete []Matrix_Category_SP_USED;
	delete []Matrix_W;
#endif
	
}
/*---------------------------------------------------------------------------------*/
/**
*色彩多尺度聚类\n
*
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::Clustering_New(void)
{
#if 0
double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	/*---------------------------------------------------*/
do {

	this->Clustering_ByHistogramOneColorGray();
	this->Clustering_ByHistogramMaxHist_NoIterationColor(10);
} while (0);

this->Cui_SurroundClassification();
GetSeedsLabxy_cuda(
	pMD->slic_current_num,
	m_width,m_height,
	m_lvec,m_avec,m_bvec,
	kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),
	CuiImgLables);
#if SaveAverageImg2Disk 
cui_GeneralImgProcess::Draw_Kseeds_AverageImg(
	kseedsl,kseedsa,kseedsb,
	CuiImgLables,
	m_width,
	m_height,
	FileReadFullPath,
	FileWritePath);
#endif
	/*---------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 单条多条聚类: %f（秒）",dfTim);
#endif
	
}
/*---------------------------------------------------------------------------------*/
/**
*未使用
*按照一定的颜色跨度在在N维色彩直方图上聚类
*@param ColorangleSpan 颜色跨度
*
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::Clustering_ByHistogramMaxHist_NoIteration(int ColorangleSpan)
{
#if 0
double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
	INT32 *Matrix_Category_Lable=new INT32[pMD->slic_current_num];
	double* Matrix_W=new double[pMD->slic_current_num*pMD->slic_current_num];
	int HistDimSPLAB=180;
	//int old_slic_num;
	//int new_slic_num;
	int IterationTimes=2-1;
	KseedsHistData seeddata(HistDimSPLAB);
	vector<vector<int>> SameLink;
	kseedsl.resize(pMD->slic_current_num);
	kseedsa.resize(pMD->slic_current_num);
	kseedsb.resize(pMD->slic_current_num);
	kseedsx.resize(pMD->slic_current_num);
	kseedsy.resize(pMD->slic_current_num);
do{
	//old_slic_num=pMD->slic_current_num;
	seeddata.ClearUsedHistArray();
	/////////////////////////////////////
	GetSeedsLabxy_cuda(
		pMD->slic_current_num,
		m_width,m_height,
		m_lvec,m_avec,m_bvec,
		kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),
		CuiImgLables);

	cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
		CuiImgLables,
		pMD->slic_current_num,
		pMD->Matrix_E.get(),
		pMD->ImgWidth,pMD->ImgHeight,
		"Compute_matrix.matrix","");

	cui_GeneralImgProcess::Get_Kseeds_Histogram(
				kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
				seeddata,
				CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
				pMD->FileReadFullPath,pMD->FileWritePath);

	cui_GeneralImgProcess::Draw_Kseeds_Histogram(
		kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
		seeddata,
		CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
		pMD->FileReadFullPath,pMD->FileWritePath);

	UINT32* Matrix_E=pMD->Matrix_E.get();

		do {
			seeddata.removeUsedHist();
			this->HistogramRange2Matrix_Category_Lable_SameLink(Matrix_Category_Lable,seeddata,SameLink,ColorangleSpan);
			cui_GeneralImgProcess::Draw_Kseeds_Histogram(
				kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
				seeddata,
				CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
				pMD->FileReadFullPath,pMD->FileWritePath);			
			for (int i=0;i<pMD->slic_current_num;i++){
				for (int j=0;j<pMD->slic_current_num;j++){
					Matrix_W[i+j*pMD->slic_current_num]=Matrix_E[i+j*pMD->slic_current_num];
				}
			}
			/*组合图块*/
		} while (seeddata.IsRemoveALL()!=true);

	cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
		CuiImgData,CuiImgLables,
		Matrix_Category_Lable,
		Matrix_W,
		pMD->slic_current_num,
		pMD->ImgWidth,pMD->ImgHeight,
		0.9,pMD);
	////////////////////////////////
	//new_slic_num=pMD->slic_current_num;
} while (IterationTimes--);
	//////////////////////////////////////
	delete []Matrix_Category_Lable;
	delete []Matrix_W;


#if 1
	GetSeedsLabxy_cuda(
		pMD->slic_current_num,
		m_width,m_height,
		m_lvec,m_avec,m_bvec,
		kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),
		CuiImgLables);
	cui_GeneralImgProcess::Get_Kseeds_Histogram(
		kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
		seeddata,
		CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
		pMD->FileReadFullPath,pMD->FileWritePath);
	seeddata.ClearUsedHistArray();
	cui_GeneralImgProcess::Draw_Kseeds_Histogram(
		kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
		seeddata,
		CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
		pMD->FileReadFullPath,pMD->FileWritePath);
#endif
	
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*按照一定的颜色跨度在在N维色彩直方图上聚类
*
*@param ColorangleSpan 颜色跨度
*
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC::Clustering_ByHistogramMaxHist_NoIterationColor(int ColorangleSpan)
{
#if 0
double*  m_lvec=NULL;
	double*  m_avec=NULL;
	double*  m_bvec=NULL;
 	INT32 *Matrix_Category_Lable=new INT32[pMD->slic_current_num];
	double* Matrix_W=new double[pMD->slic_current_num*pMD->slic_current_num];
	int HistDimSPLABColor=180;
	int old_slic_num;
	int new_slic_num;
	int IterationTimes=2-1;
	KseedsHistColorGray seeddata;
	seeddata.ColorHist.resize(HistDimSPLABColor);
	vector<vector<int>> SameLink;
	kseedsl.clear();
	kseedsa.clear();
	kseedsb.clear();
	kseedsx.clear();
	kseedsy.clear();
	kseedsl.resize(pMD->slic_current_num);
	kseedsa.resize(pMD->slic_current_num);
	kseedsb.resize(pMD->slic_current_num);
	kseedsx.resize(pMD->slic_current_num);
	kseedsy.resize(pMD->slic_current_num);
	do{
		old_slic_num=pMD->slic_current_num;
		seeddata.ColorHist.ClearUsedHistArray();
		/////////////////////////////////////
		GetSeedsLabxy_cuda(
			pMD->slic_current_num,
			m_width,m_height,
			m_lvec,m_avec,m_bvec,
			kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),
			CuiImgLables);

		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
			CuiImgLables,
			pMD->slic_current_num,
			pMD->Matrix_E.get(),
			pMD->ImgWidth,pMD->ImgHeight,
			"Compute_matrix.matrix","");

		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata,
			CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,pMD->FileWritePath);

		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
			seeddata.ColorHist,
			CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
			pMD->FileReadFullPath,pMD->FileWritePath);

		UINT32* Matrix_E=pMD->Matrix_E.get();
		do {

			SameLink.clear();
				do {
					seeddata.ColorHist.removeUsedHist();
					this->HistogramRange2Matrix_Category_Lable_SameLink(Matrix_Category_Lable,seeddata.ColorHist,SameLink,ColorangleSpan);
					cui_GeneralImgProcess::Draw_Kseeds_Histogram(
						kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
						seeddata.ColorHist,
						CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
						pMD->FileReadFullPath,pMD->FileWritePath);			
					for (int i=0;i<pMD->slic_current_num;i++){
						for (int j=0;j<pMD->slic_current_num;j++){
							Matrix_W[i+j*pMD->slic_current_num]=Matrix_E[i+j*pMD->slic_current_num];
						}
					}
					/*组合图块*/
				} while (seeddata.ColorHist.IsRemoveALL()!=true);
		} while (0);
		
#if		UseTextureInColorWithCombine
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			CuiImgLables,
			pMD->ImgWidth,pMD->ImgHeight,
			pMD->p_SpProperty,
			pMD->slic_current_num);
		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
			pMD->slic_current_num,
			pMD->p_SpProperty,
			pMD->Src_ImgData,
			CuiImgLables,
			pMD->ImgWidth,pMD->ImgHeight);
		cui_GeneralImgProcess::Cui_Combination_ImgLabsWithTextureInColor(
			CuiImgData,CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth,pMD->ImgHeight,
			0.9,pMD);
#else
		cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			CuiImgData,CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth,pMD->ImgHeight,
			0.9,pMD);		
#endif
		////////////////////////////////
		new_slic_num=pMD->slic_current_num;
	//} while (IterationTimes--);
		} while (old_slic_num!=new_slic_num);
	//////////////////////////////////////
	delete []Matrix_Category_Lable;
	delete []Matrix_W;


#if 1
	GetSeedsLabxy_cuda(
		pMD->slic_current_num,
		m_width,m_height,
		m_lvec,m_avec,m_bvec,
		kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),
		CuiImgLables);
	cui_GeneralImgProcess::Get_Kseeds_Histogram(
		kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
		seeddata.ColorHist,
		CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
		pMD->FileReadFullPath,pMD->FileWritePath);
	seeddata.ColorHist.ClearUsedHistArray();
	cui_GeneralImgProcess::Draw_Kseeds_Histogram(
		kseedsl,kseedsa,kseedsb,kseedsx,kseedsy,
		seeddata.ColorHist,
		CuiImgLables,pMD->ImgWidth,pMD->ImgHeight,
		pMD->FileReadFullPath,pMD->FileWritePath);
#endif
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/