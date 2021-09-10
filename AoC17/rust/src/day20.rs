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

fn distance( a : Point, b : Point ) -> usize {
    ( (a.0 - b.0).abs() + (a.1 - b.1).abs() + (a.2 - b.2).abs() ) as usize
}

fn add( a: Point, b: Point ) -> Point {
    ( (a.0 + b.0), (a.1 + b.1), (a.2 + b.2) )
}

// x = x0 + (v0+0.5)*t + (a*0.5)*t*t
// based on Ramanujan summation (1+2+3...)
// rough approximation just to get till when to run the simulation
// Some(0) - means the solution is  valid for any number
fn collisions_time_on_axis_inner( pva0: Point, pva1: Point ) -> Vec<f64> {
    let dp : f64 = ( pva0.0 - pva1.0 ) as f64;
    let dv : f64 = ( pva0.1 - pva1.1 ) as f64;
    let da : f64 = ( pva0.2 - pva1.2 ) as f64;
    let c = dp;
    let b = dv;
    if da == 0.0 {
        // b * t = -c => t = -c / b
        return [ match b { 0.0 => 0.0, _ => ( -c / b  ) } ].to_vec();
    }
    let a = da * 0.5;
    let det = ( b * b ) - ( 4.0 * a * c );
    if det < 0.0 { return [].to_vec(); }
    let a2 = 2.0 * a;
    if det == 0.0 { return [ ( -b / a2 ) ].to_vec(); }
    let sr = det.sqrt();
    let t0 = ( (-b+sr) / a2 );
    let t1 = ( (-b-sr) / a2 );
    [t0, t1].to_vec()
}

fn collisions_time_on_axis( pva0: Point, pva1: Point ) -> Vec<i64> { 
    collisions_time_on_axis_inner(pva0,  pva1).iter()
        .filter( |i| **i >= 0.0 ).map( |i| i.ceil() as i64 ).collect()
}

fn approx_collision_time( a: PVA, b : PVA ) -> Option<i64> {
    let x = collisions_time_on_axis(( a.0.0, a.1.0, a.2.0 ), ( b.0.0, b.1.0, b.2.0 ) );
    if x.len() == 0 { return None; }
    let y = collisions_time_on_axis(( a.0.1, a.1.1, a.2.1 ), ( b.0.1, b.1.1, b.2.1 ) );
    if y.len() == 0 { return None; }
    let z = collisions_time_on_axis(( a.0.2, a.1.2, a.2.2 ), ( b.0.2, b.1.2, b.2.2 ) );
    if z.len() == 0 { return None; }
    let mut result : HashSet<i64> = HashSet::new();
    for xi in &x {
        for yi in &y  {
            for zi in &z {
                let i = [xi,yi,zi];
                let min_i : i64 = **i.iter().min().unwrap();
                let max_i : i64 = **i.iter().max().unwrap();
                if min_i == max_i { result.insert( max_i ); }
                else if min_i > 0 && max_i > min_i { }
                else if min_i == 0 && max_i > min_i {
                    let is_other = i.iter().any(|j| j > &&min_i && j < &&max_i );
                    if !is_other { result.insert( max_i ); }
                } else {
                    panic!("unexpected {} {} {}", xi, yi, zi)
                }
            }
        }
    }
    match result.len() {
        0 => None,
        1 => Some( *result.iter().nth(0).unwrap() ),
        _  => panic!( "too many results" )
    }
}

fn get_collision_time( a: PVA, b: PVA ) -> Option<usize> {
    let mut pa = a.0;
    let mut pb = b.0;
    let mut va = a.1;
    let mut vb = b.1;
    let aa = a.2;
    let ab = b.2;
    let max_t = approx_collision_time( a, b );
    if max_t.is_none() { return None; }
    for t in 1 .. ( max_t.unwrap() + 2 ) as usize {
        // update velocities
        va = add( va, aa );
        vb = add( vb, ab );

        // update positions
        pa = add( pa, va );
        pb = add( pb, vb );

        if distance(pa, pb) == 0 { return Some(t); }
    }
    None
}

pub fn to_pva(data : &str) -> Vec<PVA> {
    return data.lines().map( parse ).collect();
}

fn min_time( d : &Vec<PVA>, all : &HashSet<usize> ) -> Option<usize> {
    let mut t : Option<usize> = None;
    for i in all {
        for j  in all {
            if i > j {
                let ti = get_collision_time( d[*i], d[*j] );
                t = match ( t, ti ) {
                    ( Some(a), Some(b) ) => Some( a.min(b) ),
                    ( None, Some(b) ) => Some(b),
                    ( a, _ ) => a
                }
            }
        }
    }
    t
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

    let all : HashSet<usize> = id.iter().map( |i| i.0 ).collect();
    let mut left = all.clone();
    loop {
        let min_collision_t: Option<usize> = min_time( &d, &left );
        if min_collision_t.is_none() { break; }
        let t : usize = min_collision_t.unwrap();
        let after : HashSet<usize> = left.iter()
            .filter( |i| !left.iter().any(|j|
                 {
                     if *j == **i { return false; }
                     let tj = get_collision_time( d[**i], d[*j] );
                     if tj.is_none() { return false; }
                     tj.unwrap() == t
                 }
            ) ).map( |i| *i )
            .collect();
        left = after;
    }
    /*
    let sure_left: HashSet<&usize> = all.iter()
        .filter( |i| all.iter()
            .any( |j| *j != **i && get_collision_time( d[**i], d[*j] ).is_some() ) == false
        ).collect();
    (a1, sure_left.len() )
    */
    (a1, left.len() )
}