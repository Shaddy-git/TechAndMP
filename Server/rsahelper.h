#ifndef RSAHELPER_H
#define RSAHELPER_H

#include <QtGlobal>
#include <QString>
#include <QByteArray>

class RSAHelper
{
public:
    // Структура для хранения ключей RSA
    struct KeyPair {
        quint64 n; // модуль (n = p * q)
        quint64 e; // открытая экспонента (публичный ключ)
        quint64 d; // закрытая экспонента (приватный ключ)
    };

    // НОД двух чисел (алгоритм Евклида)
    static quint64 gcd(quint64 a, quint64 b); // public для проверки валидности e

    // Генерация ключей RSA
    static KeyPair generateKeys();

    // Шифрование и расшифровка одного блока (целое число)
    static quint64 encrypt(quint64 message, quint64 e, quint64 n);
    static quint64 decrypt(quint64 ciphertext, quint64 d, quint64 n);

    // Шифрование и расшифровка строки (любой длины)
    static QByteArray encryptString(const QString &message, quint64 e, quint64 n);
    static QString decryptString(const QByteArray &encrypted, quint64 d, quint64 n);

private:
    // Обратное по модулю (расширенный алгоритм Евклида)
    static quint64 modInverse(quint64 a, quint64 m);

    // Быстрое возведение в степень по модулю
    static quint64 modPow(quint64 base, quint64 exp, quint64 mod);
};

#endif // RSAHELPER_H
