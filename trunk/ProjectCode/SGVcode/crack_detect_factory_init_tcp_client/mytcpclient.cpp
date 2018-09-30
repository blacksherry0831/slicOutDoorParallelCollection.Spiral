#include "mytcpclient.h"
#include "ui_mytcpclient.h"

MyTcpClient::MyTcpClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyTcpClient)
{
    ui->setupUi(this);

    //初始化TCP客户端
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
	connect(ui->action_import_serno, SIGNAL(triggered()), this, SLOT(on_action_load_serno()));
	connect(ui->action_cfg_deadline, SIGNAL(triggered()), this, SLOT(on_action_cfg_deadline()));

	mQIV = QSharedPointer<QIntValidator>(new QIntValidator(0,INT_MAX, this));

	ui->serno_0->setValidator(mQIV.data());
	ui->serno_1->setValidator(mQIV.data());
	ui->serno_2->setValidator(mQIV.data());
	ui->serno_3->setValidator(mQIV.data());

}

MyTcpClient::~MyTcpClient()
{
    delete ui;
}

void MyTcpClient::ReadData()
{
    //tcpClient->read(&cmd, 1);
    char cmd;
    char  ctl;
    int serno_0,serno_1,serno_2,serno_3;

    QByteArray byteArray;
  

    serno_0 = ui->serno_0->text().toInt();
    serno_1 = ui->serno_1->text().toInt();
    serno_2 = ui->serno_2->text().toInt();
    serno_3 = ui->serno_3->text().toInt();

    if(serno_0 == 0 || serno_1 == 0 || serno_2 == 0 || serno_3 == 0)
    {
      
		message_box_exec(tr(STR_SET_SERNO_INVALID));
      
    }
 
	const QString sern_str_t = QtThreadClientLifeCycle::GetFactorySettingCmd(
																	tcpClient->peerAddress().toString(),
																	serno_0,
																	serno_1,
																	serno_2,
																	serno_3);


	byteArray = sern_str_t.toLatin1();
	
    int number = byteArray.length();
    
	QByteArray abyte0 = QtThreadClientLifeCycle::Int2Uchar4BigEndian(number);
    


    if(tcpClient->bytesAvailable() > 0)
    {
        tcpClient->read(&cmd, 1);
        if(cmd == 9){
            tcpClient->write(abyte0);
            tcpClient->write(byteArray);
        }else if(cmd == 1){
            ctl = LC_CMD_SET_MAGIC;
			if (tcpClient->write(&ctl,sizeof(char)) == 1) {
			
			}
            ctl = LC_CMD_END;
            tcpClient->write(&ctl,sizeof(char));
            ctl = LC_CMD_CLOSE;
            tcpClient->write(&ctl,sizeof(char));
#if TRUE
					mJsonDoc = QJsonDocument::fromJson(sern_str_t.toLatin1());
					QtThreadClientLifeCycle::SaveSerialNumber(tcpClient->peerAddress().toString(), sern_str_t);
#endif
			message_box_exec(tr(STR_SET_SERNO_SUCCESS));
          
		}else{
			
			message_box_exec(tr(STR_SERVER_UNDEF_CMD));
			
		}

    }
}

void MyTcpClient::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
   
	message_box_exec(tr(STR_SOCKET_FAIL).arg(tcpClient->errorString()));
  
}

void MyTcpClient::on_pushButton_clicked()
{
    ui->serno_0->clear();
    ui->serno_1->clear();
    ui->serno_2->clear();
    ui->serno_3->clear();
    tcpClient->disconnectFromHost();
}

void MyTcpClient::on_cmd_serno_clicked()
{

    if(ui->serno_0->text() == "" || ui->serno_1->text() == ""
            || ui->serno_2->text() == ""|| ui->serno_3->text() == "")
    {
     
		message_box_exec(tr(STR_SET_SERNO_TIP));
     

    }
    else
    {
		
        tcpClient->connectToHost(ui->edtIP->text(), ui->edtPort->text().toInt());
        if (tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
        {
			mIpAddr = ui->edtIP->text();

        }else{
          
			message_box_exec(tr(STR_SOCKET_FAIL).arg(tcpClient->errorString()));
          
        }
    }
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MyTcpClient::message_box_exec(QString _msg)
{
	QMessageBox msgBox;
	msgBox.setText(_msg);
	msgBox.exec();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MyTcpClient::on_action_load_serno()
{	
	QStringList fileNames=QtThreadClientLifeCycle::OpenFileDialog2SelectCfgFile(this,"打开配置文件");  
			
	QString serno;
	
	if (fileNames.size()>0){

		serno= QtThreadClientLifeCycle::LoadSerialNumber(fileNames.at(0));

		if (!serno.isEmpty()) {
					mLoadFileName= fileNames.at(0);
					mJsonDoc = QJsonDocument::fromJson(serno.toLatin1());

					QJsonObject rootObj = mJsonDoc.object();


					QString key_str[4] = { QtThreadClientLifeCycle::KEY_SERNO_0,
						QtThreadClientLifeCycle::KEY_SERNO_1,
						QtThreadClientLifeCycle::KEY_SERNO_2,
						QtThreadClientLifeCycle::KEY_SERNO_3 };
					int  key_int[4] = { 0 };

					for (int i = 0; i < 4; i++) {

						
						if (rootObj.contains(key_str[i])) {
							QJsonValue value = rootObj.value(key_str[i]);
							if (value.isDouble()) {
								key_int[i] = value.toInt();

							}
						}


					}

					ui->serno_0->setText(QString::number(key_int[0]));
					ui->serno_1->setText(QString::number(key_int[1]));
					ui->serno_2->setText(QString::number(key_int[2]));
					ui->serno_3->setText(QString::number(key_int[3]));
		
		}


	}
	



	
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MyTcpClient::on_action_cfg_deadline()
{
	bool ok;
	
	
	if (mLoadFileName.isEmpty()) {
		
		this->message_box_exec(tr("尚未加载文件"));

	}else {
	
		int i = QInputDialog::getInt(this, tr("设置续期时间"),
			tr("续期天数:"), 90, 0, 365 * 3, 1, &ok);

		if (ok) {
			if (!mJsonDoc.isEmpty()) {

				QtThreadClientLifeCycle::ChangeDeadLine(&mJsonDoc,
					QtThreadClientLifeCycle::KEY_DEADLINE,
					i * 12);

				if (!mLoadFileName.isEmpty()) {
					QtThreadClientLifeCycle::SaveSerialNumber_ex(mLoadFileName, mJsonDoc);
				}
				mLoadFileName.clear();

			}


		}
	
	}


	
		



}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/