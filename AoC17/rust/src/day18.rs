use std::collections::HashMap;
use std::collections::VecDeque;
extern crate regex;
use regex::Regex;

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

type CommandFn = Box< dyn Fn ( &mut Registers, &mut Music, &mut Music ) -> Int >;

struct Op {
    operation : CommandFn, is_receive : bool, is_send : bool
}

fn execute( op : &Op, r: &mut Registers, mi: &mut Music, mo: &mut Music) -> Int {
    (op.operation)( r, mi, mo )
}

type Code = Vec<Op>;
type Music = VecDeque<Int>; // fast pop_front

fn op_r_v( a : char, b : &str, f : fn( a : Int, b : Int ) -> Int, r : &mut Registers ) -> Int {
    let new_value = f( get_r(r, a), get_v( &b, r ) );
    set_r( r, a, new_value );
    1
}

fn compile( line : &str, is_task1 : bool ) -> Op {
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
        let a = cap[1].to_string();
        let o : CommandFn = Box::new ( move |r, _mi, mo | {
            mo.push_back( get_v( &a, r ) ); 1 } );
        return Op { operation : o, is_send : true, is_receive : false };
    }
    for cap in RE_SET.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        let o : CommandFn = Box::new ( move |r, _mi, _mo |
                { set_r( r, a, get_v( &b, r ) ); 1 } );
        return Op { operation : o, is_send : false, is_receive : false };
    }
    for cap in RE_ADD.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        let o : CommandFn = Box::new( move |r, _mi, _mo |
            op_r_v( a, &b, |a,b| a + b, r ) );
        return Op { operation : o, is_send : false, is_receive : false };
    }
    for cap in RE_MUL.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        let o : CommandFn = Box::new( move |r, _mi, _mo |
            op_r_v( a, &b, |a,b| a * b, r ) );
        return Op { operation : o, is_send : false, is_receive : false };
    }
    for cap in RE_MOD.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        let o : CommandFn = Box::new( move |r, _mi, _mo |
            op_r_v( a, &b, |a,b| a % b, r ) );
        return Op { operation : o, is_send : false, is_receive : false };
    }
    for cap in RE_RCV.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let o : CommandFn = match is_task1 {
            true => Box::new( move |r, _mi, _mo|
                { if get_r( r, a ) != 0 { return 1000000 }
                    1 } ),
            _ => Box::new( move |r, _mi, _mo|
                { let first = _mi.pop_front().unwrap();
                    set_r( r, a, first );
                    1 } )
        };
        return Op { operation : o, is_send : false, is_receive : true };
    }
    for cap in RE_JGZ.captures_iter( line ) {
        let a = to_r(&cap[1] );
        let b = cap[2].to_string();
        let o : CommandFn = Box::new( move |r, _mi, _mo |
            { if get_r( r, a ) > 0 { return get_v( &b, r ) }
                1 } );
        return Op { operation : o, is_send : false, is_receive : false };
    }

    panic!("{}",line);
}

fn to_code(script: &str, is_task1: bool ) -> Code {
    script.lines().into_iter().map( |i| compile( i, is_task1) ).collect()
}

pub fn task1(script:  &str) -> Int {
    let code = to_code(script, true);
    let mut m_in : Music = Music::new();
    let mut m_out : Music = Music::new();
    run_duet( 0, &mut m_in, &mut m_out, &code );
    *m_out.get( m_out.len() - 1 ).unwrap()
}

fn run_duet( p_id: Int, m_in : &mut Music, m_out : &mut Music, code : &Code ) {
    let mut registers : HashMap<char,Int> = HashMap::new();
    registers.insert( 'p', p_id );
    let n = code.len() as Int;
    let mut index :  Int = 0;
    while index >= 0 && index < n {
        let cmd = code.get(index as usize );
        if cmd.is_none() { break; }
        let command = cmd.unwrap();
        let di = execute( &command, &mut registers, m_in, m_out );
        index += di;
    }
}

fn is_valid( index : Int, n : Int ) -> bool { index >= 0 && index < n }

pub fn task2(script:&str) -> i32 {
    let code = to_code(script, false);
    let mut p0_in : Music = Music::new();
    let mut p0_out : Music = Music::new();
    let mut p0_registers : HashMap<char,Int> = HashMap::new();
    p0_registers.insert( 'p', 0 );

    let mut p1_registers : HashMap<char,Int> = HashMap::new();
    p1_registers.insert( 'p', 1 );

    let n = code.len() as Int;
    let mut p0_index : Int = 0;
    let mut p1_index : Int = 0;
    let mut active_p0 = true;

    let mut answer : i32 = 0;

    while is_valid( p0_index, n ) && is_valid( p1_index, n ) {
        let index = match active_p0 { true => p0_index, _ => p1_index };
        let cmd = code.get(index as usize );
        if cmd.is_none() { break; }
        let command = cmd.unwrap();
        if command.is_receive {
            //  we are risking deadlock
            let p0_dead = p0_in.len() == 0;
            let p1_dead = p0_out.len() == 0;
            if p0_dead && p1_dead { break; } // deadlock
            if ( p0_dead && active_p0 ) || ( p1_dead && !active_p0 ) { 
                // change thread
                active_p0 = !active_p0;
                continue;
            } 
        }
        let di = match active_p0 {
            true => execute( &command,&mut p0_registers, &mut p0_in, &mut p0_out),
            _    => execute( &command,&mut p1_registers, &mut p0_out, &mut p0_in)
        };
        answer += match command.is_send && !active_p0 { true => 1, _ => 0 };
        match active_p0 { true => p0_index += di, _ => p1_index += di };
    }

    return answer;
}