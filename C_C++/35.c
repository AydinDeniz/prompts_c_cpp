
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

#define WIDTH 512
#define HEIGHT 512

void apply_grayscale(unsigned char *image) {
    for (int i = 0; i < WIDTH * HEIGHT * 3; i += 3) {
        unsigned char gray = (image[i] + image[i + 1] + image[i + 2]) / 3;
        image[i] = image[i + 1] = image[i + 2] = gray;
    }
}

void apply_grayscale_simd(unsigned char *image) {
    for (int i = 0; i < WIDTH * HEIGHT * 3; i += 24) {
        __m256i pixels = _mm256_loadu_si256((__m256i*)&image[i]);
        __m256i gray = _mm256_srli_epi32(_mm256_add_epi32(pixels, _mm256_set1_epi32(3)), 2);
        _mm256_storeu_si256((__m256i*)&image[i], gray);
    }
}

int main() {
    unsigned char *image = (unsigned char*)malloc(WIDTH * HEIGHT * 3);
    memset(image, 255, WIDTH * HEIGHT * 3);

    printf("Applying grayscale filter...\n");
    apply_grayscale_simd(image);

    free(image);
    printf("Processing complete.\n");
    return 0;
}
