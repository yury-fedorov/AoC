extern crate linked_list;
/*
fn next(p : usize, dp: usize, n : usize ) -> usize { ( p + dp ) % n }

fn task_vec( steps : usize, last : i32 ) -> ( Vec<i32>, i32) {
    let mut buffer : Vec<i32> = vec![0];
    let mut position : usize = 0;
    let last1 = last + 1;
    for i in  1 .. last1 {
        position = next( position, steps, buffer.len() ) + 1;
        buffer.insert( position, i );
    }
    position = next( position, 1, buffer.len() );
    let next_element = buffer[position];
    ( buffer, next_element )
}
*/

fn task( steps : usize, last : i32 ) -> ( Vec<i32>, i32 ) {
    let mut buffer : linked_list::LinkedList<i32> = linked_list::LinkedList::new();
    let mut c = buffer.cursor();
    c.insert(0);
    let last1 = last + 1;
    let steps1 = steps + 1;
    for i in  1 .. last1 {
        for _j in 0 .. steps1 {
            if c.next().is_none() { c.next(); }
        }
        c.insert( i );
    }
    let mut next_element = None;
    for _j in 0 .. 2 {
        next_element = c.next();
        if next_element.is_none() { next_element = c.next(); }
    }
    let r_next_element = *next_element.unwrap();
    let r_vec : Vec<i32> = buffer.into_iter().collect();
    ( r_vec, r_next_element )
}

pub fn task1( steps : usize ) -> i32 {
    let last = 2017;
    task( steps, last ).1
}

pub fn task2( steps : usize ) -> i32 {
    let last = 50000000; // fifty millionth
    let buffer = task( steps, last ).0;
    let n = buffer.len();
    for i in 0 .. n {
        if buffer[i] == 0 { return buffer[(i+1) % n]; }
    }
    panic!( "unexpected to be here ever" );
}