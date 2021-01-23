extern crate regex;
use regex::Regex;
use crate::common;
use std::str::FromStr;
use std::collections::HashSet;
// type Result<T> = std::result::Result<T, Box<dyn error::Error>>;

fn tail( t : &str ) -> Option<HashSet<String>> {
   lazy_static! {
        static ref RE_TAIL: Regex = Regex::new(r" -> (.+)").unwrap();
        static ref RE_ELEMENT: Regex = Regex::new(r"([a-z]+),?").unwrap();
   }
   for capTail in RE_TAIL.captures_iter(t) {
      let mut set = HashSet::new();
      for capElement in RE_ELEMENT.captures_iter( &capTail[1] ) {
         set.insert( capElement[1].to_string() );
      }
      return Some(set);
   }
   None
}

pub fn task1( file : &str ) -> String {
   let text = common::input(file);
   let lines = text.lines();
   // https://docs.rs/regex/1.4.3/regex/
   lazy_static! {
        static ref RE_LINE: Regex = Regex::new(r"([a-z]+) \((\d+)\)(.+)").unwrap();
   }
   // https://doc.rust-lang.org/rust-by-example/primitives/tuples.html
   type Data = (String, i32, Option<HashSet<String>>);
   // https://doc.rust-lang.org/book/ch08-01-vectors.html
   let mut v: Vec<Data> = Vec::new();
   // https://doc.rust-lang.org/rust-by-example/std_misc/file/read_lines.html
   for line in lines {
      for cap in RE_LINE.captures_iter(line) {
         v.push( (cap[1].to_string(), i32::from_str(&cap[2]).unwrap(), tail(&cap[3]) ) );
      }
   }
   let mut result : Vec<String> = Vec::new();
   for (n,_,_) in &v {
      result.push(n.to_string());
   }
   for (_,_,o) in &v {
      match o {
         Some(s) => result.retain( |x| !s.contains(x) ),
         None => {}
      }
   }
   if result.len() != 1 {
      let mut error = String::new();
      error.push_str("Unexpected content: ");
      for e in result {
         error.push_str( &e );
         error.push_str( ", ");
      }
      panic!( error );
   }
   String::from(result.get(0).unwrap())
}
