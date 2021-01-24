#[test]
fn it_works() {
    assert_eq!(2 + 2, 4);
}


fn day07( file : &str, answer1 : &str, answer2 : i32 ) {
    use crate::day07;
    let input = day07::parse( file );
    let head = day07::task1( &input );
    assert_eq!( answer1, head );
    // assert_eq!( answer2, day07::task2( &input, &head ) );
}

#[test]
fn test_day07() {
    day07( "d07/sample.txt", "tknk", 60 );
    day07( "d07/input.txt", "azqje", -1 );
}
