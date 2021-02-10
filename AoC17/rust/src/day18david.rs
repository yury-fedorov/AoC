mod parsing;
mod task1;
mod task2;

use std::ops::{Index, IndexMut};
use std::str::{FromStr, from_utf8};
use crate::day18david::Argument::ValueArgument;
use parsing::parse_instruction;
pub use task1::task1;
pub use task2::task2;

type Register = u8;
type Value = i64;

#[derive(Debug, PartialEq, Clone)]
enum Argument {
    RegisterArgument(Register),
    ValueArgument(Value),
}

use Argument::*;

#[derive(Debug, PartialEq, Clone)]
enum Instruction {
    Snd(Argument),
    Set(Register, Argument),
    Add(Register, Argument),
    Mul(Register, Argument),
    Mod(Register, Argument),
    Rcv(Register),
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

#[cfg(test)]
mod test {
    use crate::common;
    use super::*;
    use regex::internal::Input;
    use super::parsing::parse_instruction;

    #[test]
    fn test_tassk1_sample() {
        let result = task1("18/sample.txt");

        assert_eq!(result, 4)
    }

    #[test]
    fn test_task1_input_david() {
        let result = task1("18/input_david.txt");

        assert_eq!(result, 8600)
    }

    #[test]
    fn test_task2_sample2() {
        let result = task2("18/sample2.txt");

        assert_eq!(result, 3)
    }

    #[test]
    fn test_task2_input_david() {
        let result = task2("18/input_david.txt");

        assert_eq!(result, 7239)
    }
}
