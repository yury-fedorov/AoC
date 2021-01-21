#[cfg(test)]

// https://doc.rust-lang.org/book/ch11-02-running-tests.html#controlling-how-tests-are-run
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }

    #[test]
    fn day07() {
        let filename = "/Users/fedoy/projects/AoC/AoC17/scala/src/test/resources/d07/sample.txt";

        // https://doc.rust-lang.org/book/ch12-02-reading-a-file.html
        let contents = std::fs::read_to_string(filename)
            .expect("Something went wrong reading the file");

        println!( "{}", contents );

        assert_eq!(2 + 4, 6);
    }
}
