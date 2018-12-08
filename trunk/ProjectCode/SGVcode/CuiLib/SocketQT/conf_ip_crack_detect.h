#ifndef CONF_IP_CRACK_DETECT_H
#define CONF_IP_CRACK_DETECT_H
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#define PLC_ADDR				("192.168.100.12")
/*-----------------------------------*/
#define IPC_GUI_ADDR			("192.168.100.100")
/*-----------------------------------*/
#define IPC_DATA_CENTER_ADDR_00	("192.168.100.101")
#define IPC_DATA_CENTER_ADDR_01	("192.168.100.102")
#define IPC_DATA_CENTER_ADDR_02	("192.168.100.103")
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#if _DEBUG
#define TEST_IPC				("192.168.100.212")
#endif
/*-----------------------------------*/
#define NVR_IN_BIG				("192.168.100.210")
#define NVR_IN_SMALL			("192.168.100.220")
#define NVR_OUT					("192.168.100.230")
/*-----------------------------------*/
#define BORD_VIDEO_IN_LONG		("192.168.100.110")
/*-----------------------------------*/
#define IN_BIG_CHANNEL_1  (192.168.100.111)
#define IN_BIG_CHANNEL_2  (192.168.100.112)
#define IN_BIG_CHANNEL_3  (192.168.100.113)
#define IN_BIG_CHANNEL_4  (192.168.100.114)
#define IN_BIG_CHANNEL_5  (192.168.100.115)
#define IN_BIG_CHANNEL_6  (192.168.100.116)
#define IN_BIG_CHANNEL_7  (192.168.100.117)
#define IN_BIG_CHANNEL_8  (192.168.100.118)
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#define BORD_VIDEO_IN_SHORT	("192.168.100.120")

/*-----------------------------------*/
#define IN_SMALL_CHANNEL_1  (192.168.100.121)
#define IN_SMALL_CHANNEL_2  (192.168.100.122)
#define IN_SMALL_CHANNEL_3  (192.168.100.123)
#define IN_SMALL_CHANNEL_4  (192.168.100.124)
#define IN_SMALL_CHANNEL_5  (192.168.100.125)
#define IN_SMALL_CHANNEL_6  (192.168.100.126)
#define IN_SMALL_CHANNEL_7  (192.168.100.127)
#define IN_SMALL_CHANNEL_8  (192.168.100.128)
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#define BORD_VIDEO_OUT	("192.168.100.130")
/*-----------------------------------*/
#define OUT_CHANNEL_1  (192.168.100.131)
#define OUT_CHANNEL_2  (192.168.100.132)
#define OUT_CHANNEL_3  (192.168.100.133)
#define OUT_CHANNEL_4  (192.168.100.134)
#define OUT_CHANNEL_5  (192.168.100.135)
#define OUT_CHANNEL_6  (192.168.100.136)
#define OUT_CHANNEL_7  (192.168.100.137)
#define OUT_CHANNEL_8  (192.168.100.138)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#define TCP_PORT_VIDEO_TRANS (10001)
#define TCP_POET_CMD_CTRL    (10000)
#define TCP_PORT_VIDEO_RAW	 (8888)
#define TCP_PORT_SSH (22)
#define TCP_PORT_IPC_FLOW_CTRL (20001)
/*-------------------------------------*/

/*-------------------------------------*/
#define USE_THREAD_CRACK_PRIORITY (0)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#endif