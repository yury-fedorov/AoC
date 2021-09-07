use std::collections::HashMap;
extern crate regex;
use regex::Regex;

type Int = i64;

type Registers = HashMap<char,Int>;
const COUNTER: char = '#';

// TODO - XXX - a lot of code here is the same  as in day18

fn get_r( registers : &Registers, name : char ) -> Int {
    let ro = registers.get( &name );
    if ro.is_none() { return 0; }
    *ro.unwrap()
}

fn set_r( registers : &mut Registers, name : char, value : Int ) {
    registers.insert(name,value);
}

fn to_r( s : &str ) -> char { s.chars().next().unwrap() }

fn get_v( value :&str, registers : &Registers ) -> Int {
    let o = value.parse::<Int>();
    if o.is_ok() { return o.unwrap(); }
    get_r( registers, to_r( value ) )
}

type Command = Box< dyn Fn ( &mut Registers ) -> Int >;
type Code = Vec<Command>;

fn op_r_v( a : char, b : &str, f : fn( a : Int, b : Int ) -> Int, r : &mut Registers ) -> Int {
    let new_value = f( get_r(r, a), get_v( &b, r ) );
    set_r( r, a, new_value );
    1
}

fn compile( line : &str ) -> Command {
    lazy_static! {
        static ref RE_SET: Regex = Regex::new(r"^set (\w+) (\S+)$").unwrap();   // set X Y
        static ref RE_SUB: Regex = Regex::new(r"^sub (\w+) (\S+)$").unwrap();   // add X Y
        static ref RE_MUL: Regex = Regex::new(r"^mul (\w+) (\S+)$").unwrap();   // mul X Y
        static ref RE_JNZ: Regex = Regex::new(r"^jnz (\w+) (\S+)$").unwrap();   // jgz X Y
    }
    for cap in RE_SET.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r|
            { set_r( r, a, get_v( &b, r ) ); 1 } )
    }
    for cap in RE_SUB.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r|
            op_r_v( a, &b, |a,b| a - b, r ) )
    }
    for cap in RE_MUL.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r|
             {
                 let counter = get_r(r, COUNTER) + 1;
                 set_r(r, COUNTER, counter );
                 op_r_v( a, &b, |a,b| a * b, r )
             } );
    }
    for cap in RE_JNZ.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r|
            { if get_r( r, a ) != 0 { return get_v( &b, r ) }
                1 } )
    }
    panic!("{}",line);
}

pub fn to_code(script: &str) -> Code {
    script.lines().into_iter().map( compile ).collect()
}

pub fn task12( script : &str ) -> (Int,i32) {
    let code = to_code(script);

    let mut registers : HashMap<char,Int> = HashMap::new();
    let n = code.len() as Int;
    let mut index :  Int = 0;
    while index >= 0 && index < n {
        let cmd = code.get(index as usize );
        if cmd.is_none() { break; }
        let f = cmd.unwrap();
        let di = f( &mut registers );
        index += di;
    }
    (get_r(&registers, COUNTER), 0)
}