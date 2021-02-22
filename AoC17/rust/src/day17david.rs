use crate::red_black_bst_2::Tree;

struct Spinlock {
    tree: Tree<i32>,
    size: usize,
    position: usize
}

impl Spinlock {
    fn new() -> Spinlock {
        let mut tree = Tree::<i32>::new();
        tree.insert(0, 0);
        Spinlock {
            tree,
            size: 1,
            position: 0
        }
    }

    fn step_forward(&mut self, steps: usize) {
        let new_psotiion = (self.position + steps) % self.size;
        self.position = new_psotiion;
    }

    fn insert(&mut self, value: i32) {
        let new_position = self.position + 1;
        self.tree.insert(new_position, value);
        self.position = new_position;
        self.size += 1;
    }

    fn get(& self) -> i32 {
        self.tree[self.position]
    }
}

#[cfg(test)]
mod test {
    use super::Spinlock;

    #[test]
    fn test_short_example() {
        let mut spinlock = Spinlock::new();
        spinlock.step_forward(3);
        spinlock.insert(1);
        spinlock.step_forward(3);
        spinlock.insert(2);
        spinlock.step_forward(3);
        spinlock.insert(3);
        spinlock.step_forward(3);
        spinlock.insert(4);

        let result: Vec<i32> = spinlock.tree.into_iter().collect();
        assert_eq!(result, vec!(0, 2, 4, 3, 1));
    }

    #[test]
    fn test_example() {
        let mut spinlock = Spinlock::new();
        for value in 1..2018 {
            spinlock.step_forward(3);
            spinlock.insert(value);
        }

        spinlock.step_forward(1);
        assert_eq!(spinlock.get(), 638);
    }

    #[test]
    fn task1() {
        let mut spinlock = Spinlock::new();
        for value in 1..2018 {
            spinlock.step_forward(312);
            spinlock.insert(value);
        }

        spinlock.step_forward(1);
        assert_eq!(spinlock.get(), 772);
    }

    #[test]
    fn task2() {
        let mut spinlock = Spinlock::new();
        for value in 1..50000000 {
            spinlock.step_forward(312);
            spinlock.insert(value);
        }

        spinlock.position = 1;
        assert_eq!(spinlock.get(), 42729050);
    }

    #[test]
    fn task2_short() {
        let mut pos_one: i32 = -1;
        let mut position = 0;
        for value in 1..50000000 {
            let size = value;
            position = (position + 312) % size + 1;
            if position == 1 {
                pos_one = value;
            }
        }

        assert_eq!(pos_one, 42729050);
    }
}