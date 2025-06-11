#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Database : public QObject
{
    Q_OBJECT
private:
    static Database* instance;
    QSqlDatabase db;

    Database(QObject* parent = nullptr);
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

public:
    ~Database();
    static Database* getInstance();

    bool open();
    void close();

    bool createTables();
    bool addUser(const QString& login, const QString& password,const QString& email);
    bool userExists(const QString& login, const QString& email);
    bool authenticateUser(const QString& login, const QString& password);
};

#endif // DATABASE_H
