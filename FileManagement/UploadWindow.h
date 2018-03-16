#pragma once

#ifndef UPLOADWINDOW_H_
#define UPLOADWINDOW_H_
#include "stdafx.h"
#include <QWidget>  
#include <QFile>  
#include <QTime>
#include <QTGui>
#include <string>  
#include <QStackedLayout>
#include <QFileDialog>
#include <QProgressBar>
#include "UploadFile.h"
#include "ui_UploadWindow.h"
#include "UploadThread.h"
//�����

extern QString globalUserName;
extern QString ip;
extern int port;
class UploadWindow : public QWidget
{
	Q_OBJECT

public:
	explicit UploadWindow(QWidget *parent = 0);
	~UploadWindow();
	void init();
	void initWindow();
	//int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  
	void initFile();
	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);
	// �ļ��ϷŹ���ʵ��
	void dragEnterEvent(QDragEnterEvent* event = NULL);
	void dropEvent(QDropEvent* event = NULL);
	void setFileIcon(QString);  //����׺�Ž�ȥ
	void insertFile(QString);  //�����ļ�����
							   //��ȡ�ļ���ͼ��
	QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	QString fileType(const QString &extension) const;
	QString countFileSize(QString);


private:
	Ui::UploadWindow *ui;

	QMap<int, UploadFile*> *uploadFileMap;  //����һ���µ�UploadFileMap
	QMap<int, QString> fileNameMap;
	//������Ҫ��Ҫѭ����ʾ
	QMap<int,QProgressBar*> *fileProgressBarMap;  //QString�ǹؼ��֣�
	//QMap<int, QThread*> *uploadQThreadMap;
	QMap<int, UploadThread*> *uploadQThreadMap;
	QMap<int, QLabel*> *fileSpeedMap;
	QMap<int, QHBoxLayout*> *fileStatusLayoutMap;
	//QMap<int, QLabel*> *uploadSuccessMap;
	int index;
	//QProgressBar fileProgressBar[6];  //����ϴ�5�����������ǵ�����
	//QStackedLayout *stackLayout;	//  �����fileWidget��fileLayout
	//QHBoxLayout *fileInfoLayout;	//��������ļ���Ϣ
	QWidget *fileWidget;  //��������ļ�����ͼ��
	QString fileName;  //�ļ���  

	QStringList fileNameList;
	QFile *localFile;  //��

	QVBoxLayout *fileSumLayout;
	QHBoxLayout *fileInfoLayout;
	QVBoxLayout *fileNameSizeLayout;
	QProgressBar *fileProgressBar;
	QHBoxLayout *fileStatusLayout;
	QWidget *fileWindow;
	QLabel *mFileName;
	QLabel *mFileIcon;  //�ļ�����ͼ���label
	QLabel *mFileSpeed;
	QLabel *mFileSize;
	QPushButton *mButtonWait;
	QPushButton *mButtonCancel;
	int row, column;
	UploadThread *fileThread;  //�ļ��߳�
	//QTcpSocket *tcpSocket;
	//QThread *uploadThread;

private slots:
	//void send();  //�����ļ�ͷ��Ϣ  
	//void goOnSend(qint64);  //�����ļ�����  
	void ClickOpenButton();
	void ClickSendButton();
	void receiveMainwindow();
	void updataProgressBar(int, qint64, qint64);
	void updateSpeedLabel(int,double);  //�����ٶ�
	void checkSendOver(int);

	void beginToSend(int);
signals:
	void sendFileSignal();	//�����ź�
	

};
#endif // !UPLOADWINDOW_H_ 


