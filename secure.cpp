#include "secure.h"

// static const unsigned int KEY_SIZE = 32;
// static const unsigned int BLOCK_SIZE = 16;

// g++ -Wall -std=c++11 evp-encrypt.cxx -o evp-encrypt.exe -lcrypto
std::string encry(const std::string &st) {
    // Load the necessary cipher
    EVP_add_cipher(EVP_aes_256_cbc());

    // plaintext, ciphertext, recovered text
    secure_string ptext = st.c_str();

    secure_string ctext;

    byte key[KEY_SIZE], iv[BLOCK_SIZE];

    // just mannually set key
    memset(key, 3, KEY_SIZE);
    memset(iv, 5, BLOCK_SIZE);

    // gen_params(key, iv);

    aes_encrypt(key, iv, ptext, ctext);

    OPENSSL_cleanse(key, KEY_SIZE);
    OPENSSL_cleanse(iv, BLOCK_SIZE);

    return ctext.c_str();
}

std::string dencry(const std::string &st) {
    // Load the necessary cipher
    EVP_add_cipher(EVP_aes_256_cbc());

    // plaintext, ciphertext, recovered text
    secure_string ctext = st.c_str();

    secure_string rtext;

    byte key[KEY_SIZE], iv[BLOCK_SIZE];

    // just mannually set key
    memset(key, 3, KEY_SIZE);
    memset(iv, 5, BLOCK_SIZE);

    // gen_params(key, iv);

    aes_decrypt(key, iv, ctext, rtext);

    OPENSSL_cleanse(key, KEY_SIZE);
    OPENSSL_cleanse(iv, BLOCK_SIZE);

    return rtext.c_str();
}

void gen_params(byte key[KEY_SIZE], byte iv[BLOCK_SIZE]) {
    int rc = RAND_bytes(key, KEY_SIZE);
    if (rc != 1)
        throw std::runtime_error("RAND_bytes key failed");

    rc = RAND_bytes(iv, BLOCK_SIZE);
    if (rc != 1)
        throw std::runtime_error("RAND_bytes for iv failed");
}

void aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE],
                 const secure_string &ptext, secure_string &ctext) {
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptInit_ex failed");

    // Recovered text expands upto BLOCK_SIZE
    ctext.resize(ptext.size() + BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    rc = EVP_EncryptUpdate(ctx.get(), (byte *)&ctext[0], &out_len1,
                           (const byte *)&ptext[0], (int)ptext.size());
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptUpdate failed");

    int out_len2 = (int)ctext.size() - out_len1;
    rc =
        EVP_EncryptFinal_ex(ctx.get(), (byte *)&ctext[0] + out_len1, &out_len2);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptFinal_ex failed");

    // Set cipher text size now that we know it
    ctext.resize(out_len1 + out_len2);
}

void aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE],
                 const secure_string &ctext, secure_string &rtext) {
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv);
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptInit_ex failed");

    // Recovered text contracts upto BLOCK_SIZE
    rtext.resize(ctext.size());
    int out_len1 = (int)rtext.size();

    rc = EVP_DecryptUpdate(ctx.get(), (byte *)&rtext[0], &out_len1,
                           (const byte *)&ctext[0], (int)ctext.size());
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptUpdate failed");

    int out_len2 = (int)rtext.size() - out_len1;
    rc =
        EVP_DecryptFinal_ex(ctx.get(), (byte *)&rtext[0] + out_len1, &out_len2);
    if (rc != 1)
        throw std::runtime_error("EVP_DecryptFinal_ex failed");

    // Set recovered text size now that we know it
    rtext.resize(out_len1 + out_len2);
}

// // main test
// int main() {
//     std::string a, b;
//     std::cin >> a;
//     b = encry(a);
//     std::cout << b << std::endl;
//     std::cout << dencry(b) << std::endl;
//     return 0;
// }
