enum Direction {
    Down, Up, Left, Right
}

type Point = (i32,i32);

fn next( p : Point, d : Direction ) -> Point {
    let (x,y) = p;
    match d {
        Down => ( x, y + 1 ),
        Up => ( x, y - 1 ),
        Left => ( x - 1, y ),
        Rigth => ( x + 1, y )
    }
} 

pub fn task1(map : &str) -> String {
    let ml : Vec<&str> = map.lines().collect();
    let mut answer = String::new();
    let sx = ml.get(0).unwrap().len();
    let sy = ml.len();
    let start = ml.get(0).unwrap().find('|').unwrap() as i32;
    let mut p : Point = (start, 0);
    let mut d = Direction::Down;
    while true {
        let p1 = next(p, d.clone() );
    }
    map.chars()
    // .into_iter()
    .filter( |c| match c { ' ' | '-' | '|' | '+' | '\r' | '\n' => true, _ => false } );
    // .as_str()
    start.to_string()
}