use std::str::FromStr;

fn swap( list : &mut Vec<i32>, ai : usize, bi : usize ) {
    let n = list.len();
    let av = list[ai % n];
    let bv = list[bi % n];
    list[ai % n] = bv;
    list[bi % n] = av;
}

fn reverse(list: &mut Vec<i32>, cur_pos : i32, cur_len : i32 ) {
    let last = cur_pos + cur_len - 1;
    let half = cur_len / 2;
    for di in 0 .. half {
        swap(list, ( cur_pos + di ) as usize, ( last - di ) as usize );
    }
}

pub fn task1(n : i32, l : &str ) -> i32 {
    // https://doc.rust-lang.org/std/ops/struct.Range.html
    let mut list : Vec<i32> = (0 .. n).collect();
    let lengths : Vec<i32> = l.split("," ).map( |e| i32::from_str( e.trim() ).unwrap() ).collect();
    let mut cur_pos = 0;
    let mut skip_size = 0;
    for cur_len in lengths {
        reverse( &mut list, cur_pos, cur_len );
        cur_pos = ( cur_pos + cur_len + skip_size ) % n;
        skip_size += 1;
    }
    list.get(0).unwrap() * list.get(1).unwrap()
}