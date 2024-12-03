use std::path::Path;

pub fn input(short_path: &str) -> String {
    let dir = Path::new("../data");
    let filename = dir.join(short_path);
    // https://doc.rust-lang.org/book/ch12-02-reading-a-file.html
    std::fs::read_to_string(filename).expect("Something went wrong reading the file")
}

pub fn is_fast() -> bool {
    true
} // only fast tests
