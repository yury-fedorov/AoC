use pom::parser::*;
use pom::char_class;
use super::{Argument, Value, ValueArgument, Register, RegisterArgument, Instruction};
use Instruction::*;
use std::str::{from_utf8, FromStr};

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

pub(super) fn parse_instruction<'a>() -> Parser<'a, u8, Instruction> {
    parse_snd_rcv() | parse_add_set_mul_mod() | parse_jgz()
}

#[cfg(test)]
mod test {
    use super::*;
    use super::super::{Instruction::Set, ValueArgument};

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
}