
// SLICSuperpixelsDlg.h : header file
//

#include <vector>
#include <string>
#include "afxwin.h"
using namespace std;

#pragma once


// CSLICSuperpixelsDlg dialog
class CSLICSuperpixelsDlg : public CDialog
{
// Construction
public:
	CSLICSuperpixelsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SLICSUPERPIXELS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCreatesuperpixels();


private:

	bool BrowseForFolder(string& folderpath);
	void GetPictures(vector<string>& picvec);
	void DoSupervoxelVideoSegmentation();
public:
	afx_msg void OnEnChangeEditSpcount();
	afx_msg void OnEnChangeEditCompactness();
	CString temp;
	static	int m_spcount;
	static	double m_compactness;

	static string  saveLocation;
	static	vector<string> picvec;
	static	UINT THreadSuperPixel_CUDA(LPVOID lpParam);
	static	UINT THreadSuperPixel_CUDA_CollectionMethods(LPVOID lpParam);
	static	UINT Do400OutDoorIMG(LPVOID lpParam);
	static	UINT Do400OutDoorIMGBuildingOrTree(LPVOID lpParam);
	static	UINT Do120InDoorIMG(LPVOID lpParam);
	static  UINT THreadSuperPixel_400withLabColor(LPVOID lpParam);
	static CButton cui_Button;
	afx_msg void OnBnClickedOk();
};
