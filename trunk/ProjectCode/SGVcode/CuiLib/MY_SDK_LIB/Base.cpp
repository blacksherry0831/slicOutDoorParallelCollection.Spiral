#include "StdAfx.h"
#include "Base.h"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
Base::Base(void)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
Base::~Base(void)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::str2int(int & int_temp, const string & string_temp)
{
	
		stringstream stream(string_temp);
		stream >> int_temp;
		return int_temp;

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::int2str(int value_t)
{
	stringstream stream;
	stream << value_t;

	return stream.str();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::base_name(string const & path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::string Base::file_name_without_ext(string const path)
{
	std::string base_name = Base::base_name(path);
	
	std::vector<std::string> name_ext=split(base_name, ".");

	string name;
	string ext;
	
	if (name_ext.size()==2) {
		name=name_ext.at(0);
		ext=name_ext.at(1);	
	}
	return name;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::comine_str(string f_str,string s_str)
{
	return f_str.append(s_str);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<std::string> Base::split  (const std::string &s, char delim)
{

	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;

	while (std::getline(ss, item, delim)) {

		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<std::string> Base::split  (const std::string &s, string delim)
{
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	pos2 = s.find(delim);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + delim.size();
		pos2 = s.find(delim, pos1);
	}
	if(pos1 != s.length())
		v.push_back(s.substr(pos1));

	return v;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void Base::SaveString2Disk(string file_name_t,string str_t)
{
#if _DEBUG	

	ofstream ofs;
	ofs.open(file_name_t);

	assert(ofs.is_open());
	ofs<<str_t;

	ofs.close();

#endif
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::IsEqual(std::string str_0,std::string str_1)
{
	int result_t=str_0.compare(str_1);

	if (result_t==0){
		return TRUE;
	}else{
		return FALSE;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Base::sleep(int ms)
{

#if __GNUC__
	usleep(ms * 1000);
#endif

#if _MSC_VER
	Sleep(ms);
#endif

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int  Base::is_file_exist(const char * file_path)
{
	if (file_path == NULL)
		return -1;
	if (access(file_path, 0) == 0)
		return 0;
	return -1;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::get_number(string str_t)
{

	string str_new_t;

	for (size_t i = 0; i <str_t.size(); i++)
	{
			char char_t=str_t.at(i);

			if (isdigit(char_t)) {
				str_new_t += char_t;
			}
	}
		
	return atoi(str_new_t.c_str());
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
double Base::Math_GetAverageValue(double* Data, int DataNum)
{
	double sum = 0;
	ASSERT(DataNum>0);
	for (int i = 0; i<DataNum; i++) {
		sum += Data[i];
	}
	sum /= DataNum;
	return sum;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
float Base::Math_GetAverageValueF(float * Data, int DataNum)
{
	ASSERT(DataNum>0);	
	double sum = Math_GetSumF(Data, DataNum);
	sum /= DataNum;
	return sum;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
double Base::Math_GetVarianceValue(
	double* Data,
	int DataNum,
	double avg,
	double *variance) {

	*variance = 0;
	for (int i = 0; i<DataNum; i++) {
		*variance += (avg - Data[i])*(avg - Data[i]);
	}
	*variance /= DataNum;
	*variance = sqrtl(*variance);
	return *variance;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
float Base::Math_GetVarianceValueF(float * Data, int DataNum, float avg, float * variance)
{
	*variance = 0;
	for (int i = 0; i<DataNum; i++) {
		*variance += (avg - Data[i])*(avg - Data[i]);
	}
	*variance /= DataNum;
	*variance = sqrtl(*variance);
	return *variance;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
double Base::Math_GetMaxValue(double* Data, long DataNum)
{
	double *Data_cp = new double[DataNum];
	double  max_value;
	memcpy(Data_cp, Data, sizeof(double)*DataNum);
	std::sort(Data_cp, Data_cp + DataNum, greater<double>());
	max_value = Data_cp[0];
	delete[]Data_cp;
	return  max_value;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void Base::Math_AbsArray(double * Data, long Num)
{
	for (size_t i = 0; i <Num; i++) {

		Data[i] = fabs(Data[i]);
	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void Base::Math_GetMaxValueIndexF(float * data, float size, int * sort, int sort_num)
{
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void Base::Math_GetMaxValueIndex(double * data, float size, int * sort, int sort_num)
{
	double* data_t = new double[(LONGLONG)size];
	memcpy(data_t, data, sizeof(double)*((LONGLONG)size));
	/*****寻找最值***************************************************************/
	for (int sj = 0; sj<sort_num; sj++) {

		float max_value = data[0];
		int max_value_i = 0;
		/*******************************/
		for (register int i = 0; i<size; i++) {
			if (data_t[i]>max_value) {
				max_value = data_t[i];
				max_value_i = i;
			}
		}
		/*******************************/
		sort[sj] = max_value_i;
		data_t[max_value_i] = FLT_MIN;

	}
	/*******************************************************************************/
	delete[]data_t;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
float Base::Math_GetSumF(float * Data, int DataNum)
{
	float sum = 0;
#if 0
	for (size_t i = 0; i <DataNum; i++){		
		sum += Data[i];
	}
#else
	std::vector<float>   data(&Data[0], &Data[DataNum]);
	sum=std::accumulate(data.begin(),data.end(), 0.0f);
#endif // 0

	return sum;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
vector<float> Base::CombineVector(vector<float> v0, vector<float> v1)
{
	vector<float> v_out;

	v_out.insert(v_out.end(), v0.begin(), v0.end());

	v_out.insert(v_out.end(), v1.begin(), v1.end());

	return v_out;
}
/************************************************************************/
/*  获取文件夹下所有文件名
输入：
path    :   文件夹路径
exd     :   所要获取的文件名后缀，如jpg、png等；如果希望获取所有
文件名, exd = ""
输出：
files   :   获取的文件名列表

shao, 20140707
*/
/************************************************************************/
void Base::FS_getFiles(string path, string exd, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					FS_getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::CRACK_FILE_NAME_get_idx(string file_full_name)
{
	string base_name = Base::base_name(file_full_name);
	vector<string>  vs = Base::split(base_name, '.');
	string idx_str = vs.at(0);//chX.
	return Base::get_number(idx_str);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::CRACK_FILR_NAME_get_ipAddr(string file_full_name)
{
	std::vector<string>  file_cal_t;
		file_cal_t.push_back("192.168.9.0");
		file_cal_t.push_back("192.168.9.1");
		file_cal_t.push_back("192.168.9.2");
		file_cal_t.push_back("192.168.9.3");
		file_cal_t.push_back("192.168.9.4");
		file_cal_t.push_back("192.168.9.5");
		file_cal_t.push_back("192.168.9.6");
		file_cal_t.push_back("192.168.9.7");
	
		return file_cal_t.at(CRACK_FILE_NAME_get_idx(file_full_name));

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
bool  Base::FS_checkUserPath(string userPath)
{
	bool isRight = true;
	string::iterator it;
	for (it = userPath.begin(); it != userPath.end(); ++it)
	{
		if (*it == '/')
		{
			isRight = false;
			cout << "\'/\'错误，应该为\'\\',请重新输入！" << endl;
			break;
		}
	}
	return isRight;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/