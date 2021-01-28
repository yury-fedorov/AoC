type Input = (u32,u32);

fn next_a( a : u64 ) -> u64 { ( a * 16807 ) % 2147483647 }
fn next_b( b : u64 ) -> u64 { ( b * 48271 ) % 2147483647 }

pub fn task1( input : Input ) -> i32 {
    let n = 40000000; // 40 millions
    let mask = 0xFFFF; // 16 bits - 2 bytes
    let mut count = 0;
    let mut ca = input.0 as u64;
    let mut cb = input.1 as u64;
    for _ in 0 .. n {
        ca = next_a( ca );
        cb = next_b( cb );
        if ( ca & mask ) == ( cb & mask ) {
            count += 1;
        }
    }
    count
}

fn next_2( v : u64, next : fn(u64) -> u64, base : u64 ) -> u64 {
    let mut u = v;
    loop {
        u = next(u);
        if ( u & base ) == 0 { return u; } // fast way to check if dividable by 4 or 8
    }
}

fn next_a2( a : u64 ) -> u64 { next_2( a, next_a, 4 - 1 ) }
fn next_b2( b : u64 ) -> u64 { next_2( b, next_b, 8 - 1 ) }

pub fn task2( input : Input ) -> i32 {
    let n = 5000000; // 5 millions
    let mask = 0xFFFF; // 16 bits - 2 bytes
    let mut count = 0;
    let mut ca = input.0 as u64;
    let mut cb = input.1 as u64;
    for _ in 0 .. n {
        ca = next_a2( ca );
        cb = next_b2( cb );
        if ( ca & mask ) == ( cb & mask ) {
            count += 1;
        }
    }
    count
}