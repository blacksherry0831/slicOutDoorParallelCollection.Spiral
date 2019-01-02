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
std::string QBase::SYS_getCurrentTime(const std::string _format)
{
	QDateTime time = QDateTime::currentDateTime();
	
	QString time_str;

	if (_format==""){
		const std::string format_t="yyyy-MM-dd hh:mm:ss dd";
		const QString format_t_qt = QString::fromStdString(_format);
		time_str = time.toString(format_t_qt);
	}
	else
	{
		const QString format_t_qt = QString::fromStdString(_format);
		time_str=time.toString(format_t_qt);
	}
	
	return time_str.toStdString();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
QString QBase::SYS_getCurrentTime_Q(const QString _format)
{
	QDateTime time = QDateTime::currentDateTime();
	QString current_time_t;

	if (_format.isEmpty()) {
		const QString format_t=QString("yyyy-MM-dd hh:mm:ss dd");
		current_time_t = time.toString(format_t);
	}

	current_time_t = time.toString(_format);

	return current_time_t;
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
void QBase::printf_event(std::string _event, std::string _msg)
{
	std::cout << _event << ">>" << _msg << std::endl;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/