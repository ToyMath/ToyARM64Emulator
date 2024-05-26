class ARM64Emulator {
    constructor() {
        this.registers = {};
        for (let i = 0; i < 31; i++) {
            this.registers[`x${i}`] = 0;
        }
        this.memory = {};
        this.pc = 0;
        this.instructions = [];
        this.labels = {};
    }

    loadProgram(program) {
        this.instructions = program.split('\n').map(line => line.trim()).filter(line => line);
        this.parseLabels();
    }

    parseLabels() {
        this.instructions.forEach((line, i) => {
            if (line.includes(':')) {
                const label = line.split(':')[0].trim();
                this.labels[label] = i;
            }
        });
    }

    run() {
        while (this.pc < this.instructions.length) {
            const line = this.instructions[this.pc];
            if (!line.endsWith(':')) {
                this.executeInstruction(line);
            }
            this.pc++;
        }
    }

    executeInstruction(line) {
        const parts = line.split(/\s+/);
        const cmd = parts[0];

        switch (cmd) {
            case 'ldr': {
                const reg = parts[1].replace(',', '');
                const value = parts[2];
                if (value.startsWith('=')) {
                    const addr = value.substring(1);
                    this.registers[reg] = addr;
                } else {
                    const addr = this.registers[value.replace('[', '').replace(']', '')];
                    this.registers[reg] = this.memory[addr] || 0;
                }
                break;
            }
            case 'str': {
                const value = parts[1].replace(',', '');
                const reg = parts[2];
                const addr = this.registers[reg.replace('[', '').replace(']', '')];
                this.memory[addr] = this.registers[value];
                break;
            }
            case 'add': {
                const dest = parts[1].replace(',', '');
                const src1 = parts[2].replace(',', '');
                const src2 = parts[3];
                this.registers[dest] = this.registers[src1] + this.registers[src2];
                break;
            }
            case 'mul': {
                const dest = parts[1].replace(',', '');
                const src1 = parts[2].replace(',', '');
                const src2 = parts[3];
                this.registers[dest] = this.registers[src1] * this.registers[src2];
                break;
            }
            case 'mov': {
                const reg = parts[1].replace(',', '');
                const value = parseInt(parts[2].replace('#', ''));
                this.registers[reg] = value;
                break;
            }
            case 'svc': {
                // Handle syscall separately
                break;
            }
            case 'b': {
                const label = parts[1];
                this.pc = this.labels[label] - 1;
                break;
            }
            default: {
                console.log(`Unknown instruction: ${cmd}`);
                break;
            }
        }
    }

    printMemory() {
        console.log("Memory:");
        for (const [k, v] of Object.entries(this.memory)) {
            console.log(`${k}: ${v}`);
        }
    }

    printRegisters() {
        console.log("Registers:");
        for (const [k, v] of Object.entries(this.registers)) {
            console.log(`${k}: ${v}`);
        }
    }

    initializeMemory(memoryInit) {
        this.memory = { ...memoryInit };
    }
}

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
