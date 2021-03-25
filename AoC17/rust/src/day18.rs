use std::collections::HashMap;
extern crate regex;
use regex::Regex;
use std::thread;
use std::sync::mpsc;
use std::sync::mpsc::{Receiver, Sender, channel};

type Int = i64;

type Registers = HashMap<char,Int>;

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

type Command = Box< dyn Fn ( &mut Registers, &Sender<Int>, &Receiver<Int> ) -> Int >;
type Code = Vec<Command>;
type Music = Vec<Int>;

fn op_r_v( a : char, b : &str, f : fn( a : Int, b : Int ) -> Int, r : &mut Registers ) -> Int {
    let new_value = f( get_r(r, a), get_v( &b, r ) );
    set_r( r, a, new_value );
    1
}

fn compile( line : &str ) -> Command {
    lazy_static! {
        static ref RE_SND: Regex = Regex::new(r"^snd (\w+)$").unwrap();         // snd X
        static ref RE_SET: Regex = Regex::new(r"^set (\w+) (\S+)$").unwrap();   // set X Y
        static ref RE_ADD: Regex = Regex::new(r"^add (\w+) (\S+)$").unwrap();   // add X Y
        static ref RE_MUL: Regex = Regex::new(r"^mul (\w+) (\S+)$").unwrap();   // mul X Y
        static ref RE_MOD: Regex = Regex::new(r"^mod (\w+) (\S+)$").unwrap();   // mod X Y
        static ref RE_RCV: Regex = Regex::new(r"^rcv (\w+)$").unwrap();         // rcv X
        static ref RE_JGZ: Regex = Regex::new(r"^jgz (\w+) (\S+)$").unwrap();   // jgz X Y
    }
    for cap in RE_SND.captures_iter( line ) {
        let value = cap[1].to_string();
        return Box::new( move | r, s, _r | {
            s.send( get_v( &value, r ) ); 1 } )
    }
    for cap in RE_SET.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r, _s, _r |
            { set_r( r, a, get_v( &b, r ) ); 1 } )
    }
    for cap in RE_ADD.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r, _s, _r |
            op_r_v( a, &b, |a,b| a + b, r ) )
    }
    for cap in RE_MUL.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r, _s, _r |
            op_r_v( a, &b, |a,b| a * b, r ) )
    }
    for cap in RE_MOD.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r, _s, _r |
            op_r_v( a, &b, |a,b| a % b, r ) )
    }
    for cap in RE_RCV.captures_iter( line ) {
        let a = to_r(&cap[1] );
        return Box::new( move |r, _s, _r|
             { if get_r( r, a ) != 0 { return 1000000 }
               1 } )
    }
    for cap in RE_JGZ.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        return Box::new( move |r, _s, _r |
            { if get_r( r, a ) > 0 { return get_v( &b, r ) }
                1 } )
    }
    panic!(line.to_string());
}

pub fn to_code(script: &str) -> Code {
    script.lines().into_iter().map( compile ).collect()
}

pub fn task1(code: &Code) -> Int {
    let (sender, reciver) = channel();
    run_duet( 0, &sender, &reciver, &code );
    reciver.iter().last().unwrap()
}

fn run_duet_( p_id: Int, m_out : &Sender<Int>, m_in : &Receiver<Int>, script : String ) {
    let code = to_code(&script);
    run_duet( p_id, m_out, m_in, &code );
}

fn run_duet( p_id: Int, m_out : &Sender<Int>, m_in : &Receiver<Int>, code : &Code ) {
    let mut registers : HashMap<char,Int> = HashMap::new();
    registers.insert( 'p', p_id );
    let n = code.len() as Int;
    let mut index :  Int = 0;
    while index >= 0 && index < n {
        let cmd = code.get(index as usize );
        if cmd.is_none() { break; }
        let f = cmd.unwrap();
        let di = f( &mut registers, m_out, m_in );
        index += di;
    }
}

pub fn task2(script: &str) -> usize {
    // let mut m0 : Music = Vec::new(); // p0 in, p1 out
    // let mut m1 : Music = Vec::new(); // p1 in, p0 out

    let (sender0, receiver1) = mpsc::channel();
    let (sender1, receiver0) = mpsc::channel();
    // let (tx2, rx2) = mpsc::channel(); // (p_id, val)
    let s1 = script.clone().to_string();

    let handle = thread::spawn( move ||
         run_duet_(0, &sender0, &receiver0, s1) );
    run_duet(1, &sender1, &receiver1, &to_code( script ) );
    // how many times did program 1 send a value?
    // m0 -> p1 out
    handle.join().unwrap(); // wait till the end of the thread
    0
}