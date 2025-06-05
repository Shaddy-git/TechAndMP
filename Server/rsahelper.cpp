#include "rsahelper.h"
#include <QDataStream>

// Алгоритм Евклида для нахождения наибольшего общего делителя
quint64 RSAHelper::gcd(quint64 a, quint64 b) {
    while (b != 0) {
        quint64 temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Расширенный алгоритм Евклида для поиска обратного по модулю
quint64 RSAHelper::modInverse(quint64 a, quint64 m) {
    qint64 m0 = m, t, q;
    qint64 x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m; a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

// Быстрое возведение в степень по модулю
quint64 RSAHelper::modPow(quint64 base, quint64 exp, quint64 mod) {
    quint64 result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

// Генерация ключей RSA (используются фиксированные простые числа p и q)
RSAHelper::KeyPair RSAHelper::generateKeys() {
    quint64 p = 61; // простое число
    quint64 q = 53; // простое число
    quint64 n = p * q; // модуль
    quint64 phi = (p - 1) * (q - 1); // функция Эйлера

    quint64 e = 17; // открытая экспонента (должна быть взаимно простой с phi)
    while (gcd(e, phi) != 1) ++e;

    quint64 d = modInverse(e, phi); // закрытая экспонента

    return { n, e, d };
}

// Шифрование одного числа
quint64 RSAHelper::encrypt(quint64 message, quint64 e, quint64 n) {
    return modPow(message, e, n);
}

// Расшифровка одного числа
quint64 RSAHelper::decrypt(quint64 ciphertext, quint64 d, quint64 n) {
    return modPow(ciphertext, d, n);
}

// Шифрование строки (разбиение на блоки и упаковка каждого блока)
QByteArray RSAHelper::encryptString(const QString &message, quint64 e, quint64 n)
{
    QByteArray input = message.toUtf8(); // Преобразуем строку в байты
    QByteArray encrypted;
    QDataStream out(&encrypted, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);

    const int blockSize = 6; // максимум 6 байт на блок (fits in quint64)
    for (int i = 0; i < input.size(); i += blockSize) {
        QByteArray block = input.mid(i, blockSize); // выделяем блок

        // Преобразуем блок в одно число (quint64)
        quint64 numericBlock = 0;
        for (int j = 0; j < block.size(); ++j) {
            numericBlock |= (quint64)(quint8)block[j] << (8 * (block.size() - 1 - j));
        }

        quint64 encryptedBlock = encrypt(numericBlock, e, n); // Шифруем

        out << (quint32)block.size();  // Сохраняем размер исходного блока
        out << encryptedBlock;         // Сохраняем зашифрованный блок
    }

    return encrypted;
}

// Расшифровка зашифрованного QByteArray в исходную строку
QString RSAHelper::decryptString(const QByteArray &encrypted, quint64 d, quint64 n)
{
    QByteArray output;
    QDataStream in(encrypted);
    in.setByteOrder(QDataStream::BigEndian);

    while (!in.atEnd()) {
        quint32 blockSize = 0;
        quint64 encryptedBlock = 0;

        in >> blockSize;       // Читаем размер исходного блока
        in >> encryptedBlock;  // Читаем зашифрованное число

        quint64 decryptedBlock = decrypt(encryptedBlock, d, n); // Расшифровываем

        QByteArray block;
        for (int i = blockSize - 1; i >= 0; --i) {
            char ch = (decryptedBlock >> (8 * i)) & 0xFF; // Извлекаем байты из числа
            block.append(ch);
        }

        output.append(block); // Собираем итоговую строку
    }

    return QString::fromUtf8(output); // Возвращаем исходную строку
}
