#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QtNetwork>
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();
private:
    QTcpServer *mTcpServer;
    QMap<QTcpSocket*, QString> mClients; // Ключ: сокет, Значение: буфер данных
};

#endif // MYTCPSERVER_H
