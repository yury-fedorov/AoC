use std::path::Path;

pub fn input( short_path: &str ) -> String {
    let dir = Path::new("/Users/fedoy/projects/AoC/AoC17/scala/src/test/resources");
    let filename = dir.join( short_path );
    // https://doc.rust-lang.org/book/ch12-02-reading-a-file.html
    let file = std::fs::read_to_string(filename)
        .expect("Something went wrong reading the file");
    let lines = file.lines();
    for line in lines {
       return String::from(line);
    }
    String::from("")
}