#include "stdafx.h"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
const std::string  CRACK_ROLLER_SET= "crack.roller.";
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void print_tree_level(int _level,std::string _path)
{

	while (_level-->0) {
		std::cout << "-" ;
	}

	std::cout << _path << std::endl;

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void print_tree()
{
		print_tree_level(0, "root");
			print_tree_level(1, "crack.roller.*");
				print_tree_level(2, "sample");
					print_tree_level(3, "in | out");
						print_tree_level(4, "ipAddr.*");
							print_tree_level(5, "time.*");
								print_tree_level(6, "ch*n.*");
								print_tree_level(6, "ch*p.*");
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void gen_data(std::string  _path)
{
	std::vector<std::string> crack_roller_set_t;
	Base::FS_getDirs(_path, "", crack_roller_set_t);

	for (size_t seti = 0; seti < crack_roller_set_t.size(); seti++){
		std::cout << crack_roller_set_t.at(seti) << std::endl;
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;
	
	print_tree();
	const std::string path_root = "X:\\sample.test\\";//原始图集
	int test_num = 0;

	std::cout << "Please Input Execute Item:" << std::endl;
	std::cout << "1 do date gen ! " << std::endl;
		
	std::cin >> test_num;

	if (test_num == 1) {		
		gen_data(path_root);
	}else if (test_num == 2) {		
		
	}else if (test_num == 3) {	

	}else if (test_num == 4) {		

	}else if (test_num == 5) {

	}else {

	}
		
	return  EXEC_RESULT;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/