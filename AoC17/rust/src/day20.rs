extern crate regex;
use regex::Regex;
use std::collections::HashSet;

pub type Point = (i64,i64,i64); // x, y, z
pub type PVA = (Point,Point,Point); // Point, Velocity, Acceleration

fn parse( line : &str ) -> PVA {
    lazy_static! {
        // p=<-833,-499,-1391>, v=<84,17,61>, a=<-4,1,1>
        static ref RE_LINE: Regex = Regex::new(
            r"p=<([-]?\d+),([-]?\d+),([-]?\d+)>, v=<([-]?\d+),([-]?\d+),([-]?\d+)>, a=<([-]?\d+),([-]?\d+),([-]?\d+)>$").unwrap();
    }
    return RE_LINE.captures_iter(line).last()
        .map(|cap| {
            let px : i64 = cap[1].to_string().parse().unwrap();
            let py : i64 = cap[2].to_string().parse().unwrap();
            let pz : i64 = cap[3].to_string().parse().unwrap();

            let vx : i64 = cap[4].to_string().parse().unwrap();
            let vy : i64 = cap[5].to_string().parse().unwrap();
            let vz : i64 = cap[6].to_string().parse().unwrap();

            let ax : i64 = cap[7].to_string().parse().unwrap();
            let ay : i64 = cap[8].to_string().parse().unwrap();
            let az : i64 = cap[9].to_string().parse().unwrap();

            return ( (px, py, pz), (vx, vy, vz), (ax,ay,az) );
        } ).unwrap();
}

fn abs_sum( p : Point ) -> i64 {
    return p.0.abs() + p.1.abs() + p.2.abs();
}

fn distance( a : Point, b : Point ) -> i64 {
    (a.0 - b.0).abs() + (a.1 - b.1).abs() + (a.2 - b.2).abs()
}

fn add( a: Point, b: Point ) -> Point {
    ( (a.0 + b.0), (a.1 + b.1), (a.2 + b.2) )
}

pub fn to_pva(data : &str) -> Vec<PVA> {
    return data.lines().map( parse ).collect();
}

pub fn task12(data : &str) -> ( usize, usize ) {
    let d : Vec<PVA> = to_pva(data);
    let id : Vec<(usize,PVA)> = d.iter().enumerate().map(|i| (i.0, *(i.1)) ).collect();
    // we sort by minimal acceleration first
    let min_a : i64 = id.iter()
        .map( |idi| { let (_,_,a) = idi.1; return abs_sum(a); } ).min().unwrap();
    let a : Vec<&(usize,PVA)> = id.iter()
        .filter( |idi| { let (_,_,a) = idi.1; return abs_sum(a) == min_a; } )
        .collect();
    // XXX dirty - for clean way we need to sort filter also by velocity and position
    let a1 = a.last().unwrap().0;

    let mut left = id;
    let mut count_down = 100;
    for _t in 0 .. 1_000_000 {
        let mut to_remove : HashSet<usize> = HashSet::new();
        for ida in &left {
            for idb in &left {
                if ida.0 < idb.0 {
                    if distance( ida.1.0, idb.1.0 ) == 0 {
                        to_remove.insert( ida.0 );
                        to_remove.insert( idb.0 );
                    }
                }
            }
        }

        left = left.iter().filter( |i| !to_remove.contains(&i.0) )
            .map( |i| { let mut p = i.1.0; let mut v = i.1.1; let a = i.1.2;
                v = add( v, a );
                p = add( p, v);
                ( i.0, ( p, v, a ) )
            } )
            .collect();
        
        if left.len() <= 1 { break; }
        if to_remove.len() == 0  { count_down-=1; } else { count_down = 100; }
        if count_down <= 0 { break; }
    }
    (a1, left.len() )
}