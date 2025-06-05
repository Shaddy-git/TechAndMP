#include "ClientSingleton.h"
#include "ChordMethod.h"
#include "qdebug.h"

ClientSingleton& ClientSingleton::instance()
{
    static ClientSingleton inst;
    return inst;
}

ClientSingleton::ClientSingleton(QObject* parent) : QObject(parent) {}

void ClientSingleton::executeFunction(int id)
{
    switch(id) {
    case 1: {m_lastResult = "Используйте кнопку 'Метод хорд'"; qDebug() << &ClientSingleton::instance();  break;}
    case 2: {m_lastResult = "Функция 2 выполнена (заглушка)"; qDebug() << &ClientSingleton::instance(); break;}
    case 3: {m_lastResult = "Функция 3 выполнена (заглушка)"; qDebug() << &ClientSingleton::instance(); break;}
    case 4: {m_lastResult = "Функция 4 выполнена (заглушка)"; qDebug() << &ClientSingleton::instance(); break;}
    default: m_lastResult = "Неизвестная функция";
    }
    emit resultUpdated(m_lastResult);
}

void ClientSingleton::executeChordMethod(const QString& function, double a, double b)
{
    try {
        double root = ChordMethod::solve(function, a, b);
        m_lastResult = QString("Метод хорд:\n"
                               "Функция: %1\n"
                               "Корень: %2\n"
                               "Интервал: [%3, %4]")
                           .arg(function)
                           .arg(root, 0, 'f', 6)
                           .arg(a)
                           .arg(b);
    } catch (const std::exception& e) {
        m_lastResult = QString("Ошибка: %1").arg(e.what());
    }
    emit resultUpdated(m_lastResult);
}
