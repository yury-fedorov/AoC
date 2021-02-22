use super::{Instruction, Register, Registers, Value, ValueArgument, Argument, RegisterArgument, parse_instruction};
use super::Instruction::*;
use crate::common;

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
            Rcv(reg) => {
                if self.registers[*reg] != 0 {
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

pub fn task1(path: &str) -> Value {
    let text = common::input(path);
    let instructions: Result<Vec<Instruction>, pom::Error> = text
        .lines()
        .into_iter()
        .map(|line| parse_instruction().parse(line.as_bytes()))
        .collect();

    let instructions = instructions.unwrap();
    let mut duet = Duet::new(instructions);

    std::iter::from_fn(|| Some(duet.step()))
        .skip_while(|o| o.is_none())
        .next().unwrap().unwrap()
}
