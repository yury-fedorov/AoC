use super::{Instruction, Register, Registers, Value, ValueArgument, Argument, RegisterArgument};
use super::Instruction::*;

pub(super) struct Duet {
    instructions: Vec<Instruction>,
    registers: Registers,
    instruction_pointer: usize,
    sound: Option<Value>
}

impl Duet {
    pub fn new(instructions: Vec<Instruction>) -> Duet {
        Duet {
            instructions,
            registers: Registers::new(),
            instruction_pointer: 0,
            sound: None,
        }
    }

    fn eval(&self, arg: &Argument) -> Value {
        match arg {
            RegisterArgument(reg) => {
                self.registers[*reg]
            }
            ValueArgument(v) => {
                *v
            }
        }
    }

    /// rcv is the only instruction that returns some value (and only if its argument is not 0)
    pub fn step(&mut self) -> Option<Value> {
        let instruction = &self.instructions[self.instruction_pointer];
        match instruction {
            Snd(arg) => {
                self.sound = Some(self.eval(arg));
                self.instruction_pointer += 1;
                None
            },
            Set(reg, arg) => {
                self.registers[*reg] = self.eval(arg);
                self.instruction_pointer += 1;
                None
            },
            Add(reg, arg) => {
                self.registers[*reg] += self.eval(arg);
                self.instruction_pointer += 1;
                None
            },
            Mul(reg, arg) => {
                self.registers[*reg] *= self.eval(arg);
                self.instruction_pointer += 1;
                None
            },
            Mod(reg, arg) => {
                self.registers[*reg] %= self.eval(arg);
                self.instruction_pointer += 1;
                None
            },
            Rcv(arg) => {
                if self.eval(arg) != 0 {
                    self.instruction_pointer += 1;
                    self.sound
                } else {
                    self.instruction_pointer += 1;
                    None
                }
            },
            Jgz(arg1, arg2) => {
                if self.eval(arg1) > 0 {
                    self.instruction_pointer = (self.instruction_pointer as Value + self.eval(arg2)) as usize;
                } else {
                    self.instruction_pointer += 1;
                }
                None
            }
        }
    }
}
