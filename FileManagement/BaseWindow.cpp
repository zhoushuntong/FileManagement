#include "BaseWindow.h"
#include "stdafx.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include<QBitmap>
#include <QFile>
#include <qdebug.h>

BaseWindow::BaseWindow(QWidget *parent)
	: QWidget(parent)
{
	//QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	//effect->setBlurRadius(15);
	//effect->setColor(Qt::black);
	//effect->setOffset(0, 0);
	//
	//setGraphicsEffect(effect);
	// FramelessWindowHint属性设置窗口去除边框;
	// WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

	// 设置窗口背景透明;
	setAttribute(Qt::WA_TranslucentBackground);
	
	// 关闭窗口时释放资源;
	/*setAttribute(Qt::WA_DeleteOnClose);*/
	// 初始化标题栏;
	initTitleBar();

	//窗口动画渐变
	QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
	animation->setDuration(500);
	animation->setStartValue(0);
	animation->setEndValue(1);
	animation->start();



	//QBitmap bmp(this->size());
	//bmp.fill();
	//QPainter p(&bmp);
	//p.setPen(Qt::NoPen);
	//p.setBrush(Qt::black);
	//p.drawRoundedRect(bmp.rect(), 20, 20);
	//setMask(bmp);
	//setStyleSheet("border-radius:10px;");
}

BaseWindow::~BaseWindow() 
{

}

void BaseWindow::initTitleBar()
{
	m_titleBar = new TitleBar(this);
	m_titleBar->move(-8, 10);
	m_titleBar->raise();
	m_titleBar->setWindowBorderWidth(2);
	//m_titleBar->setBackgroundColor(255, 255, 255);
	m_titleBar->setButtonType(MIN_BUTTON);
	//m_titleBar->setTitleWidth(this->width());
	 
	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void BaseWindow::paintEvent(QPaintEvent* event)
{
	//设置窗口背景色
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(10, 10, this->width() - 20, this->height() - 20);

	QPainter painter1(this);
	painter1.setRenderHint(QPainter::Antialiasing, true);
	painter1.fillPath(path, QBrush(QColor(247, 238, 214)));
	painter1.setPen(Qt::transparent);

	//阴影处理，
	QRect rect = this->rect();
	rect.setWidth(rect.width() - 1);
	rect.setHeight(rect.height() - 1);
	QColor color(0, 0, 0, 50);
	for (int i = 0; i<10; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
		color.setAlpha(150 - qSqrt(i) * 50);
		painter1.setPen(color);
		painter1.drawPath(path);
	}
	

	////圆角处理
	//QPainter painter2(this);
	//painter2.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
	//painter2.setBrush(QBrush(QColor(247, 238, 214)));
	////painter2.setPen(Qt::transparent);
	//QRect rect = this->rect();
	//rect.setWidth(rect.width() - 15);
	//rect.setHeight(rect.height() - 15);
	//rect.moveTo(10, 0);
	////rect.setBottom(rect.bottom() - 15);
	////rect.setLeft(rect.left() - 15);
	////rect.setLeft(rect.height() - 10);
	////rect.setRight(rect.height() - 10);
	////painter2.drawRect(15, 15, this->width() - 20, this->height() - 20);
	//painter2.drawRoundedRect(rect, 15, 15);
	////也可用QPainterPath 绘制代替 painter.drawRoundedRect(rect, 15, 15);
	//{
	//	//QPainterPath painterPath;
	//	//painterPath.addRect(10, 10, this->width() - 20, this->height() - 20);
	//	//painterPath.addRoundedRect(rect, 15, 15);
	//	//painter2.drawPath(painterPath);
	//}

	return QWidget::paintEvent(event);
}

void BaseWindow::loadStyleSheet(const QString &sheetName)
{
	QFile file(":/Resource/qss/"+sheetName + ".qss");
	qDebug() << sheetName + ".qss";
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{

		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	else
		qDebug() << "is not qss";
}

void BaseWindow::onButtonMinClicked()
{
	showMinimized();
}

void BaseWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	m_titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void BaseWindow::onButtonMaxClicked()
{
	m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(FactRect);
}

void BaseWindow::onButtonCloseClicked()
{
	close();
}