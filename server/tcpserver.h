#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class QTcpSocket;

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	TcpServer(QObject *parent = nullptr);

private:
	QTcpSocket				*m_socket;
	QList<QTcpSocket*>		m_socketList;	// список подключенных клиентов
	QByteArray				m_data;			// данные для путешествия Клиент -> Сервер
	qint16					m_nextBlockSize;

private:
	void	sentToClient(const QString& msg);

public slots:
	void	slotIncomingConnection(qintptr socketDescriptor);
	void	slotReadyRead();
};

#endif // TCPSERVER_H
