mod parsing;
mod task1;

use std::ops::{Index, IndexMut};
use std::str::{FromStr, from_utf8};
use crate::day18david::Argument::ValueArgument;
use parsing::parse_instruction;

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
use crate::common;

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

pub fn task1(path: &str) -> Value {
    let text = common::input(path);
    let instructions: Result<Vec<Instruction>, pom::Error> = text
        .lines()
        .into_iter()
        .map(|line| parse_instruction().parse(line.as_bytes()))
        .collect();

    let instructions = instructions.unwrap();
    let mut duet = task1::Duet::new(instructions);

    std::iter::from_fn(|| Some(duet.step()))
        .skip_while(|o| o.is_none())
        .next().unwrap().unwrap()
}

#[cfg(test)]
mod test {
    use crate::common;
    use super::*;
    use regex::internal::Input;
    use super::parsing::parse_instruction;

    #[test]
    fn test_sample() {
        let result = task1("18/sample.txt");

        assert_eq!(result, 4)
    }

    #[test]
    fn test_input_david() {
        let result = task1("18/input_david.txt");

        assert_eq!(result, 8600)
    }
}
