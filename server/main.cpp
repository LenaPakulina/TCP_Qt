#include <QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Rus");
	QCoreApplication a(argc, argv);
	TcpServer server;
	return a.exec();
}
