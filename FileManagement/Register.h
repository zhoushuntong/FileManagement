#pragma once
#include "stdafx.h"
#include <QtWidgets/QDialog>
#include "ui_Register.h"
#include "BaseWindow.h"

extern TcpClient * tcp;
class Register : public BaseWindow
{
	Q_OBJECT

public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
	//void setWindowTitle(QString title, int titleFontSize = 10);  //设置标题栏

private:
	Ui::Register *ui;
	bool isAvailableUserName;
	bool isAvailablePassword;
	bool isAvailableStudentId;
	bool isokPassword;
	bool isOkTrueName;
	bool isOkTeacher;
	//void initTitleBar();  //初始化标题栏
	void init();

	//槽函数
public slots:
	void Click_Register();
	void Click_Cancel();

	void checkIsAvailableUserName();		//检查用户名是否重复
	void checkIsAvailableStudentId();    //检查学号是否重复
	void checkIsAvailablePassword();			//密码是否一致
	void checkIsokPassword();//密码格式是否正确


	void checkIsOkTrueName();
	void checkIsOkTeacher();

	void receiveLogin();  //接收login中发射的信号关联的槽函数

	void receiveDataFromClient(QString data);  //接受从服务器返回的数据

signals:
	void showMain();
	void showLogin();
};
