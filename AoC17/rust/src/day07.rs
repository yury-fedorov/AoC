extern crate regex;
use regex::Regex;
use crate::common;
use std::str::FromStr;
use std::collections::HashSet;

fn tail( t : &str ) -> HashSet<String> {
   lazy_static! {
        static ref RE_TAIL: Regex = Regex::new(r" -> (.+)").unwrap();
        static ref RE_ELEMENT: Regex = Regex::new(r"([a-z]+),?").unwrap();
   }

   for cap_tail in RE_TAIL.captures_iter(t) {
      return RE_ELEMENT
          .captures_iter( &cap_tail[1] )
          .map(|cap_element| cap_element[1].to_string())
          .collect() // collect() collects the iterator into the target type (the HashSet<String>)
   }
   HashSet::new()
}

pub fn task1( file : &str ) -> String {
   let text = common::input(file);
   // https://doc.rust-lang.org/rust-by-example/primitives/tuples.html
   type Data = (String, i32, HashSet<String>);
   // https://docs.rs/regex/1.4.3/regex/
   lazy_static! {
        static ref RE_LINE: Regex = Regex::new(r"([a-z]+) \((\d+)\)(.+)").unwrap();
   }

   // https://doc.rust-lang.org/book/ch08-01-vectors.html

   let v: Vec<Data> = text
       // https://doc.rust-lang.org/rust-by-example/std_misc/file/read_lines.html
       .lines()
       .into_iter()
       .flat_map(|line| RE_LINE.captures_iter(line))
       .map(|cap| (cap[1].to_string(), i32::from_str(&cap[2]).unwrap(), tail(&cap[3]) ))
       .collect();

   let mut result : Vec<String> = v
       .clone()
       .into_iter()
       .map(|(n,_,_)| n)
       .collect();

   for (_,_,s) in &v {
      result.retain( |x| !s.contains(x) );
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
