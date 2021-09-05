extern crate regex;
use regex::Regex;
use std::collections::HashSet;

type Point = (i64,i64,i64);//x,y,z
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
    return p.0.abs() + p.1.abs() + p.2.abs();
}

fn distance( a : Point, b : Point ) -> usize {
    ( (a.0 - b.0).abs() + (a.1 - b.1).abs() + (a.2 - b.2).abs() ) as usize
}

fn add( a: Point, b: Point ) -> Point {
    ( (a.0 + b.0), (a.1 + b.1), (a.2 + b.2) )
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
        va = add( va, aa );
        vb = add( vb, ab );

        // update positions
        when += 1;
        pa = add( pa, va );
        pb = add( pb, vb );
        let new_distance = distance(pa, pb);
        if new_distance >= prev_distance {return None;}
        if new_distance == 0 {return Some(when);}
        prev_distance = new_distance;
    }
}
/*
fn round_time(t:f64) -> f64 {
    const N : f64 = 1000.0;
    return ( N * t ).round() / N;
}

// x = x0 + (v0+0.5)*t + (a*0.5)*t*t
// based on Ramanujan summation (1+2+3...)
fn collision_time_on_axis( pva0: Point, pva1: Point ) -> Vec<f64> {
    let dp : f64 = ( pva0.0 - pva1.0 ) as f64;
    let dv : f64 = ( pva0.1 - pva1.1 ) as f64;
    let da : f64 = ( pva0.2 - pva1.2 ) as f64;
    let c = dp;
    let b = dv;
    let a = da * 0.5;
    let det = b*b - 4.0*a*c;
    if det < 0.0 { return [].to_vec(); }
    if det == 0.0 { return [ round_time( -b / (2.0*a) ) ].to_vec(); }
    let sr = det.sqrt();
    let t0 = round_time((-b+sr) / (2.0*a) );
    let t1 = round_time((-b-sr) / (2.0*a) );
    if t0 > 0.0 && t1 > 0.0 { return [t0,t1].to_vec(); }
    if t0 > 0.0 { return [t0].to_vec(); }
    return [t1].to_vec();
}

fn get_collision_time1( a: PVA, b : PVA ) -> Option<f64> {
    let x = collision_time_on_axis(( a.0.0, a.1.0, a.2.0 ), ( b.0.0, b.1.0, b.2.0 ) );
    if x.len() == 0 { return None; }
    let y = collision_time_on_axis(( a.0.1, a.1.1, a.2.1 ), ( b.0.1, b.1.1, b.2.1 ) );
    if y.len() == 0 { return None; }
    let z = collision_time_on_axis(( a.0.2, a.1.2, a.2.2 ), ( b.0.2, b.1.2, b.2.2 ) );
    if z.len() == 0 { return None; }
    if x.len() == 1 && y.len() == 1 && z.len() == 1 {
        let t = x[0];
        if t == y[0] && t == z[0] { return Some(t); }
        return None;
    }
    for t in x.iter() {
        if y.iter().any( |i| *i == *t ) && z.iter().any( |i| *i == *t ) { return Some(*t); }
    }
    return None;
}
*/
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

    // let p0_unique : HashSet<Point> = d.iter().map(|pva| pva.0 ).collect();

    let all : HashSet<usize> = id.iter().map( |i| i.0 ).collect();
    let sure_left: HashSet<&usize> = all.iter()
        .filter( |i| all.iter()
            .any( |j| *j != **i && get_collision_time( d[**i], d[*j] ).is_some() ) == false
        ).collect();

    (a1, sure_left.len())
}