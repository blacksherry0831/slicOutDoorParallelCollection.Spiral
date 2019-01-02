#pragma once

#include "cpp_stl.h"

#include <QString> 
#include <QDateTime> 
/**
*
*
*/
class QBase
{
public:
	QBase(void);
	~QBase(void);
public:
   	static  std::string SYS_getCurrentTime(const std::string _format);

	static  QString SYS_getCurrentTime_Q(const QString _format);
public:
	static QString AesEncode(QString _key, QByteArray _body, int _mode);
	static void printf_event(std::string _event, std::string _msg = "");
};

