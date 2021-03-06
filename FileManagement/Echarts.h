#ifndef Echarts_H
#define Echarts_H


#include <QWidget>
#include<QHBoxLayout>
#include "bdchart.h"
#include"stdafx.h"
extern TcpClient * tcp;    //全局变量，TCP 长连接
extern QString globalUserName;

class QWebEngineView;
class BDChart;
class WebSocketClientWrapper;
class QWebChannel;
class QWebSocketServer;

namespace Ui {
	class Echarts;
}
class Echarts : public QWidget
{
	Q_OBJECT
public:
	explicit Echarts(QWidget *parent = 0);
	~Echarts();
	//void onimportexcel();
	BDChart *mChartManager;
public slots:
	//void importexcel();
	void choosefilepath();
	//void exportexcel();
	void import_excel();
	void export_excel();
	void receiveDataFromMoney(QString);

signals:
	void signalTest(const QByteArray &message);
private:
	//void connects();

private:
	Ui::Echarts *ui;
	QWebEngineView *mWebView;
	
	WebSocketClientWrapper *mWebSocketClientWrapper;
	QWebChannel *channel;
	QWebSocketServer* mServer;
	QString dataToExcel;
};

#endif // Echarts_H
