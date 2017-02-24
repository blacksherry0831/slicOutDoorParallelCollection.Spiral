#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include <stdio.h>
//#include<stdio.h>
#include<assert.h>
#ifndef UINT32
typedef unsigned int UINT32;
#endif
#if linux||__linux||__linux__ || __GNUC__

#ifndef nullptr
#define nullptr 0
#endif

#ifndef _DEBUG
#define  _DEBUG 1
#endif
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/syscall.h>//Linux system call for thread id
#include <pthread.h>

#endif



#ifdef _MSC_VER

#include <windows.h>

#endif

/*------------------------------------------------------------------------------------------*/
/**
*ʱ��2014- 11-4
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void PerformSuperpixelSLIC_gpu(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M);
/*------------------------------------------------------------------------------------------*/
extern "C" void PerformSuperpixelSLIC_ThetaMLXY_gpu_simplify(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,   
	const int m_height,
	double*     host_sita_n,
	double*     host_m_n,
	double*     host_L_n,
	double*     host_X_n,
	double*     host_Y_n,
	double*     host_kseedsTheta,
	double*     host_kseedsM,
	double*     host_kseedsL,
	double*     host_kseedsX,
	double*     host_kseedsY,
	int*		host_klabels,
	const int			STEP);
/*------------------------------------------------------------------------------------------*/
/**
*ʱ��2014- 11-4
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void PerformSuperpixelSLIC_gpu_simplify(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M);
extern "C" void PerformSuperpixelSLIC_gpu_simplify2(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void DrawContoursAroundSegments_gpu(
	unsigned int*			ubuff,
	int*					labels,
	const int				width,
	const int				height,
	const unsigned int	color_in,
	const unsigned int  color_out);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void  GetSeedsLabxy_gpu(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				host_m_lvec,
	double*				host_m_avec,
	double*				host_m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" int GetThreadIdSelfwinlinux(void);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void Get_Nighbour_E_matrix_gpu(
	int*			labels,
	const  int		NumLabels,
	UINT32 *		Matrix_E,
	const int		width,
	const int		height);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void InitMaskTemplate(
	unsigned int*			dev_mask,
	int*					dev_labels,
	const int				width,
	const int				height,
	const int                Pixel);
void StatisticsThetaMLXY_Seed_collect_CPU_ALL(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*       host_sita_n,
	double*       host_m_n,
	double*       host_L_n,
	double*       host_X_n,
	double*       host_Y_n,	
	int*				host_klabels,
	unsigned int*       host_img_mask,
    double*	      host_kseed_sita_n,
	double*	      host_kseed_m_n,
	double*	      host_kseed_L_n,
	double*       host_kseed_X_n,
	double*	      host_kseed_Y_n);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" bool cudaInit_CUI(void);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" bool cudaDeInit_CUI(void);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void cudaGetLastError_Sync_CUI(void);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
#define  NULL_MATH    1
#define  MATH_USE  0
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__device__ int a;
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
//__inline__ __device__ double sqrt(double x)
//{
//#if 0
//	return 0;
//#else
//	/*return sqrt(x);*/
//#endif
//}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
//__inline__ __device__ double atan2(double a,double b)
//{
//#if 0
//	return 0;
//#else
//	/*return atan2(a,b);*/
//#endif
//}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
//__inline__ __device__ double fabs(double x)
//{
//#if 0
//	return 0;
//#else
//	/*return fabs(x);*/
//#endif
//}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
struct Lock {
	int *mutex;
	Lock( void ) {
		( cudaMalloc( (void**)&mutex, sizeof(int) ) );
		( cudaMemset( mutex, 0, sizeof(int) ) );
	}
	~Lock( void ) {
		cudaFree( mutex );
	}
	__device__ void lock( void ) {
		while( atomicCAS( mutex, 0, 1 ) != 0 );
	}
	__device__ void unlock( void ) {
		atomicExch( mutex, 0 );
	}
};
/*------------------------------------------------------------------------------------------*/
/**
*
*cuda��ʼ��
*@retval true ��ʼ���ɹ�
*@retval false ��ʼ��ʧ��
*/
/*------------------------------------------------------------------------------------------*/
bool cudaInit_CUI(void)
{
		 int i;
		 int device_count;

		 if( cudaGetDeviceCount(&device_count) )
		 {
			 printf(" There is zero device beyond 1.0\n"); 
			 return false;
		 }
#if _DEBUG
		//printf("CUDA DEVICE NUMS: %d \n",device_count);
#endif
		 // �ҵ�һ�����õ��豸
		 for(i=0;i<device_count;i++)
		 {
			  struct cudaDeviceProp device_prop;
			  if(cudaGetDeviceProperties(&device_prop,i)==cudaSuccess)
			  {
#if 0
	printf("device properties is :\n"
			"\t device name is %s\n"
			"\t totalGlobalMem is %d\n"
			"\t sharedMemPerBlock is %d\n"
			"\t regsPerBlock is %d\n"
			"\t warpSize is %d\n"
			"\t memPitch is %d\n"
			"\t maxThreadsPerBlock is %d\n"
			"\t maxThreadsDim [3] is %d X %d X %d\n"
			"\t maxGridSize [3] is %d X %d X %d\n"
			"\t totalConstMem is %d\n"
			"\t device version is major %d ,minor %d\n"
			"\t clockRate is %d\n"
			"\t textureAlignment is %d\n"
			"\t deviceOverlap is %d\n"
			"\t multiProcessorCount is %d\n",
			device_prop.name,
			device_prop.totalGlobalMem,
			device_prop.sharedMemPerBlock,
			device_prop.regsPerBlock,
			device_prop.warpSize,
			device_prop.memPitch,
			device_prop.maxThreadsPerBlock,
			device_prop.maxThreadsDim[0],device_prop.maxThreadsDim[1],device_prop.maxThreadsDim[2],
			device_prop.maxGridSize[0],device_prop.maxGridSize[1],device_prop.maxGridSize[2],
			device_prop.totalConstMem,
			device_prop.major,device_prop.minor,
			device_prop.clockRate,
			device_prop.textureAlignment,
			device_prop.deviceOverlap,
			device_prop.multiProcessorCount);
#endif				
					/* if(cudaSetDevice(i)==cudaSuccess){
						 printf("USE GPU ID: %d \n",i);
						 return true;
					 }*/
				break;
			  }

		 }
		 
		 if(i==device_count)
		 {
			  printf("Get the propertites of device occurred error\n");
			  return false;
		 }
		 //�п����豸
		 int default_id,expect_id;		 
		 int thread_id=GetThreadIdSelfwinlinux();
		 expect_id=thread_id%device_count;
		
		 printf("CUDA NUMS: %d, ID: %d,EXPECT: %d \n",device_count,thread_id,expect_id);

		 cudaGetDevice(&default_id);

		 if(default_id==expect_id){
			 return true;
		 }else{			 
			 //�豸����
			 if(cudaSetDevice(expect_id)==cudaSuccess){
				 //printf("USE GPU ID: %d \n",expect_id);
				 return true;
			 }
		 }
		 
		 //////////////////////////////////////////
	//	 int ket=kernelExecTimeoutEnabled();
		 /////////////////////////////////////////
		 return false;
	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*cudaȥ��ʼ��
*@return 
*@retval true ��ʼ���ɹ�
*@retval false ��ʼ��ʧ��
*/
/*------------------------------------------------------------------------------------------*/
bool cudaDeInit_CUI(void)
{
#if 0
	cudaError_t cudaStatus;
	cudaStatus=cudaDeviceReset();
	if(cudaStatus!=cudaSuccess){
		return false;
	}else{
		return true;
	}
#else
	return true;
#endif
	
}
/*------------------------------------------------------------------------------------------*/
/**
*Cuda�ȴ���ʱ��ȡ�������
*
*
*/
/*------------------------------------------------------------------------------------------*/
void cudaGetLastError_Sync_CUI(void)
{
	cudaError_t cudaStatus_syn,cudaStatus;
	const char  *errstr=NULL;


	do 
	{
		cudaStatus_syn=cudaDeviceSynchronize();
		if (cudaStatus_syn==cudaSuccess)
		{
			break;

		}else{
			cudaStatus=cudaGetLastError();
			if (cudaStatus!=cudaSuccess){
				errstr=cudaGetErrorString(cudaStatus);
				printf("cudaDeviceSynchronize: %s",errstr);
			}
		}
		


	} while (cudaStatus_syn!=cudaSuccess);

}
/*------------------------------------------------------------------------------------------*/
/**
*
*ת��LABɫ�ʿռ�ɽǶȡ�ģ�������ȱ�ʾ��
*
*@param m_width ͼ����
*@param m_height ͼ��߶�
*@param L ���ص��L����
*@param A ���ص��A����
*@param B ���ص��B����
*@param X ���ص��X����
*@param Y ���ص��Y����
*@param sita_n ���ص�ĽǶ� ����һ����[0,1]��
*@param m_n    ���ص��ģ�� ����һ����[0,1]��
*@param L_n    ���ص������ ����һ����[0,1]��
*@param X_n    ���ص��X���꣨��һ����[0,1]��
*@param Y_n    ���ص��Y���꣨��һ����[0,1]��
*
*/
/*------------------------------------------------------------------------------------------*/
__inline__ __host__ __device__ void ConvertLab2oml(
	int m_width,
	int m_height,
	double L,
	double A,
	double B,
	double X,
	double Y,
	double* sita_n,
	double* m_n,
	double* L_n,
	double* X_n,
	double* Y_n)
{
	assert(L>=0&&L<=100+1);
	assert(A>=-128&&A<=128+1);
	assert(B>=-128&&B<=128+1);
	assert(X>=0&&X<=m_width+1);
	assert(Y>=0&&Y<=m_height+1);
#if 1
	///////////////////////////////////////
	*sita_n=atan2(A,B);
	*sita_n=*sita_n/(2*3.1415927)+0.5;
	///////////////////////////////////////
	*m_n=sqrt(A*A+B*B);
	*m_n=*m_n/(128*1.415);
	///////////////////////////////////////
	*L_n=L/100;
	assert(*L_n>=0&&*L_n<=1+1E-1);
	///////////////////////////////////////
	*X_n=X/m_width;
	assert(*X_n>=0&&*X_n<=1+1E-1);
	*Y_n=Y/m_height;
	assert(*Y_n>=0&&*Y_n<=1+1E-1);
#endif
	assert(*sita_n>=0-0.1&&*sita_n<=1+0.1);
	assert(*m_n>=0-0.1&&*m_n<=1+0.1);
	assert(*L_n>=0-0.1&&*L_n<=1+0.1);
	assert(*X_n>=0-0.1&&*X_n<=1+0.1);
	assert(*Y_n>=0-0.1&&*Y_n<=1+0.1);
}
/*------------------------------------------------------------------------------------------*/
/**
*
*�����������ص�ľ���
*@image html image027.png "ɫ�ʹ�ʽ"
*@image html image029.png "���빫ʽ"
*@image html image031.png "�����ۺϹ�ʽ"
*
*
*@param  alpha     �Ƕ�ϵ��
*@param  betta     ģ��ϵ��
*@param  gama      ����ϵ��
*@param  fai       ����ϵ��
*@param  sita_n0   ����0�ĽǶ�
*@param  m_n0      ����0��ģ��
*@param  L_n0      ����0������
*@param  X_n0      ����0�ĵȼ�X����
*@param  Y_n0      ����0�ĵȼ�Y����
*@param  sita_n1   ����1�ĽǶ�
*@param  m_n1      ����1��ģ��
*@param  L_n1      ����1������
*@param  X_n1      ����1�ĵȼ�X����
*@param  Y_n1      ����1�ĵȼ�Y����
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__inline__ __device__ double  CalculateNewDistance(
	float alpha,
	float betta,
	float gama,
	float fai,
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
	assert(sita_n0>=0-0.1&&sita_n0<=1+0.1);
	assert(L_n0>=0-0.1&&L_n0<=1+0.1);
	assert(X_n0>=0-0.1&&X_n0<=1+0.1);
	assert(Y_n0>=0-0.1&&Y_n0<=1+0.1);
	assert(sita_n1>=0-0.1&&sita_n1<=1+0.1);
    assert(L_n1>=0-0.1&&L_n1<=1+0.1);
	assert(X_n1>=0-0.1&&X_n1<=1+0.1);
	assert(Y_n1>=0-0.1&&Y_n1<=1+0.1);
#if 0
	double dst=alpha*fabs(sita_n1-sita_n0)+betta*fabs(m_n1-m_n0)+gama*fabs(L_n1-L_n0);
	double dst_xy=fai*sqrt((X_n1-X_n0)*(X_n1-X_n0)+(Y_n1-Y_n0)*(Y_n1-Y_n0));
	dst+=dst_xy;
	return dst;
#else
	double dst=alpha*fabs(sita_n1-sita_n0)+betta*fabs(m_n1-m_n0)+gama*fabs(L_n1-L_n0);
	double dst_xy=fai*(fabs(X_n1-X_n0)+fabs(Y_n1-Y_n0));
	dst+=dst_xy;
	return dst;
#endif
	
}
/*------------------------------------------------------------------------------------------*/
/**
*һ���������������Ӽ�����뱣����Сֵ
*
*@param     kseedsSize ���Ӹ��� 
*@param     alpha      �Ƕ�ϵ��
*@param     betta      ģ��ϵ��
*@param     gama       ����ϵ��
*@param     fai        �������
*@param     m_width    ͼ����
*@param     m_height   ͼ��߶�
*@param   [in]  m_lvec     ͼ��L����
*@param   [in]  m_avec     ͼ��A����
*@param   [in] m_bvec     ͼ��B����
*@param   [in]  dev_sita_n  ͼ��Ƕȷ���
*@param   [in] dev_m_n    ͼ��ģ������
*@param   [in]  dev_L_n   ͼ�����ȷ���
*@param   [in] dev_X_n   ͼ��X����
*@param   [in]  dev_Y_n   ͼ��Y����
*@param   [in]  kseedsl   ���ӵ�L����
*@param   [in] kseedsa   ���ӵ�A����
*@param   [in] kseedsb   ���ӵ�B����
*@param   [in] kseedsx   ���ӵ�X�������
*@param   [in] kseedsy   ���ӵ�Y�������
*@param   [in] dev_kseed_sita_n  ���ӽǶȷ���
*@param   [in] dev_kseed_m_n     ����ģ������
*@param   [in] dev_kseed_L_n     ���ӵ�L����
*@param   [in] dev_kseed_X_n     ���ӵ�X����
*@param   [in] dev_kseed_Y_n     ���ӵ�Y����
*@param   [in] klabels           ͼ��ı궨��Ϣ
*@param   [in] distvec           δʹ��
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMinimumOnLab(
	const int kseedsSize,
	float alpha,
	float betta,
	float gama,
	float fai,
	const int m_width,
	const int m_height,
	double* m_lvec,
	double* m_avec,
	double* m_bvec,
	double* kseedsl,
	double* kseedsa,
	double* kseedsb,
	double* kseedsx,
	double* kseedsy,
	int*   klabels,
	double* distvec,
	const int			STEP,
	const double		M)
{

	int x1, y1, x2, y2;
	double l, a, b;
	double dist;
	//double distxy;
	int SpIdx = threadIdx.x+blockIdx.x*blockDim.x;
	int n=SpIdx;
	int offset = STEP;
	if (SpIdx<kseedsSize){
#if 1
///////////////////////////////////////////////
		y1 = max(0.0,			kseedsy[n]-offset);
		y2 = min((double)m_height,	kseedsy[n]+offset);
		x1 = max(0.0,			kseedsx[n]-offset);
		x2 = min((double)m_width,	kseedsx[n]+offset);
	//////////////////////////////////////////////
		for( int y = y1; y < y2; y++ )
			{
				for( int x = x1; x < x2; x++ )
				{
					int i = y*m_width + x;

					l = m_lvec[i];
					a = m_avec[i];
					b = m_bvec[i];
#if 0
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
#if 1
					double sita_n, m_n, L_n, X_n, Y_n;
					double Ksita_n, Km_n, KL_n, KX_n, KY_n;
					ConvertLab2oml(m_width,m_height,l,a,b,x,y,&sita_n,&m_n,&L_n,&X_n,&Y_n);
					ConvertLab2oml(m_width,m_height,kseedsl[n],kseedsa[n],kseedsb[n],kseedsx[n],kseedsy[n],&Ksita_n,&Km_n,&KL_n,&KX_n,&KY_n);
					dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
#endif
					if( dist < distvec[i] )
					{
						distvec[i] = dist;
						klabels[i]  = n;
						//atomicExch(&klabels[i],n);
					}
				}
			}

#endif
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*���ó�����ͼƬ��Labels
*
*@param   x_offset   Χ����������X��ƫ����
*@param   y_offset   Χ����������Y��ƫ����
*@param   offsetSize ƫ������ĳ���
*@param   alpha      �Ƕ�ϵ��
*@param   betta      ģ��ϵ��
*@param   gama       ����ϵ��
*@param   fai        ����ϵ��
*@param   SeedWH     ����ƽ����������
*@param   labelsX    �����ؿ�Ŀ��
*@param   labelsY    �����ؿ�ĳ���
*@param   x          ����X����
*@param   y          ����Y����
*@param   m_width    ͼ����
*@param   m_height   ͼ��߶�
*@param   dev_kseeds_sita_n  ͼ��ĽǶȾ���
*@param   dev_kseeds_m_n     ͼ��Ŀ�Ⱦ���
*@param   dev_kseeds_L_n     ͼ������Ⱦ���
*@param   dev_kseeds_X_n     ͼ��ĵȼ�X����
*@param   dev_kseeds_Y_n     ͼ��ĵȼ�Y����
*@param   dev_sita_n         ͼ��ĽǶȷ�������
*@param   dev_m_n			 ͼ���ģ����������
*@param   dev_L_n            ͼ������ȷ�������
*@param   dev_X_n            ͼ���X��������
*@param   dev_Y_n            ͼ���Y��������
*@param   klabels            ͼ��ı궨��Ϣ
*@param   distvec            ���������Сֵ�ľ���
*
*/
/*------------------------------------------------------------------------------------------*/
__inline__ __device__  void SetPixelLables(
	const int* x_offset,
	const int* y_offset,
	const int offsetSize,
	const float alpha,
	const float betta,
	const float gama,
	const float fai,
	const int SeedWH,
	const int labelsX,
	const int labelsY,
	const int x,
	const int y,
	const int m_width,
	const int m_height,
	const double* dev_kseeds_sita_n,
	const double* dev_kseeds_m_n,
	const double* dev_kseeds_L_n,
	const double* dev_kseeds_X_n,
	const double* dev_kseeds_Y_n,
	const double* dev_sita_n,
	const double* dev_m_n,
	const double* dev_L_n,
	const double* dev_X_n,
	const double* dev_Y_n,
	int*   klabels,
	double* distvec)
{
#if 0
	{
		double dist;
		//////////////////////////////////////////////
		int i = y*m_width + x;
		////////////////////////////////////////////
		for (int xi=0;xi<offsetSize;xi++){
			for (int yi=0;yi<offsetSize;yi++){
				int nx=labelsX+x_offset[xi];
				int ny=labelsY+y_offset[yi];
				if (nx>=0&&nx<SeedWH&&ny>=0&&ny<SeedWH){
					int ni=nx+ny*SeedWH;
					{
						dist=CalculateNewDistance(alpha,betta,gama,fai,
							dev_sita_n[i],dev_m_n[i],dev_L_n[i],dev_X_n[i],dev_Y_n[i],
							dev_kseeds_sita_n[ni],dev_kseeds_m_n[ni],dev_kseeds_L_n[ni],dev_kseeds_X_n[ni],dev_kseeds_Y_n[ni]);
						if( dist < distvec[i] )
						{
							distvec[i] = dist;
							klabels[i]  = ni;
						}

					}
				}
			}
		}
	}
#endif
#if 1
	{
		double dist;
		//////////////////////////////////////////////
		int i = y*m_width + x;
		////////////////////////////////////////////
		for (int xyi=0;xyi<offsetSize;xyi++){			
				int nx=labelsX+x_offset[xyi];
				int ny=labelsY+y_offset[xyi];
				if (nx>=0&&nx<SeedWH&&ny>=0&&ny<SeedWH){
					int ni=nx+ny*SeedWH;
					{
						dist=CalculateNewDistance(alpha,betta,gama,fai,
							dev_sita_n[i],dev_m_n[i],dev_L_n[i],dev_X_n[i],dev_Y_n[i],
							dev_kseeds_sita_n[ni],
							dev_kseeds_m_n[ni],
							dev_kseeds_L_n[ni],
							dev_kseeds_X_n[ni],
							dev_kseeds_Y_n[ni]);

						if( dist < distvec[i] )
						{
							distvec[i] = dist;
							klabels[i]  = ni;
						}

					}
				}
			
		}
	}
#endif
}
/*------------------------------------------------------------------------------------------*/
/**
*
*һ��������5�����Ӽ�����뱣����Сֵ
*
*@param     kseedsSize ���Ӹ��� 
*@param     alpha      �Ƕ�ϵ��
*@param     betta      ģ��ϵ��
*@param     gama       ����ϵ��
*@param     fai        �������
*@param     m_width    ͼ����
*@param     m_height   ͼ��߶�
*@param   [in]  m_lvec     ͼ��L����
*@param   [in]  m_avec     ͼ��A����
*@param   [in] m_bvec     ͼ��B����
*@param   [in]  dev_sita_n  ͼ��Ƕȷ���
*@param   [in] dev_m_n    ͼ��ģ������
*@param   [in]  dev_L_n   ͼ�����ȷ���
*@param   [in] dev_X_n   ͼ��X����
*@param   [in]  dev_Y_n   ͼ��Y����
*@param   [in]  kseedsl   ���ӵ�L����
*@param   [in] kseedsa   ���ӵ�A����
*@param   [in] kseedsb   ���ӵ�B����
*@param   [in] kseedsx   ���ӵ�X�������
*@param   [in] kseedsy   ���ӵ�Y�������
*@param   [in] dev_kseed_sita_n  ���ӽǶȷ���
*@param   [in] dev_kseed_m_n     ����ģ������
*@param   [in] dev_kseed_L_n     ���ӵ�L����
*@param   [in] dev_kseed_X_n     ���ӵ�X����
*@param   [in] dev_kseed_Y_n     ���ӵ�Y����
*@param   [in] klabels           ͼ��ı궨��Ϣ
*@param   [in] distvec           δʹ��
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMinimumOnLab_5Seeds(
	const int kseedsSize,
	float alpha,
	float betta,
	float gama,
	float fai,
	const int m_width,
	const int m_height,
	double* m_lvec,
	double* m_avec,
	double* m_bvec,
	double* dev_sita_n,
	double* dev_m_n,
	double* dev_L_n,
	double* dev_X_n,
	double* dev_Y_n,
	double* kseedsl,
	double* kseedsa,
	double* kseedsb,
	double* kseedsx,
	double* kseedsy,
	double*	dev_kseed_sita_n,
	double*	dev_kseed_m_n,
	double*	dev_kseed_L_n,
	double*	dev_kseed_X_n,
	double*	dev_kseed_Y_n,
	int*   klabels,
	double* distvec)
{
	int x_offset[]={0,1,-1,0,0};
	int y_offset[]={0,0,0,1,-1};
////////////////////////////////////////////////////////////////////////
#if 1
{
	int x =blockIdx.x*blockDim.x+threadIdx.x;
	int y =blockIdx.y*blockDim.y+threadIdx.y;
	int SeedWH=sqrt((float)kseedsSize);	
	/////////////////////////////////////////////
	int offsetSize=sizeof(x_offset)/sizeof(int);
	if (x>=0&&x<m_width&&y>=0&&y<m_height){
		int ind=x+y*m_width;
		int labels_ind=klabels[ind];
	assert(labels_ind>=-1&&labels_ind<kseedsSize);
#if 1
		if (labels_ind==-1){
			int x_step=m_width/SeedWH;
			int y_step=m_height/SeedWH;
			int labelsX=x/x_step;
			int labelsY=y/y_step;
			SetPixelLables(x_offset,y_offset,offsetSize,
				alpha,betta,gama,fai,
				SeedWH,labelsX,labelsY,x,y,m_width,m_height,
				dev_kseed_sita_n,dev_kseed_m_n,dev_kseed_L_n,dev_kseed_X_n,dev_kseed_Y_n,
				dev_sita_n,dev_m_n,dev_L_n,dev_X_n,dev_Y_n,				
				klabels,distvec);
		}else{
			int labelsX=labels_ind%SeedWH;
			int labelsY=labels_ind/SeedWH;
#if 1
			SetPixelLables(x_offset,y_offset,offsetSize,
				alpha,betta,gama,fai,
				SeedWH,labelsX,labelsY,x,y,m_width,m_height,
				dev_kseed_sita_n,dev_kseed_m_n,dev_kseed_L_n,dev_kseed_X_n,dev_kseed_Y_n,
				dev_sita_n,dev_m_n,dev_L_n,dev_X_n,dev_Y_n,
				klabels,distvec);
#endif	
		}
#endif
	}

}
#endif
////////////////////////////////////////////////////////////////////////
}
/*------------------------------------------------------------------------------------------*/
/**
*
*һ��������9�����Ӽ�����뱣����Сֵ
*
*@param     kseedsSize ���Ӹ��� 
*@param     alpha      �Ƕ�ϵ��
*@param     betta      ģ��ϵ��
*@param     gama       ����ϵ��
*@param     fai        �������
*@param     m_width    ͼ����
*@param     m_height   ͼ��߶�
*@param   [in]  m_lvec     ͼ��L����
*@param   [in]  m_avec     ͼ��A����
*@param   [in] m_bvec     ͼ��B����
*@param   [in]  dev_sita_n  ͼ��Ƕȷ���
*@param   [in] dev_m_n    ͼ��ģ������
*@param   [in]  dev_L_n   ͼ�����ȷ���
*@param   [in] dev_X_n   ͼ��X����
*@param   [in]  dev_Y_n   ͼ��Y����
*@param   [in]  kseedsl   ���ӵ�L����
*@param   [in] kseedsa   ���ӵ�A����
*@param   [in] kseedsb   ���ӵ�B����
*@param   [in] kseedsx   ���ӵ�X�������
*@param   [in] kseedsy   ���ӵ�Y�������
*@param   [in] dev_kseed_sita_n  ���ӽǶȷ���
*@param   [in] dev_kseed_m_n     ����ģ������
*@param   [in] dev_kseed_L_n     ���ӵ�L����
*@param   [in] dev_kseed_X_n     ���ӵ�X����
*@param   [in] dev_kseed_Y_n     ���ӵ�Y����
*@param   [in] klabels           ͼ��ı궨��Ϣ
*@param   [in] distvec           δʹ��
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMinimumOnLab_9Seeds(
	const int kseedsSize,
	float alpha,
	float betta,
	float gama,
	float fai,
	const int m_width,
	const int m_height,
	double* m_lvec,
	double* m_avec,
	double* m_bvec,
	double* dev_sita_n,
	double* dev_m_n,
	double* dev_L_n,
	double* dev_X_n,
	double* dev_Y_n,
	double* kseedsl,
	double* kseedsa,
	double* kseedsb,
	double* kseedsx,
	double* kseedsy,
	double*	dev_kseed_sita_n,
	double*	dev_kseed_m_n,
	double*	dev_kseed_L_n,
	double*	dev_kseed_X_n,
	double*	dev_kseed_Y_n,
	int*   klabels,
	double* distvec)
{
////////////////////////////////////////////////////////////////////////
	int x_offset[]={0,1,-1,0,0,1,-1,1,-1};
	int y_offset[]={0,0,0,1,-1,1,-1,-1,1};
#if 1
	{
		int x =blockIdx.x*blockDim.x+threadIdx.x;
		int y =blockIdx.y*blockDim.y+threadIdx.y;
		int SeedWH=sqrt((float)kseedsSize);	

		int offsetSize=sizeof(x_offset)/sizeof(int);
		if (x>=0&&x<m_width&&y>=0&&y<m_height){
			int ind=x+y*m_width;
			int labels_ind=klabels[ind];
			assert(labels_ind>=-1&&labels_ind<kseedsSize);
#if 1
			if (labels_ind==-1){
				float x_step=1.0*m_width/SeedWH;
				float y_step=1.0*m_height/SeedWH;
				int labelsX=x/x_step;
				int labelsY=y/y_step;
				SetPixelLables(x_offset,y_offset,offsetSize,
					alpha,betta,gama,fai,
					SeedWH,labelsX,labelsY,x,y,m_width,m_height,
					dev_kseed_sita_n,dev_kseed_m_n,dev_kseed_L_n,dev_kseed_X_n,dev_kseed_Y_n,
					dev_sita_n,dev_m_n,dev_L_n,dev_X_n,dev_Y_n,				
					klabels,distvec);
			}else{
				int labelsX=labels_ind%SeedWH;
				int labelsY=labels_ind/SeedWH;
#if 1
				SetPixelLables(x_offset,y_offset,offsetSize,
					alpha,betta,gama,fai,
					SeedWH,labelsX,labelsY,x,y,m_width,m_height,
					dev_kseed_sita_n,dev_kseed_m_n,dev_kseed_L_n,dev_kseed_X_n,dev_kseed_Y_n,
					dev_sita_n,dev_m_n,dev_L_n,dev_X_n,dev_Y_n,
					klabels,distvec);
#endif

			}
#endif
		}

	}
#endif
////////////////////////////////////////////////////////////////////////
}
/*------------------------------------------------------------------------------------------*/
/**
*
*һ��������9�����Ӽ�����뱣����Сֵ
*
*@param     kseedsSize ���Ӹ��� 
*@param     alpha      �Ƕ�ϵ��
*@param     betta      ģ��ϵ��
*@param     gama       ����ϵ��
*@param     fai        �������
*@param     m_width    ͼ����
*@param     m_height   ͼ��߶�
*@param   [in]  m_lvec     ͼ��L����
*@param   [in]  m_avec     ͼ��A����
*@param   [in] m_bvec     ͼ��B����
*@param   [in]  dev_sita_n  ͼ��Ƕȷ���
*@param   [in] dev_m_n    ͼ��ģ������
*@param   [in]  dev_L_n   ͼ�����ȷ���
*@param   [in] dev_X_n   ͼ��X����
*@param   [in]  dev_Y_n   ͼ��Y����
*@param   [in]  kseedsl   ���ӵ�L����
*@param   [in] kseedsa   ���ӵ�A����
*@param   [in] kseedsb   ���ӵ�B����
*@param   [in] kseedsx   ���ӵ�X�������
*@param   [in] kseedsy   ���ӵ�Y�������
*@param   [in] dev_kseed_sita_n  ���ӽǶȷ���
*@param   [in] dev_kseed_m_n     ����ģ������
*@param   [in] dev_kseed_L_n     ���ӵ�L����
*@param   [in] dev_kseed_X_n     ���ӵ�X����
*@param   [in] dev_kseed_Y_n     ���ӵ�Y����
*@param   [in] klabels           ͼ��ı궨��Ϣ
*@param   [in] distvec           δʹ��
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMinimumOnThetaMLXY_9Seeds(
	const int kseedsSize,
	float alpha,
	float betta,
	float gama,
	float fai,
	const int m_width,
	const int m_height,
	double* dev_sita_n,
	double* dev_m_n,
	double* dev_L_n,
	double* dev_X_n,
	double* dev_Y_n,
	double*	dev_kseed_sita_n,
	double*	dev_kseed_m_n,
	double*	dev_kseed_L_n,
	double*	dev_kseed_X_n,
	double*	dev_kseed_Y_n,
	int*   klabels,
	double* distvec)
{
////////////////////////////////////////////////////////////////////////
	int x_offset[]={0,1,-1,0,0,1,-1,1,-1};
	int y_offset[]={0,0,0,1,-1,1,-1,-1,1};
#if 1
	{
		int x =blockIdx.x*blockDim.x+threadIdx.x;
		int y =blockIdx.y*blockDim.y+threadIdx.y;
		int SeedWH=sqrt((float)kseedsSize);	

		int offsetSize=sizeof(x_offset)/sizeof(int);
		if (x>=0&&x<m_width&&y>=0&&y<m_height){
			int ind=x+y*m_width;
			int labels_ind=klabels[ind];
			assert(labels_ind>=-1&&labels_ind<kseedsSize);
#if 1
			if (labels_ind==-1){
				float x_step=1.0*m_width/SeedWH;
				float y_step=1.0*m_height/SeedWH;
				int labelsX=x/x_step;
				int labelsY=y/y_step;
				SetPixelLables(x_offset,y_offset,offsetSize,
					alpha,betta,gama,fai,
					SeedWH,labelsX,labelsY,x,y,m_width,m_height,
					dev_kseed_sita_n,dev_kseed_m_n,dev_kseed_L_n,dev_kseed_X_n,dev_kseed_Y_n,
					dev_sita_n,dev_m_n,dev_L_n,dev_X_n,dev_Y_n,				
					klabels,distvec);
			}else{
				int labelsX=labels_ind%SeedWH;
				int labelsY=labels_ind/SeedWH;
#if 1
				SetPixelLables(x_offset,y_offset,offsetSize,
					alpha,betta,gama,fai,
					SeedWH,labelsX,labelsY,x,y,m_width,m_height,
					dev_kseed_sita_n,dev_kseed_m_n,dev_kseed_L_n,dev_kseed_X_n,dev_kseed_Y_n,
					dev_sita_n,dev_m_n,dev_L_n,dev_X_n,dev_Y_n,
					klabels,distvec);
#endif

			}
#endif
		}

	}
#endif
////////////////////////////////////////////////////////////////////////
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMinimumOnLab_AllSeeds(
	const int kseedsSize,
	float alpha,
	float betta,
	float gama,
	float fai,
	const int m_width,
	const int m_height,
	double* m_lvec,
	double* m_avec,
	double* m_bvec,
	double* dev_sita_n,
	double* dev_m_n,
	double* dev_L_n,
	double* dev_X_n,
	double* dev_Y_n,
	double* kseedsl,
	double* kseedsa,
	double* kseedsb,
	double* kseedsx,
	double* kseedsy,
	double*	dev_kseed_sita_n,
	double*	dev_kseed_m_n,
	double*	dev_kseed_L_n,
	double*	dev_kseed_X_n,
	double*	dev_kseed_Y_n,
	int*   klabels,
	double* distvec)
{
	////////////////////////////////////////////////////////////////////////
#if 0
	int x =blockIdx.x*blockDim.x+threadIdx.x;
	int y =blockIdx.y*blockDim.y+threadIdx.y;
	int ind=x+y*m_width;
	int labels_ind;
	if (x>=0&&x<m_width&&y>=0&&y<m_height){
		labels_ind= klabels[ind];	
		assert(labels_ind>=-1&&labels_ind<kseedsSize);
		{
			double l, a, b;
			double dist;
			//////////////////////////////////////////////
			int i = y*m_width + x;
			l = m_lvec[i];
			a = m_avec[i];
			b = m_bvec[i];
			double sita_n, m_n, L_n, X_n, Y_n;
			double Ksita_n, Km_n, KL_n, KX_n, KY_n;
			ConvertLab2oml(m_width,m_height,l,a,b,x,y,sita_n,m_n,L_n,X_n,Y_n);
			int kseedareaXmin=max(0,x-STEP);
			int kseedareaYmin=max(0,y-STEP);
			int kseedareaXmax=min(m_width,x+STEP);
			int kseedareaYmax=min(m_height,y+STEP);

			for(int ni=0;ni<kseedsSize;ni++){
				int n=ni;
				int KseedX=kseedsx[n];
				int KseedY=kseedsy[n];
				if ((KseedX>=kseedareaXmin)&&
					(KseedX<kseedareaXmax)&&
					(KseedY>=kseedareaYmin)&&
					(KseedY<kseedareaYmax)){
						ConvertLab2oml(m_width,m_height,kseedsl[n],kseedsa[n],kseedsb[n],kseedsx[n],kseedsy[n],Ksita_n,Km_n,KL_n,KX_n,KY_n);
						dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
						if( dist < distvec[i] )
						{
							distvec[i] = dist;
							klabels[i]  = n;
						}
				}

			}

		}

	}

#endif
	////////////////////////////////////////////////////////////////////////
#if 0
	{
		int x =blockIdx.x*blockDim.x+threadIdx.x;
		int y =blockIdx.y*blockDim.y+threadIdx.y;
		int ind=x+y*m_width;
		int labels_ind;
		if (x>=0&&x<m_width&&y>=0&&y<m_height){
			labels_ind= klabels[ind];	
			assert(labels_ind>=-1&&labels_ind<kseedsSize);
			{
				double l, a, b;
				double dist;
				double sita_n, m_n, L_n, X_n, Y_n;
				double Ksita_n, Km_n, KL_n, KX_n, KY_n;
				//////////////////////////////////////////////
				int i = y*m_width + x;
				////////////////////////////////////////////
				sita_n=dev_sita_n[i];
				m_n=dev_m_n[i];
				L_n=dev_L_n[i];
				X_n=dev_X_n[i];
				Y_n=dev_Y_n[i];
				int kseedareaXmin=max(0,x-STEP);
				int kseedareaYmin=max(0,y-STEP);
				int kseedareaXmax=min(m_width,x+STEP);
				int kseedareaYmax=min(m_height,y+STEP);

				for(int ni=0;ni<kseedsSize;ni++){
					int KseedX=kseedsx[ni];
					int KseedY=kseedsy[ni];
					if ((KseedX>=kseedareaXmin)&&
						(KseedX<kseedareaXmax)&&
						(KseedY>=kseedareaYmin)&&
						(KseedY<kseedareaYmax)){
							Ksita_n=dev_sita_n[ni];
							Km_n=dev_m_n[ni];
							KL_n=dev_L_n[ni];
							KX_n=dev_X_n[ni];
							KY_n=dev_Y_n[ni];
							dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
							if( dist < distvec[i] )
							{
								distvec[i] = dist;
								klabels[i]  = ni;
							}
					}

				}

			}

		}
	}
#endif
	////////////////////////////////////////////////////////////////////////
#if 1
	{
		int x =blockIdx.x*blockDim.x+threadIdx.x;
		int y =blockIdx.y*blockDim.y+threadIdx.y;
		int SeedWH=sqrt((float)kseedsSize);	
		int x_step=m_width/SeedWH;
		int y_step=m_height/SeedWH;
		int STEP=x_step+y_step;
		int ind=x+y*m_width;
		int labels_ind=klabels[ind];
		if (labels_ind==-1){
#if 1
			if (x>=0&&x<m_width&&y>=0&&y<m_height){
				labels_ind= klabels[ind];	
				assert(labels_ind>=-1&&labels_ind<kseedsSize);
				{
					double l, a, b;
					double dist;
					double sita_n, m_n, L_n, X_n, Y_n;
					double Ksita_n, Km_n, KL_n, KX_n, KY_n;
					//////////////////////////////////////////////
					int i = y*m_width + x;
					////////////////////////////////////////////
					sita_n=dev_sita_n[i];
					m_n=dev_m_n[i];
					L_n=dev_L_n[i];
					X_n=dev_X_n[i];
					Y_n=dev_Y_n[i];
					int kseedareaXmin=max(0,x-STEP);
					int kseedareaYmin=max(0,y-STEP);
					int kseedareaXmax=min(m_width,x+STEP);
					int kseedareaYmax=min(m_height,y+STEP);

					for(int ni=0;ni<kseedsSize;ni++){
						int KseedX=kseedsx[ni];
						int KseedY=kseedsy[ni];
						if ((KseedX>=kseedareaXmin)&&
							(KseedX<kseedareaXmax)&&
							(KseedY>=kseedareaYmin)&&
							(KseedY<kseedareaYmax)){
								Ksita_n=dev_sita_n[ni];
								Km_n=dev_m_n[ni];
								KL_n=dev_L_n[ni];
								KX_n=dev_X_n[ni];
								KY_n=dev_Y_n[ni];
								dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
								if( dist < distvec[i] )
								{
									distvec[i] = dist;
									klabels[i]  = ni;
								}
						}
					}
				}
			}
#endif
		}else{
#if 1
			{
				int SeedWH=sqrt((float)kseedsSize);
				int labelsX=labels_ind%SeedWH;
				int labelsY=labels_ind/SeedWH;
				int x_offset[]={0,1,-1,0,0};
				int y_offset[]={0,0,0,1,-1};
				int offsetSize=sizeof(x_offset)/sizeof(int);
				if (x>=0&&x<m_width&&y>=0&&y<m_height){
					assert(labels_ind>=-1&&labels_ind<kseedsSize);
					{
						double l, a, b;
						double dist;
						double sita_n, m_n, L_n, X_n, Y_n;
						double Ksita_n, Km_n, KL_n, KX_n, KY_n;
						//////////////////////////////////////////////
						int i = y*m_width + x;
						////////////////////////////////////////////
						sita_n=dev_sita_n[i];
						m_n=dev_m_n[i];
						L_n=dev_L_n[i];
						X_n=dev_X_n[i];
						Y_n=dev_Y_n[i];
						int kseedareaXmin=max(0,x-STEP);
						int kseedareaYmin=max(0,y-STEP);
						int kseedareaXmax=min(m_width,x+STEP);
						int kseedareaYmax=min(m_height,y+STEP);

						for (int xi=0;xi<offsetSize;xi++){
							for (int yi=0;yi<offsetSize;yi++)
							{
								int nx=labelsX+xi;
								int ny=labelsY+yi;
								if (nx>=0&&nx<SeedWH&&ny>=0&&ny<SeedWH){
									int ni=nx+ny*SeedWH;
									{
										int KseedX=kseedsx[ni];
										int KseedY=kseedsy[ni];
										if ((KseedX>=kseedareaXmin)&&
											(KseedX<kseedareaXmax)&&
											(KseedY>=kseedareaYmin)&&
											(KseedY<kseedareaYmax)){
												Ksita_n=dev_sita_n[ni];
												Km_n=dev_m_n[ni];
												KL_n=dev_L_n[ni];
												KX_n=dev_X_n[ni];
												KY_n=dev_Y_n[ni];
												dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
												if( dist < distvec[i] )
												{
													distvec[i] = dist;
													klabels[i]  = ni;
												}
										}
									}
								}

							}
						}
					}
				}
			}
#endif
		}
	}
#endif
	////////////////////////////////////////////////////////////////////////
#if 0
	{
		int x =blockIdx.x*blockDim.x+threadIdx.x;
		int y =blockIdx.y*blockDim.y+threadIdx.y;
		int SeedWH=sqrt((float)kseedsSize);
		int ind=x+y*m_width;
		int x_offset[]={0,1,-1,0,0};
		int y_offset[]={0,0,0,1,-1};
		int offsetSize=sizeof(x_offset)/sizeof(int);
		int labels_ind=klabels[ind];
		if (labels_ind==-1){
			int x_step=m_width/SeedWH;
			int y_step=m_height/SeedWH;
			int labelsX=x/x_step;
			int labelsY=y/y_step;
#if 1
			if (x>=0&&x<m_width&&y>=0&&y<m_height){
				assert(labels_ind>=-1&&labels_ind<kseedsSize);
				{
					double l, a, b;
					double dist;
					double sita_n, m_n, L_n, X_n, Y_n;
					double Ksita_n, Km_n, KL_n, KX_n, KY_n;
					//////////////////////////////////////////////
					int i = y*m_width + x;
					////////////////////////////////////////////
					sita_n=dev_sita_n[i];
					m_n=dev_m_n[i];
					L_n=dev_L_n[i];
					X_n=dev_X_n[i];
					Y_n=dev_Y_n[i];
					int kseedareaXmin=max(0,x-STEP);
					int kseedareaYmin=max(0,y-STEP);
					int kseedareaXmax=min(m_width,x+STEP);
					int kseedareaYmax=min(m_height,y+STEP);
					for (int xi=0;xi<offsetSize;xi++){
						for (int yi=0;yi<offsetSize;yi++)
						{
							int nx=labelsX+x_offset[xi];
							int ny=labelsY+y_offset[yi];
							if (nx>=0&&nx<SeedWH&&ny>=0&&ny<SeedWH){
								int ni=nx+ny*SeedWH;
								{
									int KseedX=kseedsx[ni];
									int KseedY=kseedsy[ni];
									if ((KseedX>=kseedareaXmin)&&
										(KseedX<kseedareaXmax)&&
										(KseedY>=kseedareaYmin)&&
										(KseedY<kseedareaYmax)){
											Ksita_n=dev_sita_n[ni];
											Km_n=dev_m_n[ni];
											KL_n=dev_L_n[ni];
											KX_n=dev_X_n[ni];
											KY_n=dev_Y_n[ni];
											dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
											if( dist < distvec[i] )
											{
												distvec[i] = dist;
												klabels[i]  = ni;
											}
									}
								}
							}
						}
					}
				}
			}
#endif
		}else{
			int labelsX=labels_ind%SeedWH;
			int labelsY=labels_ind/SeedWH;
#if 1
			if (x>=0&&x<m_width&&y>=0&&y<m_height){
				assert(labels_ind>=-1&&labels_ind<kseedsSize);
				{
					double l, a, b;
					double dist;
					double sita_n, m_n, L_n, X_n, Y_n;
					double Ksita_n, Km_n, KL_n, KX_n, KY_n;
					//////////////////////////////////////////////
					int i = y*m_width + x;
					////////////////////////////////////////////
					sita_n=dev_sita_n[i];
					m_n=dev_m_n[i];
					L_n=dev_L_n[i];
					X_n=dev_X_n[i];
					Y_n=dev_Y_n[i];
					int kseedareaXmin=max(0,x-STEP);
					int kseedareaYmin=max(0,y-STEP);
					int kseedareaXmax=min(m_width,x+STEP);
					int kseedareaYmax=min(m_height,y+STEP);
					for (int xi=0;xi<offsetSize;xi++){
						for (int yi=0;yi<offsetSize;yi++)
						{
							int nx=labelsX+x_offset[xi];
							int ny=labelsY+y_offset[yi];
							if (nx>=0&&nx<SeedWH&&ny>=0&&ny<SeedWH){
								int ni=nx+ny*SeedWH;
								{
									int KseedX=kseedsx[ni];
									int KseedY=kseedsy[ni];
									if ((KseedX>=kseedareaXmin)&&
										(KseedX<kseedareaXmax)&&
										(KseedY>=kseedareaYmin)&&
										(KseedY<kseedareaYmax)){
											Ksita_n=dev_sita_n[ni];
											Km_n=dev_m_n[ni];
											KL_n=dev_L_n[ni];
											KX_n=dev_X_n[ni];
											KY_n=dev_Y_n[ni];
											dist=CalculateNewDistance(alpha,betta,gama,fai,sita_n,m_n,L_n,X_n,Y_n,Ksita_n,Km_n,KL_n,KX_n,KY_n);
											if( dist < distvec[i] )
											{
												distvec[i] = dist;
												klabels[i]  = ni;
											}
									}
								}
							}
						}
					}
				}
			}
#endif
		}
	}
#endif
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMatrix(double *matrix,int width,int height,double value_t)
{
	int i=blockIdx.x*blockDim.x+threadIdx.x;
	int j=blockIdx.y*blockDim.y+threadIdx.y;
	int ind=i+j*width;
	if(ind<width*height){
		matrix[ind]=value_t;
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void SetMatrix_Efficient(double *matrix,int width,int height,double value_t)
{
	int ind=blockIdx.x*blockDim.x+threadIdx.x;
	int offset=blockDim.x*gridDim.x;
	int size_t=width*height;
	/////////////////////////////////////////
	while(ind<size_t){
		matrix[ind]=value_t;
		ind+=offset;
	}
	/////////////////////////////////////////
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__device__ double atomicAdd(double* address, double val)
{
	unsigned long long int* address_as_ull =
		(unsigned long long int*)address;
	unsigned long long int old = *address_as_ull, assumed;

	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed,
			__double_as_longlong(val +
			__longlong_as_double(assumed)));

		// Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN)
	} while (assumed != old);

	return __longlong_as_double(old);
}
//Read more at: http://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#ixzz3HQ3voVZ1 
//Follow us: @GPUComputing on Twitter | NVIDIA on Facebook
/*------------------------------------------------------------------------------------------*/
/**
*              1*1            
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect_blocks(
	const int			kseedSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	int m=blockIdx.x*blockDim.x+threadIdx.x;
	int n=blockIdx.y*blockDim.y+threadIdx.y;
	///////////////////////////////////////
	int m_dim=blockDim.x*gridDim.x;
	int n_dim=blockDim.y*gridDim.y;
	int width_step=width/m_dim;
	int height_step=height/n_dim;
	/////////////////////////////////////////
	int w_start=m*width_step;
	int h_start=n*height_step;
	int w_end=min((m+1)*width_step,width);
	int h_end=min((n+1)*height_step,height);
	///////////////////////////////////////////
	int labels_ind;
	int ind;
	for (int xi=w_start;xi<w_end;xi++){
		for (int yi=h_start;yi<h_end;yi++){
			ind=xi+yi*width;
			labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
#if 1
					{
						atomicAdd(&dev_sigmal[labels_ind],m_lvec[ind]);
						atomicAdd(&dev_sigmaa[labels_ind],m_avec[ind]);
						atomicAdd(&dev_sigmab[labels_ind],m_bvec[ind]);
						atomicAdd(&dev_sigmax[labels_ind],xi);
						atomicAdd(&dev_sigmay[labels_ind],yi);
						atomicAdd(&dev_clustersize[labels_ind],1.0);
					}
#else
						{
							dev_sigmal[labels_ind]+=m_lvec[ind];
							dev_sigmaa[labels_ind]+=m_avec[ind];
							dev_sigmab[labels_ind]+=m_bvec[ind];
							dev_sigmax[labels_ind]+=xi;
							dev_sigmay[labels_ind]+=yi;
							dev_clustersize[labels_ind]+=1.0;
						}
#endif
		}
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*              1*1            
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect_blocks_sheard(
	const int			kseedSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	int m=blockIdx.x*blockDim.x+threadIdx.x;
	int n=blockIdx.y*blockDim.y+threadIdx.y;
	///////////////////////////////////////
	int m_dim=blockDim.x*gridDim.x;
	int n_dim=blockDim.y*gridDim.y;
	int width_step=width/m_dim;
	int height_step=height/n_dim;
	/////////////////////////////////////////
	int w_start=m*width_step;
	int h_start=n*height_step;
	int w_end=min((m+1)*width_step,width);
	int h_end=min((n+1)*height_step,height);
	///////////////////////////////////////////
	int labels_ind;
	int ind;
	for (int xi=w_start;xi<w_end;xi++){
		for (int yi=h_start;yi<h_end;yi++){
			ind=xi+yi*width;
			labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
#if 1
			{
				atomicAdd(&dev_sigmal[labels_ind],m_lvec[ind]);
				atomicAdd(&dev_sigmaa[labels_ind],m_avec[ind]);
				atomicAdd(&dev_sigmab[labels_ind],m_bvec[ind]);
				atomicAdd(&dev_sigmax[labels_ind],xi);
				atomicAdd(&dev_sigmay[labels_ind],yi);
				atomicAdd(&dev_clustersize[labels_ind],1.0);
			}
#else
			{
				dev_sigmal[labels_ind]+=m_lvec[ind];
				dev_sigmaa[labels_ind]+=m_avec[ind];
				dev_sigmab[labels_ind]+=m_bvec[ind];
				dev_sigmax[labels_ind]+=xi;
				dev_sigmay[labels_ind]+=yi;
				dev_clustersize[labels_ind]+=1.0;
			}
#endif
		}
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect(
	unsigned int*		mutex,
	const int			kseedSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	int c=blockIdx.x*blockDim.x+threadIdx.x;
	int r=blockIdx.y*blockDim.y+threadIdx.y;
	int ind=c+r*width;
	int labels_ind;
	if (ind>=0&&ind<width*height){
		labels_ind=dev_klabels[ind];
	}else{
		return;
	}

	assert(labels_ind>=0&&labels_ind<kseedSize);
	//	bool finished = false; 
	if (labels_ind>=0&&labels_ind<kseedSize)
	{
		if (ind<width*height){

			//	while(!finished)
			{
				//	if( atomicExch(mutex, 1) == 0)
				{
#if 1
					{
						atomicAdd(&dev_sigmal[labels_ind],m_lvec[ind]);
						atomicAdd(&dev_sigmaa[labels_ind],m_avec[ind]);
						atomicAdd(&dev_sigmab[labels_ind],m_bvec[ind]);
						atomicAdd(&dev_sigmax[labels_ind],c);
						atomicAdd(&dev_sigmay[labels_ind],r);
						atomicAdd(&dev_clustersize[labels_ind],1.0);
					}
#else
					{
						dev_sigmal[labels_ind]+=m_lvec[ind];
						dev_sigmaa[labels_ind]+=m_avec[ind];
						dev_sigmab[labels_ind]+=m_bvec[ind];
						dev_sigmax[labels_ind]+=c;
						dev_sigmay[labels_ind]+=r;
						dev_clustersize[labels_ind]+=1.0;
					}
#endif

					//	finished = true;
					//	atomicExch(mutex, 0);
				}
			}

		}
	}
	/*****************************************/
	//while(!finished)
	//{
	//		if( atomicExch(mutex, 1) == 0){
	//			
	//			
	//			finished = true;
	//			atomicExch(mutex, 0);
	//		}

	//}

	/*****************************************/

}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ�Ĭ����K^2��ƽ����������
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect_new2014_11_3(
	unsigned int*		mutex,
	const int			kseedSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	__shared__ double temp[4096];
	assert(kseedSize<=4096);
	{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
			__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
			atomicAdd(&temp[labels_ind],m_lvec[ind]);
			ind+=offset;
		}
			__syncthreads();
		atomicAdd(&dev_sigmal[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_sigmal[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_sigmal[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_sigmal[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
#if 0
{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
			__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
			atomicAdd(&temp[labels_ind],m_avec[ind]);
			ind+=offset;
		}
			__syncthreads();
		atomicAdd(&dev_sigmaa[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_sigmaa[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_sigmaa[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_sigmaa[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
	{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
			__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
				atomicAdd(&temp[labels_ind],m_bvec[ind]);
			ind+=offset;
		}
			__syncthreads();
		atomicAdd(&dev_sigmab[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_sigmab[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_sigmab[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_sigmab[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
	{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
			__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
				atomicAdd(&temp[labels_ind],int(ind%width));
			ind+=offset;
		}
			__syncthreads();
		atomicAdd(&dev_sigmax[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_sigmax[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_sigmax[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_sigmax[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
	{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
			__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
				atomicAdd(&temp[labels_ind],int(ind/width));
			ind+=offset;
		}
			__syncthreads();
		atomicAdd(&dev_sigmay[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_sigmay[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_sigmay[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_sigmay[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
	{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
			__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
				atomicAdd(&temp[labels_ind],1);
			ind+=offset;
		}
			__syncthreads();
		atomicAdd(&dev_clustersize[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_clustersize[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_clustersize[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_clustersize[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
#endif	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ�Ĭ����K^2��ƽ����������
*
*@param			kseedsSize		��������
*@param			width		ͼ����
*@param			height		ͼ��߶�
*@param	[in]	m_lvec		ͼ��L��������
*@param	[in]	m_avec		ͼ��A��������
*@param	[in]	m_bvec		ͼ��B��������
*@param	[in]	dev_klabels		ͼ��궨����
*@param [in]	dev_img_mask		δʹ��
*@param	[out]	dev_sigmal		��L����ͳ�����
*@param	[out]	dev_sigmaa		��A����ͳ�����
*@param	[out]	dev_sigmab		��B����ͳ�����
*@param	[out]	dev_sigmax		��X����ͳ�����
*@param	[out]	dev_sigmay		��Y����ͳ�����
*@param	[out]	dev_clustersize	 δʹ�� 	
*
*/
/*------------------------------------------------------------------------------------------*/
 void StatisticsLABXY_Seed_collect_CPU(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	unsigned int*       dev_img_mask,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	cudaError_t cudaStatus;
	unsigned int*       host_img_mask=nullptr;
	int*				host_klabels=new int[width*height];
	double*				host_sigmal=new double[kseedsSize];
	double*				host_sigmaa=new double[kseedsSize];
	double*				host_sigmab=new double[kseedsSize];
	double*				host_sigmax=new double[kseedsSize];
	double*				host_sigmay=new double[kseedsSize];
	double*				host_clustersize=new double[kseedsSize];

#if 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (dev_img_mask==nullptr){
		host_img_mask=nullptr;
	}else{
		 host_img_mask=new unsigned int[width*height];
		 cudaStatus = cudaMemcpy( host_img_mask,dev_img_mask, width*height * sizeof(unsigned int), cudaMemcpyDeviceToHost);
	}
	 cudaStatus = cudaMemcpy(host_klabels,dev_klabels, width*height * sizeof(int), cudaMemcpyDeviceToHost);
	memset(host_sigmal,0,kseedsSize * sizeof(double));
	memset(host_sigmaa,0,kseedsSize * sizeof(double));
	memset(host_sigmab,0,kseedsSize * sizeof(double));
	memset(host_sigmax,0,kseedsSize * sizeof(double));
	memset(host_sigmay,0,kseedsSize * sizeof(double));	
	memset(host_clustersize,0,kseedsSize*sizeof(double));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
	{
		int ind(0);
		for(register int r = 0; r < height; r++ )
		{
			for(register  int c = 0; c < width; c++ )
			{
				if (host_img_mask==nullptr){
					host_sigmal[host_klabels[ind]] += m_lvec[ind];
					host_sigmaa[host_klabels[ind]] += m_avec[ind];
					host_sigmab[host_klabels[ind]] += m_bvec[ind];
					host_sigmax[host_klabels[ind]] += c;
					host_sigmay[host_klabels[ind]] += r;
					host_clustersize[host_klabels[ind]] += 1.0;
				}
				if (host_img_mask!=nullptr&&host_img_mask[ind]==0x000000000){
					host_sigmal[host_klabels[ind]] += m_lvec[ind];
					host_sigmaa[host_klabels[ind]] += m_avec[ind];
					host_sigmab[host_klabels[ind]] += m_bvec[ind];
					host_sigmax[host_klabels[ind]] += c;
					host_sigmay[host_klabels[ind]] += r;
					host_clustersize[host_klabels[ind]] += 1.0;
				}				
				//------------------------------------
				//edgesum[klabels[ind]] += edgemag[ind];
				//------------------------------------
				
				ind++;
			}
		}
#if 0
				double x_max=0;
				double y_max=0;
				double *inv=new double[kseedsSize];
				{
					for( int k = 0; k < kseedsSize; k++ )
					{
						if(host_clustersize[k] <= 0 ) host_clustersize[k] = 1;
						inv[k] = 1.0/host_clustersize[k];//computing inverse now to multiply, than divide later
						////////////////////////////////////
						host_sigmal[k] = host_sigmal[k]*inv[k];
						host_sigmaa[k] = host_sigmaa[k]*inv[k];
						host_sigmab[k] = host_sigmab[k]*inv[k];
						host_sigmax[k] = host_sigmax[k]*inv[k];
						host_sigmay[k] = host_sigmay[k]*inv[k];
						if (host_sigmax[k]>x_max){
							x_max=host_sigmax[k];
						}
						if (host_sigmay[k]>y_max){
							y_max=host_sigmay[k];
						}
						//------------------------------------
						//edgesum[k] *= inv[k];
						//------------------------------------
					}
				}
				delete []inv;
#endif
	}
#if 1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (dev_img_mask==nullptr){
		host_img_mask=nullptr;
	}else{
		cudaStatus = cudaMemcpy( dev_img_mask,host_img_mask, width*height * sizeof(int), cudaMemcpyHostToDevice);
	}
	
	
	cudaStatus = cudaMemcpy(dev_sigmal, host_sigmal, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
	
	cudaStatus = cudaMemcpy(dev_sigmaa, host_sigmaa, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
	
	cudaStatus = cudaMemcpy(dev_sigmab, host_sigmab, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
	
	cudaStatus = cudaMemcpy(dev_sigmax, host_sigmax, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
	
	cudaStatus = cudaMemcpy(dev_sigmay, host_sigmay, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
	
	cudaStatus = cudaMemcpy(dev_clustersize, host_clustersize, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
	delete [] host_img_mask;
	delete [] host_klabels;
	delete [] host_sigmal;
	delete [] host_sigmaa;
	delete [] host_sigmab;
	delete [] host_sigmax;
	delete [] host_sigmay;
	delete [] host_clustersize;
 ;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ�Ĭ����K^2��ƽ����������
*
*@param			kseedsSize		��������
*@param			width		ͼ����
*@param			height		ͼ��߶�
*@param	[in]	m_lvec		ͼ��L��������
*@param	[in]	m_avec		ͼ��A��������
*@param	[in]	m_bvec		ͼ��B��������
*@param	[in]	dev_klabels		ͼ��궨����
*@param [in]	dev_img_mask		δʹ��
*@param	[out]	dev_sigmal		��L����ͳ�����
*@param	[out]	dev_sigmaa		��A����ͳ�����
*@param	[out]	dev_sigmab		��B����ͳ�����
*@param	[out]	dev_sigmax		��X����ͳ�����
*@param	[out]	dev_sigmay		��Y����ͳ�����
*@param	[out]	dev_clustersize	 δʹ�� 	
*
*/
/*------------------------------------------------------------------------------------------*/
 void StatisticsThetaMLXY_Seed_collect_CPU(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*       host_sita_n,
	double*       host_m_n,
	double*       host_L_n,
	double*       host_X_n,
	double*       host_Y_n,	
	int*				dev_klabels,
    double*	      dev_kseed_sita_n,
	double*	      dev_kseed_m_n,
	double*	      dev_kseed_L_n,
	double*       dev_kseed_X_n,
	double*	      dev_kseed_Y_n)
{
	cudaError_t cudaStatus;
	double*	      host_kseed_sita_n=new double[kseedsSize];
	double*	      host_kseed_m_n=new double[kseedsSize];
	double*	      host_kseed_L_n=new double[kseedsSize];
	double*       host_kseed_X_n=new double[kseedsSize];
	double*	      host_kseed_Y_n=new double[kseedsSize];
	int*	      host_klabels=new int[width*height];
	unsigned int*       dev_img_mask;
	unsigned int*       host_img_mask=new unsigned[width*height];
 
   cudaStatus=cudaMalloc((void**)&dev_img_mask,sizeof(unsigned int)*width*height);  
   InitMaskTemplate(dev_img_mask,dev_klabels,width,height,4);

   cudaStatus = cudaMemcpy(host_kseed_sita_n, dev_kseed_sita_n, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
   cudaStatus = cudaMemcpy(host_kseed_m_n, dev_kseed_m_n, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
   cudaStatus = cudaMemcpy(host_kseed_L_n, dev_kseed_L_n, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
   cudaStatus = cudaMemcpy(host_kseed_X_n, dev_kseed_X_n, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
   cudaStatus = cudaMemcpy(host_kseed_Y_n, dev_kseed_Y_n, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
   cudaStatus = cudaMemcpy(host_klabels, dev_klabels, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
   cudaStatus = cudaMemcpy(host_img_mask,dev_img_mask, kseedsSize * sizeof(double), cudaMemcpyDeviceToHost);
  
	StatisticsThetaMLXY_Seed_collect_CPU_ALL(
					kseedsSize,
					width,
					height,
					host_sita_n,
					host_m_n,
					host_L_n,
					host_X_n,
					host_Y_n,	
					host_klabels,
					host_img_mask,
					host_kseed_sita_n,
					host_kseed_m_n,
					host_kseed_L_n,
					host_kseed_X_n,
					host_kseed_Y_n);

    delete[]	  host_kseed_sita_n;
	delete[]      host_kseed_m_n;
	delete[]      host_kseed_L_n;
	delete[]      host_kseed_X_n;
	delete[]      host_kseed_Y_n;
	delete[]      host_klabels;
	cudaFree(dev_img_mask);
	delete[]      host_img_mask;

}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ�Ĭ����K^2��ƽ����������
*
*@param			kseedsSize		��������
*@param			width		ͼ����
*@param			height		ͼ��߶�
*@param	[in]	m_lvec		ͼ��L��������
*@param	[in]	m_avec		ͼ��A��������
*@param	[in]	m_bvec		ͼ��B��������
*@param	[in]	dev_klabels		ͼ��궨����
*@param [in]	dev_img_mask		δʹ��
*@param	[out]	dev_sigmal		��L����ͳ�����
*@param	[out]	dev_sigmaa		��A����ͳ�����
*@param	[out]	dev_sigmab		��B����ͳ�����
*@param	[out]	dev_sigmax		��X����ͳ�����
*@param	[out]	dev_sigmay		��Y����ͳ�����
*@param	[out]	dev_clustersize	 δʹ�� 	
*
*/
/*------------------------------------------------------------------------------------------*/
void StatisticsThetaMLXY_Seed_collect_CPU_ALL(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*       host_sita_n,
	double*       host_m_n,
	double*       host_L_n,
	double*       host_X_n,
	double*       host_Y_n,	
	int*				host_klabels,
	unsigned int*       host_img_mask,
    double*	      host_kseed_sita_n,
	double*	      host_kseed_m_n,
	double*	      host_kseed_L_n,
	double*       host_kseed_X_n,
	double*	      host_kseed_Y_n)
{
      double*			  host_sigmaSita=new double[kseedsSize];
	  double*			  host_sigmaM=new double[kseedsSize];
	  double*			  host_sigmaL=new double[kseedsSize];
	  double*			  host_sigmaX=new double[kseedsSize];
	  double*		      host_sigmaY=new double[kseedsSize];
	  double*			  host_clustersize=new double[kseedsSize];
	memset(host_sigmaSita,0,kseedsSize * sizeof(double));
	memset(host_sigmaM,0,kseedsSize * sizeof(double));
	memset(host_sigmaL,0,kseedsSize * sizeof(double));
	memset(host_sigmaX,0,kseedsSize * sizeof(double));
	memset(host_sigmaY,0,kseedsSize * sizeof(double));	
	memset(host_clustersize,0,kseedsSize*sizeof(double));
	{
		int ind(0);
		for(register int r = 0; r < height; r++ )
		{
			for(register  int c = 0; c < width; c++ )
			{
				if (host_img_mask==nullptr){
					host_sigmaSita[host_klabels[ind]] +=host_sita_n[ind];
					host_sigmaM[host_klabels[ind]] += host_m_n[ind];
					host_sigmaL[host_klabels[ind]] += host_L_n[ind];
					host_sigmaX[host_klabels[ind]] += host_X_n[ind];
					host_sigmaY[host_klabels[ind]] += host_Y_n[ind];
					host_clustersize[host_klabels[ind]] += 1.0;
				}
				if (host_img_mask!=nullptr&&host_img_mask[ind]==0x000000000){
					host_sigmaSita[host_klabels[ind]] +=host_sita_n[ind];
					host_sigmaM[host_klabels[ind]] += host_m_n[ind];
					host_sigmaL[host_klabels[ind]] += host_L_n[ind];
					host_sigmaX[host_klabels[ind]] += host_X_n[ind];
					host_sigmaY[host_klabels[ind]] += host_Y_n[ind];
					host_clustersize[host_klabels[ind]] += 1.0;
				}				
				//------------------------------------
				//edgesum[klabels[ind]] += edgemag[ind];
				//------------------------------------
				
				ind++;
			}
		}
		///////////////////////////////////////////////
		for(int spi=0;spi<kseedsSize;spi++){
			   host_kseed_sita_n[spi]=host_sigmaSita[spi]/host_clustersize[spi];
	           host_kseed_m_n[spi]=host_sigmaM[spi]/host_clustersize[spi];
	           host_kseed_L_n[spi]=host_sigmaL[spi]/host_clustersize[spi];
	           host_kseed_X_n[spi]=host_sigmaX[spi]/host_clustersize[spi];
	           host_kseed_Y_n[spi]=host_sigmaY[spi]/host_clustersize[spi];
		}
	}
	delete []  host_sigmaSita;
	delete []  host_sigmaM;
	delete []  host_sigmaL;
	delete []  host_sigmaX;
	delete []  host_sigmaY;
	delete []  host_clustersize;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ�Ĭ����K^2��ƽ����������
*
*@param			kseedsSize		��������
*@param			width		ͼ����
*@param			height		ͼ��߶�
*@param	[in]	m_lvec		ͼ��L��������
*@param	[in]	m_avec		ͼ��A��������
*@param	[in]	m_bvec		ͼ��B��������
*@param	[in]	dev_klabels		ͼ��궨����
*@param [in]	dev_img_mask		δʹ��
*@param	[out]	dev_sigmal		��L����ͳ�����
*@param	[out]	dev_sigmaa		��A����ͳ�����
*@param	[out]	dev_sigmab		��B����ͳ�����
*@param	[out]	dev_sigmax		��X����ͳ�����
*@param	[out]	dev_sigmay		��Y����ͳ�����
*@param	[out]	dev_clustersize	 δʹ�� 	
*
*/
/*------------------------------------------------------------------------------------------*/
void StatisticsThetaMLXY_Seed_collect_CPU_Incompletion(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	unsigned int*       dev_img_mask,
	double*	           dev_kseed_sita_n,
	double*	           dev_kseed_m_n,
	double*	           dev_kseed_L_n,
	double*            dev_kseed_X_n,
	double*	           dev_kseed_Y_n)
{
	
	cudaError_t cudaStatus;
	unsigned int*       host_img_mask=nullptr;
	int*				host_klabels=new int[width*height];
	double*				host_sigmal=new double[kseedsSize];
	double*				host_sigmaa=new double[kseedsSize];
	double*				host_sigmab=new double[kseedsSize];
	double*				host_sigmax=new double[kseedsSize];
	double*				host_sigmay=new double[kseedsSize];
	double*				host_clustersize=new double[kseedsSize];

#if 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (dev_img_mask==nullptr){
		host_img_mask=nullptr;
	}else{
		 host_img_mask=new unsigned int[width*height];
		 cudaStatus = cudaMemcpy( host_img_mask,dev_img_mask, width*height * sizeof(unsigned int), cudaMemcpyDeviceToHost);
	}
	 cudaStatus = cudaMemcpy(host_klabels,dev_klabels, width*height * sizeof(int), cudaMemcpyDeviceToHost);
	memset(host_sigmal,0,kseedsSize * sizeof(double));
	memset(host_sigmaa,0,kseedsSize * sizeof(double));
	memset(host_sigmab,0,kseedsSize * sizeof(double));
	memset(host_sigmax,0,kseedsSize * sizeof(double));
	memset(host_sigmay,0,kseedsSize * sizeof(double));	
	memset(host_clustersize,0,kseedsSize*sizeof(double));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
	{
		int ind(0);
		for(register int r = 0; r < height; r++ )
		{
			for(register  int c = 0; c < width; c++ )
			{
				if (host_img_mask==nullptr){
					host_sigmal[host_klabels[ind]] += m_lvec[ind];
					host_sigmaa[host_klabels[ind]] += m_avec[ind];
					host_sigmab[host_klabels[ind]] += m_bvec[ind];
					host_sigmax[host_klabels[ind]] += c;
					host_sigmay[host_klabels[ind]] += r;
					host_clustersize[host_klabels[ind]] += 1.0;
				}
				if (host_img_mask!=nullptr&&host_img_mask[ind]==0x000000000){
					host_sigmal[host_klabels[ind]] += m_lvec[ind];
					host_sigmaa[host_klabels[ind]] += m_avec[ind];
					host_sigmab[host_klabels[ind]] += m_bvec[ind];
					host_sigmax[host_klabels[ind]] += c;
					host_sigmay[host_klabels[ind]] += r;
					host_clustersize[host_klabels[ind]] += 1.0;
				}				
				//------------------------------------
				//edgesum[klabels[ind]] += edgemag[ind];
				//------------------------------------
				
				ind++;
			}
		}
#if 1
				
				double *inv=new double[kseedsSize];
				{
					for( int k = 0; k < kseedsSize; k++ )
					{
						if(host_clustersize[k] <= 0 ) host_clustersize[k] = 1;
						inv[k] = 1.0/host_clustersize[k];//computing inverse now to multiply, than divide later
						////////////////////////////////////
						host_sigmal[k] = host_sigmal[k]*inv[k];
						host_sigmaa[k] = host_sigmaa[k]*inv[k];
						host_sigmab[k] = host_sigmab[k]*inv[k];
						host_sigmax[k] = host_sigmax[k]*inv[k];
						host_sigmay[k] = host_sigmay[k]*inv[k];
						//------------------------------------
						//edgesum[k] *= inv[k];
						//------------------------------------
							ConvertLab2oml(
							width,
							height,
							host_sigmal[k],
							host_sigmaa[k],
							host_sigmab[k],
							host_sigmax[k],
							host_sigmay[k],
							&host_sigmal[k],
							&host_sigmaa[k],
							&host_sigmab[k],
							&host_sigmax[k],
							&host_sigmay[k]);
					}
				}
				delete []inv;
#endif
	}
#if 1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (dev_img_mask==nullptr){
		host_img_mask=nullptr;
	}else{
		cudaStatus = cudaMemcpy( dev_img_mask,host_img_mask, width*height * sizeof(int), cudaMemcpyHostToDevice);
	}
	
	cudaStatus = cudaMemcpy(dev_kseed_sita_n, host_sigmal, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);	
	cudaStatus = cudaMemcpy(dev_kseed_m_n, host_sigmaa, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);	
	cudaStatus = cudaMemcpy( dev_kseed_L_n, host_sigmab, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);	
	cudaStatus = cudaMemcpy(dev_kseed_X_n, host_sigmax, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);	
	cudaStatus = cudaMemcpy(dev_kseed_Y_n, host_sigmay, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
	delete [] host_img_mask;
	delete [] host_klabels;
	delete [] host_sigmal;
	delete [] host_sigmaa;
	delete [] host_sigmab;
	delete [] host_sigmax;
	delete [] host_sigmay;
	delete [] host_clustersize;
 ;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect_L(
	const int			kseedSize,
	const int			width,
	const int			height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	__shared__ double temp[4096];
	assert(kseedSize<=4096);
	{
		int ind=threadIdx.x+blockIdx.x*blockDim.x;
		int offset=blockDim.x*gridDim.x;
		temp[threadIdx.x]=temp[threadIdx.x+1024]=temp[threadIdx.x+2048]=temp[threadIdx.x+3072]=0;
		__syncthreads();
		while(ind<width*height){
			int labels_ind=dev_klabels[ind];
			assert(labels_ind>=0&&labels_ind<kseedSize);
			atomicAdd(&temp[labels_ind],m_lvec[ind]);
			ind+=offset;
		}
		__syncthreads();
		atomicAdd(&dev_sigmal[threadIdx.x],temp[threadIdx.x]);
		atomicAdd(&dev_sigmal[threadIdx.x+1024],temp[threadIdx.x+1024]);
		atomicAdd(&dev_sigmal[threadIdx.x+2048],temp[threadIdx.x+2048]);
		atomicAdd(&dev_sigmal[threadIdx.x+3072],temp[threadIdx.x+3072]);
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ�Ĭ����K^2��ƽ����������
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect_gpu_search(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*				kseedsx,
	double*				kseedsy,
	double*				dev_m_lvec,
	double*				dev_m_avec,
	double*				dev_m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	int SeedIdx=threadIdx.x+blockIdx.x*blockDim.x;
	int offset=blockDim.x*gridDim.x;

	while(SeedIdx<kseedsSize){
		////////////////////////////////////////
		int kseedWH=sqrt((float)kseedsSize);
		int x_step=width/(kseedWH);
		int y_step=height/(kseedWH);
		/////////////////////////////////////////
		int x_min=max((int)(kseedsx[SeedIdx]-x_step),(int)0);
		int x_max=min((int)(kseedsx[SeedIdx]+x_step),(int)width);
		int y_min=max((int)(kseedsy[SeedIdx]-y_step),(int)0);
		int y_max=min((int)(kseedsy[SeedIdx]+y_step),(int)height);
		/////////////////////////////////////////

		for (register int xi=x_min;xi<x_max;xi++){
			for (register int yi=y_min;yi<y_max;yi++){
				int ImgIdx=xi+yi*width;
				int labels_t=dev_klabels[ImgIdx];
#if 1
				if (labels_t==SeedIdx){
					dev_sigmal[SeedIdx] += dev_m_lvec[ImgIdx];
					dev_sigmaa[SeedIdx] += dev_m_avec[ImgIdx];
					dev_sigmab[SeedIdx] += dev_m_bvec[ImgIdx];
					dev_sigmax[SeedIdx] += xi;
					dev_sigmay[SeedIdx] += yi;
					dev_clustersize[SeedIdx] += 1.0;
				}
#endif
			}
		}

		
		/////////////////////////////////////////
			SeedIdx+=offset;
	}
	
	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*�ռ�ȫͼ�����ӣ����㿪ʼȫͼ����
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_collect_gpu_search_AllImg(
	const int			kseedsSize,
	const int			width,
	const int			height,
	double*				dev_m_lvec,
	double*				dev_m_avec,
	double*				dev_m_bvec,
	int*				dev_klabels,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize)
{
	int SeedIdx=threadIdx.x+blockIdx.x*blockDim.x;
	int offset=blockDim.x*gridDim.x;
	while(SeedIdx<kseedsSize){
		/*--------------------------------------*/
		int kseedWH=sqrt((float)kseedsSize);
		int x_step=width/(kseedWH);
		int y_step=height/(kseedWH);
		/*--------------------------------------*/
		int x_min=0;
		int x_max=width;
		int y_min=0;
		int y_max=height;
		/*--------------------------------------*/
		for (register int xi=x_min;xi<x_max;xi++){
			for (register int yi=y_min;yi<y_max;yi++){
				int ImgIdx=xi+yi*width;
				int labels_t=dev_klabels[ImgIdx];
#if 1
				if (labels_t==SeedIdx){
					dev_sigmal[SeedIdx] += dev_m_lvec[ImgIdx];
					dev_sigmaa[SeedIdx] += dev_m_avec[ImgIdx];
					dev_sigmab[SeedIdx] += dev_m_bvec[ImgIdx];
					dev_sigmax[SeedIdx] += xi;
					dev_sigmay[SeedIdx] += yi;
					dev_clustersize[SeedIdx] += 1.0;
				}
#endif
			}
		}
		/*--------------------------------------*/
		SeedIdx+=offset;
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*�����µ�ͼ�������   
*
*
*@param		    	   kseedSize			��������	
*@param				    m_width				ͼ����	
*@param				    m_height			ͼ��߶�	
*@param		[in]		dev_sigmal			��L����ͳ�����
*@param		[in]		dev_sigmaa			ͼ��A��������
*@param		[in]		dev_sigmab			ͼ��B��������
*@param		[in]		dev_sigmax			��X����ͳ�����
*@param		[in]		dev_sigmay			��Y����ͳ�����
*@param		[in]		dev_clustersize		������ͼ��������С
*@param		[out]		dev_kseedsl		    ����L����
*@param		[out]		dev_kseedsa		    ͼ��A����
*@param		[out]		dev_kseedsb		    ͼ��B����
*@param		[out]		dev_kseedsx		    ����X����
*@param		[out]		dev_kseedsy		    ����Y����
*@param		[out]		dev_kseed_sita_n	ͼ��ĽǶȷ���
*@param		[out]		dev_kseed_m_n	    ͼ���ģ������
*@param		[out]		dev_kseed_L_n	    ͼ������ȷ���
*@param		[out]		dev_kseed_X_n       ͼ���X�������
*@param		[out]		dev_kseed_Y_n	    ͼ���Y�������
*
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void StatisticsLABXY_Seed_calculate(
	const int			kseedSize,
	const int			m_width,
	const int			m_height,
	double*				dev_sigmal,
	double*				dev_sigmaa,
	double*				dev_sigmab,
	double*				dev_sigmax,
	double*				dev_sigmay,
	double*				dev_clustersize,
	double*				dev_kseedsl,
	double*				dev_kseedsa,
	double*				dev_kseedsb,
	double*				dev_kseedsx,
	double*				dev_kseedsy,
	double*				dev_kseed_sita_n,
	double*				dev_kseed_m_n,
	double*				dev_kseed_L_n,
	double*				dev_kseed_X_n,
	double*				dev_kseed_Y_n)
{
	int ki=blockIdx.x*blockDim.x+threadIdx.x;
	int offset=blockDim.x*gridDim.x;
	while(ki<kseedSize){

		{
				if (dev_clustersize[ki]<=0)  dev_clustersize[ki]=1;
				double inv_k=1.0/dev_clustersize[ki];
				/////////////////////////////////////////////////
				assert(dev_clustersize[ki]>=0&&dev_clustersize[ki]<=m_width*m_height);
				///////////////////////////////////////////////////
				dev_kseedsl[ki] = dev_sigmal[ki]*inv_k;
				dev_kseedsa[ki] = dev_sigmaa[ki]*inv_k;
				dev_kseedsb[ki] = dev_sigmab[ki]*inv_k;
				dev_kseedsx[ki] = dev_sigmax[ki]*inv_k;
				dev_kseedsy[ki] = dev_sigmay[ki]*inv_k;
				///////////////////////////////////////////////////
#if 1
					ConvertLab2oml(
					m_width,
					m_height,
					dev_kseedsl[ki],
					dev_kseedsa[ki],
					dev_kseedsb[ki],
					dev_kseedsx[ki],
					dev_kseedsy[ki],
					&dev_kseed_sita_n[ki],
					&dev_kseed_m_n[ki],
					&dev_kseed_L_n[ki],
					&dev_kseed_X_n[ki],
					&dev_kseed_Y_n[ki]);
#endif
				
		}
		ki+=offset;

	}
	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*@param			kseedsSize    ���Ӹ���
*@param			m_width       ͼ����
*@param			m_height      ͼ��߶�
*@param			dev_kseedsl   ͼ���L����
*@param			dev_kseedsa   ͼ���A����
*@param			dev_kseedsb   ͼ���B����
*@param			dev_kseedsx   ͼ���X�������
*@param			dev_kseedsy   ͼ���X�������
*@param			dev_kseeds_sita_n  ͼ��ĽǶȷ���
*@param			dev_kseeds_m_n     ͼ���ģ������
*@param			dev_kseeds_L_n     ͼ������ȷ���
*@param			dev_kseeds_X_n     ͼ���X�������
*@param			dev_kseeds_Y_n     ͼ���Y�������
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void PerformSuperpixelSLIC_Init_ColorConvert(
	const int			kseedsSize,
	const int			m_width,
	const int			m_height,
	double*				dev_kseedsl,
	double*				dev_kseedsa,
	double*				dev_kseedsb,
	double*				dev_kseedsx,
	double*				dev_kseedsy,
	double*				dev_kseeds_sita_n,
	double*				dev_kseeds_m_n,
	double*				dev_kseeds_L_n,
	double*				dev_kseeds_X_n,
	double*				dev_kseeds_Y_n)
{
	///////////////////////////////////////////
	int ind=blockIdx.x*blockDim.x+threadIdx.x;
	int step=blockDim.x*gridDim.x;
	//////////////////////////////////////////
	while(ind<kseedsSize){
			/////////////////////////////////////
#if 1
			ConvertLab2oml(
				m_width,
				m_height,
				dev_kseedsl[ind],
				dev_kseedsa[ind],
				dev_kseedsb[ind],
				dev_kseedsx[ind],
				dev_kseedsy[ind],
				&dev_kseeds_sita_n[ind],
				&dev_kseeds_m_n[ind],
				&dev_kseeds_L_n[ind],
				&dev_kseeds_X_n[ind],
				&dev_kseeds_Y_n[ind]);
#endif
			ind+=step;
	}
	///////////////////////////////////////
}
/*------------------------------------------------------------------------------------------*/
/**
*
*��LABɫ��װ�����Ƕȡ�ģ�������� ��X��Y��ɵķ���
*
*@param   kseedsSize  ���Ӹ���
*@param   m_width     ͼ����
*@param   m_height    ͼ��߶�
*@param  [in]  dev_m_lvec  ͼ���L����
*@param  [in] dev_m_avec  ͼ���A����
*@param  [in] dev_m_bvec  ͼ���B����
*@param  [out] dev_sita_n  ͼ��ĽǶȷ���
*@param  [out] dev_m_n	  ͼ���ģ������
*@param  [out] dev_L_n     ͼ������ȷ���
*@param  [out] dev_X_n     ͼ���X�������
*@param  [out] dev_Y_n     ͼ���Y�������
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void PerformSuperpixelSLIC_ColorConvert(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				dev_m_lvec,
	double*				dev_m_avec,
	double*				dev_m_bvec,
	double*				dev_sita_n,
	double*				dev_m_n,
	double*				dev_L_n,
	double*				dev_X_n,
	double*				dev_Y_n)
{
	///////////////////////////////////////////
	int xi=blockIdx.x*blockDim.x+threadIdx.x;
	int yj=blockIdx.y*blockDim.y+threadIdx.y;
	int ind=xi+yj*m_width;
	//////////////////////////////////////////
	if(xi<m_width&&
		xi>=0&&
		yj>=0&&
		yj<m_height){
		/////////////////////////////////////
#if 1
			ConvertLab2oml(
				m_width,
				m_height,
				dev_m_lvec[ind],
				dev_m_avec[ind],
				dev_m_bvec[ind],
				xi,
				yj,
				&dev_sita_n[ind],
				&dev_m_n[ind],
				&dev_L_n[ind],
				&dev_X_n[ind],
				&dev_Y_n[ind]);
#endif

		///////////////////////////////////
	}
	///////////////////////////////////////
}
/*------------------------------------------------------------------------------------------*/
/**
*��LABɫ��װ�����Ƕȡ�ģ�������� ��X��Y��ɵķ���
*
*@param   kseedsSize  ���Ӹ���
*@param   m_width     ͼ����
*@param   m_height    ͼ��߶�
*@param  [in]  dev_m_lvec  ͼ���L����
*@param  [in] dev_m_avec  ͼ���A����
*@param  [in] dev_m_bvec  ͼ���B����
*@param  [out] dev_sita_n  ͼ��ĽǶȷ���
*@param  [out] dev_m_n	  ͼ���ģ������
*@param  [out] dev_L_n     ͼ������ȷ���
*@param  [out] dev_X_n     ͼ���X�������
*@param  [out] dev_Y_n     ͼ���Y�������
*
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_ColorConvert_gpu(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				dev_m_lvec,
	double*				dev_m_avec,
	double*				dev_m_bvec,
	double*				dev_sita_n,
	double*				dev_m_n,
	double*				dev_L_n,
	double*				dev_X_n,
	double*				dev_Y_n)
{
#if  _DEBUG
	cudaEvent_t start,stop;
	cudaError_t cudaStatus;
	float costtime_ms=0;
	float costtime_us=0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	///////////////////////////////////////////////////////////////////////////////////////
	cudaEventRecord(start,0);
#endif
	{
		dim3    threadsPerBlock(16,16);
		dim3    numBlock((m_width+threadsPerBlock.x-1)/threadsPerBlock.x,(m_height+threadsPerBlock.y-1)/threadsPerBlock.y);
		PerformSuperpixelSLIC_ColorConvert<<<numBlock,threadsPerBlock>>>(
			kseedsSize,
			m_width,
			m_height,
			dev_m_lvec,
			dev_m_avec,
			dev_m_bvec,
			dev_sita_n,
			dev_m_n,
			dev_L_n,
			dev_X_n,
			dev_Y_n);
	}
	////////////////////////////////////////////////////////////////////////////////////////
	cudaGetLastError_Sync_CUI();
#if _DEBUG
	cudaStatus=cudaEventRecord(stop,0);	
	cudaGetLastError_Sync_CUI();
	cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
	costtime_us=costtime_ms*1000;
#endif
	cudaGetLastError_Sync_CUI();
#if _DEBUG
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
#endif

}
/*------------------------------------------------------------------------------------------*/
/**
*����GPUʵ�ֳ������㷨
*
*@param			alpha			�Ƕ�ϵ��
*@param			betta			ģ��ϵ��
*@param			gama			����ϵ��
*@param			fai				�������
*@param			kseedsSize		���Ӹ��� 
*@param			m_width			ͼ����
*@param			m_height		ͼ��߶�
*@param		[]	host_m_lvec		ͼ��L����
*@param		[]	host_m_avec		ͼ��A����
*@param		[]	host_m_bvec		ͼ��B����
*@param		[]	host_kseedsl	���ӵ�L����
*@param		[]	host_kseedsa	���ӵ�A����
*@param		[]	host_kseedsb	���ӵ�B����
*@param		[]	host_kseedsx	���ӵ�X�������
*@param		[]	host_kseedsy	���ӵ�Y�������
*@param		[]	host_klabels	ͼ��ı궨��Ϣ
*@param			STEP
*@param		[]	host_edgemag
*@param  		M
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_gpu(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				host_m_lvec,
	double*				host_m_avec,
	double*				host_m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M)
{
	cudaError_t cudaStatus;
	///////////////////////////////////////////
	double*				dev_kseedsl;
	double*				dev_kseedsa;
	double*				dev_kseedsb;
	double*				dev_kseedsx;
	double*				dev_kseedsy;
	int*				dev_klabels;
	double*				dev_m_lvec;
	double*				dev_m_avec;
	double*				dev_m_bvec;
	double*				dev_sita_n;
	double*				dev_m_n;
	double*				dev_L_n;
	double*				dev_X_n;
	double*				dev_Y_n;
	double*				dev_edgemag=nullptr;
	///////////////////////////////////////////
	double* dev_distvec;
	///////////////////////////////////////////
	assert(cudaInit_CUI()==true);

	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop,0);
	int blocks=prop.multiProcessorCount;

	{
		cudaStatus = cudaMalloc((void**)&dev_kseedsl, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsb, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsx, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsy, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_klabels,m_width*m_height * sizeof(int));
		cudaStatus = cudaMalloc((void**)&dev_m_lvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_avec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_bvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_distvec, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_sita_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_L_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_X_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_Y_n, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMemcpy(dev_kseedsl, host_kseedsl, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsa, host_kseedsa, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsb, host_kseedsb, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsx, host_kseedsx, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsy, host_kseedsy, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_klabels, host_klabels, m_width*m_height * sizeof(int), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_lvec, host_m_lvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_avec, host_m_avec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_bvec, host_m_bvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		////////////////////////////////////////////////////////////////////////////////
	}
	double* dev_sigmal;
	double* dev_sigmaa;
	double* dev_sigmab;
	double* dev_sigmax;
	double* dev_sigmay;
	double* dev_clustersize;
	/////////////////////////////////////////////////////////////////////////////////////
	double*  dev_kseed_sita_n;
	double*  dev_kseed_m_n;
	double*  dev_kseed_L_n;
	double*  dev_kseed_X_n;
	double*  dev_kseed_Y_n;
	/////////////////////////////////////////////////////////////////////////////////////
	{
		cudaStatus = cudaMalloc((void**)&dev_sigmal, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmaa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmab, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmax, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmay, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_clustersize, kseedsSize * sizeof(double));
		cudaStatus = cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));
		cudaGetLastError_Sync_CUI();
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_kseed_sita_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_m_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_L_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_X_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_Y_n, kseedsSize * sizeof(double));
	}
	cudaEvent_t start,stop;
	float costtime_ms=0;
	float costtime_us=0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
#if 1
	///////////////////////////////////////////
	PerformSuperpixelSLIC_ColorConvert_gpu(
		kseedsSize,
		m_width,
		m_height,
		dev_m_lvec,
		dev_m_avec,
		dev_m_bvec,
		dev_sita_n,
		dev_m_n,
		dev_L_n,
		dev_X_n,
		dev_Y_n);
	//////////////////////////////////////////
	PerformSuperpixelSLIC_Init_ColorConvert<<<1,256>>>(
		kseedsSize,
		m_width,
		m_height,
		dev_kseedsl,
		dev_kseedsa,
		dev_kseedsb,
		dev_kseedsx,
		dev_kseedsy,
		dev_kseed_sita_n,
		dev_kseed_m_n,
		dev_kseed_L_n,
		dev_kseed_X_n,
		dev_kseed_Y_n);
	//////////////////////////////////////////
#endif

	for( int itr = 0; itr < 10; itr++ )
	{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		{
			cudaStatus=cudaEventRecord(start,0);
#if 0
			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+15)/threadsPerBlock.x,(m_height+15)/threadsPerBlock.y);
			SetMatrix<<<numBlock,threadsPerBlock>>>(dev_distvec,m_width,m_height,1.7976931348623158e+308);
#else
			SetMatrix_Efficient<<<16,16*16>>>(dev_distvec,m_width,m_height,1.7976931348623158e+308);
#endif
			
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
		}
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			cudaStatus=cudaEventRecord(start,0);
#if 0
			SetMinimumOnLab<<<(kseedsSize+511)/512,512>>>(
				kseedsSize,
				alpha,
				betta,
				gama,
				fai,
				m_width,
				m_height,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_kseedsl,
				dev_kseedsa,
				dev_kseedsb,
				dev_kseedsx,
				dev_kseedsy,
				dev_klabels,
				dev_distvec,
				STEP,
				M);
#else
			dim3    threadsPerBlock(32,32);
			dim3    numBlock((m_width+31)/threadsPerBlock.x,(m_height+31)/threadsPerBlock.y);
			SetMinimumOnLab_AllSeeds<<<numBlock,threadsPerBlock>>>(
				kseedsSize,
				alpha,
				betta,
				gama,
				fai,
				m_width,
				m_height,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_sita_n,
				dev_m_n,
				dev_L_n,
				dev_X_n,
				dev_Y_n,
				dev_kseedsl,
				dev_kseedsa,
				dev_kseedsb,
				dev_kseedsx,
				dev_kseedsy,
				dev_kseed_sita_n,
				dev_kseed_m_n,
				dev_kseed_L_n,
				dev_kseed_X_n,
				dev_kseed_Y_n,
				dev_klabels,
				dev_distvec);
#endif
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			/*cudaStatus=cudaEventRecord(start,0);*/
			/*---------------------------------------------------------------------------------*/
//			const int numk=kseedsSize;
			cudaMemset(dev_sigmal,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmaa,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmab,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmax,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmay,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			/*----------------------------------------------------------------------------------*/
			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+15)/threadsPerBlock.x,(m_height+15)/threadsPerBlock.y);
			unsigned int *mutex; 
			unsigned int zero = 0; 
			cudaMalloc( (void**)&mutex, sizeof(unsigned int) ); 
			cudaMemcpy(mutex, &zero, sizeof(unsigned int), cudaMemcpyHostToDevice );
#if 0
			cudaStatus=cudaEventRecord(start,0);
			//1225.2045ms
			StatisticsLABXY_Seed_collect<<<numBlock,threadsPerBlock>>>(
				mutex,
				kseedsSize,
				m_width,
				m_height,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_klabels,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
#endif
#if 0
			//69.114082ms
			StatisticsLABXY_Seed_collect_CPU(
				kseedsSize,
				m_width,
				m_height,
				host_m_lvec,
				host_m_avec,
				host_m_bvec,
				dev_klabels,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
#endif
#if 1
			cudaStatus=cudaEventRecord(start,0);
			//344.65720ms
			StatisticsLABXY_Seed_collect_new2014_11_3<<<blocks*2,1024>>>(
				mutex,
				kseedsSize,
				m_width,
				m_height,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_klabels,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
#endif
#if 0
			cudaStatus=cudaEventRecord(start,0);
			dim3    threadsPerBlock_Quad(32,32);
			dim3    numBlock_Quad(4,4);
			StatisticsLABXY_Seed_collect_blocks<<<numBlock_Quad,threadsPerBlock_Quad>>>(
				kseedsSize,
				m_width,
				m_height,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_klabels,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;

#endif
			cudaStatus=cudaEventRecord(start,0);
			cudaGetLastError_Sync_CUI();
			StatisticsLABXY_Seed_calculate<<<1,512>>>(
				kseedsSize,
				m_width,
				m_height,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize,
				dev_kseedsl,
				dev_kseedsa,
				dev_kseedsb,
				dev_kseedsx,
				dev_kseedsy,
				dev_kseed_sita_n,
				dev_kseed_m_n,
				dev_kseed_L_n,
				dev_kseed_X_n,
				dev_kseed_Y_n);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
		}
		
	}
	cudaMemcpy(host_klabels,dev_klabels,m_width*m_height*sizeof(int),cudaMemcpyDeviceToHost);
	{
	cudaFree(dev_kseedsl);
	cudaFree(dev_kseedsa);
	cudaFree(dev_kseedsb);
	cudaFree(dev_kseedsx);
	cudaFree(dev_kseedsy);
	cudaFree(dev_klabels);
	cudaFree(dev_m_lvec);
	cudaFree(dev_m_avec);
	cudaFree(dev_m_bvec);
	cudaFree(dev_distvec);
	////////////////////////
	cudaFree(dev_sita_n);
	cudaFree(dev_m_n);
	cudaFree(dev_L_n);
	cudaFree(dev_X_n);
	cudaFree(dev_Y_n);
		{
			cudaFree(dev_sigmal);
			cudaFree(dev_sigmaa);
			cudaFree(dev_sigmab);
			cudaFree(dev_sigmax);
			cudaFree(dev_sigmay);
			cudaFree(dev_clustersize);
			//////////////////////////
			cudaFree(dev_kseed_sita_n) ;
			cudaFree(dev_kseed_m_n);
			cudaFree(dev_kseed_L_n);
			cudaFree(dev_kseed_X_n);
			cudaFree(dev_kseed_Y_n);
		}
	}
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	assert(cudaDeInit_CUI()==true);
	
}
/*------------------------------------------------------------------------------------------*/
/**
*cuda����ʵ�г������㷨��
*
*
*@param    alpha  �ǶȲ���alpha
*@param    betta  ģֵbetta
*@param    gama   ����gama
*@param    fai    XY����
*@param    kseedsSize  ���Ӹ���
*@param    m_width   ͼ����
*@param    m_height  ͼ��߶�
*@param	   m_lvec  ͼ��L����
*@param	   m_avec  ͼ��A����
*@param	   m_bvec  ͼ��B����
*@param	   kseedsl  ����L����
*@param	   kseedsa  ����A����
*@param	   kseedsb  ����B����
*@param	   kseedsx  ����X����
*@param	   kseedsy  ����Y����
*@param	   klabels  ͼ��ͼ��ı궨ͼ��
*@param	   STEP     δʹ��
*@param	   edgemag  δʹ��
*@param	   M        δʹ���ܶ�����
*
*
*@note
*����GPUʵ�ֳ������㷨
*Сͼ1.8S  ��ͼ7.2S
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_gpu_simplify(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,   
	const int m_height,
	double*				host_m_lvec,
	double*				host_m_avec,
	double*				host_m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M)
{
	cudaError_t cudaStatus;
	///////////////////////////////////////////
	double*				dev_kseedsl;
	double*				dev_kseedsa;
	double*				dev_kseedsb;
	double*				dev_kseedsx;
	double*				dev_kseedsy;
	int*				dev_klabels;
	double*				dev_m_lvec;
	double*				dev_m_avec;
	double*				dev_m_bvec;
	double*				dev_sita_n;
	double*				dev_m_n;
	double*				dev_L_n;
	double*				dev_X_n;
	double*				dev_Y_n;
	double*				dev_edgemag=nullptr;
	///////////////////////////////////////////
	double* dev_distvec;
	///////////////////////////////////////////
	assert(cudaInit_CUI()==true);
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop,0);
	int blocks=prop.multiProcessorCount;
	{
		cudaStatus = cudaMalloc((void**)&dev_kseedsl, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsb, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsx, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsy, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_klabels,m_width*m_height * sizeof(int));
		cudaStatus = cudaMalloc((void**)&dev_m_lvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_avec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_bvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_distvec, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_sita_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_L_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_X_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_Y_n, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMemcpy(dev_kseedsl, host_kseedsl, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsa, host_kseedsa, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsb, host_kseedsb, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsx, host_kseedsx, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsy, host_kseedsy, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_klabels, host_klabels, m_width*m_height * sizeof(int), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_lvec, host_m_lvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_avec, host_m_avec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_bvec, host_m_bvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		////////////////////////////////////////////////////////////////////////////////
	}
	double* dev_sigmal;
	double* dev_sigmaa;
	double* dev_sigmab;
	double* dev_sigmax;
	double* dev_sigmay;
	double* dev_clustersize;
	/////////////////////////////////////////////////////////////////////////////////////
	double*  dev_kseed_sita_n;
	double*  dev_kseed_m_n;
	double*  dev_kseed_L_n;
	double*  dev_kseed_X_n;
	double*  dev_kseed_Y_n;
	/////////////////////////////////////////////////////////////////////////////////////
	{
		cudaStatus = cudaMalloc((void**)&dev_sigmal, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmaa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmab, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmax, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmay, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_clustersize, kseedsSize * sizeof(double));
		cudaStatus=cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));
		cudaGetLastError_Sync_CUI();
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_kseed_sita_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_m_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_L_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_X_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_Y_n, kseedsSize * sizeof(double));
	}
	cudaEvent_t start,stop;
	float costtime_ms=0;
	float costtime_us=0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
#if 1
	///////////////////////////////////////////
	PerformSuperpixelSLIC_ColorConvert_gpu(
		kseedsSize,
		m_width,
		m_height,
		dev_m_lvec,
		dev_m_avec,
		dev_m_bvec,
		dev_sita_n,
		dev_m_n,
		dev_L_n,
		dev_X_n,
		dev_Y_n);
	//////////////////////////////////////////
	PerformSuperpixelSLIC_Init_ColorConvert<<<1,256>>>(
		kseedsSize,
		m_width,
		m_height,
		dev_kseedsl,
		dev_kseedsa,
		dev_kseedsb,
		dev_kseedsx,
		dev_kseedsy,
		dev_kseed_sita_n,
		dev_kseed_m_n,
		dev_kseed_L_n,
		dev_kseed_X_n,
		dev_kseed_Y_n);
	cudaGetLastError_Sync_CUI();
	//////////////////////////////////////////
#endif
	for( int itr = 0; itr <10; itr++ )
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		{
			cudaStatus=cudaEventRecord(start,0);
			SetMatrix_Efficient<<<16,16*16>>>(dev_distvec,m_width,m_height,1.7976931348623158e+308);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			cudaStatus=cudaEventRecord(start,0);
			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+threadsPerBlock.x-1)/threadsPerBlock.x,(m_height+threadsPerBlock.y-1)/threadsPerBlock.y);

			SetMinimumOnThetaMLXY_9Seeds<<<numBlock,threadsPerBlock>>>(		
					kseedsSize,
					alpha,
					betta,
					gama,
					fai,
					m_width,
					m_height,			
					dev_sita_n,
					dev_m_n,
					dev_L_n,
					dev_X_n,
					dev_Y_n,				
					dev_kseed_sita_n,
					dev_kseed_m_n,
					dev_kseed_L_n,
					dev_kseed_X_n,
					dev_kseed_Y_n,
					dev_klabels,
					dev_distvec);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{			
#if 1
			cudaStatus=cudaEventRecord(start,0);
			/*---------------------------------------------------------------------------------*/
			//			const int numk=kseedsSize;
			cudaMemset(dev_sigmal,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmaa,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmab,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmax,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_sigmay,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));
			cudaGetLastError_Sync_CUI();
			/*----------------------------------------------------------------------------------*/
			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+15)/threadsPerBlock.x,(m_height+15)/threadsPerBlock.y);
			cudaStatus=cudaEventRecord(start,0);
			cudaGetLastError_Sync_CUI();
//69.114082ms
			StatisticsLABXY_Seed_collect_CPU(
				kseedsSize,
				m_width,
				m_height,
				host_m_lvec,
				host_m_avec,
				host_m_bvec,
				dev_klabels,
				nullptr,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
						cudaStatus=cudaEventRecord(start,0);
			cudaGetLastError_Sync_CUI();
			StatisticsLABXY_Seed_calculate<<<1,512>>>(
				kseedsSize,
				m_width,
				m_height,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize,
				dev_kseedsl,
				dev_kseedsa,
				dev_kseedsb,
				dev_kseedsx,
				dev_kseedsy,
				dev_kseed_sita_n,
				dev_kseed_m_n,
				dev_kseed_L_n,
				dev_kseed_X_n,
				dev_kseed_Y_n);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
			
#else

			cudaStatus=cudaEventRecord(start,0);
			cudaGetLastError_Sync_CUI();
			StatisticsLABXY_Seed_collect_gpu_search<<<(kseedsSize+511)/512,512>>>(
				kseedsSize,
				m_width,
				m_height,
				dev_kseedsx,
				dev_kseedsy,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_klabels,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
#endif
		}
	}
#if 1
	////////////////////////////////////////////////////////////////////////////////////////
	cudaStatus=cudaMemcpy(host_klabels,dev_klabels,m_width*m_height*sizeof(int),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsl,dev_kseedsl, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsa,dev_kseedsa, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsb,dev_kseedsb, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsx,dev_kseedsx, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsy,dev_kseedsy, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	{
		cudaFree(dev_kseedsl);
		cudaFree(dev_kseedsa);
		cudaFree(dev_kseedsb);
		cudaFree(dev_kseedsx);
		cudaFree(dev_kseedsy);
		cudaFree(dev_klabels);
		cudaFree(dev_m_lvec);
		cudaFree(dev_m_avec);
		cudaFree(dev_m_bvec);
		cudaFree(dev_distvec);
		////////////////////////
		cudaFree(dev_sita_n);
		cudaFree(dev_m_n);
		cudaFree(dev_L_n);
		cudaFree(dev_X_n);
		cudaFree(dev_Y_n);
		{
			cudaFree(dev_sigmal);
			cudaFree(dev_sigmaa);
			cudaFree(dev_sigmab);
			cudaFree(dev_sigmax);
			cudaFree(dev_sigmay);
			cudaFree(dev_clustersize);
			//////////////////////////
			cudaFree(dev_kseed_sita_n) ;
			cudaFree(dev_kseed_m_n);
			cudaFree(dev_kseed_L_n);
			cudaFree(dev_kseed_X_n);
			cudaFree(dev_kseed_Y_n);
		}
	}
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	assert(cudaDeInit_CUI()==true);
}
/*------------------------------------------------------------------------------------------*/
/**
*cuda����ʵ�г������㷨��
*
*
*@param    alpha  �ǶȲ���alpha
*@param    betta  ģֵbetta
*@param    gama   ����gama
*@param    fai    XY����
*@param    kseedsSize  ���Ӹ���
*@param    m_width   ͼ����
*@param    m_height  ͼ��߶�
*@param	   m_lvec  ͼ��L����
*@param	   m_avec  ͼ��A����
*@param	   m_bvec  ͼ��B����
*@param	   kseedsl  ����L����
*@param	   kseedsa  ����A����
*@param	   kseedsb  ����B����
*@param	   kseedsx  ����X����
*@param	   kseedsy  ����Y����
*@param	   klabels  ͼ��ͼ��ı궨ͼ��
*@param	   STEP     δʹ��
*@param	   edgemag  δʹ��
*@param	   M        δʹ���ܶ�����
*
*
*@note
*����GPUʵ�ֳ������㷨
*Сͼ1.8S  ��ͼ7.2S
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_gpu_simplify2(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,   
	const int m_height,
	double*				host_m_lvec,
	double*				host_m_avec,
	double*				host_m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M)
{
	cudaError_t cudaStatus;
	///////////////////////////////////////////
	double*				dev_kseedsl;
	double*				dev_kseedsa;
	double*				dev_kseedsb;
	double*				dev_kseedsx;
	double*				dev_kseedsy;
	int*				dev_klabels;
	double*				dev_m_lvec;
	double*				dev_m_avec;
	double*				dev_m_bvec;
	double*				dev_sita_n;
	double*				dev_m_n;
	double*				dev_L_n;
	double*				dev_X_n;
	double*				dev_Y_n;
	double*				dev_edgemag=nullptr;
	///////////////////////////////////////////
	double* dev_distvec;
	///////////////////////////////////////////
	assert(cudaInit_CUI()==true);

	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop,0);
	int blocks=prop.multiProcessorCount;

	{
		cudaStatus = cudaMalloc((void**)&dev_kseedsl, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsb, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsx, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsy, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_klabels,m_width*m_height * sizeof(int));
		cudaStatus = cudaMalloc((void**)&dev_m_lvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_avec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_bvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_distvec, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_sita_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_L_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_X_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_Y_n, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMemcpy(dev_kseedsl, host_kseedsl, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsa, host_kseedsa, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsb, host_kseedsb, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsx, host_kseedsx, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseedsy, host_kseedsy, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_klabels, host_klabels, m_width*m_height * sizeof(int), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_lvec, host_m_lvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_avec, host_m_avec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_bvec, host_m_bvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		////////////////////////////////////////////////////////////////////////////////
	}

	double* dev_clustersize;
	/////////////////////////////////////////////////////////////////////////////////////
	double*  dev_kseed_sita_n;
	double*  dev_kseed_m_n;
	double*  dev_kseed_L_n;
	double*  dev_kseed_X_n;
	double*  dev_kseed_Y_n;
	/////////////////////////////////////////////////////////////////////////////////////
	{

		cudaStatus = cudaMalloc((void**)&dev_clustersize, kseedsSize * sizeof(double));
		cudaStatus=cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));
		cudaGetLastError_Sync_CUI();
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_kseed_sita_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_m_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_L_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_X_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_Y_n, kseedsSize * sizeof(double));
	}
	cudaEvent_t start,stop;
	float costtime_ms=0;
	float costtime_us=0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
#if 1
	///////////////////////////////////////////
	PerformSuperpixelSLIC_ColorConvert_gpu(
		kseedsSize,
		m_width,
		m_height,
		dev_m_lvec,
		dev_m_avec,
		dev_m_bvec,
		dev_sita_n,
		dev_m_n,
		dev_L_n,
		dev_X_n,
		dev_Y_n);
	//////////////////////////////////////////
	PerformSuperpixelSLIC_Init_ColorConvert<<<1,256>>>(
		kseedsSize,
		m_width,
		m_height,
		dev_kseedsl,
		dev_kseedsa,
		dev_kseedsb,
		dev_kseedsx,
		dev_kseedsy,
		dev_kseed_sita_n,
		dev_kseed_m_n,
		dev_kseed_L_n,
		dev_kseed_X_n,
		dev_kseed_Y_n);
	cudaGetLastError_Sync_CUI();
	//////////////////////////////////////////
#endif
	for( int itr = 0; itr <2; itr++ )
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		{
			cudaStatus=cudaEventRecord(start,0);
			SetMatrix_Efficient<<<16,16*16>>>(dev_distvec,m_width,m_height,1.7976931348623158e+308);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			cudaStatus=cudaEventRecord(start,0);
			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+threadsPerBlock.x-1)/threadsPerBlock.x,(m_height+threadsPerBlock.y-1)/threadsPerBlock.y);

			SetMinimumOnThetaMLXY_9Seeds<<<numBlock,threadsPerBlock>>>(		
					kseedsSize,
					alpha,
					betta,
					gama,
					fai,
					m_width,
					m_height,			
					dev_sita_n,
					dev_m_n,
					dev_L_n,
					dev_X_n,
					dev_Y_n,				
					dev_kseed_sita_n,
					dev_kseed_m_n,
					dev_kseed_L_n,
					dev_kseed_X_n,
					dev_kseed_Y_n,
					dev_klabels,
					dev_distvec);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{			

			StatisticsThetaMLXY_Seed_collect_CPU_Incompletion(
								kseedsSize,
									m_width,
									m_height,
									host_m_lvec,
									host_m_avec,
									host_m_bvec,
									dev_klabels,
								    NULL,
									dev_kseed_sita_n,
									dev_kseed_m_n,
									dev_kseed_L_n,
									dev_kseed_X_n,
									dev_kseed_Y_n);
		}
	}
#if 1
	////////////////////////////////////////////////////////////////////////////////////////
	cudaStatus=cudaMemcpy(host_klabels,dev_klabels,m_width*m_height*sizeof(int),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsl,dev_kseedsl, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsa,dev_kseedsa, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsb,dev_kseedsb, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsx,dev_kseedsx, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
	cudaStatus=cudaMemcpy(host_kseedsy,dev_kseedsy, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);
#endif
	///////////////////////////////////////////////////////////////////////////////////////
	{
		cudaFree(dev_kseedsl);
		cudaFree(dev_kseedsa);
		cudaFree(dev_kseedsb);
		cudaFree(dev_kseedsx);
		cudaFree(dev_kseedsy);
		cudaFree(dev_klabels);
		cudaFree(dev_m_lvec);
		cudaFree(dev_m_avec);
		cudaFree(dev_m_bvec);
		cudaFree(dev_distvec);
		////////////////////////
		cudaFree(dev_sita_n);
		cudaFree(dev_m_n);
		cudaFree(dev_L_n);
		cudaFree(dev_X_n);
		cudaFree(dev_Y_n);
		{
			//////////////////////////
			cudaFree(dev_kseed_sita_n) ;
			cudaFree(dev_kseed_m_n);
			cudaFree(dev_kseed_L_n);
			cudaFree(dev_kseed_X_n);
			cudaFree(dev_kseed_Y_n);
		}
	}
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	assert(cudaDeInit_CUI()==true);
}
/*------------------------------------------------------------------------------------------*/
/**
*cuda����ʵ�г������㷨��
*
*
*@param    alpha  �ǶȲ���alpha
*@param    betta  ģֵbetta
*@param    gama   ����gama
*@param    fai    XY����
*@param    kseedsSize  ���Ӹ���
*@param    m_width   ͼ����
*@param    m_height  ͼ��߶�

*@param	   klabels  ͼ��ͼ��ı궨ͼ��
*@param	   STEP     δʹ��
*@param	   edgemag  δʹ��
*@param	   M        δʹ���ܶ�����
*
*
*@note
*����GPUʵ�ֳ������㷨
*Сͼ1.8S  ��ͼ7.2S
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_ThetaMLXY_gpu_simplify(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,   
	const int m_height,
	double*     host_sita_n,
	double*     host_m_n,
	double*     host_L_n,
	double*     host_X_n,
	double*     host_Y_n,
	double*     host_kseedsTheta,
	double*     host_kseedsM,
	double*     host_kseedsL,
	double*     host_kseedsX,
	double*     host_kseedsY,
	int*		host_klabels,
	const int			STEP)
{
	cudaError_t cudaStatus;

	double*  dev_kseed_sita_n;
	double*  dev_kseed_m_n;
	double*  dev_kseed_L_n;
	double*  dev_kseed_X_n;
	double*  dev_kseed_Y_n;	
	double*	 dev_sita_n;
	double*	 dev_m_n;
	double*	 dev_L_n;
	double*	 dev_X_n;
	double*	 dev_Y_n;
	int*     dev_klabels;
	double*  dev_distvec;
	assert(cudaInit_CUI()==true);
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop,0);
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_klabels,m_width*m_height * sizeof(int));
		cudaStatus = cudaMalloc((void**)&dev_distvec, m_width*m_height* sizeof(double));

		cudaStatus = cudaMalloc((void**)&dev_kseed_sita_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_m_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_L_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_X_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_Y_n, kseedsSize* sizeof(double));

		cudaStatus = cudaMalloc((void**)&dev_sita_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_L_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_X_n, m_width*m_height* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_Y_n, m_width*m_height* sizeof(double));
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMemcpy(dev_klabels, host_klabels, m_width*m_height * sizeof(int), cudaMemcpyHostToDevice);
		
		cudaStatus = cudaMemcpy(dev_kseed_sita_n, host_kseedsTheta, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseed_m_n, host_kseedsM, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseed_L_n, host_kseedsL, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseed_X_n, host_kseedsX, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_kseed_Y_n, host_kseedsY, kseedsSize * sizeof(double), cudaMemcpyHostToDevice);

		cudaStatus = cudaMemcpy(dev_sita_n, host_sita_n, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_n, host_m_n,m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_L_n, host_L_n,m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_X_n, host_X_n,m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_Y_n, host_Y_n,m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
	cudaEvent_t start,stop;
	float costtime_ms=0;
	float costtime_us=0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	for( int itr = 0; itr <10; itr++ )
	{
		{
			cudaStatus=cudaEventRecord(start,0);
			SetMatrix_Efficient<<<16,16*16>>>(dev_distvec,m_width,m_height,1.7976931348623158e+308);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
		}
			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+threadsPerBlock.x-1)/threadsPerBlock.x,(m_height+threadsPerBlock.y-1)/threadsPerBlock.y);
#if 1
			SetMinimumOnThetaMLXY_9Seeds<<<numBlock,threadsPerBlock>>>(			
							kseedsSize,
							alpha,
							betta,
							gama,
							fai,
							m_width,
							m_height,			
							dev_sita_n,
							dev_m_n,
							dev_L_n,
							dev_X_n,
							dev_Y_n,				
							dev_kseed_sita_n,
							dev_kseed_m_n,
							dev_kseed_L_n,
							dev_kseed_X_n,
							dev_kseed_Y_n,
							dev_klabels,
							dev_distvec);

			cudaGetLastError_Sync_CUI();

			StatisticsThetaMLXY_Seed_collect_CPU(
							kseedsSize,
							m_width,
							m_height,
							host_sita_n,
							host_m_n,
							host_L_n,
							host_X_n,
							host_Y_n,	
							dev_klabels,
    						dev_kseed_sita_n,
							dev_kseed_m_n,
							dev_kseed_L_n,
							dev_kseed_X_n,
							dev_kseed_Y_n);
#endif
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);
			costtime_us=costtime_ms*1000;
}	
		cudaMemcpy(host_klabels,dev_klabels,m_width*m_height*sizeof(int),cudaMemcpyDeviceToHost);		
		cudaMemcpy(host_kseedsTheta,dev_kseed_sita_n,kseedsSize * sizeof(double),cudaMemcpyDeviceToHost);		
		cudaMemcpy(host_kseedsM,dev_kseed_m_n,kseedsSize * sizeof(double),cudaMemcpyDeviceToHost);		
		cudaMemcpy(host_kseedsL,dev_kseed_L_n,kseedsSize * sizeof(double),cudaMemcpyDeviceToHost);		
		cudaMemcpy(host_kseedsX,dev_kseed_X_n,kseedsSize * sizeof(double),cudaMemcpyDeviceToHost);		
		cudaMemcpy(host_kseedsY,dev_kseed_Y_n,kseedsSize * sizeof(double),cudaMemcpyDeviceToHost);

	    cudaFree(dev_sita_n);
		cudaFree(dev_m_n);
		cudaFree(dev_L_n);
		cudaFree(dev_X_n);
		cudaFree(dev_Y_n);
		cudaFree(dev_kseed_sita_n) ;
		cudaFree(dev_kseed_m_n);
		cudaFree(dev_kseed_L_n);
		cudaFree(dev_kseed_X_n);
		cudaFree(dev_kseed_Y_n);
		cudaFree(dev_distvec);
		cudaFree(dev_klabels);
	assert(cudaDeInit_CUI()==true);
}
/*------------------------------------------------------------------------------------------*/
/**
*���������˺���
*
*@param		[in,out]	ubuff  ͼ������
*@param		[in]		labels ͼ��labels
*@param					width  ͼ����
*@param					height ͼ��߶�
*@param					color_in   ��ɫ�ڱ߽���ɫ
*@param					color_out  ��ɫ��߽���ɫ
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void DrawContoursAroundSegments_thread(
	unsigned int*			ubuff,
	int*					labels,
	const int				width,
	const int				height,
	const unsigned int		color_in,
	const unsigned int      color_out) 
{
	int k =blockIdx.x*blockDim.x+threadIdx.x;
	int j =blockIdx.y*blockDim.y+threadIdx.y;
	int mainindex = j*width + k;
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	////////////////////////////////////////////////
		int np(0);
		if (k>=0&&k<width&&j>=0&&j<height){
				for(register int i = 0; i < 8; i++ ){
					int x = k + dx8[i];
					int y = j + dy8[i];
					if( (x >= 0 && x < width) && (y >= 0 && y < height) ){
								int index = y*width + x;
								//if( false == istaken[index] )//comment this to obtain internal contours
								{
									if( labels[mainindex] != labels[index] ){
										np++;
										//ubuff[index] =0xff000000; //2pixel����
										ubuff[index] =color_out; //2pixel����
									}
								}
					}
				}
				if( np > 1 ){
						ubuff[mainindex] =color_in;//2pixel����
				}
		}		
}
/*------------------------------------------------------------------------------------------*/
/**
*����GPU��������������ͼ����
*@param	  [in,out] ubuff     ͼ��
*@param	  [in] labels    labels����
*@param	   width     ͼ����
*@param	   height    ͼ��߶�
*@param	   color_in  �����ڱ߽���ɫ
*@param    color_out ������߽���ɫ
*
*/
/*------------------------------------------------------------------------------------------*/
void DrawContoursAroundSegments_gpu(
	unsigned int*			ubuff,
	int*					labels,
	const int				width,
	const int				height,
	const unsigned int	color_in,
	const unsigned int  color_out)
{
	unsigned int* ubuff_dev;
	int*  labels_dev;
	cudaError_t cudaStatus;
	assert(cudaInit_CUI()==true);
	cudaStream_t stream;//���Ľṹ��

	cudaStatus=cudaStreamCreate(&stream);	
	cudaStatus=cudaMalloc((void**)&ubuff_dev, width*height*sizeof(unsigned int));
	cudaStatus=cudaMalloc((void**)&labels_dev,width*height*sizeof(int));

	cudaStatus = cudaMemcpy(ubuff_dev,ubuff, width*height*sizeof(unsigned int), cudaMemcpyHostToDevice);
	cudaStatus = cudaMemcpy(labels_dev,labels,width*height*sizeof(int), cudaMemcpyHostToDevice);
	/////////////////////////////////////////////////////////////////////////////////////////
	dim3    threadsPerBlock(16,16);
	dim3    numBlock((width+threadsPerBlock.x-1)/threadsPerBlock.x,(height+threadsPerBlock.y-1)/threadsPerBlock.y);
	DrawContoursAroundSegments_thread<<<numBlock,threadsPerBlock>>>(ubuff_dev,
											labels_dev,
											width,
											height,
											color_in,
											color_out);
	cudaGetLastError_Sync_CUI();
	cudaMemcpy(ubuff,ubuff_dev,width*height*sizeof(unsigned int),cudaMemcpyDeviceToHost);

	cudaStatus=cudaStreamDestroy(stream);
	cudaFree(ubuff_dev);
	cudaFree(labels_dev);
	/////////////////////////////////////////////////////////////////////////////////////////
	assert(cudaDeInit_CUI()==true);
}
/*------------------------------------------------------------------------------------------*/
/**
*Normal  0x00 00 00 00 
*MASK    0xff ff ff ff
*���ó����ص���������\n
*���ε�����
*@param
*@param	[out]	dev_mask      �����ε�����
*@param	[in]	dev_labels    labels����
*@param		width         ͼ����
*@param		height        ͼ��߶�
*@param     Pixel         ������
*/
/*------------------------------------------------------------------------------------------*/
void InitMaskTemplate(
	unsigned int*			dev_mask,
	int*					dev_labels,
	const int				width,
	const int				height,
	const int                Pixel)
{
	int color_in;
	int color_out;
	if (Pixel==2){
		color_in=0xffffffff;
		color_out=0x00000000;
	}else if(Pixel==4){		
		color_in=0xffffffff;
		color_out=0xffffffff;
	}
	
	cudaMemset(dev_mask,0,sizeof(unsigned int)*width*height);
	dim3    threadsPerBlock(16,16);
	dim3    numBlock((width+threadsPerBlock.x-1)/threadsPerBlock.x,(height+threadsPerBlock.y-1)/threadsPerBlock.y);
	DrawContoursAroundSegments_thread<<<numBlock,threadsPerBlock>>>(dev_mask,
		dev_labels,
		width,
		height,
		color_in,
		color_out);
	cudaGetLastError_Sync_CUI();
}
/*------------------------------------------------------------------------------------------*/
/**
*
*����GPU������ȡ����
*.cu���ص���
*
*@param			 kseedsSize   ���Ӹ���
*@param			 m_width      ͼ����
*@param			 m_height     ͼ��߶�
*@param 	[in]	 host_m_lvec  ����L����
*@param 	[in]	 host_m_avec  ����A����
*@param 	[in]	 host_m_bvec  ����B����
*@param 	[out]	 host_kseedsl  ����L����
*@param 	[out]	 host_kseedsa  ����A����
*@param 	[out]	 host_kseedsb  ����B����
*@param 	[out]	 host_kseedsx  ����X����
*@param 	[out]	 host_kseedsy  ����Y����
*@param 	[in]	 host_klabels  ͼ��ͼ��ı궨ͼ��
*
*/
/*------------------------------------------------------------------------------------------*/
void  GetSeedsLabxy_gpu(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				host_m_lvec,
	double*				host_m_avec,
	double*				host_m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels)
{
	cudaEvent_t start,stop;
	float costtime_ms=0;
	float costtime_us=0;
	cudaError_t cudaStatus;
	unsigned int*       mask_img;
	double*				dev_kseedsl;
	double*				dev_kseedsa;
	double*				dev_kseedsb;
	double*				dev_kseedsx;
	double*				dev_kseedsy;
	int*				dev_klabels;
	double*			dev_m_lvec;
	double*			dev_m_avec;
	double*			dev_m_bvec;
	{
		cudaEventCreate(&start);
		cudaEventCreate(&stop);
		cudaStatus=cudaMalloc((void**)&mask_img,sizeof(unsigned int)*m_width*m_height);
		cudaStatus = cudaMalloc((void**)&dev_kseedsl, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsb, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsx, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseedsy, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_klabels,m_width*m_height * sizeof(int));
		cudaStatus = cudaMalloc((void**)&dev_m_lvec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_avec,m_width*m_height * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_m_bvec,m_width*m_height * sizeof(double));
#if 1
		cudaStatus = cudaMemset(dev_kseedsl,0, kseedsSize * sizeof(double));
		cudaStatus = cudaMemset(dev_kseedsa,0, kseedsSize * sizeof(double));
		cudaStatus = cudaMemset(dev_kseedsb,0, kseedsSize * sizeof(double));
		cudaStatus = cudaMemset(dev_kseedsx,0, kseedsSize * sizeof(double));
		cudaStatus = cudaMemset(dev_kseedsy,0, kseedsSize * sizeof(double));
#endif
		cudaStatus = cudaMemcpy(dev_klabels, host_klabels, m_width*m_height * sizeof(int), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_lvec, host_m_lvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_avec, host_m_avec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
		cudaStatus = cudaMemcpy(dev_m_bvec, host_m_bvec, m_width*m_height * sizeof(double), cudaMemcpyHostToDevice);
	}
	
	double*			dev_sigmal;
	double*			dev_sigmaa;
	double*			dev_sigmab;
	double*			dev_sigmax;
	double*			dev_sigmay;
	double*			dev_clustersize;
	double*				dev_kseed_sita_n;
	double*				dev_kseed_m_n;
	double*				dev_kseed_L_n;
	double*				dev_kseed_X_n;
	double*			    dev_kseed_Y_n;
	{
		cudaStatus = cudaMalloc((void**)&dev_sigmal, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmaa, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmab, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmax, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_sigmay, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_clustersize, kseedsSize * sizeof(double));
		cudaStatus=cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));
		cudaGetLastError_Sync_CUI();
		////////////////////////////////////////////////////////////////////////////////
		cudaStatus = cudaMalloc((void**)&dev_kseed_sita_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_m_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_L_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_X_n, kseedsSize * sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_Y_n, kseedsSize * sizeof(double));
	}
		/*----------------------------------------------------------------------------------*/
			cudaStatus =cudaMemset(dev_sigmal,0,kseedsSize*sizeof(double));			
			cudaStatus =cudaMemset(dev_sigmaa,0,kseedsSize*sizeof(double));			
			cudaStatus =cudaMemset(dev_sigmab,0,kseedsSize*sizeof(double));			
			cudaStatus =cudaMemset(dev_sigmax,0,kseedsSize*sizeof(double));			
			cudaStatus =cudaMemset(dev_sigmay,0,kseedsSize*sizeof(double));			
			cudaStatus =cudaMemset(dev_clustersize,0,kseedsSize*sizeof(double));			
		/*----------------------------------------------------------------------------------*/

			InitMaskTemplate(mask_img,dev_klabels,m_width,m_height,4);

			dim3    threadsPerBlock(16,16);
			dim3    numBlock((m_width+15)/threadsPerBlock.x,(m_height+15)/threadsPerBlock.y);
			cudaStatus=cudaEventRecord(start,0);
			cudaGetLastError_Sync_CUI();
#if 0
			StatisticsLABXY_Seed_collect_gpu_search_AllImg<<<(kseedsSize+511)/512,512>>>(
				kseedsSize,
				m_width,
				m_height,
				dev_m_lvec,
				dev_m_avec,
				dev_m_bvec,
				dev_klabels,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
#else
			StatisticsLABXY_Seed_collect_CPU(
				kseedsSize,
				m_width,
				m_height,
				host_m_lvec,
				host_m_avec,
				host_m_bvec,
				dev_klabels,
				mask_img,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize);
#endif
			
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;			
			cudaStatus=cudaEventRecord(start,0);
			cudaGetLastError_Sync_CUI();
			StatisticsLABXY_Seed_calculate<<<1,512>>>(
				kseedsSize,
				m_width,
				m_height,
				dev_sigmal,
				dev_sigmaa,
				dev_sigmab,
				dev_sigmax,
				dev_sigmay,
				dev_clustersize,
				dev_kseedsl,
				dev_kseedsa,
				dev_kseedsb,
				dev_kseedsx,
				dev_kseedsy,
				dev_kseed_sita_n,
				dev_kseed_m_n,
				dev_kseed_L_n,
				dev_kseed_X_n,
				dev_kseed_Y_n);
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventRecord(stop,0);	
			cudaGetLastError_Sync_CUI();
			cudaStatus=cudaEventElapsedTime(&costtime_ms,start,stop);	
			costtime_us=costtime_ms*1000;
#if 1
		////////////////////////////////////////////////////////////////////////////////////////
		cudaStatus=cudaMemcpy(host_klabels,dev_klabels,m_width*m_height*sizeof(int),cudaMemcpyDeviceToHost);	
		cudaStatus=cudaMemcpy(host_kseedsl,dev_kseedsl, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
		cudaStatus=cudaMemcpy(host_kseedsa,dev_kseedsa, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
		cudaStatus=cudaMemcpy(host_kseedsb,dev_kseedsb, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
		cudaStatus=cudaMemcpy(host_kseedsx,dev_kseedsx, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);	
		cudaStatus=cudaMemcpy(host_kseedsy,dev_kseedsy, kseedsSize*sizeof(double),cudaMemcpyDeviceToHost);
#endif
		{
			cudaEventDestroy(start);
			cudaEventDestroy(stop);
			cudaFree(mask_img);
			cudaFree(dev_kseedsl);
			cudaFree(dev_kseedsa);
			cudaFree(dev_kseedsb);
			cudaFree(dev_kseedsx);
			cudaFree(dev_kseedsy);
			cudaFree(dev_klabels);
			cudaFree(dev_m_lvec);
			cudaFree(dev_m_avec);
			cudaFree(dev_m_bvec);
		/*	cudaFree(dev_distvec);*/
			////////////////////////
			/*cudaFree(dev_sita_n);
			cudaFree(dev_m_n);
			cudaFree(dev_L_n);
			cudaFree(dev_X_n);
			cudaFree(dev_Y_n);*/
			{
				cudaFree(dev_sigmal);
				cudaFree(dev_sigmaa);
				cudaFree(dev_sigmab);
				cudaFree(dev_sigmax);
				cudaFree(dev_sigmay);
				cudaFree(dev_clustersize);
				//////////////////////////
				cudaFree(dev_kseed_sita_n) ;
				cudaFree(dev_kseed_m_n);
				cudaFree(dev_kseed_L_n);
				cudaFree(dev_kseed_X_n);
				cudaFree(dev_kseed_Y_n);
			}
		}
}
/*------------------------------------------------------------------------------------------*/
/**
*
*����GPU������ȡ����
*.cu���ص���
*
*@param			 kseedsSize   ���Ӹ���
*@param			 m_width      ͼ����
*@param			 m_height     ͼ��߶�
*@param 	[in]	 host_m_lvec  ����L����
*@param 	[in]	 host_m_avec  ����A����
*@param 	[in]	 host_m_bvec  ����B����
*@param 	[out]	 host_kseedsl  ����L����
*@param 	[out]	 host_kseedsa  ����A����
*@param 	[out]	 host_kseedsb  ����B����
*@param 	[out]	 host_kseedsx  ����X����
*@param 	[out]	 host_kseedsy  ����Y����
*@param 	[in]	 host_klabels  ͼ��ͼ��ı궨ͼ��
*
*/
/*------------------------------------------------------------------------------------------*/
void  GetSeedsThetaMLXY_gpu(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*     host_sita_n,
	double*     host_m_n,
	double*     host_L_n,
	double*     host_X_n,
	double*     host_Y_n,
	double*     host_kseedsTheta,
	double*     host_kseedsM,
	double*     host_kseedsL,
	double*     host_kseedsX,
	double*     host_kseedsY,
	int*		host_klabels)
{
	/*		cudaStatus = cudaMalloc((void**)&dev_kseed_sita_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_m_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_L_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_X_n, kseedsSize* sizeof(double));
		cudaStatus = cudaMalloc((void**)&dev_kseed_Y_n, kseedsSize* sizeof(double));*/

}
/*------------------------------------------------------------------------------------------*/
/**
*����ͼ������Ӿ���\n
*.cu���ص���
*
*@param	[in]	host_labels      labels����
*@param		    NumLabels       label����
*@param	[out]	host_Matrix_E   ���Ӿ���
*@param		    width         ͼ����
*@param		    height        ͼ��߶�
*
*/
/*------------------------------------------------------------------------------------------*/
__global__ void Nighbour_E_matrix_thread(
	int*			labels,
	const  int		NumLabels,
	UINT32 *		Matrix_E,
	const int		width,
	const int		height)
{
#if 0
int k =blockIdx.x*blockDim.x+threadIdx.x;
	int j =blockIdx.y*blockDim.y+threadIdx.y;
	int mainindex = j*width + k;
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	////////////////////////////////////////////////
	int cind(0);
//	static int DoCount=0;
	for (register int ii=0;ii<NumLabels;ii++) {
		Matrix_E[ii*NumLabels+ii]=0;
	}

	int np(0);
	for(register  int i = 0; i < 8; i++ ){
		int x = k + dx8[i];
		int y = j + dy8[i];
		if( (x >= 0 && x < width) && (y >= 0 && y <height) ){
			int index = y*width + x;
			if( labels[mainindex] != labels[index] ){
				UINT32  matrix_x=labels[index];
				UINT32  matrix_y=labels[mainindex];
				Matrix_E[matrix_x*NumLabels+matrix_y]=++Matrix_E[matrix_y*NumLabels+matrix_x];
				if (Matrix_E[matrix_x*NumLabels+matrix_y]>999*2){
					Matrix_E[matrix_x*NumLabels+matrix_y]=Matrix_E[matrix_y*NumLabels+matrix_x]=999*2;
				}
			}
		}
	}
	mainindex++;
	for(register  int i = 0; i <NumLabels; i++ ){
		for(register  int j = 0; j <NumLabels; j++ ){			
			Matrix_E[i*NumLabels+j]=Matrix_E[i*NumLabels+j]>0?1:0;
		}
	}
	{
		double  threshold;		//���㳬�������
		threshold=4*sqrt((float)width*height/NumLabels);	//�ܳ�
		threshold=threshold/8; //8��֮һ�ܳ�

		for(register  int i = 0; i <NumLabels; i++ ){
			for(register  int j = 0; j <NumLabels; j++ ){
				if (Matrix_E[i*NumLabels+j]>0){
					Matrix_E[i*NumLabels+j]=1;     
				}else{
					Matrix_E[i*NumLabels+j]=0;    
				}
			}
		}
	}
#endif
	int k =blockIdx.x*blockDim.x+threadIdx.x;
	int j =blockIdx.y*blockDim.y+threadIdx.y;
	int mainindex = j*width + k;
	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
	/*********************************************************************************************/
	if( (k >= 0 && k < width) && (j >= 0 && j <height) ){
			for(register  int i = 0; i < 8; i++ ){
					int x = k + dx8[i];
					int y = j + dy8[i];
					if( (x >= 0 && x < width) && (y >= 0 && y <height) ){
						int index = y*width + x;
						if( labels[mainindex] != labels[index] ){
							UINT32  matrix_x=labels[index];
							UINT32  matrix_y=labels[mainindex];
							Matrix_E[matrix_x*NumLabels+matrix_y]=Matrix_E[matrix_y*NumLabels+matrix_x]=1;
							//if (matrix_x==matrix_y) assert(Matrix_E[matrix_x*NumLabels+matrix_y]==0);
						}
					}
				}
	}
	/*********************************************************************************************/
}
/*------------------------------------------------------------------------------------------*/
/**
*����ͼ������Ӿ���\n
*C��������
*
*@param	[in]	host_labels      labels����
*@param		    NumLabels       label����
*@param	[out]	host_Matrix_E   ���Ӿ���
*@param		    width         ͼ����
*@param		    height        ͼ��߶�
*
*/
/*------------------------------------------------------------------------------------------*/
void Get_Nighbour_E_matrix_gpu(
	int*			host_labels,
	const  int		NumLabels,
	UINT32 *		host_Matrix_E,
	const int		width,
	const int		height)
{
	/*---------------------------*/
	cudaError_t cudaStatus;
	int*			dev_labels;
	UINT32 *		dev_Matrix_E;
	/*---------------------------*/
	cudaStatus = cudaMalloc((void**)&dev_labels, width*height * sizeof(int));
	cudaStatus = cudaMalloc((void**)&dev_Matrix_E, NumLabels*NumLabels* sizeof(UINT32));
	cudaStatus=cudaMemcpy(dev_labels,host_labels,width*height*sizeof(int),cudaMemcpyHostToDevice);
	cudaStatus=cudaMemcpy(dev_Matrix_E,host_Matrix_E,NumLabels*NumLabels*sizeof(UINT32),cudaMemcpyHostToDevice);
	/*--------------------------------------------*/
	dim3    threadsPerBlock(16,16);
	dim3    numBlock((width+threadsPerBlock.x-1)/threadsPerBlock.x,(height+threadsPerBlock.y-1)/threadsPerBlock.y);
	 Nighbour_E_matrix_thread<<<numBlock,threadsPerBlock>>>(dev_labels,
		NumLabels,
		dev_Matrix_E,
		width,
		height);
	cudaGetLastError_Sync_CUI();
	/*--------------------------------------------*/
	cudaStatus=cudaMemcpy(host_Matrix_E,dev_Matrix_E,NumLabels*NumLabels*sizeof(UINT32),cudaMemcpyDeviceToHost);
	cudaFree(dev_labels);
	cudaFree(dev_Matrix_E);
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
int GetThreadIdSelfwinlinux(void)
{
		
		  int thread_id=0;
#if _DEBUG
		  int device_count;
		  cudaGetDeviceCount(&device_count);
#endif

		
#if _MSC_VER
		  thread_id =::GetCurrentThreadId();
#elif linux||__linux||__linux__||__GNUC__
		  //thread_id=(struct pthread_fake *)pthread_self()->tid;
		  thread_id=syscall( __NR_gettid );
#else
		  thread_id=-1;
		  assert(0);
#endif

	return thread_id;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
//int GetCudaNum(){
//
//}

/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/