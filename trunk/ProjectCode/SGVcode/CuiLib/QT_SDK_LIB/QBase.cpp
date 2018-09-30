#include "QBase.h"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
QBase::QBase(void)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
QBase::~QBase(void)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::string QBase::SYS_getCurrentTime(std::string _format)
{
	QDateTime time = QDateTime::currentDateTime();

	if (_format==""){
		_format = "yyyy-MM-dd hh:mm:ss dd";
	}

	QString str = time.toString(QString::fromStdString(_format));

	return str.toStdString();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
QString QBase::AesEncode(QString _key, QByteArray _body,int _mode)
{
	
	
	QString decodedString;

	

	return decodedString;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/