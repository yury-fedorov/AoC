use crate::common;

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

#[test]
fn test_day09() {
    use crate::day09;
    assert_eq!( 1, day09::task1("{}") );
    assert_eq!( 6, day09::task1("{{{}}}") );
    assert_eq!( 5, day09::task1("{{},{}}") );
    assert_eq!( 16, day09::task1("{{{},{},{{}}}}") );
    assert_eq!( 1, day09::task1("{<a>,<a>,<a>,<a>}") );
    assert_eq!( 9, day09::task1("{{<ab>},{<ab>},{<ab>},{<ab>}}") );
    assert_eq!( 9, day09::task1("{{<!!>},{<!!>},{<!!>},{<!!>}}" ) );
    assert_eq!( 3, day09::task1("{{<a!>},{<a!>},{<a!>},{<ab>}}") );

    assert_eq!(0, day09::task2( "<>" ) );
    assert_eq!(17, day09::task2( "<random characters>" ) );
    assert_eq!(3, day09::task2( "<<<<>" ) );
    assert_eq!(2, day09::task2( "<{!>}>" ) );
    assert_eq!(0, day09::task2( "<!!>" ) );
    assert_eq!(0, day09::task2( "<!!!>>" ) );
    assert_eq!(10, day09::task2( "<{o\"i!a,<{i<a>" ) );

    let text = common::input("09/input.txt");
    let answers = day09::tasks(&text);
    assert_eq!( 10050, answers.0 );
    assert_eq!( 4482, answers.1 );
}

#[test]
fn test_day10() {
    use crate::day10;
    assert_eq!(12, day10::task1( 5, "3,4,1,5"));

    let lengths = common::input("10/input.txt");
    let list_size = 256;
    assert_eq!( 23715, day10::task1( list_size, &lengths ) );
}