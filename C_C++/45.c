
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define AES_KEY_SIZE 32
#define AES_BLOCK_SIZE 16

void handleErrors() {
    fprintf(stderr, "Error occurred. Exiting...\n");
    exit(1);
}

void generate_key_iv(unsigned char *key, unsigned char *iv) {
    if (!RAND_bytes(key, AES_KEY_SIZE) || !RAND_bytes(iv, AES_BLOCK_SIZE)) {
        handleErrors();
    }
}

void encrypt_file(const char *input_filename, const char *output_filename, unsigned char *key, unsigned char *iv) {
    FILE *input = fopen(input_filename, "rb");
    FILE *output = fopen(output_filename, "wb");
    
    if (!input || !output) {
        handleErrors();
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char encrypted[AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytes_read, encrypted_len;

    while ((bytes_read = fread(buffer, 1, AES_BLOCK_SIZE, input)) > 0) {
        EVP_EncryptUpdate(ctx, encrypted, &encrypted_len, buffer, bytes_read);
        fwrite(encrypted, 1, encrypted_len, output);
    }

    EVP_EncryptFinal_ex(ctx, encrypted, &encrypted_len);
    fwrite(encrypted, 1, encrypted_len, output);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input);
    fclose(output);

    printf("Encryption successful!\n");
}

void decrypt_file(const char *input_filename, const char *output_filename, unsigned char *key, unsigned char *iv) {
    FILE *input = fopen(input_filename, "rb");
    FILE *output = fopen(output_filename, "wb");

    if (!input || !output) {
        handleErrors();
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char buffer[AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH];
    unsigned char decrypted[AES_BLOCK_SIZE];
    int bytes_read, decrypted_len;

    while ((bytes_read = fread(buffer, 1, AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH, input)) > 0) {
        EVP_DecryptUpdate(ctx, decrypted, &decrypted_len, buffer, bytes_read);
        fwrite(decrypted, 1, decrypted_len, output);
    }

    EVP_DecryptFinal_ex(ctx, decrypted, &decrypted_len);
    fwrite(decrypted, 1, decrypted_len, output);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input);
    fclose(output);

    printf("Decryption successful!\n");
}

int main() {
    unsigned char key[AES_KEY_SIZE];
    unsigned char iv[AES_BLOCK_SIZE];

    generate_key_iv(key, iv);

    encrypt_file("plaintext.txt", "encrypted.dat", key, iv);
    decrypt_file("encrypted.dat", "decrypted.txt", key, iv);

    return 0;
}
