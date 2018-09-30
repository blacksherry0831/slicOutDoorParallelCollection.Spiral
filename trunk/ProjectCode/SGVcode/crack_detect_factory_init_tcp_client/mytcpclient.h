#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QJsonObject>
#include<QJsonDocument>
#include <QDebug>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QInputDialog>

#include "module_my.h"

#define STR_SET_SERNO_TIP			("请设置序列号")
#define STR_SET_SERNO_INVALID		("设置序列号无效！")
#define STR_SET_SERNO_SUCCESS		("设置序列号成功了")
#define STR_SOCKET_FAIL				("连接服务器失败，%1")
#define STR_SERVER_UNDEF_CMD		("服务器未定义命令错误")

namespace Ui {
class MyTcpClient;
}

class MyTcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyTcpClient(QWidget *parent = 0);
    ~MyTcpClient();

private:
    Ui::MyTcpClient *ui;
    QTcpSocket *tcpClient;
	QString mIpAddr;
	QString mLoadFileName;
	QSharedPointer<QIntValidator> mQIV;
	QJsonDocument mJsonDoc;

	void message_box_exec(QString _msg);
private slots:
    //客户端槽函数
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);
    void on_pushButton_clicked();
    void on_cmd_serno_clicked();
	void on_action_load_serno();
	void on_action_cfg_deadline();
};

#endif // MYTCPCLIENT_H
