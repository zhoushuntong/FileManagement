#include "UploadFile.h"  
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <Qtime>
#include <QtWin>
#include <QFileIconProvider>
#include <QStackedLayout>
#include "MyMessageBox.h"


//问题1： 用户拖曳了文件但是没有点击发送的话，服务器已经接收了“接收文件”的信号。这个时候
//就会出问题，服务器不能正常接收常规的命令。
int sendtimes = 0;
UploadFile::UploadFile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadFile)
{
	ui->setupUi(this);
	init();  //初始化
}

UploadFile::~UploadFile()
{
	delete ui;
}

void UploadFile::init()
{
	QStackedLayout *stackLayout = new QStackedLayout();
	//stackLayout->addWidget(ui->mFileIcon);
	//stackLayout->addWidget(ui->mFileName);
	//重叠显示
	stackLayout->addWidget(ui->horizontalWidget);
	stackLayout->addWidget(ui->sendProgressBar);
	ui->sendProgressBar->setTextVisible(false);   //不显示百分比
	stackLayout->setStackingMode(QStackedLayout::StackAll);
	ui->horizontalLayout_3->addLayout(stackLayout);

	//速度啊，标签什么的都先隐藏起来
	ui->uploadSpeedLabel->hide();
	ui->mFileIcon->hide();
	ui->mFileName->hide();
	ui->sendProgressBar->hide();


	setAcceptDrops(true);
	QPixmap DragDrop(":/Resource/FunIcon/Drag-Drop.png");  //用来放文件的图标
											 //Drag = fil.scaled(QSize(50, 60), Qt::KeepAspectRatio);
	ui->Drag->setPixmap(DragDrop);
	//ui->Drag->setAlignment(Qt::AlignCenter);
	//connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //当连接成功时，就开始传送文件 

	loadStyleSheet("UploadFile");  //

}
void UploadFile::loadStyleSheet(const QString &sheetName)
{
	QFile file("Resource/qss/" + sheetName + ".qss");
	qDebug() << sheetName + ".qss";
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		qDebug() << "is  css";
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	else
		qDebug() << "is not css";
}
void UploadFile::dragEnterEvent(QDragEnterEvent *event)
{
	//如果为文件，则支持拖放
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void UploadFile::dropEvent(QDropEvent *event)
{
	//注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
	//如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;
	//往文本框中追加文件名
	foreach(QUrl url, urls) {
		 fileName = url.toLocalFile();
		//ui->sendStatusLabel->setText(fileName);
		initFile();
		setFileIcon(fileName);  //设置文件图标 
		localFile = new QFile(fileName);
		localFile->open(QFile::ReadOnly);

		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("已打开文件 %1").arg(fileName));
	}
}

void UploadFile::initFile()
{
	//准备发文件的时候把这个类放进线程
	uploadThread = new QThread();
	this->moveToThread(uploadThread);
	uploadThread->start();

	tcpSocket = new QTcpSocket();
	tcpSocket->abort();   //取消已有的连接
	tcpSocket->connectToHost(ip, port);  //将这个socket连接到服务器

	tcpSocket->write("uploadFile");
	qDebug() << globalUserName << " will send a file to server";
	loadSize = 0;
	byteToWrite = 0;
	totalSize = 0;
	sendtimes = 0;
	ui->sendProgressBar->setValue(0);  //非第一次发送  
	outBlock.clear();
}
void UploadFile::ClickOpenButton()  //打开文件并获取文件名（包括路径）  
{
	//ui->sendStatusLabel->setText(QString::fromLocal8Bit("正在打开文件..."));
	//告诉服务器，我要发送文件了
	
	//点击打开按钮的时候初始化数据
	
	fileName = QFileDialog::getOpenFileName(this);
	if(!fileName.isEmpty())
	{
		initFile();
		localFile = new QFile(fileName);
		localFile->open(QFile::ReadOnly);
		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("已打开文件 %1").arg(fileName));
	}
	
}

void UploadFile::send()  //发送文件头信息  
{
	if (!fileName.isNull())
	{
		//第一次发送的时候建立连接
		connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		byteToWrite = localFile->size();  //剩余数据的大小  
		qDebug() << "the file bytetowrite: " << byteToWrite;
		totalSize = localFile->size();

		loadSize = 4 * 1024;  //每次发送数据的大小  

		QDataStream out(&outBlock, QIODevice::WriteOnly);
		//获取文件名字
		QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
		//占位符,这里必须要先这样占位，然后后续读算出整体长度后在插入
		//前面两个是文件大小和发送文件头的大小（为什么是qint64呢？），后面是文件名和用户名
		out << qint64(0) << qint64(0) << currentFileName << globalUserName;

		totalSize += outBlock.size();  //总大小为文件大小加上文件名等信息大小  
		byteToWrite += outBlock.size();
		qDebug() << "the total bytetowrite: " << byteToWrite;
		out.device()->seek(0);  //回到字节流起点来写好前面连个qint64，分别为总大小和文件名等信息大小  
		out << totalSize << qint64(outBlock.size());   //这个就是对应前面的qint64？
		qDebug() << "the file head:" << outBlock;
		tcpSocket->write(outBlock);  //将读到的文件信息发送到套接字  

		sendTime.start();  //发送时间开始计时
						   //UI信息
		//ui->progressLabel->show();
		ui->sendProgressBar->setMaximum(totalSize);
		ui->sendProgressBar->setValue(totalSize - byteToWrite);
	}
	else
	{
		//弹出请先添加文件提示！
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先添加文件!"), MESSAGE_INFORMATION, BUTTON_OK,true);
	}

}


//继续发送
void UploadFile::goOnSend(qint64 numBytes)  
{
	sendtimes++;  
	qDebug() << sendtimes<<" get in goOnSend";
		qDebug() << "the numBytes: " << numBytes <<"the loadSize:" <<loadSize
			<< "the " << sendtimes << "  left byteTowrite: " << byteToWrite;

	byteToWrite -= numBytes;  //剩余数据大小  
	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //如果剩余数据比每次发送的小则发送剩余的

	tcpSocket->write(outBlock);    //将这个信息写入socket
	
	//qDebug() << "information:" <<outBlock;

	//这里是速度处理，显示速度和已下载
	float useTime = sendTime.elapsed();
	double speed = (totalSize - byteToWrite) / useTime;
	ui->uploadSpeedLabel->show();
	ui->uploadSpeedLabel->setText(QString::fromLocal8Bit("已发送 %1MB (%2MB/s) 共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
		.arg((totalSize - byteToWrite) / (1024 * 1024))//已接收
		.arg(speed * 1000 / (1024 * 1024), 0, 'f', 2)//速度
		.arg(totalSize / (1024 * 1024))//总大小
		.arg(useTime / 1000, 0, 'f', 0)//用时
		.arg(totalSize / speed / 1000 - useTime / 1000, 0, 'f', 0));//剩余时间


	//设置UI的进度条，这个可以考虑隐藏起来
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);

	if (byteToWrite == 0)  //发送完毕  
	{
		//发送完毕且点击了点击了确定按钮。
		if (!MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("上传完成!"), MESSAGE_INFORMATION, BUTTON_OK, true))
		{
			ui->sendBtn->setEnabled(true);
		}
		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("文件发送完毕!"));
		//发送完毕就断开这个写字节的槽函数。
		disconnect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		sendtimes = 0;

		//发送完后取消连接，把线程退出？
		uploadThread->quit();
		tcpSocket->abort();
		qDebug() << "the file had send";
	}
}


void UploadFile::ClickSendButton()
{
	send();  //第一次发送的时候是由connectToHost出发connect信号才能调用send，第二次之后就需要调用send了  
	ui->sendBtn->setEnabled(false);
	//connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
	//ui->sendStatusLabel->setText(QString::fromLocal8Bit("正在发送文件 %1").arg(fileName));
}


void UploadFile::setFileIcon(QString fileName)
{

	QString suffix = (fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)).left(3); //获取文件后缀前三个字符
	QString allName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1); //获取文件名字
	qDebug() << "the suffix :" << suffix << " and the allName:" << allName;
		// 获取图标、类型
	QIcon icon = fileIcon(suffix);
	QString strType = fileType(suffix);
	qDebug() << strType;
	QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(30, 40)));
	//QPixmap fileIcon(icon.);  //用来放文件的图标
	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
	ui->mFileIcon->setPixmap(pixmap);
	ui->mFileIcon->setAlignment(Qt::AlignCenter);
	//QPixmap fileIcon("Resource/fileIcon/" + suffix);  //用来放文件的图标
	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
	//ui->mFileIcon->setPixmap(fileIcon);
	//ui->mFileIcon->setAlignment(Qt::AlignCenter);
	ui->mFileName->setText(allName);
	ui->mFileIcon->show();
	ui->mFileName->show();
	//放文件的时候重置速度和标签
	ui->uploadSpeedLabel->hide();
	ui->sendProgressBar->show();
}

void UploadFile::receiveMainwindow()
{
	this->show();
}

QIcon UploadFile::fileIcon(const QString &extension) const
{
	QFileIconProvider provider; 
	QIcon icon;
	QString strTemplateName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strTemplateName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		tmpFile.close();
		icon = provider.icon(QFileInfo(fileName));
		// tmpFile.remove();
	}
	else
	{
		qDebug() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return icon;
}

QString UploadFile::fileType(const QString &extension) const
{
	QFileIconProvider provider;
	QString strType;
	QString strFileName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strFileName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		tmpFile.close();
		strType = provider.type(QFileInfo(tmpFile.fileName()));
		// tmpFile.remove();
	}
	else
	{
		qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return strType;
}


////画图，发送一个总数和当前的进度
//void UploadFile::drawProgress(qint64 total, qint64 now)
//{
//
//}