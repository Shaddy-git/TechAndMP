#include "rsa.h"
#include <openssl/pem.h>
#include <openssl/err.h>
#include <QFile>
#include <QByteArray>
#include <QDebug>

RSAEncryptor::RSAEncryptor(const QString &publicKeyPath, const QString &privateKeyPath)
{
    // Загрузка открытого ключа
    QFile pubFile(publicKeyPath);
    if (pubFile.open(QIODevice::ReadOnly)) {
        QByteArray keyData = pubFile.readAll();
        BIO *bio = BIO_new_mem_buf(keyData.constData(), keyData.size());
        publicKey_ = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
        BIO_free(bio);
    }

    if (!publicKey_) {
        qWarning() << "Error: failed to load public key";
    }

    // Загрузка закрытого ключа
    QFile privFile(privateKeyPath);
    if (privFile.open(QIODevice::ReadOnly)) {
        QByteArray keyData = privFile.readAll();
        BIO *bio = BIO_new_mem_buf(keyData.constData(), keyData.size());
        privateKey_ = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
        BIO_free(bio);
    }

    if (!privateKey_) {
        qWarning() << "Error: failed to load private key";
    }
}

QString RSAEncryptor::encrypt(const QString &plaintext)
{
    QByteArray input = plaintext.toUtf8();

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(publicKey_, nullptr);
    if (!ctx || EVP_PKEY_encrypt_init(ctx) <= 0) {
        qWarning() << "Error: encryption initialization error";
        EVP_PKEY_CTX_free(ctx);
        return {};
    }

    size_t outlen = 0;
    if (EVP_PKEY_encrypt(ctx, nullptr, &outlen,
                         reinterpret_cast<const unsigned char *>(input.constData()), input.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return {};
    }

    QByteArray encrypted(outlen, 0);
    if (EVP_PKEY_encrypt(ctx,
                         reinterpret_cast<unsigned char *>(encrypted.data()), &outlen,
                         reinterpret_cast<const unsigned char *>(input.constData()), input.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return {};
    }

    EVP_PKEY_CTX_free(ctx);
    return encrypted.left(outlen).toBase64();
}

QString RSAEncryptor::decrypt(const QString &base64_ciphertext)
{
    QByteArray ciphertext = QByteArray::fromBase64(base64_ciphertext.toUtf8());

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privateKey_, nullptr);
    if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0) {
        qWarning() << "Error: initializing decryption error";
        EVP_PKEY_CTX_free(ctx);
        return {};
    }

    size_t outlen = 0;
    if (EVP_PKEY_decrypt(ctx, nullptr, &outlen,
                         reinterpret_cast<const unsigned char *>(ciphertext.constData()), ciphertext.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return {};
    }

    QByteArray decrypted(outlen, 0);
    if (EVP_PKEY_decrypt(ctx,
                         reinterpret_cast<unsigned char *>(decrypted.data()), &outlen,
                         reinterpret_cast<const unsigned char *>(ciphertext.constData()), ciphertext.size()) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return {};
    }

    EVP_PKEY_CTX_free(ctx);
    return QString::fromUtf8(decrypted.left(outlen));
}

RSAEncryptor::~RSAEncryptor()
{
    if (publicKey_) EVP_PKEY_free(publicKey_);
    if (privateKey_) EVP_PKEY_free(privateKey_);
}
