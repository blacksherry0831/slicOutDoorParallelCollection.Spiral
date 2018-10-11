#pragma once

#include "cpp_stl.h"

#include "qt_all.h"

#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"
#include "CMD_CTRL_Q.hpp"

#include "conf_ip.h"

#include "module_socket_qt_import.h"

#define LC_CMD_SET_TIME		(32)
#define LC_CMD_SET_DEADLINE	(0x21)
#define LC_CMD_SET_ONLINE	(0x20)
#define LC_CMD_SET_SERNO	(0x22)
#define LC_CMD_END			(0x4)
#define LC_CMD_CLOSE		(0x10)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadClientLifeCycle :public QtThreadSocketClient
{
	Q_OBJECT
public:
	
public:
	QtThreadClientLifeCycle();
	
	~QtThreadClientLifeCycle();
protected:

	void FactorySetting();

	void UpdateDeadline(QString _cmd_json);
public:
	static QString GetFactorySettingCmd();
	static QString GetFactorySettingCmd(
		QString _ip_addr,
		int _serno_0,
		int _serno_1,
		int _serno_2,
		int _serno_3);

	static const QString KEY_SERNO;
	static const QString KEY_SERNO_0;
	static const QString KEY_SERNO_1;
	static const QString KEY_SERNO_2;
	static const QString KEY_SERNO_3;
	static const QString KEY_STATUS;
	static const QString KEY_DEADLINE;
	static const QString KEY_IP_ADDR;
	static const QString KEY_ON_LINE;

	static bool SaveStringMy(QString _path,QString _str);
	static QVector<QString> LoadStringMy(QString _path);

	static bool SaveSerialNumber(QString _ip, QString _str);
	static bool SaveSerialNumber_ex(QString _path, QString _str);
	static bool SaveSerialNumber_ex(QString _path, QJsonDocument _jdoc);

	static QString LoadSerialNumber(QString _path);
	
	static QJsonDocument* ChangeDeadLine(QJsonDocument* _jsonDoc,QString _key,int _value);
	static QJsonDocument* RemoveDeadLine(QJsonDocument* _jsonDoc);
	static int CheckDeadLine(QJsonDocument* _jsonDoc);
	static QString GetIpAddr(QJsonDocument* _jsonDoc);

	static QStringList OpenFileDialog2SelectCfgFile(QWidget *_parent, QString _title);
	

	static QByteArray Int2Uchar4BigEndian(int _number);
protected:
	QString GetUpdateDeadlineSettingCmd();
	
	int SendParamJson(QString _param);

	int SendCmd2SetParam(char _cmd);
	int SendCmdEndClose();
	QString GetMagicOnlyJson();

public:
	static QString GetJsonKey(QString _str);
public:
	
	void run_factorySetting();
	void run_updateDeadline();

public slots:




protected: 
	virtual void run();

};