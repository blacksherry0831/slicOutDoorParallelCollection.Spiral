#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#include <QString> 
#include <QDateTime> 
/*-------------------------------------*/
#include "IPrint.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QBase: public IPrint
{
public:
	QBase(void);
	~QBase(void);
public:
   	static  std::string SYS_getCurrentTime(const std::string _format);

	static  QString SYS_getCurrentTime_Q(const QString _format);
	static  QString uint2Qstring(uint _num);
 
public:
	static QString AesEncode(QString _key, QByteArray _body, int _mode);
	
};

