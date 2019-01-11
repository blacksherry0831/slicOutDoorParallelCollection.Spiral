#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "module_my.h"

#if defined(QT_VERSION)
#include  "QT_THREAD_MODULE/QtThread8ImgProcess.hpp"
#include  "QT_THREAD_MODULE/QtThread8Video.hpp"
#endif

#if _MSC_VER
	#ifndef _X86_
		#define _X86_
	#endif // !_X86_
	#include <minwindef.h>
	#include <minwinbase.h>
	#include <WinBase.h>
#endif

#if TRUE
	#include "cpp_stl.h"
	
#endif

#ifdef QT_VERSION

#include <QtCore/QVariant>
#include <QtWidgets/QLabel>
#include <QTimer>

#endif // QT_VERSION

#if TRUE
#include "dialog.h"
#endif // TRUE

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
namespace Ui {
class MainWindow;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
	QLabel* labelImage[8];
	QString mFpgaArmLinuxIpAddr;
	QSharedPointer<CMD_CTRL> cmd_ctrl_image[8];
	int mWorkMode;
	QSharedPointer<QImage> mQimageGray;
	
	
private:
	void closeEvent(QCloseEvent *event);
public:
	void initUI();
	void initUIlabelImage();
	void initUIlabelImageView();
	void SetMaxWidthMy();
	bool m_test_result;
public:
	void WorkStart();
	void WorkDone();
private:
	IMG_PROC mImgProc;
public:

	QSharedPointer<QtThread8ImgProcess>		mImg8Process;

	QSharedPointer<QtThreadStepMotor>		mStepMotor;

	QSharedPointer<QtThreadClientCtrl>		mCtrlServer;

	QSharedPointer<QtThread8Video>			mVideoDataServer;

	QSharedPointer<QThread>					mthread;

	QSharedPointer<QtLink>					mLink;
	
	QSharedPointer<QTimer>					mTimer;
private:
	void init_class_member();
	void init_ping_ssh();
	
	void destory_ping_ssh();
	void init_menu();
	void init_controls();
	void disableInputCtrls(bool _flag=true);
	int  IsBgThreadRunning();
	void destory_all();
	void connect_img_ch(int _connect, const QObject *receiver);
	void SetFpgaArmLinuxIpAddr(QString _str);
public:
	static void ShowImage(QLabel* _qlab, QImage *_p_qimg);
	static void ShowImageFast(QLabel* _qlab, QImage* const _p_qimg);
public:
	static QImage * IplImageToQImage( IplImage* const img);
public:
	void message_test();
public slots:
		
		void ClickButton_CutArea();
		void StartVideoBasic(int mode);
		void StartVideoModeCutArea();
		void StartVideoModeSelected();
		void StopVideo();
		void StopVideoForce();
		void stopVideoBasic();
		void stopVideoBasicForce();
		void ConnectVideo();

#if TRUE
		void img_stat_show_ex(int  _p_stat, int _channel, int _frames, void* _data);
		void start_ping_ssh();
#endif // TRUE
				
public slots:
	void ClickButton_Test();
	void ClickButton_SetSerialPort();
	void ClickButton_CameraStart();
	void ClickButton_MotorRun();
	void WorkProgressShow(QString str);
	void CheckBox_ping(int _stat_t);
	void CheckBox_ssh(int _stat_t);
	void CheckBox_fpga_ctrl(int _stat_t);
	void CheckBox_fpga_image_video(int _stat_t);
	void CheckBox_img_mode_update();
	void ComboBox_IpAddr_changed(const QString& _str);
	void Slider_img_sigma_change(int _sigma);
	void main_test();
	void SetCutRectMethod();
	int openImageShowQDialog(QLabel* _qabel);
	
	void toggleShowCutArea();
	void toggleShowBinaryImg();
	void toggleShowClassifyThickly(bool _checked);

	void toggleImgCollect();

	void statusBarshowMessage(QString _msg);
};

#endif // MAINWINDOW_H
