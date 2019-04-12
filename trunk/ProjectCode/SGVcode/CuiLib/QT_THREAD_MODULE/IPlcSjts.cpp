#include "IPlcSjts.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
IPlcSjts::IPlcSjts()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
IPlcSjts::~IPlcSjts()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::print_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsRoolerReady()) {
		//rooler is ready !!!
		printf_event("EVENT", "Rooler is Ready !");

	}
	else if (_cmd->IsRoolerPosReady()) {

		printf_event("EVENT", "Rooler  Pos is Ready !");

	}
	else if (_cmd->IsIntoInnerReady()) {

		printf_event("EVENT", "Now into inter  !");
	}
	else if (_cmd->isHeartbeatCmd()) {
		printf_event("@r", "hb !");
	}
	else if (_cmd->IsResp()) {
		printf_event("EVENT", "RESP");
	}
	else if (_cmd->IsRoolerReadyError()) {
		printf_event("EVENT", "Rooler is Ready ERROR !");
	}
	else if (_cmd->IsAbortStop()) {
		printf_event("EVENT", "Rooler is Abort Stop !");
	}
	else if (_cmd->IsOperationMode()) {
		printf_event("EVENT", "Operation Mode !");
	}
	else {
		print_undefined_cmd(_cmd);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::print_undefined_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	printf_event("ERROR", "Error Cmd!");
	printf_event("ERROR", "this is Error Cmd ! do you know why ????????????????");
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::print_socket_connected()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int IPlcSjts::Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd)
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::process_fatal_error_msg(QString _msg)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::process_fatal_error(QSharedPointer<CMD_CTRL> _cmd)
{

	if (_cmd->IsRoolerReady()) {

	}
	else if (_cmd->IsRoolerPosReady()) {

	}
	else if (_cmd->IsOperationMode()) {

	}
	else if (_cmd->IsIntoInnerReady()) {

	}
	else if (_cmd->isHeartbeatCmd()) {

	}
	else if (_cmd->IsResp()) {

	}
	else if (_cmd->IsRoolerReadyError()) {
		this->process_fatal_error_msg("IsRoolerReadyError");
	}
	else if (_cmd->IsAbortStop()) {
		this->process_fatal_error_msg("IsAbortStop");
	}
	else {
		this->process_fatal_error_msg("UnKonw CMD");
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::do_sjts_resp()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::do_sjts_roller_ready()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::do_sjts_roller_pos_ready()
{
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::do_sjts_roller_into_inner()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IPlcSjts::do_sjts_roller_run(const int __step)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/