pub fn task1(map : &str) -> String {
    let ml : Vec<&str> = map.lines().collect();
    let mut answer = String::new();
    map.chars()
    // .into_iter()
    .filter( |c| match c { ' ' | '-' | '|' | '+' | '\r' | '\n' => true, _ => false } )
    // .as_str()
    // answer
}