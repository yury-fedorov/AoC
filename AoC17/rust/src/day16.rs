use std::iter::FromIterator;
extern crate regex;
use regex::Regex;
use std::str::FromStr;
use std::collections::HashMap;

pub type Position = Vec<char>;
// https://doc.rust-lang.org/book/ch19-05-advanced-functions-and-closures.html
pub type Step = Box< dyn Fn ( &mut Position ) >; // closure

fn do_spin( pos : &mut Position, n: i32 ) {
    let last = pos.len() - 1;
    for _ in 0..n {
        let last_element = pos.remove(last);
        pos.insert(0, last_element );
    }
}

fn spin( step : &str ) -> Option<Step> {
    lazy_static! { static ref RE_SPIN: Regex = Regex::new(r"^s(\d+)$").unwrap(); }
    for c in RE_SPIN.captures_iter(step) {
        let n = i32::from_str( &c[1] ).unwrap();
        return Some( Box::new( move |pos| do_spin( pos, n ) ) )
    }
    None
}

fn do_exchange( pos : &mut Position, a :usize, b :usize ) { pos.swap( a, b ); }

fn exchange( step : &str ) -> Option<Step> {
    lazy_static! { static ref RE_EXCHANGE: Regex = Regex::new(r"^x(\d+)/(\d+)$").unwrap(); }
    for c in RE_EXCHANGE.captures_iter(step) {
        let a = usize::from_str( &c[1] ).unwrap();
        let b = usize::from_str( &c[2] ).unwrap();
        return Some( Box::new( move |pos| do_exchange( pos, a, b ) ) )
    }
    None
}

fn do_partner( pos : &mut Position, a :char, b :char ) {
    let fi = |ch| pos.clone().into_iter().position( |c| c == ch ).unwrap();
    let ai = fi(a);
    let bi = fi(b);
    pos.swap(  ai, bi );
}

fn partner( step : &str ) -> Option<Step> {
    lazy_static! { static ref RE_PARTNER: Regex = Regex::new(r"^p(\w)/(\w)$").unwrap(); }
    for c in RE_PARTNER.captures_iter(step) {
        let a = c[1].chars().next().unwrap();
        let b = c[2].chars().next().unwrap();
        return Some( Box::new( move |pos| do_partner( pos, a, b ) ) )
    }
    None
}

type Factory = fn(&str) -> Option<Step>;
pub type Dance = Vec<Step>;

pub fn init( dance : &str ) -> Dance {
    let dance_steps = dance.trim().split( "," );
    let mut steps : Vec<Step> = Vec::new();
    let factories : Vec<Factory> = vec![ spin, exchange, partner ];
    for s in dance_steps {
        let factory = factories.clone().into_iter()
            .find( |f| f(s).is_some() ).unwrap();
        let step = factory(s).unwrap();
        steps.push( step );
    }
    steps
}

fn task(start : &str, dance : &Dance, n: i32) -> String {
    let mut pos : Position = start.chars().into_iter().collect();
    let mut pos_i : HashMap<String,i32> = HashMap::new();
    let mut i = 0;
    while i < n {
        let p = String::from_iter(pos.clone() );
        let r = pos_i.insert( p, i );
        if r.is_none() {
            for s in dance {
                s( &mut pos );
            }
            i += 1;
        } else {
            // we found the version
            let di : i32 = i - r.unwrap();
            let rest = n - i;
            let n = rest / di;
            i += n * di;
            pos_i.clear();
        }
    }
    String::from_iter(pos)
}

pub fn task1(start : &str, dance : &Dance ) -> String { task( start, dance, 1 ) }

pub fn task2( start : &str, dance : &Dance ) -> String {
    let n = 1000000000; // one billion times
    task(start, dance, n)
}