use std::collections::HashMap;
use std::collections::HashSet;

type Int = i32;
type Point = (Int,Int);
#[derive(Copy, Clone)]
enum Direction { Up, Left, Down, Right }
enum State { Clean, Weakened, Infected, Flagged }

const INFECTED : char = '#';

fn turn( d: Direction, to_right : bool )  -> Direction {
    match to_right {
        true => match d {
            Direction::Up => Direction::Right,
            Direction::Right => Direction::Down,
            Direction::Down => Direction::Left,
            Direction::Left => Direction::Up
        },
        false => match d {
            Direction::Up => Direction::Left,
            Direction::Left => Direction::Down,
            Direction::Down => Direction::Right,
            Direction::Right => Direction::Up
        }
    }
}

fn move_position( position : Point, direction : Direction ) -> Point {
    match direction {
        Direction::Up => ( position.0, position.1 + 1 ),
        Direction::Down => ( position.0, position.1 - 1 ),
        Direction::Left => ( position.0 - 1, position.1 ),
        Direction::Right => ( position.0 + 1, position.1 )
    }
}

pub fn task12( data : &str ) -> (Int,i32) {
    // initialization of the map
    let mut map : HashSet<Point> = HashSet::new();
    let initial_map : Vec<&str> = data.lines().collect();
    let n = initial_map.len() as Int;
    let c = n / 2 as Int; // 3 / 2 = 1
    for i in 0 .. n {
        for j in 0 .. n {
            let ch : char = initial_map[j as usize].chars().nth(i as usize).unwrap();
            if ch == INFECTED {
                map.insert( ( i - c, c - j ) );
            }
        }
    }
    let mut map2 : HashMap<Point,State> = map.iter().map( |i| (*i, State::Infected) ).collect();
    // infection 1
    let mut direction = Direction::Up;
    let mut position : Point = (0,0);
    let mut a1 : Int = 0;
    for _ in 0 .. 10_000 {
        let is_infected = map.contains( &position );
        direction = turn( direction.clone(), is_infected );
        match is_infected {
            true => map.remove( &position ),
            false => { a1 += 1; map.insert(position) } };
        position = move_position( position, direction  );
    }
    // infection 2
    direction = Direction::Up;
    position = (0,0);
    let mut a2 : Int = 0;
    const LEFT : bool = false;
    const RIGHT: bool = true;
    for _ in 0 .. 10_000_000 {
        let state0 = map2.get(&position).unwrap_or( &State::Clean );
        let state1 = match state0 {
            State::Clean => State::Weakened,
            State::Weakened => State::Infected,
            State::Infected => State::Flagged,
            State::Flagged => State::Clean
        };
        let direction0 = direction.clone();
        direction = match state0 {
            State::Clean => turn( direction0, LEFT ),
            State::Weakened => direction0,
            State::Infected => turn(direction0, RIGHT ),
            State::Flagged => turn (turn( direction0, LEFT ), LEFT ) // invert
        };
        a2 += match state1 { State::Infected => 1, _ => 0 };
        match state1 {
            State::Clean => map2.remove( &position ),
            _ => map2.insert(position, state1 ) };
        position = move_position( position, direction  );
    }
    (a1,a2)
}