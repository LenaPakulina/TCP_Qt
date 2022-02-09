#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QTime>

#define DATA_STREAM_VERSION (QDataStream::Qt_5_6)

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_socket = new QTcpSocket(this);
	m_nextBlockSize = 0;

	connect(ui->pb_connect,
			&QPushButton::clicked,
			this,
			&MainWindow::slotConnectionHandler);
	connect(ui->pb_sendMsg,
			&QPushButton::clicked,
			this,
			&MainWindow::slotSentToServer);
	connect(ui->le_msg,
			&QLineEdit::returnPressed,
			this,
			&MainWindow::slotSentToServer);
	connect(m_socket,
			&QTcpSocket::readyRead,
			this,
			&MainWindow::slotReadyRead);
	connect(m_socket,
			&QTcpSocket::disconnected,
			m_socket,
			&QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotSentToServer()
{
	m_data.clear();
	QDataStream out(&m_data, QIODevice::WriteOnly);
	out.setVersion(DATA_STREAM_VERSION);
	out << qint16(0)
		<< QTime::currentTime()
		<< ui->le_msg->text();
	out.device()->seek(0);
	out << qint16(m_data.size() - sizeof(qint16));
	m_socket->write(m_data);

	ui->le_msg->clear();
}

void MainWindow::slotConnectionHandler()
{
	m_socket->connectToHost("127.0.0.1", 1234);
}

void MainWindow::slotReadyRead()
{
	QDataStream in(m_socket);
	in.setVersion(DATA_STREAM_VERSION);
	if (in.status() == QDataStream::Ok) {
		while (true) { // бесстрашные
			if (m_nextBlockSize == 0) {
				if (m_socket->bytesAvailable() < 2) {
					break;
				}
				in >> m_nextBlockSize;
			}
			if (m_socket->bytesAvailable() < m_nextBlockSize) {
				break;
			}
			QString msg;
			QTime time;
			in >> time >> msg;
			m_nextBlockSize = 0;
			ui->textBrowser->append(time.toString() + " " + msg);
		}
	} else {
		qDebug() << "Код ошибки: 003. Не удалось подготовить поток данных";
	}
}

