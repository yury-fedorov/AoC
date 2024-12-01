use std::collections::HashMap;
use std::str::FromStr;
extern crate regex;
use regex::Regex;

enum Command {
    Inc,
    Dec,
}
enum ComparisonOperator {
    Equal,
    NotEqual,
    Greater,
    Less,
    GreaterEqual,
    LessEqual,
}

fn to_cmd(text: &str) -> Command {
    match text {
        "inc" => Command::Inc,
        "dec" => Command::Dec,
        _ => panic!("Unknown command"),
    }
}

fn to_op(text: &str) -> ComparisonOperator {
    match text {
        "==" => ComparisonOperator::Equal,
        "!=" => ComparisonOperator::NotEqual,
        "<" => ComparisonOperator::Less,
        ">" => ComparisonOperator::Greater,
        "<=" => ComparisonOperator::LessEqual,
        ">=" => ComparisonOperator::GreaterEqual,
        _ => panic!("Unknown comparison operator"),
    }
}

fn check(o: ComparisonOperator, a: i32, b: i32) -> bool {
    match o {
        ComparisonOperator::Equal => a == b,
        ComparisonOperator::NotEqual => a != b,
        ComparisonOperator::GreaterEqual => a >= b,
        ComparisonOperator::Greater => a > b,
        ComparisonOperator::LessEqual => a <= b,
        ComparisonOperator::Less => a < b,
    }
}

fn calculate(c: Command, a: i32, b: i32) -> i32 {
    match c {
        Command::Inc => a + b,
        Command::Dec => a - b,
    }
}

type Instruction = (String, Command, i32, String, ComparisonOperator, i32);

fn get_r(registers: &HashMap<String, i32>, name: &str) -> i32 {
    match registers.get(name) {
        Some(result) => *result,
        None => 0,
    }
}

fn set_r(registers: &mut HashMap<String, i32>, name: &str, value: i32) {
    registers.insert(name.to_string(), value);
}

fn max_r(registers: &HashMap<String, i32>) -> Option<i32> {
    registers.iter().map(|e| *e.1).max()
}

pub fn tasks(text: &String) -> (i32, i32) {
    lazy_static! {
         // b inc 5 if a > 1
         static ref RE_LINE: Regex = Regex::new(
             r"^([a-z]+) ([a-z]+) (.+) if ([a-z]+) ([^ ]+) (.+)$").unwrap();
    }
    let instructions: Vec<Instruction> = text
        .lines()
        .into_iter()
        .flat_map(|line| RE_LINE.captures_iter(line))
        .map(|cap| {
            (
                cap[1].to_string(),
                to_cmd(&cap[2]),
                i32::from_str(&cap[3]).unwrap(),
                cap[4].to_string(),
                to_op(&cap[5]),
                i32::from_str(&cap[6]).unwrap(),
            )
        })
        .collect();

    let mut registers: HashMap<String, i32> = HashMap::new();
    let mut max_r_ever: Option<i32> = None;
    for i in instructions {
        let (r1, cmd, n1, r2, op, n2) = i;
        if check(op, get_r(&registers, &r2), n2) {
            let new_value = calculate(cmd, get_r(&registers, &r1), n1);
            set_r(&mut registers, &r1, new_value);
        }
        let new_max_r = max_r(&registers);
        if new_max_r.is_some() {
            if max_r_ever.is_none()
                || (max_r_ever.is_some() && max_r_ever.unwrap() < new_max_r.unwrap())
            {
                max_r_ever = new_max_r;
            }
        }
    }
    (max_r(&registers).unwrap(), max_r_ever.unwrap())
}
