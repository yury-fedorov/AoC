fn knot_hash( s : &str ) -> String {
    use crate::day10;
    let list_size = 256;
    let hash = day10::task2( list_size, s );
    assert_eq!(32, hash.len());
    hash
}

fn key( input : &str, row : i32 ) -> String { format!( "{}-{}", input, row ) }

fn hex_to_byte( ch : char ) -> u8 {
    u8::from_str_radix(&ch.to_string(), 16).unwrap()
}

pub fn count( hash : &str ) -> u32 {
    hash.chars().into_iter().map( |c| hex_to_byte(c).count_ones() ).sum()
}

pub fn task1( input: &str ) -> u32 {
    let map : Vec<String> = ( 0 .. 128 ).into_iter()
        .map( |r| knot_hash( &key( input, r ) ) ).collect();
    map.into_iter().map( |h| count(&h) ).sum()
}