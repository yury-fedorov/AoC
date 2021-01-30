use std::sync::mpsc;
use std::thread;

pub type Input = (u32,u32);

const BASIS : u64 = 2147483647;

const CHANNEL_BOUND: usize = 100;
const CHUNK_SIZE: usize = 100000;
const MASK: u64 = 0xFFFF;

type FnNext = fn(u64) -> u64;

fn next_a( a : u64 ) -> u64 { ( a * 16807 ) % BASIS }
fn next_b( b : u64 ) -> u64 { ( b * 48271 ) % BASIS }


fn task( input : Input, na : FnNext, nb : FnNext, n : i32 ) -> i32 {
    let mask = 0xFFFF; // 16 bits - 2 bytes
    let mut count = 0;
    let mut ca = input.0 as u64;
    let mut cb = input.1 as u64;
    for _ in 0 .. n {
        ca = na( ca );
        cb = nb( cb );
        if ( ca & mask ) == ( cb & mask ) {
            count += 1;
        }
    }
    count
}

fn generate(sender: mpsc::SyncSender<Vec<u64>>, input: u64, fn_next: FnNext, n: usize) {
    let mut i: usize = 0;
    let mut total_i: usize = 0;
    let mut c: u64 = input;
    loop {
        let mut v: Vec<u64> = Vec::with_capacity(CHUNK_SIZE);
        while i < CHUNK_SIZE && total_i < n {
            v.push(c);
            c = fn_next(c);
            i += 1;
            total_i += 1;
        }
        sender.send(v).unwrap();
        i = 0;
        if total_i == n { return }
    }
}

pub fn task1_concurrent(input: Input) -> i32 {
    let n: usize = 40000000; // 40 millions
    let (sender1, receiver1) = mpsc::sync_channel(CHANNEL_BOUND);
    let (sender2, receiver2) = mpsc::sync_channel(CHANNEL_BOUND);

    thread::spawn(move|| generate(sender1, input.0 as u64, next_a, n));
    thread::spawn(move|| generate(sender2, input.1 as u64, next_b, n));

    let mut count: i32 = 0;
    let mut total_i: usize = 0;

    loop {
        let v1 = receiver1.recv().unwrap();
        let v2 = receiver2.recv().unwrap();
        for i in 0..CHUNK_SIZE {
            if ( v1[i] & MASK ) == ( v2[i] & MASK ) {
                count += 1;
            }
            total_i += 1;
            if total_i == n { return count }
        }
    }

}

pub fn task1( input : Input ) -> i32 {
    let n = 40000000; // 40 millions
    task( input, next_a, next_b, n )
}

fn next_2( v : u64, next : FnNext, base : u64 ) -> u64 {
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
    task( input, next_a2, next_b2, n )
}