fn distance( x:i32, y:i32 ) -> i32 {
    let ax = x.abs();
    let ay = y.abs();
    let dy = ay - ax;
    if dy > 0 { return ax + ( dy as f32 / 2.0 ).round() as i32 }
    ax
}

pub fn task1(path: &str) -> i32 { tasks(path).0 }

pub fn tasks(path: &str) -> (i32,i32) {
    let steps : Vec<&str>= path.trim().split(",").collect();
    let mut x : i32 = 0;
    let mut y : i32 = 0;
    let mut max_distance = -1;
    for s in &steps {
        let ss = String::from(*s);
        match *s {
            "n" => y -= 2,
            "s" => y += 2,
            "nw" => { y -= 1; x -= 1; },
            "sw" => { y += 1; x -= 1; },
            "ne" => { y -= 1; x += 1; },
            "se" => { y += 1; x += 1; },
            _ => panic!( ss )
        }
        max_distance = max_distance.max( distance(x, y) );
    }
    ( distance(x, y), max_distance )
}