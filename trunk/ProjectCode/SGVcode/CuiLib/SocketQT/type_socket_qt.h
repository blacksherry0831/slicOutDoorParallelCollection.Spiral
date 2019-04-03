#ifndef TYPE_SOCKET_QT_H

#define TYPE_SOCKET_QT_H

typedef struct {
	
	int ShowCutArea;
		
	int ImgProc_Binary_Thickly_Classify;
	int ImgProc_Hough_Block_Classify;
	
	int ImgProc_Binary;
	int ImgProc_Denoise;
	int ImgProc_Hough;
	int ImgProc_Block;
		
	int CurrentChannel;
	int Start;
	int End;
	int Frames;
	int CurrentSerialNumber;
	int IsSaveFrame;
	int Sigma;
}IMG_PROC;


#endif // DEBUG



