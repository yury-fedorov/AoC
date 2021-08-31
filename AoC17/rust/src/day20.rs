extern crate regex;
use regex::Regex;

struct Point3 {
    pub x: i64,
    pub y: i64,
    pub z: i64,
}

fn parse( line : &str ) -> ( Point3, Point3, Point3 ) {
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

            return ( Point3 {x:px, y:py, z:pz}, Point3{x:vx, y:vy,z:vz}, Point3{x:ax,y:ay,z:az} );
        } ).unwrap();
}

pub fn task12(data : &str) -> ( usize, i32 ) {
    let d : Vec<( Point3, Point3, Point3 )> = data.lines().map( |l| parse(l) ).collect();
    let a : Vec<i64> = d.iter().map( |pva| { let (p,v,a) = pva; return a.x.abs() + a.y.abs() + a.z.abs(); } ).collect();
    let min_a = a.iter().min().unwrap();
    let a1 = a.iter().position(|ai| ai == min_a).unwrap();
    (a1,-1)
}