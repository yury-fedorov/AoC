use std::str::FromStr;
use std::collections::{HashMap, HashSet};

fn parse_line(line: &str ) -> (i32, i32) {
    let a : Vec<i32> = line.trim().split( ": " )
        .map( |e| i32::from_str( e ).unwrap() ).collect();
    ( a[0], a[1] )
}

fn severity( map: &HashMap<i32,i32>, caught: &HashSet<i32> ) -> i32 {
    let r : Vec<i32> = caught.into_iter().map( |c| ( map[c] * c ) ).collect();
    r.into_iter().sum()
}

fn inverse(direction: i32) -> i32 { -1 * direction }

// ( cur position, direction of scanner (+1, -1), max range )
type Position = (i32, i32, i32);

fn run( depth_position: &mut HashMap<i32,Position>, max_depth: i32,
        all_scanners : &Vec<i32>, stop_when_caught : bool ) -> HashSet<i32> {
    let mut caught : HashSet<i32> = HashSet::new();
    for cur_depth in 0 .. max_depth {
        let position = depth_position.get(&cur_depth );
        if position.is_some() {
            // we have a scanner at this depth going here and there
            let cur_pos = position.unwrap().0;
            if cur_pos == 0 {
                // caught in the layer (current depth)
                caught.insert(cur_depth);
                if stop_when_caught { return caught } // stop immediately
            }
        }
        move_scanners(depth_position, &all_scanners);
    }
    caught
}

fn move_scanners(depth_position: &mut HashMap<i32, (i32, i32, i32)>, all_scanners: &Vec<i32>) {
    // we move scanners
    for depth in all_scanners {
        let (cur_pos, dir, max_range) = depth_position[&depth];
        let mut next_pos = cur_pos + dir; // just for check
        let next_dir = if next_pos > max_range || next_pos < 0 { inverse(dir) } else { dir };
        next_pos = cur_pos + next_dir; // real next position
        depth_position.insert(*depth, (next_pos, next_dir, max_range));
    }
}

type State = ( HashMap<i32,i32>, HashMap<i32,Position>, Vec<i32>, i32 );

pub fn input( conf: &str ) -> State {
    let map : HashMap<i32,i32> = conf.lines().into_iter().map( parse_line ).collect();
    let depth_position: HashMap<i32,Position> = map.clone().into_iter()
        .map( |e| ( e.0, (0, 1, e.1 - 1) ) ).collect();
    let all_scanners : Vec<i32> = map.clone().into_iter().map( |e| e.0 ).collect();
    let max_depth = all_scanners.clone().into_iter().max().unwrap();
    ( map, depth_position, all_scanners, max_depth + 1 )
}

pub fn task1( state: &State ) -> i32 {
    let (map, dp, scanners, max_depth ) = state;
    let mut depth_position = dp.clone();
    severity( &map, &run( &mut depth_position, *max_depth,
                          scanners,  false) )
}

pub fn task2( state : &State ) -> i32 {
    let (_, dp, scanners, max_depth ) = state;
    let mut depth_position = dp.clone();
    let mut t = 0;
    loop {
        let mut copy = depth_position.clone();
        let c = run( &mut copy, *max_depth,
                     scanners,  true);
        if c.is_empty() { return t; }
        move_scanners( &mut depth_position, scanners );
        t += 1;
    }
}