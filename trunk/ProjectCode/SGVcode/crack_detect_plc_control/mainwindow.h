#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "QT_THREAD_MODULE/QtThreadPLC.hpp"

#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

	QSharedPointer<QtThreadPLC> mPlcdataServer;
	QSharedPointer<QtThreadFlowCtrlServer> mFlowServerServer;

private:
	void init_connect();
	void printf_event(std::string _event, std::string _msg);
public slots:
	   void sjts_status(int _sjts_status_int);

};

#endif // MAINWINDOW_H
