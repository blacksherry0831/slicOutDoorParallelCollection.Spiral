#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class checkerboard
{
public:
	static int	board_w;
	static int board_h;
	static int black;
	static int num_board_is_use;
	static int camera_id;
	static int per_frame;
	static int CAL_ROUTINE;
public:
	checkerboard(void);
	~checkerboard(void);

};