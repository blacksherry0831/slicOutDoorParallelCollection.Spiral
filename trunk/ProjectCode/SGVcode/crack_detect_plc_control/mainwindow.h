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
public slots:
	   void sjts_status(QtThreadPLC::SJTS_MACHINE_STATUS _sjts_status);

};

#endif // MAINWINDOW_H
