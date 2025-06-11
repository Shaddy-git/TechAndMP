#include "database.h"

Database* Database::instance = nullptr;

Database::Database(QObject* parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/All/Учёба/1 курс/ТИМП/TaMP/sqlite.db");

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
    } else {
        createTables();
    }
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
    }
}

Database* Database::getInstance()
{
    if (!instance) {
        instance = new Database();
    }
    return instance;
}

bool Database::open()
{
    if (!db.isOpen()) {
        return db.open();
    }
    return true;
}

void Database::close()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::createTables()
{
    QSqlQuery query;
    bool success = true;

    if (!query.exec("CREATE TABLE IF NOT EXISTS users("
                    "login VARCHAR(20) PRIMARY KEY NOT NULL, "
                    "password VARCHAR(20) NOT NULL, "
                    "email VARCHAR(50) NOT NULL)")) {
        qDebug() << "Create table error:" << query.lastError().text();
        success = false;
    }

    return success;
}

bool Database::addUser(const QString& login, const QString& password, const QString& email)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users(login, password, email) VALUES (:login, :password, :email)");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Add user error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::userExists(const QString& login, const QString& email)
{
    QSqlQuery query;
    query.prepare("SELECT login FROM users WHERE login = :login OR email = :email");
    query.bindValue(":login", login);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Check user error:" << query.lastError().text();
        return false;
    }

    return query.next();
}


bool Database::authenticateUser(const QString& login, const QString& password)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    qDebug() << "Trying to auth: login=" << login << ", password=" << password;

    if (!query.exec()) {
        qDebug() << "Auth error:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        QString foundLogin = query.value("login").toString();
        QString foundPass = query.value("password").toString();
        qDebug() << "Found in DB: login=" << foundLogin << ", password=" << foundPass;
    }

    if (!query.exec()) {
        qDebug() << "Auth error:" << query.lastError().text();
        return false;
    }

    return query.next(); // Если есть результат - пользователь найден
}
