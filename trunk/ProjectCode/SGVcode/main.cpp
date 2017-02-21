#include "stdafx.h"
#include "module_all_cui.h"
/*----------------------------------------------*/
/*
C++:编写跨平台程序的关键，C/C++中的内置宏定义
分两部分：

操作系统判定：

Windows:   WIN32

Linux:   linux

Solaris:   __sun

编译器判定：

VC:  _MSC_VER

GCC/G++:   __GNUC__

SunCC:   __SUNPRO_C和__SUNPRO_CC
*/
/*----------------------------------------------*/
void PrintARG(int argc,char *argv[])
{
  for(int i=0;i<argc;i++){
	  printf("###########################################\n");
	  printf("Param: %d>>%s \n",i,argv[i]);
	  printf("###########################################\n");
  }
}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/
void PrintHelp(){

}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/
void TestOpenCV()
{
#if _DEBUG
	{
		IplImage *img=cvCreateImage(cvSize(100,100),IPL_DEPTH_8U,4);
		cvReleaseImage(&img);
	}
#endif
}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/
int main(int argc,char *argv[])
{
	TestOpenCV();
	PrintARG(argc,argv);

  vector<string> file;
  string out;
  if (argc==1)
  {


  }else if (argc>1){
	  for (int i=1;i<argc;i++){

		  if ( 
#if linux||__linux||__linux__||__GNUC__			  
			  !access(argv[i], F_OK)
#endif	
#if _WIN64 ||_WIN32 ||_MSC_VER
			  access(argv[i], 0) == 0			
#endif
			  ){
			  printf("this is a file /n");
			  file.push_back(argv[i]);
		  }else if (strcmp(argv[i],"-Save")==0){
			  cui_GeneralImgProcess::SAVE_IMAGE_2DISK=TRUE;
		  }else if(strcmp(argv[i],"-NotSave")==0){
		      cui_GeneralImgProcess::SAVE_IMAGE_2DISK=FALSE;
			  printf("Will not save Image \n");
		  }else if (strcmp(argv[i],"-DebugInfo")==0){
			  cui_GeneralImgProcess::SAVE_DEBUG_2DISK=TRUE;
		  }else if(strcmp(argv[i],"-NotDebugInfo")==0){
			  cui_GeneralImgProcess::SAVE_DEBUG_2DISK=FALSE;
			  printf("Will not save debug info \n");
		  }
		  else{
				printf("not support cmd   /n");
		  }
			

			
		
		
     }
  }else{

  }
#if _WIN64 ||_WIN32 ||_MSC_VER
  if (file.size()==0){
	file.push_back("D:\\ImageDataBase\\400img\\img-op39-p-015t000.jpg");
	//file.push_back("D:\\ImageDataBase\\400img\\img-10.21op7-p-046t000.jpg");
}
if (out.empty())
{
    out="E:\\OutPutImg\\";
}
#endif 
#if linux||__linux||__linux__||__GNUC__
if (file.size()==0){
   file.push_back("/home/blacksherry/400/400img/img-op39-p-015t000.jpg");
   file.push_back("/home/blacksherry/400/400img/img-10.21op7-p-046t000.jpg");
}
if (out.empty()){
   out="/home/blacksherry/400/400out/";
}
#endif

    
	cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(0,file,out,1000);


	printf("Done ! \n");
	return 0;

}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/