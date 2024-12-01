use std::collections::{HashMap, HashSet};
use std::str::FromStr;
extern crate regex;
use regex::Regex;

type Data = (i32, Vec<i32>);

fn parse_list( list : &str ) -> Vec<i32> {
    list.split(", " ).map( |s| i32::from_str( s.trim() ).unwrap() ).collect()
}

fn parse_line( line : &str ) -> Data {
    lazy_static! {
        static ref RE_LINE: Regex = Regex::new(r"^(\d+) <-> (.*)$").unwrap();
    }
    RE_LINE.captures_iter(line)
        .map( |cap| ( i32::from_str( &cap[1] ).unwrap(), parse_list( &cap[2] ) ) )
        .last().unwrap()
}

pub fn input(conf : &str) -> HashMap<i32, Vec<i32>> {
    conf.lines().into_iter().map( parse_line ).collect()
}

fn group( input : &HashMap<i32, Vec<i32>>, element : i32 ) -> HashSet<i32> {
    let mut group : HashSet<i32> = HashSet::new();
    let mut to_process : HashSet<i32> = HashSet::new();
    to_process.insert(element);
    let mut processed : HashSet<i32> = HashSet::new();
    while !to_process.is_empty() {
        let mut to_process_1 : HashSet<i32> = HashSet::new();
        for id in to_process {
            group.insert(id);
            processed.insert(id);
            let subgroup = &input[&id];
            to_process_1.extend( subgroup.into_iter()
                .filter( |e| !processed.contains( e ) ) );
        }
        to_process = to_process_1;
    }
    group
}

pub fn task1( input : &HashMap<i32, Vec<i32>> ) -> usize {
    group(input, 0 ).len()
}

pub fn task2( input : &HashMap<i32, Vec<i32>> ) -> usize {
    let mut all : HashSet<i32> = HashSet::new();
    all.extend( input.into_iter().map( |e| e.0 ) );
    all.extend( input.into_iter().flat_map( |e| e.1 ) );
    let mut count = 0;

    loop {
        let eo = all.clone().into_iter().last();
        if eo.is_none() { break; }
        let g = group(input, eo.unwrap());
        all.retain(|e| !g.contains(e));
        count += 1;
    }
    count
}

pub fn tasks(conf : &str) -> (usize,usize) {
    let i = input( conf );
    ( task1(&i), task2( &i) )
}