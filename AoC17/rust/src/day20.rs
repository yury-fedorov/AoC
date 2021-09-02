extern crate regex;
use regex::Regex;

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

pub fn task12(data : &str) -> ( usize, i32 ) {
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
    /*
    let v : Vec<i64> = a1.iter().filter( |pva| { let (_,v,_) = pva; return abs_sum(v); } ).collect();
    let min_v = v.iter().min().unwrap();
    let v1 = d.iter().filter( |pva| { let (_,v,_) = pva; return abs_sum(v) == min_v; } ).collect();
    */
    (a1,0)
}