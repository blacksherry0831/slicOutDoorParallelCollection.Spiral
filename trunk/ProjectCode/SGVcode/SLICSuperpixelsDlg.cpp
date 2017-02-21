
// SLICSuperpixelsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modules.h"
#include "SLICSuperpixels.h"
#include "SLICSuperpixelsDlg.h"
//#include "PictureHandler.h"







// CSLICSuperpixelsDlg dialog
vector<string>CSLICSuperpixelsDlg:: picvec(0);
int CSLICSuperpixelsDlg:: m_spcount;
double  CSLICSuperpixelsDlg::m_compactness;
string  CSLICSuperpixelsDlg::saveLocation;
CButton CSLICSuperpixelsDlg:: cui_Button;


CSLICSuperpixelsDlg::CSLICSuperpixelsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSLICSuperpixelsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSLICSuperpixelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPCOUNT, m_spcount);
	DDV_MinMaxInt(pDX, m_spcount, 1, 10000000);
	DDX_Text(pDX, IDC_EDIT_COMPACTNESS, m_compactness);
	DDV_MinMaxDouble(pDX, m_compactness, 1.0, 80.0);
	DDX_Control(pDX, IDC_BUTTON_CREATESUPERPIXELS, cui_Button);
}

BEGIN_MESSAGE_MAP(CSLICSuperpixelsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CREATESUPERPIXELS, &CSLICSuperpixelsDlg::OnBnClickedButtonCreatesuperpixels)
	ON_EN_CHANGE(IDC_EDIT_SPCOUNT, &CSLICSuperpixelsDlg::OnEnChangeEditSpcount)
	ON_EN_CHANGE(IDC_EDIT_COMPACTNESS, &CSLICSuperpixelsDlg::OnEnChangeEditCompactness)
	ON_BN_CLICKED(IDOK, &CSLICSuperpixelsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSLICSuperpixelsDlg message handlers

BOOL CSLICSuperpixelsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_spcount =1000;
	m_compactness = 8.0;
	UpdateData(FALSE);//FALSE to set data, TRUE to retrieve data

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSLICSuperpixelsDlg::OnEnChangeEditSpcount()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);//FALSE to set data, TRUE to retrieve data
}

void CSLICSuperpixelsDlg::OnEnChangeEditCompactness()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);//FALSE to set data, TRUE to retrieve data
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSLICSuperpixelsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSLICSuperpixelsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//=================================================================================
///	GetPictures
///
///	This function collects all the pictures the user chooses into a vector.
//=================================================================================
void CSLICSuperpixelsDlg::GetPictures(vector<string>& picvec)
{
#if TRUE
	//AfxOleInit();
	//	::CoInitialize(NULL);
	CFileDialog cfd(TRUE, L"*.*", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST,
		L"All Files(*.*)|*.*||", NULL);
	//cfd.m_ofn.lpstrInitialDir=L"C:\\";
#if TRUE
	/*CFileDialog cfd(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,L"*.*|*.*|",AfxGetMainWnd(),0,false);
	cfd.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	cfd.m_ofn.lpstrTitle = _T("Load the CVLGroup object");*/
#else
	CFileDialog cfd(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"All Files(*.*)|*.*||",AfxGetMainWnd(),0,false);
	//CFileDialog cfd(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"All Files(*.*)|*.*||",AfxGetMainWnd());
	cfd.m_ofn.lpstrTitle = _T("Load the CVLGroup object");
#endif
	/*cfd.bVistaStyle=FALSE;*/

	//cfd.PostMessage(WM_COMMAND, 40964, NULL);

	CString strFileNames;
#if 0
	#define MAX_BUFFER (1000 * 1024)
	cfd.m_ofn.lpstrFile = new TCHAR[MAX_BUFFER];
	cfd.m_ofn.lpstrFile[0] = 0;
	cfd.m_ofn.nMaxFile = MAX_BUFFER;
#else
	cfd.m_ofn.lpstrFile = strFileNames.GetBuffer(1024*1024);
	cfd.m_ofn.nMaxFile = 1024*1024;
#endif
	
	
	Sleep(500);
	BOOL bResult;
	if (cfd.DoModal()==IDOK){
		bResult=TRUE;
	}else{
		bResult=FALSE;
	}

	strFileNames.ReleaseBuffer();
	//	   ::CoUninitialize();
	//if(cfd.DoModal() == IDOK)
	if( bResult )
	{
		POSITION pos = cfd.GetStartPosition();
		while (pos) 
		{
			CString imgFile = cfd.GetNextPathName(pos);			
			string name = SLIC::Wide2Narrow(imgFile.GetString());
			picvec.push_back(name);
		}
	}
	else return;
#endif
}

//===========================================================================
///	BrowseForFolder
///
///	The main function
//===========================================================================
bool CSLICSuperpixelsDlg::BrowseForFolder(string& folderpath)
{
	IMalloc* pMalloc = 0;
	if(::SHGetMalloc(&pMalloc) != NOERROR)
	return false;

	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = L"Please select a folder and press 'OK'.";

	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
	if(pIDL == NULL)
	return false;

	TCHAR buffer[_MAX_PATH];
	if(::SHGetPathFromIDList(pIDL, buffer) == 0)
	return false;
//	PictureHandler pichand;
	folderpath = SLIC::Wide2Narrow(buffer);
	folderpath.append("\\");
	return true;
}

//===========================================================================
///	DoSupervoxelVideoSegmentation
///
/// Supervoxel segmentation demo
//===========================================================================
void CSLICSuperpixelsDlg::DoSupervoxelVideoSegmentation()
{
	const int width	= 7;
    const int height = 7;
    const int depth = 7;
    const int sz = (width*height);

    int supervoxelsize = 8;//size in pixels (2x2x2)
    double compactness = 10.0;
    int numlabels = 0;
    int** klabels = new int*[depth];
    unsigned int** ubuff = new unsigned int *[depth];
    for(int i=0; i < depth; i++)
    {
            ubuff[i] = new unsigned int[sz];
            klabels[i] = new int[sz];

            for (int k=0; k < sz; k++)
                    ubuff[i][k] = (unsigned int)rand();//normally this would be 2d slice pixel data
    }
    SLIC slic;
    slic.DoSupervoxelSegmentation( ubuff, width, height, depth, klabels, numlabels, supervoxelsize, compactness );

	for(int i=0; i < depth; i++)
	{
		delete [] klabels[i];
		delete [] ubuff[i];
	}
	delete [] klabels; delete [] ubuff;
}

void CSLICSuperpixelsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//CDialog::OnOK();
}
//===========================================================================
///	OnBnClickedButtonCreatesuperpixels
///
///	The main function
//===========================================================================
void CSLICSuperpixelsDlg::OnBnClickedButtonCreatesuperpixels()
{
		
	//-------------------------------------------------------------------
	//DoSupervoxelVideoSegmentation();AfxMessageBox(L"Done!", 0, 0);return;
	//Uncomment the line for a Supervoxel demo
	//------------------------------------------------- ------------------
	// TODO: 在此添加控件通知处理程序代码
#if 0
	int num_devices=gpu::getCudaEnabledDeviceCount();
	assert(num_devices>0);
	int enable_device_id=-1;
	for(int i=0;i<num_devices;i++){
		cv::gpu::DeviceInfo dev_info(i);
		if(dev_info.isCompatible()){
			enable_device_id=i;
			string dev_name=dev_info.name();
			int proCount=dev_info.multiProcessorCount();
		}
	}
	gpu::setDevice(enable_device_id);
	gpu::resetDevice();
#endif
#if 1
//	malloc(100);
			GetPictures(picvec);//user chooses one or more pictures
			BrowseForFolder(saveLocation);
#else
			picvec.push_back("C:\\Users\\Administrator\\Desktop\\img\\1024_768.jpg");
			saveLocation="C:\\Users\\Administrator\\Desktop\\";
#endif
	

#if TRUE
	AfxBeginThread(THreadSuperPixel_CUDA_CollectionMethods,NULL);
#endif
#if FALSE
	AfxBeginThread(THreadSuperPixel_400withLabColor,NULL);
#endif
#if FALSE
	AfxBeginThread(Do400OutDoorIMG,NULL);
#endif
	HANDLE hHeaps[40];
	DWORD    dwHeapNum = GetProcessHeaps(40, hHeaps);
	TRACE("堆总数：GetProcessHeaps()==%d\n",dwHeapNum);
}
/*---------------------------------------------------------------------------*/
/**
*<pre>
*    采用角度*Alpha+LAB向量在AB平面上的投影*Betta+亮度*Gama 计算距离，混合CUDA并行计算
*的方法。
*</pre>
*模糊函数的并行计算未使用，基本功能已完成
*
*@image html image027.png "色彩公式"
*@image html image029.png "距离公式"
*@image html image031.png "距离综合公式"
*@return 返回值无意义
*
*/
/*---------------------------------------------------------------------------*/
UINT CSLICSuperpixelsDlg::THreadSuperPixel_CUDA(LPVOID lpParam)
{

	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	cui_Button.EnableWindow(FALSE);
	/****************************************/
	//float datafai[]={0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
	//int datasize=sizeof(datafai)/sizeof(float);

#if 1
//for (int i=0;i<sizeof(datafai)/sizeof(float);i++)
#else
int i=6;
#endif	
	{
		for(int k = 0; k < numPics; k++ ){
			LARGE_INTEGER litmp;
			LONGLONG QPart1,QPart2;
			double dfMinus, dfFreq, dfTim;
			QueryPerformanceFrequency(&litmp);
			dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
			QueryPerformanceCounter(&litmp);
			QPart1 = litmp.QuadPart;// 获得初始值		
													
									{
										ImageMemData MemData(picvec[k],saveLocation,m_spcount,m_compactness,0.5);
										SLIC slic(&MemData);
										/*slic.alpha=datafai[i];
										slic.gama=(1-datafai[i])/2;
										slic.betta=(1-datafai[i])/2;*/
										/*对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像*/
#if 1
										slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy(m_spcount);//得到lable
										{											
											MemData.AllocateMemRelated2spnumber();
											slic.ExportMemData();
											slic.Clustering_New();
											slic.ExportMemData();
										}
#else
									
										/*获得图像数据*/
										slic.CuiGetImageData(picvec[k],saveLocation);
										/*完成超像素算法*/
										slic.CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(m_spcount,m_compactness);//得到lable
										/*为以后的算法分配公共内存*/
										MemData.AllocateMemRelated2spnumber(MemData.slic_current_num);
										/*完成谱聚类算法*/
										while(slic.Cui_Spectral_Clustering_B()){

										}
										slic.ExportMemData();
#endif		
									
										ComputeSVG computesvg(&MemData);
										{
										LARGE_INTEGER litmp;
										LONGLONG QPart1,QPart2;
										double dfMinus, dfFreq, dfTim;
										QueryPerformanceFrequency(&litmp);
										dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
										QueryPerformanceCounter(&litmp);
										QPart1 = litmp.QuadPart;// 获得初始值		
#if 1				
										
										computesvg.GetImgData(
											MemData.Src_ImgData,
											MemData.ImgWidth,
											MemData.ImgHeight,
											MemData.ImgLables.get(),
											MemData.slic_current_num);
										/*代表天空、地面及立面物体的重力场模糊分布密度函数分别与这些大图块像素相乘，并求出这些大图块的期望值，从而产生天空、地面及
										立面物体的初步分类，通过进一步的单层小波采样、曼哈顿方向提取等特性分类方法，提取出较为准确的天空、地面和立面物体分类图*/
										computesvg.separateSVG_Zlm();
#endif
										QueryPerformanceCounter(&litmp);
										QPart2 = litmp.QuadPart;//获得中止值
										dfMinus = (double)(QPart2-QPart1);
										dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
										TRACE("\n 模糊函数: %f（秒）",dfTim);
										//模糊函数: 5.557960（秒）线程 'Win32 线程' (0x1924) 已退出，返回值为 0 (0x0)。
										}
										CuiHough3  hough(&MemData);
										{
										LARGE_INTEGER litmp;
										LONGLONG QPart1,QPart2;
										double dfMinus, dfFreq, dfTim;
										QueryPerformanceFrequency(&litmp);
										dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
										QueryPerformanceCounter(&litmp);
										QPart1 = litmp.QuadPart;// 获得初始值		
#if TRUE
										
										hough.GetImageData(computesvg.cui_ImgData,computesvg.cui_Width,computesvg.cui_Height,
											computesvg.cui_ImgLables,computesvg.cui_ImgLables_SVG,computesvg.cui_NumLabels,
											computesvg.cui_Matrix_Category_Lable);
										hough.ImageEenvironment=BT_unknow;
										hough.ClassifyGV();
										/*最后，基于小孔成像模型及地面线性透视信息生成空间深度感知图*/
										Cui_DrawDepth  drawdepth(&MemData);

										drawdepth.GetImageData(hough.cui_ImgData,hough.cui_Width,hough.cui_Height,hough.cui_ImgLables_SVG);

										drawdepth.SetCameraParameter(1,0.8,1,HORIZONTAL_LINE_POS);
										drawdepth.DrawDepth();


#endif										
										QueryPerformanceCounter(&litmp);
										QPart2 = litmp.QuadPart;//获得中止值
										dfMinus = (double)(QPart2-QPart1);
										dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
										TRACE("\n 绘制深度图 : %f（秒）",dfTim);

										}
									}
								QueryPerformanceCounter(&litmp);
								QPart2 = litmp.QuadPart;//获得中止值
								dfMinus = (double)(QPart2-QPart1);
								dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
								TRACE("\n 超像素聚类完成天空地面立面分割总时间 : %f（秒）",dfTim);
				
							}

	}
	/****************************************/
	cui_Button.EnableWindow(TRUE);
	return AfxMessageBox(L"Done!", 0, 0);
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/
UINT CSLICSuperpixelsDlg::Do400OutDoorIMG(LPVOID lpParam)
{
	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	cui_Button.EnableWindow(FALSE);
	/****************************************/
	for( int k = 0; k < numPics; k++ ){
		/*获得水平线位置*/

		float HorizontalLine=0.5; 
#if (TRUE)				
		/*计算用的中间变量*/
		ImageMemData MemData(picvec[k],saveLocation,m_spcount,m_compactness,HorizontalLine);
#endif
#if (OUT_DOOR||IN_DOOR)				

		SLIC slic(&MemData);
		/*获得图像数据*/
		slic.CuiGetImageData(picvec[k],saveLocation);
		/*完成超像素算法*/
		slic.CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(m_spcount,m_compactness);//得到lable
		/*为以后的算法分配公共内存*/
		MemData.AllocateMemRelated2spnumber(MemData.slic_current_num);
		/*完成谱聚类算法*/
		while(slic.Cui_Spectral_Clustering_B()){

		}
		slic.ExportMemData();
#endif
#if (OUT_DOOR||IN_DOOR)					
		ComputeSVG computesvg(&MemData);
		/*获取计算数据*/

		computesvg.GetImgData(
			slic.CuiImgData,
			slic.CuiWidth,
			slic.CuiHeight,
			slic.CuiImgLables,
			MemData.slic_current_num);

		//以密度函数为主要因素分割图像
		computesvg.separateSVG_Zlm();
#endif
#if (OUT_DOOR)
		CuiHough3  hough(&MemData);
		hough.GetImageData(computesvg.cui_ImgData,
			computesvg.cui_Width,computesvg.cui_Height,
			computesvg.cui_ImgLables,computesvg.cui_ImgLables_SVG,
			computesvg.cui_NumLabels,
			computesvg.cui_Matrix_Category_Lable);

		hough.GetAllHoughLineBySourceImg();
		hough.GetAllHoughLineByContour();
		hough.GetAllHoughLineByContourVG();
		hough.GetClassifyMethod();				
		hough.ClassifyGV();
#endif
#if OUT_DOOR&&FALSE

		SplitTreeandBuilding  STB(hough.cui_ImgLables_SVG,hough.cui_ImgLables,&MemData);
		STB.SlpitTreeBuilding();

		SeparateSpBlock ssp(MemData.Src_ImgData,hough.cui_ImgLables,&MemData);
		ssp.SeparateSp();
#endif	
		Cui_DrawDepth  drawdepth(&MemData);
#if OUT_DOOR			

		drawdepth.GetImageData(
			hough.cui_ImgData,
			hough.cui_Width,
			hough.cui_Height,
			hough.cui_ImgLables_SVG);
		drawdepth.SetCameraParameter(1,0.8,1,MemData.Seg_HorizontalLinePosScale);
		drawdepth.DrawDepth();
#endif

	}

	/****************************************/
	cui_Button.EnableWindow(TRUE);
	return AfxMessageBox(L"Done!", 0, 0);
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/
UINT CSLICSuperpixelsDlg::Do400OutDoorIMGBuildingOrTree(LPVOID lpParam)
{
#if 0
	ULONGLONG Do_Time;	
	ULONGLONG Do_Time_S;
	ULONGLONG Do_Time_E;
#endif

	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	cui_Button.EnableWindow(FALSE);
	/****************************************/
	for( int k = 0; k < numPics; k++ ){
		/*获得水平线位置*/
#if GetHorLineFromFile
		PointWithDistance  pwd;
		pwd.Parse(CString(picvec[k].c_str()));				
		float HorizontalLine=pwd.point.CalculateHorLinePos();
		pwd.point.SaveImgandParam(CString(saveLocation.c_str()),pwd.XmlFileName);
#else
		float HorizontalLine=0.5; 
#endif

#if (TRUE)				
		/*计算用的中间变量*/
		ImageMemData MemData(picvec[k],saveLocation,m_spcount,m_compactness,HorizontalLine);
#endif
#if FALSE
		HoughAnalyze ha(&MemData);
		HistDataInDoor histdata=ha.GetALlImgHistogram_InDoor();
		ha.AdjustHistFrom0to100(&histdata,AngLeDividedInDoor);
#endif

#if (OUT_DOOR||IN_DOOR)				

		SLIC slic(&MemData);
		/*获得图像数据*/
		slic.CuiGetImageData(picvec[k],saveLocation);
		/*完成超像素算法*/
		slic.CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(m_spcount,m_compactness);//得到lable
		/*为以后的算法分配公共内存*/
		MemData.AllocateMemRelated2spnumber(MemData.slic_current_num);

		/*完成谱聚类算法*/
		while(slic.Cui_Spectral_Clustering_B()){

		}
		slic.ExportMemData();

#endif


#ifdef IN_DOOR
#if IN_DOOR
		LabColorAnalyzeCluster Labanalyze(&MemData);
		Labanalyze.ColorPointIteration();

		SpectralClusterInDoor	spInDoor(&MemData);
		spInDoor.Spectral_Clustering_B();	
#endif
#endif


#if (OUT_DOOR||IN_DOOR)					
		ComputeSVG computesvg(&MemData);
		/*获取计算数据*/
#if OUT_DOOR
		computesvg.GetImgData(
			slic.CuiImgData,
			slic.CuiWidth,
			slic.CuiHeight,
			slic.CuiImgLables,
			MemData.slic_current_num);
#endif

#if IN_DOOR
		computesvg.GetImgData(
			MemData.Src_ImgData,
			MemData.ImgWidth,
			MemData.ImgHeight,
			MemData.ImgLables.get(),
			MemData.slic_current_num);
#endif

#if OUT_DOOR
		//以密度函数为主要因素分割图像
		computesvg.separateSVG_Zlm();
#endif


#if IN_DOOR
		computesvg.separateSVG_InDoor();

#endif

#endif
#if (OUT_DOOR)
		CuiHough3  hough(&MemData);
		hough.GetImageData(computesvg.cui_ImgData,
			computesvg.cui_Width,computesvg.cui_Height,
			computesvg.cui_ImgLables,computesvg.cui_ImgLables_SVG,
			computesvg.cui_NumLabels,
			computesvg.cui_Matrix_Category_Lable);

		hough.GetAllHoughLineBySourceImg();
		hough.GetAllHoughLineByContour();
		hough.GetAllHoughLineByContourVG();
		hough.GetClassifyMethod();				
		hough.ClassifyGV();
#endif

#if OUT_DOOR&&FALSE

		SplitTreeandBuilding  STB(hough.cui_ImgLables_SVG,hough.cui_ImgLables,&MemData);
		STB.SlpitTreeBuilding();

		SeparateSpBlock ssp(MemData.Src_ImgData,hough.cui_ImgLables,&MemData);
		ssp.SeparateSp();
#endif	
		Cui_DrawDepth  drawdepth(&MemData);
#if OUT_DOOR			

		drawdepth.GetImageData(
			hough.cui_ImgData,
			hough.cui_Width,
			hough.cui_Height,
			hough.cui_ImgLables_SVG);
		drawdepth.SetCameraParameter(1,0.8,1,MemData.Seg_HorizontalLinePosScale);
		drawdepth.DrawDepth();
#endif
#if IN_DOOR&&0
		drawdepth.GetImageData(
			MemData.Src_ImgData,
			MemData.ImgWidth,
			MemData.ImgHeight,
			MemData.p_ImgLables_SVG);
		drawdepth.SetCameraParameter(1,0.8,1,MemData.Seg_HorizontalLinePosScale);
		drawdepth.DrawDepth();
#endif	




	}

	/****************************************/
	cui_Button.EnableWindow(TRUE);
	return AfxMessageBox(L"Done!", 0, 0);
}
/*---------------------------------------------------------------------------*/
/**
*类地重力场环境下室内场景单目视觉空间识别\n
*1.首先对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像；\n
*2.基于人类视觉多尺度感知特性的色彩空间谱聚类方法将超像素图像进一步聚类，生成接近人类视觉色彩分类感知的图块图像，以降维超像素图像；\n
*3.对色彩分类图块进行基于轮廓形态谱聚类，将立面物体的图像聚类成大立面图块；\n
*4.采用室外重力场视觉模糊分布密度函数进行室内场景的初步空间分类，初步找出可能的天花板、立面及地面的空间区域；\n
*5.并利用曼哈顿强度特性将具有强立面轮廓特征的图块归类成立面；\n
*6.基于室内场景透视原理的天花板与立面、地面与立面边界进行搜索；\n
*7.采用室内重力场视觉模糊分布密度函数进行室内场景图像空间识别。\n
*\image html image003.jpg "室内聚类过程"
*
*@note 室内120张出图。
*/
/*---------------------------------------------------------------------------*/
UINT CSLICSuperpixelsDlg::Do120InDoorIMG(LPVOID lpParam)
{
	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	cui_Button.EnableWindow(FALSE);
	/****************************************/
	for( int k = 0; k < numPics; k++ ){
		/*获得水平线位置*/

		float HorizontalLine=0.5; 
#if (TRUE)
		/*计算用的中间变量*/
		ImageMemData MemData(picvec[k],saveLocation,m_spcount,m_compactness,HorizontalLine);
#endif
#if 0
		/*1.首先对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像；*/
		SLIC slic(&MemData);
		slic.fai=30;
		/*获得图像数据*/
		slic.CuiGetImageData(picvec[k],saveLocation);
		/*完成超像素算法*/
		slic.CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(m_spcount,m_compactness);//得到lable
		/*为以后的算法分配公共内存*/
		//MemData.AllocateMemRelated2spnumber(MemData.slic_current_num);

		/*完成谱聚类算法*/
		//while(slic.Cui_Spectral_Clustering_B()){
		//
		//}
		//slic.ExportMemData();

#endif
		/*2.基于人类视觉多尺度感知特性的色彩空间谱聚类方法将超像素图像进一步聚类，生成接近人类视觉色彩分类感知的图块图像，以降维超像素图像；*/
		//LabColorAnalyzeCluster Labanalyze(&MemData);
		//Labanalyze.ColorPointIteration();
#if 0
		/*3.对色彩分类图块进行基于轮廓形态谱聚类，将立面物体的图像聚类成大立面图块；*/
		SpectralClusterInDoor	spInDoor(&MemData);
		spInDoor.Spectral_Clustering_B();	
#endif
		

#if 0
		/*4.采用室外重力场视觉模糊分布密度函数进行室内场景的初步空间分类，初步找出可能的天花板、立面及地面的空间区域；*/
		ComputeSVG computesvg(&MemData);
		/*获取计算数据*/

		computesvg.GetImgData(
			MemData.Src_ImgData,
			MemData.ImgWidth,
			MemData.ImgHeight,
			MemData.ImgLables.get(),
			MemData.slic_current_num);
			/*5.并利用曼哈顿强度特性将具有强立面轮廓特征的图块归类成立面；*/
			/*6.基于室内场景透视原理的天花板与立面、地面与立面边界进行搜索；*/
			/*7.采用室内重力场视觉模糊分布密度函数进行室内场景图像空间识别。*/
		computesvg.separateSVG_InDoor();

#endif	
	
#if 0
		Cui_DrawDepth  drawdepth(&MemData);
		drawdepth.GetImageData(
			MemData.Src_ImgData,
			MemData.ImgWidth,
			MemData.ImgHeight,
			MemData.p_ImgLables_SVG);
		drawdepth.SetCameraParameter(1,0.8,1,MemData.Seg_HorizontalLinePosScale);
		drawdepth.DrawDepth_Indoor();
#endif	

	}

	/****************************************/
	cui_Button.EnableWindow(TRUE);
	return AfxMessageBox(L"Done!", 0, 0);
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/
UINT CSLICSuperpixelsDlg::THreadSuperPixel_400withLabColor(LPVOID lpParam)
{


	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	cui_Button.EnableWindow(FALSE);
	/****************************************/
			for( int k = 0; k < numPics; k++ ){

				ImageMemData MemData(picvec[k],saveLocation,m_spcount,m_compactness,HORIZONTAL_LINE_POS);


				
				SLIC slic(&MemData);
				slic.CuiGetImageData(picvec[k],saveLocation);
				
				/*首先对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像*/

				slic.CuiDoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(m_spcount,m_compactness);//得到lable

			
				MemData.AllocateMemRelated2spnumber(MemData.slic_current_num);
				
				/*通过运用基于超像素颜色空间距离、纹理特征向量距离及几何邻接关系的普聚类方法将超像素图像降维到接近10%以下的图块聚类图像*/
				while(slic.Cui_Spectral_Clustering_B()){
				
				}
				slic.ExportMemData();


				LabColorAnalyzeCluster_Veins Labanalyze(&MemData);
				Labanalyze.ColorPointIteration_Veins();
			
				ComputeSVG computesvg(&MemData);

				computesvg.GetImgData(
					MemData.Src_ImgData,
					MemData.ImgWidth,
					MemData.ImgHeight,
					MemData.ImgLables.get(),
					MemData.slic_current_num);
				/*代表天空、地面及立面物体的重力场模糊分布密度函数分别与这些大图块像素相乘，并求出这些大图块的期望值，从而产生天空、地面及
				立面物体的初步分类，通过进一步的单层小波采样、曼哈顿方向提取等特性分类方法，提取出较为准确的天空、地面和立面物体分类图*/
				computesvg.FillWeightArrayZlm(HORIZONTAL_LINE_POS);
				computesvg.separateSVG_Zlm();

				CuiHough3  hough(&MemData);
				hough.GetImageData(computesvg.cui_ImgData,computesvg.cui_Width,computesvg.cui_Height,
					computesvg.cui_ImgLables,computesvg.cui_ImgLables_SVG,computesvg.cui_NumLabels,
					computesvg.cui_Matrix_Category_Lable);
				hough.ImageEenvironment=BT_unknow;
				hough.ClassifyGV();
				/*最后，基于小孔成像模型及地面线性透视信息生成空间深度感知图*/
				Cui_DrawDepth  drawdepth(&MemData);

				drawdepth.GetImageData(hough.cui_ImgData,hough.cui_Width,hough.cui_Height,hough.cui_ImgLables_SVG);

				drawdepth.SetCameraParameter(1,0.8,1,HORIZONTAL_LINE_POS);
				drawdepth.DrawDepth();

			}

	/****************************************/
	cui_Button.EnableWindow(TRUE);
	return AfxMessageBox(L"Done!", 0, 0);
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/
UINT CSLICSuperpixelsDlg::THreadSuperPixel_CUDA_CollectionMethods(LPVOID lpParam)
{
	
	vector <double>  Super_Pixel_num_f;
	vector <double>  Do_Time_f;	
	int numPics( picvec.size() );
	/****************************************/
	cui_Button.EnableWindow(FALSE);
	/****************************************/
/*		for(int k = 0; k < numPics; k++ ){	
			LARGE_INTEGER litmp;
			LONGLONG QPart1,QPart2;
			double dfMinus, dfFreq, dfTim;
			QueryPerformanceFrequency(&litmp);
			dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
			QueryPerformanceCounter(&litmp);
			QPart1 = litmp.QuadPart;// 获得初始值
			/////////////////////////////////////////////
				ImageData MemData(picvec[k],saveLocation,m_spcount,0.5);
				
				SLIC slic(&MemData);
				slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//得到lable				


				ColorBarCluster colorBarCluster(&MemData);
				colorBarCluster.Clustering();


				ComputeSVG2 svg(&MemData);
				svg.separateSVG_Zlm();
				/*聚类步骤.docx*/

			///////////////////////////////////////////////
/*			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;//获得中止值
			dfMinus = (double)(QPart2-QPart1);
			dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
			TRACE("\n 全部时间: %f（秒）",dfTim);
			/*************************************************************/
/*#if	!(SaveContours2Disk)
			MemData.SaveImgWithContours();			
#endif	}*/
		
	cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(NULL,picvec,saveLocation,m_spcount);
	/****************************************/
	cui_Button.EnableWindow(TRUE);
	return AfxMessageBox(L"Done!", 0, 0);
}
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*---------------------------------------------------------------------------*/