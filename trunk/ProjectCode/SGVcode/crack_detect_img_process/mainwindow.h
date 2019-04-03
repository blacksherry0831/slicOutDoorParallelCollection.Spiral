#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "module_my.h"

#if defined(QT_VERSION)

#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlClient.hpp"
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlLocal.hpp"


#endif

#if defined(QT_VERSION)

#include "QT_THREAD_MODULE/QtThread8ImgProcess.hpp"
#include "QT_THREAD_MODULE/QtThread8Video.hpp"

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

#if  defined(QT_VERSION)

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
#define IMG_PROCESS_USE_STEP_MOTOR (1)
#define FLOW_CTRL_USE_LOCAL_SERVER (1)
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
	QSharedPointer<QImage>	  mQimageGray;
	QPalette  mPaletteStatus[2];
	QString   mCheckBoxRunStatus[2];
private:
	static  QString gRemoteBord[];
private:
	void initGlobal();
	void destoryGlobal();
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

	QSharedPointer<QtThreadFlowCtrlLocal>	mFlowCtrlLocal;


#if FLOW_CTRL_USE_LOCAL_SERVER 
	QSharedPointer<QtThreadFlowCtrlServer> mFlowServerServerLocal;
#endif
	
public:

	QSharedPointer<QtThreadFlowCtrlClient> mFlowCtrlClient;

	QSharedPointer<QtThreadClientCtrl>		mCtrlServer;

	QSharedPointer<QtThread8Video>			mVideoDataServer;

	QSharedPointer<QThread>					mthread;

	QSharedPointer<QtLink>					mLink;
	
	QSharedPointer<QTimer>					mTimer;
private:
	void init_class_member();
	void init_class_member_ptr();
	void init_class_member_base();
	void init_ping_ssh();

#if FLOW_CTRL_USE_LOCAL_SERVER 
	void init_connect_work_flow();
#endif
private:
	std::string GetInOut();
	std::string GetIpAddrProperty(QString _ipAddr);
public:
	void destory_ping_ssh();
	void init_menu();
	void init_controls();

	void init_controls_by_cfg();

	void disableInputCtrls(bool _flag=true);
	int  IsBgThreadRunning();
	void destory_all();
	void connect_img_ch(int _connect, const QObject *receiver);
	void SetFpgaArmLinuxIpAddr(QString _str);
	void ShowImageChGray(int _ch);
	void ShowImageCh(int _ch, QImage *_p_qimg);
public:
	static void DrawUnqualified(QSharedPointer<CMD_CTRL> _cmd);
	static void ShowImage(QLabel* _qlab, QImage *_p_qimg);
	static void ShowImageFast(QLabel* _qlab, QImage* const _p_qimg);
public:
	QSharedPointer<QSettings> mAppSetting;
	QString  mAppKeyXilinxFpgaArm;
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

		void NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, int _cmd_idx=0);
		void beforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, QString _msg="");

#if TRUE
		void img_stat_show_ex(int  _p_stat, int _channel, int _frames, void* _data);
		void start_ping_ssh();
#endif // TRUE
				
public slots:

#if TRUE
	void main_test();
	void ClickButton_Test();
#endif // TRUE
	
	void ClickButton_SetSerialPort();
	void ClickButton_CameraStart();
	void ClickButton_MotorRun();
	void WorkProgressShow(QString str);

#if TRUE
	void CheckBox_ping(int _stat_t);
	void CheckBox_ssh(int _stat_t);
	void CheckBox_fpga_ctrl(int _stat_t);
	void CheckBox_fpga_image_video(int _stat_t);
	void CheckBox_flow_ctrl_video(int _stat_t);
#endif // TRUE

#if TRUE
	void CheckBox_thread_status_ping(int _stat_t);
	void CheckBox_thread_status_ssh(int _stat_t);
	void CheckBox_thread_status_fpga_ctrl(int _stat_t);
	void CheckBox_thread_status_fpga_image_video(int _stat_t);
	void CheckBox_thread_status_flow_ctrl_video(int _stat_t);
#endif // TRUE
	
#if TRUE

	void CheckBox_img_mode_change();	
	void ComboBox_IpAddr_changed(const QString& _str);
	void Slider_img_sigma_change(int _sigma);

#endif // TRUE

#if TRUE
	void toggleImageClassifyHoughBlock(bool _status);
#endif

#if TRUE
	void toggleImgProcDenoise(bool _status);
	void toggleImgProcBinary(bool _status);
#endif // TRUE

#if TRUE

	void toggleShowCutArea();
	void toggleShowClassifyThickly(bool _checked);
	void toggleImgCollect();
	void toggleBlockStep02(bool _block);

#endif // TRUE

#if TRUE
	void workflow_local();
	void workflow_remote();
	void tcp_server_work_flow_dones(int _status,int _quality);
	void tcp_server_running_client_sessions(int _running_sessions);
#endif // TRUE

	void SetCutRectMethod();
	int openImageShowQDialog(QLabel* _qabel);
	
	void statusBarshowMessage(QString _msg);

	void sjts_status(const int _sjts_status_int, QString _msg);
	void sjts_fpga_work_flow_status_rcv(const int _arm_linux_fpga_work_flow_int, QString _seq);

};

#endif // MAINWINDOW_H
