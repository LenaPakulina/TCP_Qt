#include "tcpserver.h"
#include <QDebug>
#include <QTcpSocket>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
	m_socket = nullptr;
	m_socketList.clear();
	m_data.clear();

	if (listen(QHostAddress::Any, 1234)) {
		qDebug() << "************* Сервер запущен *************";
	} else {
		qDebug() << "************* Не удалось запустить сервер *************";
	}
}

void TcpServer::sentToClient()
{

}

void TcpServer::slotIncomingConnection(qintptr socketDescriptor)
{
	m_socket = new QTcpSocket(nullptr);
	m_socket->setSocketDescriptor(socketDescriptor);

	connect(m_socket,
			&QTcpSocket::readyRead,
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

}
