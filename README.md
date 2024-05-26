# ToyARM64Emulator

## Overview

The `ARM64Emulator` class simulates a basic ARM64 CPU, capable of executing a limited set of instructions. It provides functionalities to load and run a program written in ARM64 assembly language, manage registers and memory, and print their states.

## Features

- Emulates 31 general-purpose registers (`x0` to `x30`).
- Supports basic ARM64 instructions: `ldr`, `str`, `add`, `mul`, `mov`, `svc`, and `b`.
- Handles memory operations.
- Can print the current state of registers and memory.

## Usage

### Initialization

To use the `ARM64Emulator`, you need to create an instance of the class, initialize the memory, load a program, and then run the program.

### Example

Here's an example of how to use the `ARM64Emulator`:

```javascript
// Define the program
const program = `
ldr x0, =num1
ldr x1, [x0]
ldr x0, =num2
ldr x2, [x0]
add x3, x1, x2
ldr x0, =multiplier
ldr x4, [x0]
mul x5, x3, x4
ldr x0, =result
str x5, [x0]
`;

// Initialize the memory
const memoryInit = {
    'num1': 5,
    'num2': 7,
    'multiplier': 3,
    'result': 0
};

// Create the emulator instance
const emulator = new ARM64Emulator();
emulator.initializeMemory(memoryInit);
emulator.loadProgram(program);
emulator.run();
emulator.printRegisters();
emulator.printMemory();
```

### Methods

- **constructor()**: Initializes the emulator with empty registers and memory, and sets the program counter (`pc`) to 0.
- **loadProgram(program)**: Loads a program into the emulator. The program should be a string of ARM64 assembly instructions.
- **run()**: Runs the loaded program.
- **printMemory()**: Prints the current state of the memory.
- **printRegisters()**: Prints the current state of the registers.
- **initializeMemory(memoryInit)**: Initializes the emulator's memory with the given key-value pairs.

### Supported Instructions

- **ldr**: Loads a value into a register.
- **str**: Stores a value from a register into memory.
- **add**: Adds two register values and stores the result in a destination register.
- **mul**: Multiplies two register values and stores the result in a destination register.
- **mov**: Moves an immediate value into a register.
- **svc**: (Not implemented) Placeholder for handling system calls.
- **b**: Branches to a labeled instruction.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing

Contributions are welcome! Please submit issues or pull requests for any improvements or bug fixes.
