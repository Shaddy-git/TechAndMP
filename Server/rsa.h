#ifndef RSA_H
#define RSA_H

#include <QString>
#include <openssl/evp.h>

class RSAEncryptor {
public:
    RSAEncryptor(const QString &publicKeyPath, const QString &privateKeyPath);
    ~RSAEncryptor();

    QString encrypt(const QString &plaintext);
    QString decrypt(const QString &base64_ciphertext);

private:
    EVP_PKEY *publicKey_ = nullptr;
    EVP_PKEY *privateKey_ = nullptr;
};

#endif // RSA_H
