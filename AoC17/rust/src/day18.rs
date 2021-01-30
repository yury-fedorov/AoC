use std::collections::HashMap;
extern crate regex;
use regex::Regex;

type Registers = HashMap<char,i32>;

fn get_r( registers : &Registers, name : char ) -> i32 {
    let ro = registers.get( &name );
    if ro.is_none() { return 0; }
    *ro.unwrap()
}

fn set_r( registers : &mut Registers, name : char, value : i32 ) {
    registers.insert(name,value);
}

fn get_v( value :&str, registers : &Registers ) -> i32 {
    let o = value.parse::<i32>();
    if o.is_ok() { return o.unwrap(); }
    let name = value.chars().next().unwrap();
    get_r( registers, name )
}

type Command = fn ( &mut Registers, &mut Music ) -> i32;
type Music = Vec<i32>;

fn compile( line : &str ) -> Command {
    lazy_static! {
        static ref RE_SND: Regex = Regex::new(r"^snd (\w+)$").unwrap();         // snd X
        static ref RE_SET: Regex = Regex::new(r"^set (\w+) (\w+)$").unwrap();   // set X Y
        static ref RE_ADD: Regex = Regex::new(r"^add (\w+) (\w+)$").unwrap();   // add X Y
        static ref RE_MUL: Regex = Regex::new(r"^mul (\w+) (\w+)$").unwrap();   // mul X Y
        static ref RE_MOD: Regex = Regex::new(r"^mod (\w+) (\w+)$").unwrap();   // mod X Y
        static ref RE_RCV: Regex = Regex::new(r"^rcv (\w+)$").unwrap();         // rcv X
        static ref RE_JGZ: Regex = Regex::new(r"^jgz (\w+) (\w+)$").unwrap();   // jgz X Y
    }
    |r, m | -1000
}

pub fn task1(script: &str) -> i32 {
    let code : Vec<Command> = script.lines().into_iter().map( compile ).collect();
    let n = code.len() as i32;
    let mut registers : HashMap<char,i32> = HashMap::new();
    let mut music : Music = Vec::new();
    let mut index :  i32 = 0;
    while index >= 0 && index < n {
        let cmd = code.get(index as usize );
        if cmd.is_none() { break; }
        let f = cmd.unwrap();
        let di = f( &mut registers, &mut music );
        index += di;
    }
    music[0]
}