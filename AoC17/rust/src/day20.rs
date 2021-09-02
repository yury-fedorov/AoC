extern crate regex;
use regex::Regex;
use std::collections::HashSet;

type Point = (i64,i64,i64);
type PVA = (Point,Point,Point);

fn parse( line : &str ) -> PVA {
    lazy_static! {
        // p=<-833,-499,-1391>, v=<84,17,61>, a=<-4,1,1>
        static ref RE_LINE: Regex = Regex::new(r"p=<([-]?\d+),([-]?\d+),([-]?\d+)>, v=<([-]?\d+),([-]?\d+),([-]?\d+)>, a=<([-]?\d+),([-]?\d+),([-]?\d+)>$").unwrap();

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
    let (x,y,z) = p;
    return x.abs() + y.abs() + z.abs();
}

fn distance( a : Point, b : Point ) -> usize {
    ( (a.0 - b.0).abs() + (a.1 - b.1).abs() + (a.2 - b.2).abs() ) as usize
}

fn get_collision_time( a: PVA, b: PVA ) -> Option<usize> {
    let mut pa = a.0;
    let mut pb = b.0;
    let mut va = a.1;
    let mut vb = b.1;
    let aa = a.2;
    let ab = b.2;
    let mut prev_distance = distance(pa, pb);
    let mut when : usize = 0;
    if prev_distance == 0 { return Some(when) };
    loop {
        // update velocities
        va.0 += aa.0; va.1 += aa.1; va.2 += aa.2;
        vb.0 += ab.0; vb.1 += ab.1; vb.2 += ab.2;
        // update positions
        when += 1;
        pa.0 += va.0; pb.0 += vb.0;
        let dx = distance(pa, pb);
        if dx > prev_distance { return None; }
        if dx == 0 { return Some(when); }

        when += 1;
        pa.1 += va.1; pb.1 += vb.1;
        let dy = distance(pa, pb);
        if dy > dx { return None; }
        if dy == 0 { return Some(when); }

        when += 1;
        pa.2 += va.2; pb.2 += vb.2;
        let dz = distance(pa, pb);
        if dz > dx {return None;}
        if dz == 0 {return Some(when);}
        if prev_distance == dz {return None;}
        prev_distance = dz;
    }
}

pub fn task12(data : &str) -> ( usize, usize ) {
    let d : Vec<PVA> = data.lines().map( parse ).collect();
    let id : Vec<(usize,PVA)> = d.iter().enumerate().map(|i| (i.0, *(i.1)) ).collect();
    // we sort by minimal acceleration first
    let min_a : i64 = id.iter()
        .map( |idi| { let (_,_,a) = idi.1; return abs_sum(a); } ).min().unwrap();
    let a : Vec<&(usize,PVA)> = id.iter()
        .filter( |idi| { let (_,_,a) = idi.1; return abs_sum(a) == min_a; } )
        .collect();
    // XXX dirty - for clean way we need to sort filter also by velocity and position
    let a1 = a.last().unwrap().0;

    let all : HashSet<usize> = id.iter().map( |i| i.0 ).collect();
    let sureLeft : HashSet<&usize> = all.iter()
        .filter( |i| all.iter()
            .any( |j| *j != **i && get_collision_time( d[**i], d[*j] ).is_some() ) == false
        ).collect();

    (a1,sureLeft.len())
}