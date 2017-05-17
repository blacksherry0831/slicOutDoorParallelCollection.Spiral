#include "stdafx.h"
#include "module_all_cui.h"

int main()
{
#if 1
	std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}";
	
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strValue, value))
	{
		std::string out = value["key1"].asString();
		std::cout << out << std::endl;
		const Json::Value arrayObj = value["array"];
		for (int i=0; i<arrayObj.size(); i++)
		{
			out = arrayObj[i]["key2"].asString();
			std::cout << out;
			if (i != arrayObj.size()-1 )
				std::cout << std::endl;
		}
	}

#endif
	return 0;


}