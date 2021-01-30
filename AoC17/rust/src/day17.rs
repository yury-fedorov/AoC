fn next( p : usize, dp: usize, n : usize ) -> usize { ( p + dp ) % n }

fn task( steps : usize, last : i32 ) -> ( Vec<i32>, usize ) {
    let mut buffer : Vec<i32> = vec![0];
    let mut position : usize = 0;
    let last1 = last + 1;
    for i in  1 .. last1 {
        position = next( position, steps, buffer.len() ) + 1;
        buffer.insert( position, i );
    }
    (buffer, position)
}

pub fn task1( steps : usize ) -> i32 {
    let last = 2017;
    let (buffer, mut position ) = task( steps, last );
    position = next( position, 1, buffer.len() );
    buffer[position]
}

pub fn task2( steps : usize ) -> i32 {
    let last = 50000000; // fifty millionth
    let (buffer, _ ) = task( steps, last );
    let n = buffer.len();
    for i in 0 .. n {
        if buffer[i] == 0 { return buffer[(i+1) % n]; }
    }
    panic!( "unexpected to be here ever" );
}