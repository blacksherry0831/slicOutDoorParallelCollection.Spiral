#include "DriveCarByCmd.h"

#include <MY_SDK_LIB/Base.h>

/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
DriveCarByCmd::DriveCarByCmd(void)
{
	m_AGV=AGV_Dirver::getInstance();
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
DriveCarByCmd::~DriveCarByCmd(void)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void DriveCarByCmd::ExeCommand( ResponseData cmd_t)
{
	assert(cmd_t.IsCmd()==TRUE);
		
	if (cmd_t.IsAutoCmd()){
			ExeCommandAuto(cmd_t);
	}else if (cmd_t.IsManualCmd()){
			ExeCommandManual(cmd_t);
	}else{

	}
	
	
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void DriveCarByCmd::ExeCommandManual(ResponseData cmd_t)
{
	
#if TRUE
	if (Base::IsEqual(cmd_t.m_manual_run_direction,"head")){
		m_AGV->RunForward();
	}else if (Base::IsEqual(cmd_t.m_manual_run_direction,"back")){
		m_AGV->RunBack();
	}else{

	}
#endif
#if TRUE
	if (Base::IsEqual(cmd_t.m_manual_turn,"direct")){
		m_AGV->RunStraight();
	}else if (Base::IsEqual(cmd_t.m_manual_turn,"left")){
		m_AGV->RunLeft();
	}else if (Base::IsEqual(cmd_t.m_manual_turn,"right")){
		m_AGV->RunRight();
	}
#endif
#if TRUE
	if (Base::IsEqual(cmd_t.m_cmd,"stop")){	
		m_AGV->StopRun();	
	}else if (Base::IsEqual(cmd_t.m_cmd,"run")){		
		m_AGV->StartRun();
	}else{

	}
#endif	
	

	
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void DriveCarByCmd::ExeCommandAuto(ResponseData cmd_t)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/