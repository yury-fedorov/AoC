mod day07;

#[cfg(test)]
// https://doc.rust-lang.org/book/ch11-02-running-tests.html#controlling-how-tests-are-run
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }

    #[test]
    fn test_day07() {
        crate::day07::task1();
        assert_eq!(2 + 4, 6);
    }
}
