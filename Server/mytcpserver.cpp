#include "mytcpserver.h"
#include "functionsforserver.h"
#include <QDebug>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server is not started";
    } else {
        qDebug() << "Server is started on port" << mTcpServer->serverPort();
    }
}

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();

    for (QTcpSocket* client : mClients.keys()) {
        client->disconnectFromHost();
        client->deleteLater();
    }
    mClients.clear();
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
    if (!clientSocket) return;

    mClients.insert(clientSocket, QString());

   // clientSocket->write("Hello, World!!! I am multi-client server!\r\n");

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    qDebug() << "New client connected from" << clientSocket->peerAddress().toString();
}

void MyTcpServer::slotServerRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket || !mClients.contains(clientSocket))
        return;

    QByteArray data = clientSocket->readAll();
    QString& buffer = mClients[clientSocket];

    buffer.append(QString::fromUtf8(data));

    // Завершение команды по \x01 или \r\n
    if (buffer.contains("\x01") || buffer.contains("\r\n")) {
        buffer.remove("\x01").remove("\r").remove("\n");

        QByteArray response = parsing(buffer.toUtf8());
        clientSocket->write(response);
        buffer.clear();
    }
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << "Client disconnected:" << clientSocket->peerAddress().toString();

    mClients.remove(clientSocket);
    clientSocket->deleteLater();
}
