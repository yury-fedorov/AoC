#[test]
fn it_works() {
    assert_eq!(2 + 2, 4);
}

#[test]
fn test_day07() {
    use crate::day07;
    assert_eq!( "tknk", day07::task1( "d07/sample.txt" ) );
    assert_eq!( "azqje", day07::task1( "d07/input.txt" ) );
}
