#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
#include "SocketQT/CMD_CTRL.hpp"
/*-------------------------------------*/
#include "QT_SDK_LIB/IPrint.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class IPlcSjts :public IPrint
{

public:

	IPlcSjts();
	~IPlcSjts();

public:
	void			print_cmd(QSharedPointer<CMD_CTRL> _cmd);
	void			print_undefined_cmd(QSharedPointer<CMD_CTRL> _cmd);
public:
	virtual int		Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd);
public:
	virtual void	process_fatal_error_msg(QString _msg);
	virtual void	process_fatal_error(QSharedPointer<CMD_CTRL> _cmd);
public:
	virtual void	do_sjts_resp();
	virtual void	do_sjts_roller_ready();
	virtual void    do_sjts_roller_pos_ready();
	virtual void    do_sjts_roller_into_inner();
	virtual void    do_sjts_roller_run(const int __step);
};