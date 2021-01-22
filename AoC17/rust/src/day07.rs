pub fn task1() {
    let filename = "/Users/fedoy/projects/AoC/AoC17/scala/src/test/resources/d07/sample.txt";

    // https://doc.rust-lang.org/book/ch12-02-reading-a-file.html
    let contents = std::fs::read_to_string(filename)
        .expect("Something went wrong reading the file");

    println!( "{}", contents );
}