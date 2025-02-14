
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MEM_SIZE 256

typedef struct {
    uint8_t memory[MEM_SIZE];
    uint8_t registers[4];
    uint8_t pc;
} VM;

enum Instructions {
    LOAD = 0x01,
    ADD = 0x02,
    STORE = 0x03,
    PRINT = 0x04,
    HALT = 0xFF
};

void execute(VM *vm) {
    while (1) {
        uint8_t opcode = vm->memory[vm->pc++];

        switch (opcode) {
            case LOAD:
                vm->registers[vm->memory[vm->pc++]] = vm->memory[vm->pc++];
                break;

            case ADD:
                vm->registers[0] += vm->registers[1];
                break;

            case STORE:
                vm->memory[vm->memory[vm->pc++]] = vm->registers[0];
                break;

            case PRINT:
                printf("Register 0: %d\n", vm->registers[0]);
                break;

            case HALT:
                return;

            default:
                printf("Unknown instruction: 0x%02X\n", opcode);
                return;
        }
    }
}

int main() {
    VM vm = {0};
    uint8_t program[] = {
        LOAD, 0, 10,  // Load 10 into register 0
        LOAD, 1, 20,  // Load 20 into register 1
        ADD,          // Add register 1 to register 0
        PRINT,        // Print register 0
        HALT          // Stop execution
    };

    for (int i = 0; i < sizeof(program); i++) {
        vm.memory[i] = program[i];
    }

    execute(&vm);
    return 0;
}
