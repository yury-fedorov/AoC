fn day07( file : &str, answer1 : &str, answer2 : i32 ) {
    use crate::day07;
    let input = day07::parse( file );
    let head = day07::task1( &input );
    assert_eq!( answer1, head );
    assert_eq!( answer2, day07::task2( &input, &head ) );
}

#[test]
fn test_day07() {
    day07( "07/sample.txt", "tknk", 60 );
    day07( "07/input.txt", "azqje", 646 );
}

fn day08( file : &str, answer1 : i32, answer2 : i32 ) {
    use crate::common;
    use crate::day08;
    let text = common::input(file);
    let (a1,a2) = day08::tasks( &text );
    assert_eq!( answer1, a1 );
    assert_eq!( answer2, a2 );
}

#[test]
fn test_day08() {
    day08( "08/sample.txt", 1, 10 );
    day08( "08/input.txt", 5849, 6702 );
}


