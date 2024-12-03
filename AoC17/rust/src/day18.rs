use std::collections::HashMap;
extern crate regex;
use regex::Regex;

type Int = i64; // 32 bits leads to "attempt to multiply with overflow"

type Registers = HashMap<char, Int>;

fn get_r(registers: &Registers, name: char) -> Int {
    let ro = registers.get(&name);
    *ro.unwrap_or(&0)
}

fn set_r(registers: &mut Registers, name: char, value: Int) {
    registers.insert(name, value);
}

fn to_r(s: &str) -> char {
    s.chars().next().unwrap()
}

fn get_v(value: &str, registers: &Registers) -> Int {
    let o = value.parse::<Int>();
    if o.is_ok() {
        return o.unwrap();
    }
    get_r(registers, to_r(value))
}

type CommandFn = Box<dyn Fn(&mut Registers, &mut Music, &mut Music) -> Int>;

struct Command {
    operation: CommandFn,
    is_receive: bool,
    is_send: bool,
}

fn execute(op: &Command, r: &mut Registers, mi: &mut Music, mo: &mut Music) -> Int {
    (op.operation)(r, mi, mo)
}

type Code = Vec<Command>;
type Music = Vec<Int>; // easier debug

fn op_r_v(a: char, b: &str, f: fn(a: Int, b: Int) -> Int, r: &mut Registers) -> Int {
    let new_value = f(get_r(r, a), get_v(&b, r));
    set_r(r, a, new_value);
    1
}

fn compile(line: &str, is_task1: bool) -> Command {
    lazy_static! {
        static ref RE_SND: Regex = Regex::new(r"^snd (\w+)$").unwrap();         // snd X
        static ref RE_SET: Regex = Regex::new(r"^set (\w+) (\S+)$").unwrap();   // set X Y
        static ref RE_ADD: Regex = Regex::new(r"^add (\w+) (\S+)$").unwrap();   // add X Y
        static ref RE_MUL: Regex = Regex::new(r"^mul (\w+) (\S+)$").unwrap();   // mul X Y
        static ref RE_MOD: Regex = Regex::new(r"^mod (\w+) (\S+)$").unwrap();   // mod X Y
        static ref RE_RCV: Regex = Regex::new(r"^rcv (\w+)$").unwrap();         // rcv X
        static ref RE_JGZ: Regex = Regex::new(r"^jgz (\w+) (\S+)$").unwrap();   // jgz X Y
    }
    for cap in RE_SND.captures_iter(line) {
        let a = cap[1].to_string();
        let o: CommandFn = Box::new(move |r, _mi, mo| {
            mo.push(get_v(&a, r));
            1
        });
        return Command {
            operation: o,
            is_send: true,
            is_receive: false,
        };
    }
    for cap in RE_SET.captures_iter(line) {
        let a = to_r(&cap[1]);
        let b = cap[2].to_string();
        let o: CommandFn = Box::new(move |r, _mi, _mo| {
            set_r(r, a, get_v(&b, r));
            1
        });
        return Command {
            operation: o,
            is_send: false,
            is_receive: false,
        };
    }
    for cap in RE_ADD.captures_iter(line) {
        let a = to_r(&cap[1]);
        let b = cap[2].to_string();
        let op_add: CommandFn = Box::new(move |r, _mi, _mo| op_r_v(a, &b, |a, b| a + b, r));
        return Command {
            operation: op_add,
            is_send: false,
            is_receive: false,
        };
    }
    for cap in RE_MUL.captures_iter(line) {
        let a = to_r(&cap[1]);
        let b = cap[2].to_string();
        let o: CommandFn = Box::new(move |r, _mi, _mo| op_r_v(a, &b, |a, b| a * b, r));
        return Command {
            operation: o,
            is_send: false,
            is_receive: false,
        };
    }
    for cap in RE_MOD.captures_iter(line) {
        let a = to_r(&cap[1]);
        let b = cap[2].to_string();
        let o: CommandFn = Box::new(move |r, _mi, _mo| op_r_v(a, &b, |a, b| a % b, r));
        return Command {
            operation: o,
            is_send: false,
            is_receive: false,
        };
    }
    for cap in RE_RCV.captures_iter(line) {
        let a = to_r(&cap[1]);
        let o: CommandFn = match is_task1 {
            true => Box::new(move |r, _mi, _mo| {
                if get_r(r, a) != 0 {
                    return 1_000_000;
                }
                1
            }),
            _ => Box::new(move |r, mi, _mo| {
                let first = mi.remove(0);
                set_r(r, a, first);
                1
            }),
        };
        return Command {
            operation: o,
            is_send: false,
            is_receive: true,
        };
    }
    for cap in RE_JGZ.captures_iter(line) {
        let a = cap[1].to_string();
        let b = cap[2].to_string();
        let o: CommandFn = Box::new(move |r, _mi, _mo| {
            let value = get_v(&a, r);
            if value > 0 {
                return get_v(&b, r);
            }
            1
        });
        return Command {
            operation: o,
            is_send: false,
            is_receive: false,
        };
    }
    panic!("Unexpected command: {}", line);
}

fn to_code(script: &str, is_task1: bool) -> Code {
    script
        .lines()
        .into_iter()
        .map(|i| compile(i, is_task1))
        .collect()
}

pub fn task1(script: &str) -> Int {
    let code = to_code(script, true);
    let mut m_in: Music = Music::new();
    let mut m_out: Music = Music::new();
    run_duet(0, &mut m_in, &mut m_out, &code);
    *m_out.get(m_out.len() - 1).unwrap()
}

fn create_registers(p_id: Int) -> HashMap<char, Int> {
    let mut registers: HashMap<char, Int> = HashMap::new();
    registers.insert('p', p_id);
    registers
}

fn run_duet(p_id: Int, m_in: &mut Music, m_out: &mut Music, code: &Code) {
    let mut registers = create_registers(p_id);
    let n = code.len() as Int;
    let mut index: Int = 0;
    while index >= 0 && index < n {
        let cmd = code.get(index as usize);
        if cmd.is_none() {
            break;
        }
        let command = cmd.unwrap();
        let di = execute(&command, &mut registers, m_in, m_out);
        index += di;
    }
}

pub fn task2(script: &str) -> i32 {
    let code = to_code(script, false);

    let mut p1_to_p0: Music = Music::new();
    let mut p0_to_p1: Music = Music::new();
    let mut p0_registers = create_registers(0);
    let mut p1_registers = create_registers(1);
    let mut p0_index: Int = 0;
    let mut p1_index: Int = 0;
    let mut is_p0_active = true;

    let mut answer: i32 = 0;

    for _ii in 0..1_000_000 {
        let index = match is_p0_active {
            true => p0_index,
            _ => p1_index,
        };
        let cmd = code.get(index as usize);
        if cmd.is_none() {
            break;
        }
        let command = cmd.unwrap();
        if command.is_receive {
            //  we are risking deadlock
            let p0_dead = p1_to_p0.len() == 0 && code.get(p0_index as usize).unwrap().is_receive;
            let p1_dead = p0_to_p1.len() == 0 && code.get(p1_index as usize).unwrap().is_receive;
            if p0_dead && p1_dead {
                return answer;
            } // deadlock
            if (p0_dead && is_p0_active) || (p1_dead && !is_p0_active) {
                // change thread to avoid deadlock
                is_p0_active = !is_p0_active;
                continue;
            }
        }
        let di;
        if is_p0_active {
            di = execute(&command, &mut p0_registers, &mut p1_to_p0, &mut p0_to_p1);
        } else {
            di = execute(&command, &mut p1_registers, &mut p0_to_p1, &mut p1_to_p0);
        }

        let is_p1_send = command.is_send && !is_p0_active;

        // how many times did program 1 send a value?
        answer += match is_p1_send {
            true => 1,
            _ => 0,
        };
        if is_p0_active {
            p0_index += di
        } else {
            p1_index += di
        };
    }

    return answer;
}
