use crate::common;
use std::iter::FromIterator;

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
    assert_eq!( "a2582a3a0e66e6e86e3812dcb672a272", day10::task2( list_size, "" ) );
    assert_eq!( "33efeb34ea91902bb2f59c9920caa6cd", day10::task2( list_size, "AoC 2017" ) );
    assert_eq!( "3efbe78a8d82f29979031a4aa0b16a9d", day10::task2( list_size, "1,2,3" ) );
    assert_eq!( "63960835bcdc130f0b66d7ff4f6a5a8e", day10::task2( list_size, "1,2,4" ) );
    assert_eq!( "541dc3180fd4b72881e39cf925a50253", day10::task2( list_size, &lengths ) ); // 1675acadf1ab6a1df6ef286836dd9865
}

#[test]
fn test_day11() {
    use crate::day11;
    assert_eq!(3, day11::task1("ne,ne,ne"));
    assert_eq!(0, day11::task1("ne,ne,sw,sw"));
    assert_eq!(2, day11::task1("ne,ne,s,s"));
    assert_eq!(3, day11::task1("se,sw,se,sw,sw"));

    let path = common::input("11/input.txt");
    let answers = day11::tasks(&path);
    assert_eq!(764, answers.0 );
    assert_eq!(1532, answers.1 );
}

#[test]
fn test_day12() {
    use crate::day12;
    let a0 = day12::tasks( &common::input("12/sample.txt") );
    assert_eq!(6, a0.0);
    assert_eq!(2, a0.1);

    let answers = day12::tasks( &common::input("12/input.txt") );
    assert_eq!(141, answers.0);
    assert_eq!(171, answers.1);
}

fn day13( file : &str, answer1: i32, answer2 : i32 ) {
    use crate::day13;
    let map = day13::input( &common::input(file ) );
    assert_eq!( answer1, day13::task1( &map ) );
    if common::is_fast() && answer2 > 10000 { return }
    assert_eq!( answer2, day13::task2( &map ) );
}

#[test]
fn test_day13() {
    day13( "13/sample.txt", 24, 10 );
    day13( "13/input.txt", 1900, 3966414 ); // 9 min 26 sec
}

#[test]
fn test_day14() {
    use crate::day14;
    assert_eq!( 5, day14::count( "d401" ) );
    let map1 = day14::input( "flqrgnkx" );
    assert_eq!( 8108, day14::task1( &map1 ) );

    let input = "jzgqcdpd";
    let map = day14::input( input );
    assert_eq!( 8074, day14::task1( &map ) );

    if common::is_fast() { return; } // takes 25 seconds
    assert_eq!( 1242, day14::task2( &map1 ) );
    assert_eq!( 1212, day14::task2( &map ) );
}

fn day15( input : crate::day15::Input, answer1 : i32, answer2 : i32 ) {
    use crate::day15;
    assert_eq!( answer1, day15::task1( input ) );
    if common::is_fast() { return } // takes 7-8 seconds
    assert_eq!( answer2, day15::task2( input ) );
}

#[test]
fn test_day15() {
    day15( (65, 8921), 588, 309 );
    day15( (116, 299), 569, 298 );
}

#[test]
fn test_day16() {
    use crate::day16;
    let dance = day16::init( &common::input( "16/input.txt" ) );
    let input = String::from_iter(( 0 .. 16 ).into_iter().map( |dc| ( 'a' as u8 + dc ) as char )  );
    assert_eq!( "hmefajngplkidocb", day16::task1( &input, &dance ) );
    assert_eq!( "fbidepghmjklcnoa", day16::task2( &input, &dance ) );
}

#[test]
fn test_day17() {
    use crate::day17;
    assert_eq!( 638, day17::task1( 3 ) );
    let input = 337;
    assert_eq!( 600, day17::task1( input ) );
    if common::is_fast() { return } // it takes 31 minutes
    assert_eq!( 31220910, day17::task2( input ) );
}

#[test]
fn test_day18() {
    use crate::day18;
    let tc = common::input( "18/sample.txt" );
    assert_eq!( 4, day18::task1( &tc ) );
    let c1 = common::input( "18/input.txt"  );
    assert_eq!( 1187, day18::task1( &c1 ) );
    let tc2 = common::input( "18/sample2.txt" );
    assert_eq!( 3, day18::task2( &tc2 ) );
    let c2 = common::input( "18/input.txt"  );
    assert_eq!( 5969, day18::task2( &c2 ) );
}

#[test]
fn test_day19() {
    use crate::day19;
    let (a1, a2) = day19::task12( &common::input( "19/input" ) );
    assert_eq!( "VEBTPXCHLI", a1 );
    assert_eq!( 18702, a2 );
}

#[test]
fn test_day20() {
    use crate::day20;
    let (_, t2) = day20::task12( &common::input( "20/test2.txt" ) );
    assert_eq!(1, t2);

    let (a1, a2) = day20::task12( &common::input( "20/input" ) );
    assert_eq!( 457, a1 );
    assert_eq!( 448, a2 );
}

#[test]
fn test_day21() {
    // if common::is_fast() { return } // takes x seconds
    use crate::day21;
    let (a1, a2) = day21::task12( &common::input( "21/input" ) );
    assert_eq!( -1, a1 );
    assert_eq!( -1, a2 );
}

#[test]
fn test_day22() {
    if common::is_fast() { return } // takes 25 seconds (mostly on the second part)
    use crate::day22;
    let (t1, _) = day22::task12( &common::input( "22/sample" ) );
    assert_eq!( 5587, t1 );
    let (a1, a2) = day22::task12( &common::input( "22/input" ) );
    assert_eq!( 5462, a1 );
    assert_eq!( 2512135, a2 );
}

#[test]
fn test_day24() {
    if common::is_fast() { return } // takes 24 seconds
    use crate::day24;
    let (a1, a2) = day24::task12( &common::input( "24/input" ) );
    assert_eq!( 1511, a1 );
    assert_eq!( 1471, a2 );
}

#[test]
fn test_day25() {
    if common::is_fast() { return } // takes 21 seconds
    use crate::day25;
    let a = day25::task1();
    assert_eq!(3578, a);
}