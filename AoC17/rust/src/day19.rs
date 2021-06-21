#[derive(PartialEq, Copy, Clone)]
enum Direction {
    Down, Up, Left, Right
}

#[derive(PartialEq, Copy, Clone)]
enum Part {
    Cable, Letter, Space
}

type Point = (i32,i32);
type Map = Vec<String>;

fn next( p : &Point, d : &Direction ) -> Point {
    let (x,y) = p;
    match d {
        Direction::Down  => ( *x, *y + 1 ),
        Direction::Up    => ( *x, *y - 1 ),
        Direction::Left  => ( *x - 1, *y ),
        Direction::Right => ( *x + 1, *y )
    }
}

fn at( m : &Map, p : &Point ) -> char {
    let (x,y) = p;
    let empty = ' ';
    if *x < 0 || *y < 0 { return empty; }
    let sy = m.len() as i32;
    if *y >= sy { return empty; }
    let row : &str = m.get(*y as usize).unwrap();
    let sx = row.len() as i32;
    if *x >= sx { return empty; }
    return row.chars().nth( *x as usize).unwrap();
}

fn opposite( d: &Direction ) -> Direction {
    match *d {
        Direction::Right => Direction::Left,
        Direction::Left  => Direction::Right,
        Direction::Up    => Direction::Down,
        Direction::Down  => Direction::Up
    }
}

fn directions( d: &Direction ) -> Vec<Direction> {
    let mut r = vec![Direction::Down, Direction::Left, Direction::Right, Direction::Up];
    let o : Direction = opposite(d);
    let i = r.iter().position( |e| *e == o ).unwrap();
    r.remove(i);
    r
}

fn is_cross( c : char ) -> bool { c == '+' }

fn is_cable( c : char ) -> bool {
    match c {
        '-' | '|' | '+' => true,
        _ => false
    }
}

fn is_space( c : char ) -> bool { c == ' ' }

fn what( c : char ) -> Part {
    if is_cable(c) { return Part::Cable }
    if is_space( c ) { return Part::Space }
    Part::Letter
}

fn next_point( m: &Map, p : &Point, d : &Direction ) -> Option<( Point, Direction )> {
    let nd = directions(d);
    for d1 in nd {
        let p1 = next(&p, &d1);
        let c1 = at(&m, &p1);
        if !is_space(c1 ) {
            return Some(  ( p1, d1 ) );
        }
    }
    None
}

pub fn task12(map : &str) -> ( String, i32 ) {
    let m : Map = map.lines().map( |s| s.to_string() ).collect();
    let mut answer = String::new();
    let start = m.get(0).unwrap().find('|').unwrap() as i32;
    let mut p : Point = (start, 0);
    let mut d = Direction::Down;
    let mut steps = 1;
    loop {
        let c = at(&m, &p);
        let part = what(c);
        match part {
            Part::Letter => answer.push(c),
            Part::Space => 
                panic!( "must not ever happen" ),
            Part::Cable => {}
        }
        let p1 = next(&p, &d);
        let cross = is_cross(c) || ( part == Part::Letter && is_space( at( &m, &p1 ) ) );
        if cross {
            let o1 = next_point( &m, &p, &d );
            match o1 {
                None => break,
                Some( (p1, d1) ) => { p = p1; d = d1 }
            }
        } else {
            p = p1;
        }
        steps += 1;
    }
    ( answer, steps )
}
