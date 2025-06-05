#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QByteArray>
#include <QString>
#include <QList>

QByteArray parsing(QString data_from_client);
QByteArray auth(const QString& log, const QString& pass);  // Добавлены const и ссылки
QByteArray reg(const QString& log, const QString& pass, const QString& mail);
QByteArray rsa(const QString& str);
QByteArray sha(const QString& param1);

#endif // FUNCTIONSFORSERVER_H
