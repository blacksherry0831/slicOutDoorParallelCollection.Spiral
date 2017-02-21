#ifndef SVG_MODULES
#define SVG_MODULES         

//MY FILE
#include <opencv_stl.h>
#if TRUE

#else

#endif
#if TRUE
#include <SGVcode/ImageType.h>
#include <SGVcode/ManhattanHistogram.h>
#include <SGVcode/SeparateSpBlock.h>
#include <SGVcode/SpectralClustering.h>
#include <SGVcode/SpAnalyze.h>
#include <SGVcode/HsHg.h>
#include <SGVcode/SpSetProperty.h>

#include <SGVcode/ImageMemData.h>
#include <SGVcode/ImageData.h>
#include <SGVcode/SLIC.h>
#include <SGVcode/cui_GeneralImgProcess.h>
#include <SGVcode/Cui_DrawDepth.h>

#include <SGVcode/ComputeSVG.h>
#include <SGVcode/ComputeSVG2.h>
#include <SGVcode/SpectralClusterInDoor.h>
#include <SGVcode/SplitTreeandBuilding.h>
#include <SGVcode/LabColorAnalyzeCluster.h>
/*----------------------------------------*/
#include <SGVcode/CuiHough/CuiHough3.h>
#include <SGVcode/CuiHough/HoughAnalyze.h>
//#include <SGVcode/CuiHough/HoughAnalyze.h>
#include <SGVcode/Harr/CuiHarrTransformLet.h>
#include <SGVcode/Harr/HarrWaveletTransImg.h>
//#include <SGVcode/PointWithDistance.h>
#include <SGVcode/ColorAnalyzeOnLab.h>
#include <SGVcode/rtthread.h>
#include <SGVcode/MallocCui.h>
#include <SGVcode/LabColorAnalyzeCluster_veins.h>
#include <SGVcode/ColorBarCluster.h>
#endif

#if TRUE
#include <tinyxml2/tinyxml2.h>
using namespace tinyxml2;
#endif

#if TRUE
#include <cudaLib/CUDA_Code.h>
#endif

#if TRUE
#include <SVMcode/TrainTreeBuilding.h>
#include <SVMcode/TrainImgUseSvm.h>
#include <SVMcode/svmlight/svmlight.h>

/*------------------------------------------*/
#if linux||__linux||__linux__
#include <dirent.h>
#elif _MSC_VER
#include <SVMcode/dirent.h>
#else

#endif
/*------------------------------------------*/

#include <ML/Kmean/Kmean.h>
#endif

#if TRUE
#include <FilePathName/FileNameSplit.h>
#include <FilePathName/PointWithDistance.h>
#endif

#endif









