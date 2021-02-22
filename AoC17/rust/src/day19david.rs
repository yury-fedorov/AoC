use std::collections::HashSet;
use std::ops::{Index, IndexMut};
use crate::common;
use std::convert::identity;

#[derive(Copy, Clone, PartialEq)]
enum Element {
    Empty,
    Horizontal,
    Vertical,
    Cross,
    Letter(char)
}

use Element::*;

impl Element {
    fn from_char(c: char) -> Element {
        match c {
            ' ' => Empty,
            '|' => Horizontal,
            '-' => Vertical,
            '+' => Cross,
            l if l >= 'A' && l <= 'Z' => Letter(l),
            o => panic!("Unkown element {}", o),
        }
    }
}

#[derive(Copy, Clone, Debug)]
struct Coord{
    x: usize,
    y: usize
}

struct RoutingDiagram {
    lines: Vec<Vec<Element>>,
}

impl RoutingDiagram {
    fn new(text: &str) -> RoutingDiagram {
        let lines: Vec<Vec<Element>> = text
            .lines()
            .map(|s| s.chars()
                .into_iter()
                .map(Element::from_char)
                .collect::<Vec<Element>>())
            .collect();

        // validate all lines have the same length
        assert_eq!(lines
            .iter()
            .map(|l| l.len())
            .collect::<HashSet<usize>>()
            .len(), 1);

        RoutingDiagram {
            lines
        }
    }
}

impl Index<Coord> for RoutingDiagram {
    type Output = Element;

    fn index(&self, index: Coord) -> &Element {
        &self.lines[index.y][index.x]
    }
}

#[derive(Debug, Copy, Clone)]
enum Direction {
    Up,
    Right,
    Left,
    Down,
}

use Direction::*;

impl Direction {
    fn turn_right(self) -> Direction {
        match self {
            Up => Right,
            Right => Down,
            Down => Left,
            Left => Up,
        }
    }

    fn turn_left(self) -> Direction {
        match self {
            Up => Left,
            Right => Up,
            Down => Right,
            Left => Down,
        }
    }

    fn apply(self, coord: Coord) -> Coord {
        match self {
            Up => Coord {x: coord.x, y: coord.y - 1},
            Right => Coord {x: coord.x + 1, y: coord.y},
            Left => Coord {x: coord.x - 1, y: coord.y},
            Down => Coord {x: coord.x, y: coord.y + 1},
        }
    }
}

fn fits_direction(elem: Element) -> bool {
    match elem {
        Empty => false,
        _ => true
    }
}

fn fits_direction_after_cross(direction: Direction, elem: Element) -> bool {
    match (direction, elem) {
        (Up, Horizontal) | (Up, Letter(_)) => true,
        (Right, Vertical) | (Right, Letter(_)) => true,
        (Down, Horizontal) | (Down, Letter(_)) => true,
        (Left, Vertical) | (Left, Letter(_)) => true,
        _ => false
    }
}

struct State<'a> {
    routing_diagram: &'a RoutingDiagram,
    coord: Coord,
    direction: Direction,
}

impl<'a> State<'a> {
    fn new(routing_diagram: &'a RoutingDiagram) -> State<'a> {
        let mut x: usize = 0;
        while routing_diagram[Coord{x, y: 0}] != Horizontal {
            x += 1;
        }

        State {
            routing_diagram,
            coord: Coord{x, y: 0},
            direction: Down,
        }
    }

    fn step(&mut self) -> Option<()> {
        let element = self.routing_diagram[self.coord];
        match element {
            Cross => {
                let turn_right_direction = self.direction.turn_right();
                let turn_right_coord = turn_right_direction.apply(self.coord);

                let turn_left_direction = self.direction.turn_left();
                let turn_left_coord = turn_left_direction.apply(self.coord);

                if fits_direction_after_cross(turn_right_direction, self.routing_diagram[turn_right_coord]) {
                    self.coord = turn_right_coord;
                    self.direction = turn_right_direction;
                    Some(())
                } else if fits_direction_after_cross(turn_left_direction, self.routing_diagram[turn_left_coord]) {
                    self.coord = turn_left_coord;
                    self.direction = turn_left_direction;
                    Some(())
                } else {
                    None
                }
            },
            Horizontal | Vertical | Letter(_) => {
                let ahead_coord = self.direction.apply(self.coord);
                if fits_direction( self.routing_diagram[ahead_coord]) {
                    self.coord = ahead_coord;
                    Some(())
                } else {
                    None
                }
            },
            Empty => panic!("I am on Emtpy element, coord: {:?}", self.coord),
        }
    }

    fn collect_letter(&self) -> Option<char> {
        let elem = self.routing_diagram[self.coord];
        match elem {
            Letter(l) => Some(l),
            _ => None,
        }
    }
}

impl<'a> Iterator for State<'a> {
    type Item = Option<char>;

    // the outer Option indicates whether the iterator has a next element
    // the inner Option indicates whether a letter was collected
    fn next(&mut self) -> Option<Option<char>> {
        self.step()
            .map(|_| {
                self.collect_letter()
            })
    }
}

pub fn task1(path: &str) -> String {
    let text = common::input(path);
    let routing_diagram = RoutingDiagram::new(&text);
    let state = State::new(&routing_diagram);
    state.filter_map(identity).collect()
}

pub fn task2(path: &str) -> usize {
    let text = common::input(path);
    let routing_diagram = RoutingDiagram::new(&text);
    let state = State::new(&routing_diagram);
    state.count() + 1
}

#[cfg(test)]
mod test {
    use super::{task1, task2};

    #[test]
    fn test_sample_task1() {
        let result = task1("19/sample.txt");
        assert_eq!(result, "ABCDEF");
    }

    #[test]
    fn test_sample_task2() {
        let result = task2("19/sample.txt");
        assert_eq!(result, 38);
    }

    #[test]
    fn test_input_task1() {
        let result = task1("19/input_david.txt");
        assert_eq!(result, "FEZDNIVJWT");
    }

    #[test]
    fn test_input_task2() {
        let result = task2("19/input_david.txt");
        assert_eq!(result, 17200);
    }
}
