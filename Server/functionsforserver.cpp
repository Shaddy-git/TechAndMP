#include "functionsforserver.h"
#include "database.h"
#include "rsa.h"

QByteArray parsing(QString data_from_client)
{
    QStringList data_from_client_list = data_from_client.split('&');

    if(data_from_client_list.isEmpty()) {
        return "error: empty request";
    }

    // Разделяем функцию на параметры
    QString nameOfFunc = data_from_client_list.front().trimmed();
    data_from_client_list.pop_front();

    // Чистим все параметры
    for (int i = 0; i < data_from_client_list.size(); ++i) {
        data_from_client_list[i] = data_from_client_list[i].trimmed();
    }

    // Пробуем найти команду
    if (nameOfFunc == "auth" && data_from_client_list.size() >= 2) {
        return auth(data_from_client_list.at(0), data_from_client_list.at(1));
    }
    else if (nameOfFunc == "reg" && data_from_client_list.size() >= 3) {
        return reg(data_from_client_list.at(0), data_from_client_list.at(1),
                   data_from_client_list.at(2));
    }
    else if (nameOfFunc == "rsa" && data_from_client_list.size() >= 1) {
        return rsa(data_from_client_list.at(0));
    }
    else if (nameOfFunc == "sha" && data_from_client_list.size() >= 1) {
        return sha(data_from_client_list.at(0));
    }
    else {
        return "error: invalid command or parameters\r\n";
    }
}

QByteArray auth(const QString& log, const QString& pass)
{
    // Проверка доступно ли БД
    if (!Database::getInstance()->open()) {
        return "auth:db_error\r\n";
    }

    // Попытка аунтефикации
    if (Database::getInstance()->authenticateUser(log, pass)) {
        return "auth:success\r\n";
    } else {
        return "auth:failed\r\n";
    }
}


QByteArray reg(const QString& log, const QString& pass, const QString& mail)
{
    if (!Database::getInstance()->open()) {
        return "reg:db_error\r\n";
    }

    // Проверка на существование
    if (Database::getInstance()->userExists(log, mail)) {
        return "reg:exists\r\n";
    }

    // Добавление нового пользователя
    if (Database::getInstance()->addUser(log, pass, mail)) {
        return "reg:success\r\n";
    } else {
        return "reg:db_error\r\n";
    }
}


QByteArray rsa(const QString& str)
{
    RSAEncryptor rsa("C:/All/Учёба/1 курс/ТИМП/TaMP/public.pem", "C:/All/Учёба/1 курс/ТИМП/TaMP/private.pem");

    qDebug() << str.toUtf8();
    QString result;
    QString encrypted = rsa.encrypt(str.toUtf8());
    result += "rsa:input = " + str + "\r\n";
    result += "rsa:encrypted = " + encrypted + "\r\n";
    result += "rsa:decrypted = " + rsa.decrypt(encrypted.toUtf8()) + "\r\n";

    return result.toUtf8();
}

QByteArray sha(const QString& param1)
{
    // Заглушка для SHA
    return "sha is not available\r\n";
}
