
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

typedef double (*compiled_func)();

unsigned char* generate_machine_code(const char* expression) {
    unsigned char* code = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC,
                               MAP_ANON | MAP_PRIVATE, -1, 0);

    if (code == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    unsigned char instructions[] = {
        0xF3, 0x0F, 0x1E, 0xFA,  // Endbr64 (for security on some CPUs)
        0x48, 0xC7, 0xC0, 0x00, 0x00, 0x00, 0x00,  // mov rax, 0 (placeholder for value)
        0xC3  // ret
    };

    double value = atof(expression);
    memcpy(&instructions[7], &value, sizeof(double));

    memcpy(code, instructions, sizeof(instructions));
    return code;
}

int main() {
    char expression[100];
    printf("Enter a number: ");
    scanf("%99s", expression);

    unsigned char* code = generate_machine_code(expression);
    compiled_func func = (compiled_func)code;
    
    printf("Result: %f\n", func());

    munmap(code, 4096);
    return 0;
}
