#include "QtThreadClientLifeCycle.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
const QString QtThreadClientLifeCycle::KEY_SERNO	=	"serno";
const QString QtThreadClientLifeCycle::KEY_SERNO_0	=	"serno0";
const QString QtThreadClientLifeCycle::KEY_SERNO_1	=	"serno1";
const QString QtThreadClientLifeCycle::KEY_SERNO_2	=	"serno2";
const QString QtThreadClientLifeCycle::KEY_SERNO_3	=	"serno3";
const QString QtThreadClientLifeCycle::KEY_STATUS	=	"status";
const QString QtThreadClientLifeCycle::KEY_DEADLINE	=	"deadline";
const QString QtThreadClientLifeCycle::KEY_IP_ADDR	=	"ip";
const QString QtThreadClientLifeCycle::KEY_ON_LINE = "online";
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadClientLifeCycle::QtThreadClientLifeCycle()
{
#if 1
	this->mIpAddr = BORD_VIDEO_IN_SHORT;
#else
	this->mIpAddr = "127.0.0.1";
#endif

	this->mPort = 5678;
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadClientLifeCycle::~QtThreadClientLifeCycle()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientLifeCycle::run()
{

	if (M_THREAD_RUN){

		this->connect2ServerIfNoConnected();
		
		if (1) {
			this->FactorySetting();
		}else {
			this->UpdateDeadline("");
		}

		this->close_destory_socket_4_server();
	
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientLifeCycle::FactorySetting()
{
	
	while (M_THREAD_RUN) {
	
						read_n_byte(1);

						int Ctrl_t = getByteTcpRead();

						if (Ctrl_t == 0xffffffff) {
							//error
							break;
						}else if (Ctrl_t == 9) {
							//
							QString cmd = this->GetMagicOnlyJson();
							this->SendParamJson(cmd);

						}else if (Ctrl_t == 1) {
							//
							this->SendCmd2SetParam(LC_CMD_SET_SERNO);
							this->SendCmdEndClose();

						}else {
							break;
						}
	
	
	}	
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::GetFactorySettingCmd()
{	
	return  GetFactorySettingCmd("",0,0,0,0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientLifeCycle::SendParamJson(QString _param)
{
	QByteArray _param_qba = _param.toLatin1();
	const int LEN = _param_qba.length();
	
	const char *BODY = _param_qba.data();
	
	QByteArray QLEN = QtThreadClientLifeCycle::Int2Uchar4BigEndian(LEN);


	ResultMy result_my_t= write_n_byte(QLEN.data(), QLEN.size());
	
	if (result_my_t == ResultMy::TRUE_MY) {
			
			result_my_t=write_n_byte(BODY, LEN);
				
				if (result_my_t == ResultMy::TRUE_MY) {

						return TRUE;
				
				}

	}

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientLifeCycle::SendCmd2SetParam(char _cmd)
{

	ResultMy result_my_t =write_n_byte(&_cmd,sizeof(_cmd));

	if (result_my_t==ResultMy::TRUE_MY){
		return TRUE;

	}

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::GetMagicOnlyJson()
{
	QJsonDocument qjd_t;
	QJsonObject qjo_t;

	qjo_t.insert("magic0", 55555);
	qjo_t.insert("magic1", 66666);
	qjo_t.insert("magic2", 77777);
	qjo_t.insert("magic3", 88888);

	qjd_t.setObject(qjo_t);

	return qjd_t.toJson();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::GetUpdateDeadlineSettingCmd()
{
	QJsonDocument qjd_t;
	QJsonObject qjo_t;
	
	qjo_t.insert(QtThreadClientLifeCycle::KEY_DEADLINE	,	1000);
	qjo_t.insert(QtThreadClientLifeCycle::KEY_SERNO	,	123456789);
	
	qjd_t.setObject(qjo_t);
	return qjd_t.toJson();
}
/*-------------------------------------*/
/**

	
*
*/
/*-------------------------------------*/
int QtThreadClientLifeCycle::SendCmdEndClose()
{
	this->SendCmd2SetParam(LC_CMD_END);
	this->SendCmd2SetParam(LC_CMD_CLOSE);

	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientLifeCycle::UpdateDeadline(QString _cmd_json)
{

	while (M_THREAD_RUN) {

		read_n_byte(1);

		int Ctrl_t = getByteTcpRead();

		if (Ctrl_t == 0xffffffff) {
			//error
			qDebug() << "remote is closed !" << endl;
			break;
		}else if (Ctrl_t == 9) {
			//
			qDebug() <<"Rcv[9]: Send Param !" <<endl;
			this->SendParamJson(_cmd_json);

		}else if (Ctrl_t == 1) {
			//
			qDebug() <<"Rcv[1]: Send Cmd !" << endl;
			this->SendCmd2SetParam(LC_CMD_SET_DEADLINE);
			this->SendCmdEndClose();

		}
		else {
			qDebug() << "Rcv[x]: Error Cmd !" << endl;
			break;
		}


	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientLifeCycle::run_factorySetting()
{
	if (M_THREAD_RUN) {

		this->connect2ServerIfNoConnected();
				
		this->FactorySetting();
	
		this->close_destory_socket_4_server();

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientLifeCycle::run_updateDeadline()
{

	QStringList qsl_t = OpenFileDialog2SelectCfgFile(NULL, "open serial number file");
	const QString file_full_path_t = qsl_t.at(0);
	QString serial_number_t = LoadSerialNumber(file_full_path_t);
	QJsonDocument qjd_t = QJsonDocument::fromJson(serial_number_t.toLatin1());

	QString ip_addr_t = GetIpAddr(&qjd_t);

	if (ip_addr_t.isEmpty()){
		qDebug() << "ip addr is empty !" << endl;
		return;
	}

	this->SetIpAddr(ip_addr_t);

	if (M_THREAD_RUN) {

		this->connect2ServerIfNoConnected();				

				if (qsl_t.size()>0){
												
						if (!serial_number_t.isEmpty()){
							
								if (this->CheckDeadLine(&qjd_t)){

									this->UpdateDeadline(serial_number_t);
									RemoveDeadLine(&qjd_t);
									SaveSerialNumber_ex(file_full_path_t,qjd_t);
									qDebug() << "/*---------------*/" << endl;
									qDebug() << "set deadline  done !" << endl;
									qDebug() << "/*---------------*/" << endl;

								}else {
									qDebug() << "serno is invalid !" << endl;
								}

					
						}else{							
							qDebug() << "serno is empty !" << endl;							
						}
				

				}

		
				this->close_destory_socket_4_server();

		

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::GetFactorySettingCmd(
	QString _ip_addr,
	int _serno_0,
	int _serno_1,
	int _serno_2,
	int _serno_3)
{
	QJsonObject argsJson;
	QJsonDocument argsJsonDoc;

	if (_serno_0+_serno_1+_serno_2+_serno_3==0){		
		qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		_serno_0 = qrand();
		_serno_1 = qrand();
		_serno_2 = qrand();
		_serno_3 = qrand();
	}else {
		if (_serno_0 == 0) _serno_0=1;
		if (_serno_1 == 0) _serno_1= _serno_0 + 1;
		if (_serno_2 == 0) _serno_2= _serno_1 + 1;
		if (_serno_3 == 0) _serno_3= _serno_2 + 1;
	
	}

	argsJson.insert(KEY_SERNO_0, _serno_0);
	argsJson.insert(KEY_SERNO_1, _serno_1);
	argsJson.insert(KEY_SERNO_2, _serno_2);
	argsJson.insert(KEY_SERNO_3, _serno_3);

	argsJson.insert(KEY_STATUS, "init");
	argsJson.insert(KEY_DEADLINE,365*12);

	argsJson.insert(KEY_IP_ADDR, _ip_addr);

	argsJson.insert(KEY_ON_LINE, 0);

	qDebug() << argsJson;

	argsJsonDoc.setObject(argsJson);

	return argsJsonDoc.toJson(QJsonDocument::Compact);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool QtThreadClientLifeCycle::SaveStringMy(QString _path, QString _str)
{

	QFile saveFile(_path);
	
	if (!saveFile.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
		return false;
	}

	saveFile.write(_str.toLatin1());

	saveFile.flush();
	saveFile.close();

	return true;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QVector<QString> QtThreadClientLifeCycle::LoadStringMy(QString _path)
{
	
	QFile file(_path);

	QVector<QString> v_str_t;

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
	}



	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString str(line);
		//qDebug() << str;
		v_str_t.append(str);
	}
	

	return 	v_str_t;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool QtThreadClientLifeCycle::SaveSerialNumber(QString _ip, QString _str)
{
	std::string time_std_str_t=QBase::SYS_getCurrentTime("yyyyMMddhhmmssdd");

	QString filefullpath_t = _ip.append(".")
							.append(time_std_str_t.c_str()).append(".")
							.append(KEY_SERNO);

	return SaveSerialNumber_ex(filefullpath_t, _str);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool QtThreadClientLifeCycle::SaveSerialNumber_ex(QString _path, QString _str)
{	
	QString filefullpath_t = _path;
	
	QString CRYPTOGRAPH_T = QtThreadClientLifeCycle::GetJsonKey(_str).toLatin1().toBase64();

	QString  BODY_T = _str.toLatin1().toBase64();
	
	QString  data = BODY_T + QString("\n") + QString(CRYPTOGRAPH_T);

	return QtThreadClientLifeCycle::SaveStringMy(filefullpath_t, data);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool QtThreadClientLifeCycle::SaveSerialNumber_ex(QString _path, QJsonDocument _jdoc)
{
	QString json_str_t=_jdoc.toJson(QJsonDocument::Compact);
	return SaveSerialNumber_ex(_path,json_str_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::LoadSerialNumber(QString _path)
{	
	QString  CRYPTOGRAPH_T;
	QString  BODY_T;

	QVector<QString> q_str_t=LoadStringMy(_path);

	if (q_str_t.size()>=2)
	{
		BODY_T = QByteArray::fromBase64(q_str_t.at(0).toLatin1());
		CRYPTOGRAPH_T= QByteArray::fromBase64(q_str_t.at(1).toLatin1());
		
		if (QtThreadClientLifeCycle::GetJsonKey(BODY_T).compare(CRYPTOGRAPH_T)!=0) {
				BODY_T.clear();
				qDebug() << _path << endl;
				qDebug() << "Check Error !" << endl;
		}


	}

	return BODY_T;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QByteArray QtThreadClientLifeCycle::Int2Uchar4BigEndian(int _number)
{
	QByteArray abyte0;
	abyte0.resize(4);

	abyte0[3] = (uchar)(0x000000ff & _number);
	abyte0[2] = (uchar)((0x0000ff00 & _number) >> 8);
	abyte0[1] = (uchar)((0x00ff0000 & _number) >> 16);
	abyte0[0] = (uchar)((0xff000000 & _number) >> 24);

	return abyte0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::GetJsonKey(QString _str)
{
	QString str_t = _str.remove('{').remove('}');

	QByteArray byteArray=str_t.toLatin1();
	
	QByteArray hashKey = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);

	return hashKey.toHex();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QJsonDocument* QtThreadClientLifeCycle::ChangeDeadLine(QJsonDocument* _jsonDoc, QString _key, int _value)
{
	if (_jsonDoc->isEmpty())
		return _jsonDoc;
	
	QJsonObject rootObj = _jsonDoc->object();
	
	int use_key_t=-1;

	QString key_str[4] = {	QtThreadClientLifeCycle::KEY_SERNO_0,
							QtThreadClientLifeCycle::KEY_SERNO_1,
							QtThreadClientLifeCycle::KEY_SERNO_2,
							QtThreadClientLifeCycle::KEY_SERNO_3 };

	for (int i = 0; i < 4; i++) {
		if (rootObj.contains(key_str[i])) {
			QJsonValue value = rootObj.value(key_str[i]);
			if (value.isDouble()) {
				use_key_t = value.toInt();
				break;
			}
		}
	}

	if (use_key_t>=0){
		rootObj.insert(_key, _value);
		rootObj.insert(KEY_SERNO, use_key_t);
		rootObj.insert(KEY_STATUS,"setting");
	}

	_jsonDoc->setObject(rootObj);

	return _jsonDoc;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QJsonDocument * QtThreadClientLifeCycle::RemoveDeadLine(QJsonDocument * _jsonDoc)
{
	if (_jsonDoc->isEmpty())
		return _jsonDoc;

	QJsonObject rootObj = _jsonDoc->object();

	int serno_t = -1;

	QString key_str[4] = { QtThreadClientLifeCycle::KEY_SERNO_0,
		QtThreadClientLifeCycle::KEY_SERNO_1,
		QtThreadClientLifeCycle::KEY_SERNO_2,
		QtThreadClientLifeCycle::KEY_SERNO_3 };

	if (rootObj.contains(QtThreadClientLifeCycle::KEY_SERNO)) {
		QJsonValue value = rootObj.value(QtThreadClientLifeCycle::KEY_SERNO);
		if (value.isDouble()) {
			serno_t = value.toInt();
		}
		rootObj.remove(QtThreadClientLifeCycle::KEY_SERNO);
	}


	if (serno_t>=0){

			for (int i = 0; i < 4; i++) {
					if (rootObj.contains(key_str[i])) {
							QJsonValue value = rootObj.value(key_str[i]);
								if (value.isDouble()) {
										int serno_n_t = value.toInt();
										if (serno_n_t==serno_t){
											rootObj.remove(key_str[i]);
											rootObj.insert(key_str[i] +"used", serno_n_t);
										}

								}
					}
			}
		
	}

	

	

	_jsonDoc->setObject(rootObj);

	return _jsonDoc;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QStringList QtThreadClientLifeCycle::OpenFileDialog2SelectCfgFile(QWidget *_parent,QString _title)
{
	//定义文件对话框类
	QFileDialog *fileDialog = new QFileDialog(_parent);
	//定义文件对话框标题 
	fileDialog->setWindowTitle(tr(_title.toStdString().data()));
	//设置默认文件路径    
	fileDialog->setDirectory(QCoreApplication::applicationDirPath());
	
	//设置文件过滤器    
	fileDialog->setNameFilter(tr("serial(*.serno)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	//设置视图模式    
	fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径    
	QStringList fileNames;

	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
	}

	return fileNames;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientLifeCycle::CheckDeadLine(QJsonDocument * _jsonDoc)
{

	if (_jsonDoc->isEmpty())
		return FALSE;

	QJsonObject rootObj = _jsonDoc->object();

	int serno_t = -1;

	QString key_str[4] = { QtThreadClientLifeCycle::KEY_SERNO_0,
		QtThreadClientLifeCycle::KEY_SERNO_1,
		QtThreadClientLifeCycle::KEY_SERNO_2,
		QtThreadClientLifeCycle::KEY_SERNO_3 };

	if (rootObj.contains(QtThreadClientLifeCycle::KEY_SERNO)) {
		QJsonValue value = rootObj.value(QtThreadClientLifeCycle::KEY_SERNO);
		if (value.isDouble()) {
			serno_t = value.toInt();
		}
		
	}


	if (serno_t >= 0) {

		for (int i = 0; i < 4; i++) {
			if (rootObj.contains(key_str[i])) {
				QJsonValue value = rootObj.value(key_str[i]);
				if (value.isDouble()) {
					int serno_n_t = value.toInt();
					if (serno_n_t == serno_t) {
						return TRUE;
					}

				}
			}
		}

	}

	return FALSE;


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadClientLifeCycle::GetIpAddr(QJsonDocument * _jsonDoc)
{
	QJsonObject rootObj = _jsonDoc->object();
	
	const QString key = KEY_IP_ADDR;
	QString value_t;

		if (rootObj.contains(key)) {
		QJsonValue value = rootObj.value(key);
		if (value.isString()) {
			value_t = value.toString();
		}
	}
		return value_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/