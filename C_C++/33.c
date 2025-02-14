
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define FIRMWARE_PATH "firmware.bin"
#define SIGNATURE_PATH "signature.bin"
#define PUBLIC_KEY_PATH "public.pem"

int verify_signature(const char *firmware_path, const char *signature_path, const char *public_key_path) {
    FILE *firmware = fopen(firmware_path, "rb");
    FILE *signature = fopen(signature_path, "rb");
    FILE *public_key_file = fopen(public_key_path, "r");

    if (!firmware || !signature || !public_key_file) {
        printf("Error opening files\n");
        return 0;
    }

    fseek(firmware, 0, SEEK_END);
    long firmware_size = ftell(firmware);
    rewind(firmware);

    unsigned char *firmware_data = malloc(firmware_size);
    fread(firmware_data, 1, firmware_size, firmware);
    fclose(firmware);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(firmware_data, firmware_size, hash);

    RSA *rsa = PEM_read_RSA_PUBKEY(public_key_file, NULL, NULL, NULL);
    fclose(public_key_file);

    unsigned char signature_data[256];
    fread(signature_data, 1, 256, signature);
    fclose(signature);

    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, signature_data, 256, rsa);
    RSA_free(rsa);
    free(firmware_data);

    return result;
}

void load_firmware() {
    if (verify_signature(FIRMWARE_PATH, SIGNATURE_PATH, PUBLIC_KEY_PATH)) {
        printf("Firmware verified. Booting...\n");
        system("./firmware.bin");
    } else {
        printf("Firmware verification failed! Aborting.\n");
    }
}

int main() {
    printf("Starting Secure Bootloader...\n");
    load_firmware();
    return 0;
}
