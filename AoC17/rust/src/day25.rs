use std::collections::HashSet;

#[derive(PartialEq, Copy, Clone)]
enum Direction { Left, Right }
#[derive(PartialEq, Copy, Clone)]
enum State { A, B, C, D, E, F }
type Instruction = ( bool, Direction, State ); // write_value, move_slot_to_x, continue_with_state
type StateCases = (Instruction, Instruction); // if the current value is 0, if it is 1

pub fn task1() -> usize {
    let steps = 12861455;
    let mut tape : HashSet<i64> = HashSet::new();
    let mut cur : i64 = 0;
    let mut state : State = State::A;
    const V1 : bool = true;
    const V0 : bool = false;
    let code = [
        ( State::A, ( (V1, Direction::Right, State::B), (V0, Direction::Left, State::B) ) ),
        ( State::B, ( (V1, Direction::Left, State::C), (V0, Direction::Right, State::E) ) ),
        ( State::C, ( (V1, Direction::Right, State::E), (V0, Direction::Left, State::D) ) ),
        ( State::D, ( (V1, Direction::Left, State::A), (V1, Direction::Left, State::A) ) ),
        ( State::E, ( (V0, Direction::Right, State::A), (V0, Direction::Right, State::F) ) ),
        ( State::F, ( (V1, Direction::Right, State::E), (V1, Direction::Right, State::A) ) ) ];

    for _ in 0 .. steps {
        let cases : &StateCases = &code.iter().filter( |c| c.0 == state ).map(|i| i.1 ).last().unwrap();
        let instruction : &Instruction = match tape.contains(&cur)
            { V0 => &cases.0, V1 => &cases.1 };
        match instruction.0 { V1 => tape.insert(cur), V0 => tape.remove(&cur) };
        cur += match instruction.1 { Direction::Left => -1, Direction::Right => 1 };
        state = instruction.2;
    }
    return tape.len();
}