use std::collections::HashSet;

enum Direction { Left, Right }
enum State { A, B, C, D, E, F }
type Instruction = ( bool, Direction, State ); // write_value, move_slot_to_x, continue_with_state

pub fn task1() -> usize {
    let steps = 12861455;
    let mut tape : HashSet<i64> = HashSet::new();
    let mut cur : i64 = 0;
    let mut state : State = State::A;
    return tape.len();
}