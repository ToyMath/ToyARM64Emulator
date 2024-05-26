class ARM64Simulator:
    def __init__(self):
        self.registers = {f'x{i}': 0 for i in range(31)}
        self.memory = {}
        self.pc = 0
        self.instructions = []
        self.labels = {}
        
    def load_program(self, program):
        self.instructions = [line.strip() for line in program.split('\n') if line.strip()]
        self.parse_labels()

    def parse_labels(self):
        for i, line in enumerate(self.instructions):
            if ':' in line:
                label = line.split(':')[0].strip()
                self.labels[label] = i

    def run(self):
        while self.pc < len(self.instructions):
            line = self.instructions[self.pc]
            if not line.endswith(':'):
                self.execute_instruction(line)
            self.pc += 1

    def execute_instruction(self, line):
        parts = line.split()
        cmd = parts[0]

        if cmd == 'ldr':
            reg, value = parts[1].strip(','), parts[2]
            if value.startswith('='):
                addr = value[1:]
                self.registers[reg] = addr
            else:
                addr = self.registers[value.strip('[]')]
                self.registers[reg] = self.memory.get(addr, 0)
        elif cmd == 'str':
            value, reg = parts[1].strip(','), parts[2]
            addr = self.registers[reg.strip('[]')]
            self.memory[addr] = self.registers[value]
        elif cmd == 'add':
            dest, src1, src2 = parts[1].strip(','), parts[2].strip(','), parts[3]
            self.registers[dest] = self.registers[src1] + self.registers[src2]
        elif cmd == 'mul':
            dest, src1, src2 = parts[1].strip(','), parts[2].strip(','), parts[3]
            self.registers[dest] = self.registers[src1] * self.registers[src2]
        elif cmd == 'mov':
            reg, value = parts[1].strip(','), int(parts[2].strip('#'))
            self.registers[reg] = value
        elif cmd == 'svc':
            pass  # We will handle syscall separately
        elif cmd == 'b':
            label = parts[1]
            self.pc = self.labels[label] - 1
        else:
            print(f"Unknown instruction: {cmd}")

    def print_memory(self):
        print("Memory:")
        for k, v in self.memory.items():
            print(f"{k}: {v}")

    def print_registers(self):
        print("Registers:")
        for k, v in self.registers.items():
            print(f"{k}: {v}")

    def initialize_memory(self, memory_init):
        for var, value in memory_init.items():
            self.memory[var] = value

# Define the program
program = """
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
"""

# Initialize the memory
memory_init = {
    'num1': 5,
    'num2': 7,
    'multiplier': 3,
    'result': 0
}

# Create the simulator instance
simulator = ARM64Simulator()
simulator.initialize_memory(memory_init)
simulator.load_program(program)
simulator.run()
simulator.print_registers()
simulator.print_memory()
