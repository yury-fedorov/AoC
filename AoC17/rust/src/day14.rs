use std::collections::{HashMap, HashSet};

fn knot_hash(s: &str) -> String {
    use crate::day10;
    let list_size = 256;
    let hash = day10::task2(list_size, s);
    assert_eq!(32, hash.len());
    hash
}

fn key(input: &str, row: i32) -> String {
    format!("{}-{}", input, row)
}

fn hex_to_byte(ch: char) -> u8 {
    u8::from_str_radix(&ch.to_string(), 16).unwrap()
}

pub fn count(hash: &str) -> u32 {
    hash.chars()
        .into_iter()
        .map(|c| hex_to_byte(c).count_ones())
        .sum()
}

pub fn input(input: &str) -> Vec<String> {
    (0..128)
        .into_iter()
        .map(|r| knot_hash(&key(input, r)))
        .collect()
}

pub fn task1(map: &Vec<String>) -> u32 {
    map.into_iter().map(|h| count(&h)).sum()
}

fn get_bit(input: u8, n: u8) -> bool {
    input & (1 << n) != 0
}

fn hex_to_bin(ch: char) -> Vec<bool> {
    let v = hex_to_byte(ch);
    let mut result: Vec<bool> = Vec::new();
    let n = 4;
    for i in 0..n {
        result.insert(0, get_bit(v, i));
    }
    result
}

type Point = (i32, i32);

fn close(a: &Point, b: &Point) -> bool {
    let dx = (a.0 - b.0).abs();
    let dy = (a.1 - b.1).abs();
    (dx == 0 && dy == 1) || (dx == 1 && dy == 0)
}

// takes more than 10 seconds, could have sense to use for the search instead of on a no_group subset
pub fn task2(map: &Vec<String>) -> usize {
    let n: i32 = 128;
    let mut on: HashMap<Point, i32> = HashMap::new();
    let no_group = -1;
    for y in 0..n {
        let r: Vec<bool> = map
            .get(y as usize)
            .unwrap()
            .chars()
            .flat_map(|c| hex_to_bin(c))
            .collect();
        assert_eq!(n as usize, r.len());
        for x in 0..n {
            let is_set = r.get(x as usize).unwrap();
            if *is_set {
                on.insert((x, y), no_group);
            }
        }
    }
    let max_gr = on.len() as i32;
    for cur_gr in 0..max_gr {
        let oe = on.clone().into_iter().find(|e| e.1 == no_group);
        if oe.is_none() {
            break;
        }
        // this is a new group
        let p0 = oe.unwrap().0;
        on.insert(p0, cur_gr);
        let mut s: HashSet<Point> = HashSet::new();
        s.insert(p0);
        while !s.is_empty() {
            let mut s1: HashSet<Point> = HashSet::new();
            for p in s {
                let close_points: Vec<Point> = on
                    .clone()
                    .into_iter()
                    .filter(|e| e.1 == no_group && close(&e.0, &p))
                    .map(|e| e.0)
                    .collect();
                s1.extend(close_points);
            }
            for p in &s1 {
                on.insert(*p, cur_gr);
            }
            s = s1;
        }
    }

    let set: HashSet<i32> = on.into_iter().map(|e| e.1).collect();
    assert!(!set.contains(&no_group));
    set.len()
}
