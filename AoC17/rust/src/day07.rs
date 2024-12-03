extern crate regex;
use crate::common;
use regex::Regex;
use std::collections::{HashMap, HashSet};
use std::str::FromStr;

fn tail(t: &str) -> HashSet<String> {
    lazy_static! {
        static ref RE_TAIL: Regex = Regex::new(r" -> (.+)").unwrap();
        static ref RE_ELEMENT: Regex = Regex::new(r"([a-z]+),?").unwrap();
    }

    for cap_tail in RE_TAIL.captures_iter(t) {
        return RE_ELEMENT
            .captures_iter(&cap_tail[1])
            .map(|cap_element| cap_element[1].to_string())
            .collect(); // collect() collects the iterator into the target type (the HashSet<String>)
    }
    HashSet::new()
}

// https://doc.rust-lang.org/rust-by-example/primitives/tuples.html
type Data = (String, i32, HashSet<String>);
pub fn parse(file: &str) -> Vec<Data> {
    let text = common::input(file);
    // https://docs.rs/regex/1.4.3/regex/
    lazy_static! {
        static ref RE_LINE: Regex = Regex::new(r"^([a-z]+) \((\d+)\)(.*)$").unwrap();
    }
    // https://doc.rust-lang.org/book/ch08-01-vectors.html
    let v: Vec<Data> = text
        // https://doc.rust-lang.org/rust-by-example/std_misc/file/read_lines.html
        .lines()
        .into_iter()
        .flat_map(|line| RE_LINE.captures_iter(line))
        .map(|cap| {
            (
                cap[1].to_string(),
                i32::from_str(&cap[2]).unwrap(),
                tail(&cap[3]),
            )
        })
        .collect();
    v
}

pub fn task1(v: &Vec<Data>) -> String {
    let mut result: Vec<String> = v.clone().into_iter().map(|(n, _, _)| n).collect();

    for (_, _, s) in v {
        result.retain(|x| !s.contains(x));
    }

    if result.len() != 1 {
        panic!("Unexpected state. There must be one head.");
    }
    String::from(result.get(0).unwrap())
}

fn get_weight(tree: &HashMap<&str, &Data>, head: &str) -> i32 {
    let (_, w, leaves) = tree.get(head).unwrap();
    let mut result: i32 = *w;
    for l in leaves {
        result += get_weight(tree, l);
    }
    result
}

pub fn task2(v: &Vec<Data>, head: &str) -> i32 {
    let mut tree: HashMap<&str, &Data> = HashMap::new();
    for e in v {
        let (name, _, _) = e;
        tree.insert(name, e);
    }
    let (_, _, leaves) = tree.get(head).unwrap();

    let mut level_up = check_balance(&tree, leaves);
    while level_up.is_some() {
        let (node_to_fix, diff) = level_up.unwrap();
        let unbalanced_node = tree.get(&*node_to_fix);
        let (_, weight, subnodes) = unbalanced_node.unwrap();
        let level_down = check_balance(&tree, subnodes);
        if level_down.is_none() {
            return weight + diff;
        }
        level_up = level_down;
    }
    panic!("unexpected");
}

fn check_balance(
    tree: &HashMap<&str, &(String, i32, HashSet<String>)>,
    leaves: &HashSet<String>,
) -> Option<(String, i32)> {
    let mut a: i32 = -1;
    let mut b: i32 = -1;
    let mut a_set = HashSet::new();
    let mut b_set = HashSet::new();
    for l in leaves {
        let w = get_weight(&tree, l);
        if a < 0 {
            a = w;
            a_set.insert(l);
        } else if a == w {
            a_set.insert(l);
        } else if b < 0 {
            b = w;
            b_set.insert(l);
        } else if b == w {
            b_set.insert(l);
        } else {
            panic!("Unexpected 3rd different weight");
        }
    }

    let node_to_fix: &str;
    let diff: i32;
    if a_set.len() == 1 && b_set.len() > 1 {
        diff = b - a;
        node_to_fix = a_set.iter().last().unwrap();
    } else if b_set.len() == 1 && a_set.len() > 1 {
        diff = a - b;
        node_to_fix = b_set.iter().last().unwrap();
    } else if b_set.len() == 0 {
        return None;
    } else {
        panic!("Unexpected distribution: only one bad node is expected");
    }
    return Some((node_to_fix.to_string(), diff));
}
