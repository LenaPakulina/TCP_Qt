#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTcpSocket;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QTcpSocket			*m_socket;
	QByteArray			m_data;
	qint16				m_nextBlockSize;

public slots:
	void	slotSentToServer();
	void	slotConnectionHandler();
	void	slotReadyRead();
};
#endif // MAINWINDOW_H
