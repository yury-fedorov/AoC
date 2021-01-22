extern crate regex;
use regex::Regex;
use crate::common;
use std::str::FromStr;
// type Result<T> = std::result::Result<T, Box<dyn error::Error>>;

pub fn task1() -> String {
   let text = common::input( "d07/sample.txt" );
   let lines = text.lines();
   // https://docs.rs/regex/1.4.3/regex/
   lazy_static! {
        static ref re: Regex = Regex::new(r"(\w{4}) \((\d+)\)(.+)").unwrap();
   }
   // https://doc.rust-lang.org/rust-by-example/primitives/tuples.html
   type Data = (String, i32);
   // https://doc.rust-lang.org/book/ch08-01-vectors.html
   let mut v: Vec<Data> = Vec::new();
   // https://doc.rust-lang.org/rust-by-example/std_misc/file/read_lines.html
   for line in lines {
      for cap in re.captures_iter(line) {
         v.push( (cap[1].to_string(), i32::from_str(&cap[2]).unwrap() ) );
      }
   }
   let (name,_w) = v.get(0).unwrap();
   String::from(name)
}