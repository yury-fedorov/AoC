extern crate linked_list; // LinkedList in standard library is not usable,
                          // the solution on Vector does not finish even in 6 hours

fn forward(c: &mut linked_list::Cursor<i32>, times: usize) -> i32 {
    let mut value: Option<&mut i32> = None;
    for _j in 0..times {
        value = c.next();
        if value.is_none() {
            value = c.next();
        }
    }
    *value.unwrap()
}

fn task(steps: usize, last: i32) -> (Vec<i32>, i32) {
    let mut buffer: linked_list::LinkedList<i32> = linked_list::LinkedList::new();
    let mut c = buffer.cursor();
    c.insert(0);
    let last1 = last + 1;
    let steps1 = steps + 1;
    for i in 1..last1 {
        forward(&mut c, steps1);
        c.insert(i);
    }
    let r_next_element = forward(&mut c, 2);
    let r_vec: Vec<i32> = buffer.into_iter().collect();
    (r_vec, r_next_element)
}

pub fn task1(steps: usize) -> i32 {
    let last = 2017;
    task(steps, last).1
}

pub fn task2(steps: usize) -> i32 {
    let last = 50000000; // fifty million
    let buffer = task(steps, last).0;
    let n = buffer.len();
    for i in 0..n {
        if buffer[i] == 0 {
            return buffer[(i + 1) % n];
        }
    }
    panic!("unexpected to be here ever");
}
