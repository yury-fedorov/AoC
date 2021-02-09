use std::ops::{Index, IndexMut};
use pom::parser::*;
use pom::char_class;
use std::str::{FromStr, from_utf8};
use crate::day18david::Argument::ValueArgument;

type Register = u8;
type Value = i64;

#[derive(Debug, PartialEq)]
enum Argument {
    RegisterArgument(Register),
    ValueArgument(Value),
}

use Argument::*;

#[derive(Debug, PartialEq)]
enum Instruction {
    Snd(Argument),
    Set(Register, Argument),
    Add(Register, Argument),
    Mul(Register, Argument),
    Mod(Register, Argument),
    Rcv(Argument),
    Jgz(Argument, Argument),
}

use Instruction::*;

struct Registers {
    inner: Vec<Value>,
}

impl Registers {
    fn new() -> Registers {
        let len = 'z' as usize - 'a' as usize + 1;
        Registers {
            inner: vec![0; len],
        }
    }
}

impl Index<Register> for Registers {
    type Output = Value;

    fn index(&self, index: Register) -> &Value {
        &self.inner[index as usize - 'a' as usize]
    }
}

impl IndexMut<Register> for Registers {
    fn index_mut(&mut self, index: Register) -> &mut Value {
        &mut self.inner[index as usize - 'a' as usize]
    }
}

struct Duet {
    instructions: Vec<Instruction>,
    registers: Registers,
    instruction_pointer: usize,
    sound: Option<Value>
}

impl Duet {
    fn new(instructions: Vec<Instruction>) -> Duet {
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
    fn step(&mut self) -> Option<Value> {
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

fn parse_value_argument<'a>() -> Parser<'a, u8, Argument> {
    (sym(b'-').opt() + is_a(char_class::digit).repeat(1..))
        .collect()
        .map(|slice| Value::from_str(from_utf8(slice).unwrap()).unwrap())
        .map(ValueArgument)
}

fn char_class_register(term: u8) -> bool {
    term >= b'a' && term <= b'z'
}

fn parse_register<'a>() -> Parser<'a, u8, Register> {
    is_a(char_class_register)
}

fn parse_register_argument<'a>() -> Parser<'a, u8, Argument> {
    parse_register()
        .map(RegisterArgument)
}

fn parse_argument<'a>() -> Parser<'a, u8, Argument> {
    parse_register_argument() | parse_value_argument()
}

fn parse_snd_rcv<'a>() -> Parser<'a, u8, Instruction> {
    fn parse_instr_arg<'a>(instruction_token: &'a [u8], constructor: fn(Argument) -> Instruction) -> Parser<'a, u8, Instruction> {
        (seq(instruction_token) + is_a(char_class::space) + parse_argument())
            .map(move |(_, argument)| constructor(argument))
    }

    parse_instr_arg(b"snd", Snd) |
        parse_instr_arg(b"rcv", Rcv)
}

fn parse_add_set_mul_mod<'a>() -> Parser<'a, u8, Instruction> {
    fn parse_instr_reg_arg<'a>(instruction_token: &'a [u8], constructor: fn(Register, Argument) -> Instruction) -> Parser<'a, u8, Instruction> {
        (seq(instruction_token) + is_a(char_class::space) + parse_register() + is_a(char_class::space) + parse_argument())
            .map(move |((((_, _), reg), _), arg)| constructor(reg, arg))
    }

    parse_instr_reg_arg(b"add", Add) |
        parse_instr_reg_arg(b"set", Set) |
        parse_instr_reg_arg(b"mul", Mul) |
        parse_instr_reg_arg(b"mod", Mod)
}

fn parse_jgz<'a>() -> Parser<'a, u8, Instruction> {
    (seq(b"jgz") + is_a(char_class::space) + parse_argument() + is_a(char_class::space) + parse_argument())
        .map(move |((((_, _), reg), _), arg)| Jgz(reg, arg))
}

fn parse_instruction<'a>() -> Parser<'a, u8, Instruction> {
    parse_snd_rcv() | parse_add_set_mul_mod() | parse_jgz()
}


pub fn task1() {
    let mut instructions: Vec<Instruction> = Vec::new();
}

#[cfg(test)]
mod test {
    use crate::common;
    use crate::day18david::*;
    use regex::internal::Input;

    #[test]
    fn test_set_a_1() {
        let result = parse_add_set_mul_mod().parse(b"set a 1").unwrap();
        assert_eq!(result, Set(b'a', ValueArgument(1)))
    }

    #[test]
    fn test_jgz_a_1() {
        let result = parse_jgz().parse(b"jgz b -1").unwrap();
        assert_eq!(result, Jgz(RegisterArgument(b'b'), ValueArgument(-1)))
    }

    #[test]
    fn test_sample() {
        let text = common::input("18/sample.txt");
        let instructions: Result<Vec<Instruction>, pom::Error> = text
            .lines()
            .into_iter()
            .map(|line| parse_instruction().parse(line.as_bytes()))
            .collect();

        let instructions = instructions.unwrap();
        let mut duet = Duet::new(instructions);

        let result = std::iter::from_fn(|| Some(duet.step()))
            .skip_while(|o| o.is_none())
            .next().unwrap().unwrap();

        assert_eq!(result, 4)
    }

    #[test]
    fn test_input_david() {
        let text = common::input("18/input_david.txt");
        let instructions: Result<Vec<Instruction>, pom::Error> = text
            .lines()
            .into_iter()
            .map(|line| parse_instruction().parse(line.as_bytes()))
            .collect();

        let instructions = instructions.unwrap();
        let mut duet = Duet::new(instructions);

        let result = std::iter::from_fn(|| Some(duet.step()))
            .skip_while(|o| o.is_none())
            .next().unwrap().unwrap();

        assert_eq!(result, 8600)
    }
}