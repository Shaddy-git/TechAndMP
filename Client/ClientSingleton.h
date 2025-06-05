#ifndef CLIENTSINGLETON_H
#define CLIENTSINGLETON_H

#include <QObject>

class ClientSingleton : public QObject
{
    Q_OBJECT
public:
    static ClientSingleton& instance();

    void executeFunction(int id);
    void executeChordMethod(const QString& function, double a, double b);

    // Запрещаем копирование
    ClientSingleton(const ClientSingleton&) = delete;
    ClientSingleton& operator=(const ClientSingleton&) = delete;

signals:
    void resultUpdated(const QString& result);

private:
    ClientSingleton(QObject* parent = nullptr);
    QString m_lastResult;
};

#endif // CLIENTSINGLETON_H
