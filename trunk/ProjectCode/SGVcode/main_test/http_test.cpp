#include <stdio.h>

#if 1
#include "cv.h"
using namespace cv;
#endif

#include "opencv_stl.h"



#if 1
#include "HW_SDK_LIB/ipc.h"
#include "HW_SDK_LIB/live_set.h"
#include "HW_SDK_LIB/live_video.h"
#include "HW_SDK_LIB/dc_save.h"
#endif


#if 1
#include "SerialPort/Compass_HCM365.h"
#include "SerialPort/Gps_WG_8020.h"

#endif

#if 1
#include "libcurl/include/curl/curl.h"
#endif

/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
string ltos(long l)  
{  
	ostringstream os;  
	os<<l;  
	string result;  
	istringstream is(os.str());  
	is>>result;  
	return result;  

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
#include <stdio.h>
//#include <curl/curl.h>
bool getUrl(char *filename)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)  // 返回结果用文件存储
        return false;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init();    // 初始化
    if (curl)
    {
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        curl_easy_setopt(curl, CURLOPT_URL,"http://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件
        res = curl_easy_perform(curl);   // 执行
        if (res != 0) {

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        fclose(fp);
        return true;
    }
	return false;
}

bool postUrl(char *filename)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
        return false;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "&logintype=uid&u=xieyan&psw=xxx86");    // 指定post内容
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
        curl_easy_setopt(curl, CURLOPT_URL, " http://mail.sina.com.cn/cgi-bin/login.cgi ");   // 指定url
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    return true;
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/

static size_t getVersionCode(void *ptr, size_t size, size_t nmemb, void *userdata)
{//参数userdata是存放数据的指针  其他三个获取内容
 std::string *version = (std::string*)userdata;
 version->append((char*)ptr, size * nmemb);

 printf("%s",(char*)ptr);

 return (size * nmemb);
}

int main()
{	
	TimeCountStart();
	{
		 CURL* curl;
		 CURLcode res;

		 char buffer[10] ={0};

		 curl = curl_easy_init();//curl初始化
		 std::string _version;
		 if (curl)
		 {
		  curl_easy_setopt(curl,  CURLOPT_URL,"http://222.186.117.98/zjtdtproxy/proxy.aspx?postStr={'orig':'118.75105,32.06269','dest':'118.79192,32.03713','mid':'','style':'0'}&type=search");
		  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);//设为不验证证书
		  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getVersionCode);//设置处理数据的函数
		  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_version);//数据存储的对象指针
		  res = curl_easy_perform(curl);//curl链接
		  curl_easy_cleanup(curl);//清除curl

  
		 }
	}

	TimeCountStop("Threads Done : ");

	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/