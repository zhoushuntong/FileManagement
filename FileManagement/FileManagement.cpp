#include "FileManagement.h"

#include "DownloadFile.h"
#include <QMessageBox> 
#include <QLabel>
#include <QMovie>
#include <QThread>
#include "MyMessageBox.h"
#include "SignShow.h"
#include <QIcon>
#include <QPixmap>
#include "Sign.h"
extern QString globalUserName;
FileManagement::FileManagement(QWidget *parent)
	: BaseWindow(parent), ui(new Ui::FileManagement)
{
	ui->setupUi(this);
	init();
	StackWindow();
	//initTitleBar();
	loadStyleSheet("mainWindow");
	connect(ui->SignButton, SIGNAL(clicked()), this, SLOT(sendSignToServer()));
	connect(ui->LookSignButton, SIGNAL(clicked()), this, SLOT(showSignList()));
	connect(ui->LookMySignButton, SIGNAL(clicked()), this, SLOT(showMySign()));
	connect(tcp, SIGNAL(sendDataToSign(QString)), this, SLOT(receiveDataFromClient(QString)));
}

FileManagement::~FileManagement()
{
	delete ui;
}
void FileManagement::sendSignToServer()
{
	QString data = "InsertSign#" + globalUserName;
	QByteArray datasend = data.toUtf8();
	qDebug() << datasend;
	tcp->tcpSocket->write(datasend);


}

void FileManagement::showSignList()
{
	Sign *sign = new Sign(this);
	sign->move(300, 300);
	sign->show();
}

void FileManagement::showMySign()
{
	SignShow *sign = new SignShow();
	sign->move(300, 300);
	sign->show();

}
void FileManagement::receiveDataFromClient(QString data)
{
	if (QString::compare(data, "sign_T") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("签到成功!"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);

	}
	else if (QString::compare(data, "repeat") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("今天已经签到,请勿重复签到!"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
	}
	else if (QString::compare(data, "notInLab") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请回实验室签到!"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
	}
	else if (QString::compare(data, "sign_F") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("签到失败!"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
	}
}

void FileManagement::init()
{

	ui->titleLabel->setStyleSheet("background:transparent");//设置label透明色
	ui->titleLabel->setStyleSheet("color:rgb(140, 95, 255)");//label字体颜色为蓝色
	QPalette Pal(palette());
	// set black background
	Pal.setColor(QPalette::Background, Qt::gray);
	ui->ToolButtonWidget->setAutoFillBackground(true);
	ui->ToolButtonWidget->setPalette(Pal);
	//ui->ToolButtonWidget->setStyleSheet("backgroud-color : black;");

	QIcon personico(":/Resource/FunIcon/userInfo.png"); //创建QIcon对象
	ui->personButton->setIcon(personico); //将图片设置到按钮上
	ui->personButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->personButton->setStyleSheet("border:none");   //隐藏button的边框线(true);
													  //ui->personButton->setStyleSheet("QToolButton{image-align : left;}");

	ui->personButton->setCheckable(true);

	ui->personButton->setAutoExclusive(true);

	//ui->personButton->setAlignment(Qt::AlignLeft);
	//ui->personLayout->setAlignment(Qt::AlignLeft);
	//toolbutton添加图片
	QIcon uploadico(":/Resource/FunIcon/upload.png"); //创建QIcon对象
	ui->uploadButton->setIcon(uploadico); //将图片设置到按钮上
	ui->uploadButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->uploadButton->setStyleSheet("border:none");   //隐藏button的边框线(true);
	ui->uploadButton->setCheckable(true);

	ui->uploadButton->setAutoExclusive(true);

	QIcon downloadico(":/Resource/FunIcon/download.png"); //创建QIcon对象
	ui->downloadButton->setIcon(downloadico); //将图片设置到按钮上
	ui->downloadButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->downloadButton->setStyleSheet("border:none");

	//检查check状态
	ui->downloadButton->setCheckable(true);
	ui->downloadButton->setAutoExclusive(true);

	QIcon chatico(":/Resource/FunIcon/showFile.png"); //创建QIcon对象
	ui->showFileButton->setIcon(chatico); //将图片设置到按钮上
	ui->showFileButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->showFileButton->setStyleSheet("border:none");
	ui->showFileButton->setCheckable(true);

	ui->showFileButton->setAutoExclusive(true);

	QIcon deleteico(":/Resource/FunIcon/exit.png"); //创建QIcon对象
	ui->deleteButton->setIcon(deleteico); //将图片设置到按钮上
	ui->deleteButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->deleteButton->setStyleSheet("border:none");
	ui->deleteButton->setCheckable(true);
	ui->deleteButton->setAutoExclusive(true);

	QIcon reportEditicon(":/Resource/FunIcon/reportEdit.png"); //创建QIcon对象
	ui->reportEditButton->setIcon(reportEditicon); //将图片设置到按钮上
	ui->reportEditButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->reportEditButton->setStyleSheet("border:none");
	ui->reportEditButton->setCheckable(true);
	ui->reportEditButton->setAutoExclusive(true);

	QIcon reportLookicon(":/Resource/FunIcon/reportLook.png"); //创建QIcon对象
	ui->reportLookButton->setIcon(reportLookicon); //将图片设置到按钮上
	ui->reportLookButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->reportLookButton->setStyleSheet("border:none");
	ui->reportLookButton->setCheckable(true);
	ui->reportLookButton->setAutoExclusive(true);

	QIcon moneyManageicon(":/Resource/FunIcon/moneyManage.png"); //创建QIcon对象
	ui->moneyButton->setIcon(moneyManageicon); //将图片设置到按钮上
	ui->moneyButton->setIconSize(QSize(80, 20));//根据实际调整图片大小
	ui->moneyButton->setStyleSheet("border:none");
	ui->moneyButton->setCheckable(true);
	ui->moneyButton->setAutoExclusive(true);

	//QMovie *movie = new QMovie(":/Resource/MainWindow.gif");//加载图片gif
	//ui->photoLabel->setGeometry(500, 20, 200, 0);
	//movie->setScaledSize(QSize(900, 580));//设置图片大小
	//ui->photoLabel->setMovie(movie);
	//	movie->start();
	setWindowIcon(QIcon(":/Resource/icon1.png"));//设置窗口左上角图标
	setFixedSize(1000, 700); // 禁止改变窗口大小
	ui->personButton->setToolTip(tr("upload your photo"));//点击按钮提示相应信息
	ui->uploadButton->setToolTip(tr("upload"));
	ui->downloadButton->setToolTip(tr("download"));
	ui->showFileButton->setToolTip(tr("chat"));
	ui->deleteButton->setToolTip(tr("退出系统"));



	

	////自定义退出按钮
	//QToolButton *closeButton = new QToolButton(this);
	//QPixmap quitPixmap = QPixmap(":/Resource/icon2.png");
	//closeButton->setStyleSheet("QToolButton{border:none}");
	//closeButton->setFixedSize(QSize(20, 20));
	//closeButton->setIcon(quitPixmap);
	//closeButton->setGeometry(this->width() - 20, 0, 20, 20);
	//connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));


	//设置窗口背景色
	/*QPalette  palette(this->palette());
	palette.setColor(QPalette:: Background, QColor(255, 255,255, 255));
	this->setPalette(palette);*/
	//设置标题label字体颜色
	//ui.titleLabel->setStyleSheet("background-color:rgb(255, 255, 255)");//label背景颜色为白色
	//

	//this->setAttribute(Qt::WA_TranslucentBackground);
	////边框阴影效果
	//QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	//effect->setBlurRadius(6);
	//effect->setColor(Qt::black);
	//effect->setOffset(0, 0);
	//this->setGraphicsEffect(effect);
	//this->update();
}
//
//void FileManagement::initTitleBar()
//{ 
//	//m_titleBar->move(1, 2);
//	//m_titleBar->raise();
//	//m_titleBar->setWindowBorderWidth(2);
//	//m_titleBar->setBackgroundColor(255, 255, 255);
//	//m_titleBar->setButtonType(MIN_BUTTON);
//	//m_titleBar->setTitleWidth(this->width());
//}

//窗口重叠
void FileManagement::StackWindow()
{
	m_pStackedLayout = new QStackedLayout();
	
	//这里主要是多个窗口切换，并把这些类都实例化
	uploadWindow = new UploadWindow();
	downloadWindow = new DownloadWindow();
	downloadManage = new DownloadManage();
	userInformation = new UserInformation();
	reportEdit = new ReportEdit();
	reportLook = new ReportLook();
	moneyManage = new MoneyManage();

	WindowSlotBind();
	//添加类
	m_pStackedLayout->addWidget(uploadWindow);
	m_pStackedLayout->addWidget(downloadWindow);
	m_pStackedLayout->addWidget(downloadManage);
	m_pStackedLayout->addWidget(userInformation);
	m_pStackedLayout->addWidget(reportEdit);
	m_pStackedLayout->addWidget(reportLook);
	m_pStackedLayout->addWidget(moneyManage);

	ui->SubLayout->insertLayout(1, m_pStackedLayout);

}

void FileManagement::WindowSlotBind()
{
	//发送信号槽
	connect(downloadWindow, SIGNAL(addDownloadFile(QString, QString))
		, downloadManage, SLOT(insertDownloadFile(QString, QString)));
}
void FileManagement::receiveLogin()
{
	this->show();
}
void FileManagement::Btn_Click()
{
	QMessageBox msg;
	msg.setText("hello world!");
	msg.exec();
}


//点击上传文件按钮显示上传界面
void FileManagement::ClickUploadWindow()
{
	//准备发文件的时候把这个类放进线程
	//QThread *uploadThread = new QThread();
	//uploadFile->moveToThread(uploadThread);
	//uploadThread->start();

	m_pStackedLayout->setCurrentWidget(uploadWindow);

}

//点击下载文件按钮显示下载界面
void FileManagement::ClickDownloadWindow()
{
	m_pStackedLayout->setCurrentWidget(downloadWindow);
	downloadWindow->sendFileInfo();
}

void FileManagement::ClickDownloadManage()
{
	m_pStackedLayout->setCurrentWidget(downloadManage);
	
}

void FileManagement::ClickUserInformation()
{
	m_pStackedLayout->setCurrentWidget(userInformation);
	userInformation->sendUserInfo();
}

//点击ReportEdit
void FileManagement::ClickReportEdit()
{
	m_pStackedLayout->setCurrentWidget(reportEdit);
	reportEdit->sendReportEdit();
}

void FileManagement::ClickReportLook()
{
	m_pStackedLayout->setCurrentWidget(reportLook);
	reportLook->sendReportLook();
}

void FileManagement::ClickMoneyManage()
{
	m_pStackedLayout->setCurrentWidget(moneyManage);
	moneyManage->sendAccountLook();
}

void FileManagement::ClickReturn()
{
	this->close();
	emit showLogin();
}
