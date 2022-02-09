#include "tcpserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QTime>

#define DATA_STREAM_VERSION (QDataStream::Qt_5_6)

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
	m_socket = nullptr;
	m_socketList.clear();
	m_data.clear();
	m_nextBlockSize = 0;

	if (this->listen(QHostAddress::Any, 1234)) {
		qDebug() << "************* Сервер запущен *************";
	} else {
		qDebug() << "************* Не удалось запустить сервер *************";
	}
}

void TcpServer::sentToClient(const QString& msg)
{
	m_data.clear();
	QDataStream out(&m_data, QIODevice::WriteOnly);
	out.setVersion(DATA_STREAM_VERSION);
	out << qint16(0) << msg; // резервируем 2 байта сначала
	out.device()->seek(0);
	out << qint16(m_data.size() - sizeof(qint16));
	for (QTcpSocket *socket: m_socketList) {
		socket->write(m_data);
	}
}

void TcpServer::slotIncomingConnection(qintptr socketDescriptor)
{
	m_socket = new QTcpSocket(nullptr);
	m_socket->setSocketDescriptor(socketDescriptor);

	connect(m_socket,
			&QTcpSocket::readyRead,
			this,
			&TcpServer::slotReadyRead);
	connect(m_socket,
			&QTcpSocket::disconnected,
			m_socket,
			&QTcpSocket::deleteLater);

	m_socketList.append(m_socket);
	qDebug() << "---- Клиент подключен " << socketDescriptor;
}

void TcpServer::slotReadyRead()
{
	m_socket = (QTcpSocket*)sender();
	if (m_socket == nullptr) {
		qDebug() << "Код ошибки: 001. Кастование в nullptr";
		return;
	}

	QDataStream in(m_socket);
	in.setVersion(DATA_STREAM_VERSION);
	if (in.status() == QDataStream::Ok) {
		while (true) { // бесстрашные
			if (m_nextBlockSize == 0) {
				qDebug() << "m_nextBlockSize == 0";
				if (m_socket->bytesAvailable() < 2) {
					qDebug() << "m_nextBlockSize = " << m_nextBlockSize;
					break;
				}
				in >> m_nextBlockSize;
			}
			if (m_socket->bytesAvailable() < m_nextBlockSize) {
				qDebug() << "Data not full, break";
				break;
			}
			QString msg;
			QTime time;
			in >> time >> msg;
			m_nextBlockSize = 0;
			sentToClient(time.toString() + msg);
			break;
		}
	} else {
		qDebug() << "Код ошибки: 002. Не удалось подготовить поток данных";
	}
}
