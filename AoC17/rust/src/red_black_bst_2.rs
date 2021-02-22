//! Implements a red-black-binary-search-tree as described in the course.
//! Choses an approach with only one struct for both link and node

use self::Color::{Red, Black};
use self::Link::{ColoredLink, End};
#[allow(unused_imports)] // I get a warning without this, and an error without the import...
use std::ops::Range;
#[allow(unused_imports)] // I get a warning without this, and an error without the import...
use std::cmp;
use std::mem;
use std::iter::FromIterator;
use std::ops::{Index, Deref};

pub struct Tree<Item> {
    top: Link<Item>
}

impl<Item> Tree<Item>
    where Item: PartialOrd {

    pub fn new() -> Tree<Item> {
        Tree {
            top: End,
        }
    }

    /// inserting in position n means: insert after position n-1, and shift all elements that
    /// have been in positions >=n before one position to the right.
    pub fn insert(&mut self, position: usize, value: Item) {
        let top = mem::replace(&mut self.top, End);
        let top = top.insert(position, value);
        let top = top.make_black();
        mem::replace(&mut self.top, top);
    }

    pub fn size(&self) -> usize {
        self.top.size()
    }


    pub fn contains(&self, value: &Item) -> bool {
        self.top.contains(value)
    }

    pub fn iter(&self) -> Iter<Item> {
        Iter::new(&self.top)
    }
}

impl<A> FromIterator<A> for Tree<A>
    where A: PartialOrd {

    fn from_iter<T>(iter: T) -> Tree<A>
        where T: IntoIterator<Item=A> {

        let mut tree = Tree::<A>::new();
        for (val, pos) in iter.into_iter().zip(0..) {
            tree.insert(pos, val);
        }

        tree
    }
}

impl<A> IntoIterator for Tree<A> {
    type Item = A;
    type IntoIter = IntoIter<A>;

    fn into_iter(self) -> IntoIter<A> {
        IntoIter::new(self.top)
    }
}

impl<A> Index<usize> for Tree<A> {
    type Output = A;

    fn index(&self, index: usize) -> &A {
        &self.top[index]
    }
}

#[derive(Eq, PartialEq, Debug)]
enum Color {
    Red,
    Black,
}

#[derive(Eq, PartialEq, Debug)]
enum Link<Item> {
    End,
    ColoredLink {
        color: Color,
        left_children_count: usize,
        value: Item,
        left: Box<Link<Item>>,
        right: Box<Link<Item>>,
    },
}

impl<Item> Link<Item>
    where Item: PartialOrd {

    /// inserting in position n means: insert after position n-1, and shift all elements that
    /// have been in positions >=n before one position to the right.
    fn insert(self, position: usize, value: Item) -> Link<Item> {
        match self {
            End => if position != 0 {
                panic!("Inserting with position {} at End", position)
            } else {
                ColoredLink {
                    color: Red,
                    left_children_count: 0,
                    value,
                    left: Box::new(End),
                    right: Box::new(End),
                }
            },
            ColoredLink {
                color,
                left_children_count,
                value: self_value,
                left,
                right
            } => {
                (
                    if position > left_children_count {
                        ColoredLink {
                            color,
                            left_children_count,
                            value: self_value,
                            left,
                            right: Box::new(right.insert(position - left_children_count - 1, value)),
                        }
                    } else /*if value <= self_value*/ {
                        ColoredLink {
                            color,
                            left_children_count: left_children_count + 1,
                            value: self_value,
                            left: Box::new(left.insert(position, value)),
                            right,
                        }
                    }
                )
                    .rotate_left()
                    .rotate_right()
                    .color_flip()
            }
        }
    }

    fn size(&self) -> usize {
        match self {
            End => 0,
            ColoredLink {
                left,
                right,
                ..
            } => left.size() + right.size() + 1
        }
    }

    fn contains(&self, value: &Item) -> bool {
        match self {
            End => false,
            ColoredLink {
                value: self_value,
                left,
                right,
                ..
            } => value == self_value || left.contains(value) || right.contains(value)
        }
    }

    // make a ColoredLink black. Used for the top of the tree
    fn make_black(self) -> Link<Item> {
        match self {
            End => End,
            ColoredLink {
                left_children_count,
                value,
                left,
                right,
                ..
            } => ColoredLink {
                color: Black,
                left_children_count,
                value,
                left,
                right
            }
        }
    }

    fn rotate_left(self) -> Link<Item> {
        if let ColoredLink {
            color,
            left_children_count: top_left_children_count,
            value: top_value,
            left,
            right
        } = self {
            let right = *right;
            if let ColoredLink {
                color: Red,
                left_children_count: right_left_children_count,
                value: right_value,
                left: middle,
                right
            } = right {
                // construct the rotated link
                ColoredLink {
                    color,
                    left_children_count: top_left_children_count + right_left_children_count + 1,
                    value: right_value,
                    left: Box::new(ColoredLink {
                        color: Red,
                        left_children_count: top_left_children_count,
                        value: top_value,
                        left,
                        right: middle,
                    }),
                    right,
                }
            } else {
                // the second pattern does not match, so we need to re-construct the first one,
                // because we already de-constructed it
                ColoredLink {
                    color,
                    left_children_count: top_left_children_count,
                    value: top_value,
                    left,
                    right: Box::new(right),
                }
            }
        } else {
            self
        }
    }

    fn rotate_right(self) -> Link<Item> {
        if let ColoredLink {
            color,
            left_children_count: top_left_children_count,
            value: top_value,
            left,
            right
        } = self {
            let left = *left;
            if let ColoredLink {
                color: Red,
                left_children_count: left_left_children_count,
                value: left_value,
                left,
                right: middle
            } = left {
                match left.as_ref() { // we are only checking the color of the link here, so we can match on its ref and do not need to deconstruct
                    ColoredLink { color: Red, .. } => {
                        // construct the rotated link
                        ColoredLink {
                            color,
                            left_children_count: left_left_children_count,
                            value: left_value,
                            left,
                            right: Box::new(ColoredLink {
                                color: Red,
                                left_children_count: top_left_children_count - 1 - left_left_children_count,
                                value: top_value,
                                left: middle,
                                right,
                            }),
                        }
                    }
                    _ => {
                        // the third pattern (the red color of the link) does not match, so we need to re-construct the first one,
                        // because we already de-constructed it
                        ColoredLink {
                            color,
                            left_children_count: top_left_children_count,
                            value: top_value,
                            left: Box::new(ColoredLink {
                                color: Red,
                                left_children_count: left_left_children_count,
                                value: left_value,
                                left,
                                right: middle,
                            }),
                            right,
                        }
                    }
                }
            } else {
                // the second pattern does not match, so we need to re-construct the first one,
                // because we already de-constructed it
                ColoredLink {
                    color,
                    left_children_count: top_left_children_count,
                    value: top_value,
                    left: Box::new(left),
                    right,
                }
            }
        } else {
            self
        }
    }

    fn color_flip(self) -> Link<Item> {
        if let ColoredLink {
            color: Black,
            left_children_count,
            value,
            left,
            right
        } = self {
            let left = *left;
            let right = *right;

            match (left, right) {
                (ColoredLink {
                    color: Red,
                    left_children_count: left_left_children_count,
                    value: left_value,
                    left: left_left,
                    right: left_right,
                }, ColoredLink {
                    color: Red,
                    left_children_count: right_left_children_count,
                    value: right_value,
                    left: right_left,
                    right: right_right,
                }) => {
                    ColoredLink {
                        color: Red,
                        left_children_count,
                        value,
                        left: Box::new(ColoredLink {
                            color: Black,
                            left_children_count: left_left_children_count,
                            value: left_value,
                            left: left_left,
                            right: left_right,
                        }),
                        right: Box::new(ColoredLink {
                            color: Black,
                            left_children_count: right_left_children_count,
                            value: right_value,
                            left: right_left,
                            right: right_right,
                        }),
                    }
                }
                (l, r) => ColoredLink {
                    color: Black,
                    left_children_count,
                    value,
                    left: Box::new(l),
                    right: Box::new(r),
                }
            }
        } else {
            self
        }
    }

    /// tests if the link is the top of a binary search tree, i.e. if the left of each node is
    /// lower than the node itself, and if the rigth of each node is greater than the node itself
    #[cfg(test)]
    fn is_bst(&self) -> bool {
        match self {
            End => true,
            ColoredLink { value, left, right, .. } => {
                (
                    match left.as_ref() {
                        End => true,
                        ColoredLink { value: left_value, .. } => *value > *left_value && left.is_bst()
                    }
                ) && (
                    match right.as_ref() {
                        End => true,
                        ColoredLink { value: right_value, .. } => *value < *right_value && left.is_bst()
                    }
                )
            }
        }
    }

    /// returns the number of black links found under the node, as a range The start of the range is the
    /// lowest depth found, the end of the range minus one is the highest depth found.
    #[cfg(test)]
    fn black_depth(&self) -> Range<usize> {
        match &self {
            End => 0..1,
            ColoredLink { color: Black, left, right, .. } => {
                let left_depth = left.black_depth();
                let right_depth = right.black_depth();
                cmp::min(left_depth.start, left_depth.start) + 1..cmp::max(left_depth.end, right_depth.end) + 1
            }
            ColoredLink { color: Red, left, right, .. } => {
                let left_depth = left.black_depth();
                let right_depth = right.black_depth();
                cmp::min(left_depth.start, left_depth.start)..cmp::max(left_depth.end, right_depth.end)
            }
        }
    }

    /// returns the number of links (of whatever color) found under the node, as a range The start of the range is the
    /// lowest depth found, the end of the range minus one is the highest depth found.
    #[cfg(test)]
    fn total_depth(&self) -> Range<usize> {
        match &self {
            End => 0..1,
            ColoredLink { left, right, .. } => {
                let left_depth = left.total_depth();
                let right_depth = right.total_depth();
                cmp::min(left_depth.start, left_depth.start) + 1..cmp::max(left_depth.end, right_depth.end) + 1
            }
        }
    }

    /// checks if this link has a black balanced tree under it, i.e. the range returned by black_depth()
    /// is not wider than 1
    #[cfg(test)]
    fn is_black_balanced(&self) -> bool {
        let black_depth = self.black_depth();
        black_depth.end - black_depth.start <= 1
    }

    /// checks if this link has any right leaning red links under it
    #[cfg(test)]
    fn has_right_leaning_red_links(&self) -> bool {
        match &self {
            End => false,
            ColoredLink { left, right, .. } => {
                (
                    match right.as_ref() {
                        ColoredLink { color: Red, .. } => true,
                        _ => false
                    }
                ) || left.has_right_leaning_red_links() || right.has_right_leaning_red_links()
            }
        }
    }

    /// checks if the link has any consecutive red links under it
    #[cfg(test)]
    fn has_consecutive_red_links(&self) -> bool {
        match &self {
            End => false,

            ColoredLink { color: Red, left, right, .. } =>
                (
                    match left.as_ref() {
                        ColoredLink { color: Red, .. } => true,
                        _ => false
                    }
                ) || (
                    match right.as_ref() {
                        ColoredLink { color: Red, .. } => true,
                        _ => false
                    }
                ) || left.has_consecutive_red_links() || right.has_consecutive_red_links(),

            ColoredLink { left, right, .. } =>
                left.has_consecutive_red_links() || right.has_consecutive_red_links()
        }
    }
}

impl<A> Index<usize> for Link<A> {
    type Output = A;

    fn index(&self, index: usize) -> &A {
        match &self {
            End => panic!("Trying to index End with {}", index),
            ColoredLink {
                value,
                left_children_count,
                left,
                right,
                ..
            } => {
                if index < *left_children_count {
                    &left[index]
                } else if index == *left_children_count {
                    value
                } else {
                    &right[index - left_children_count - 1]
                }
            }
        }
    }
}

pub struct Iter<'a, Item> {
  link_path: Vec<&'a Link<Item>>
}

impl<'a, Item> Iter<'a, Item> {
    fn new(tree: &'a Link<Item>) -> Iter<'a, Item> {
        let mut link_path: Vec<&'a Link<Item>> = vec!();
        let mut link = tree;
        let mut done = false;

        while !done {
            match link {
                End => {
                    done = true;
                },
                ColoredLink{
                    left,
                    ..
                } => {
                    link_path.push(link);
                    link = left.as_ref();
                }
            }
        }

        Iter {
            link_path
        }
    }
}

impl<'a, Item> Iterator for Iter<'a, Item> {
    type Item = &'a Item;

    fn next(&mut self) -> Option<&'a Item> {
        let tail = self.link_path.pop();
        match tail {
            None => None,
            Some(ColoredLink {
                     value,
                     right,
                     ..
                 }) => {
                let mut link = right.as_ref();

                let mut done = false;

                while !done {
                    match link {
                        End => {
                            done = true;
                        },
                        ColoredLink{
                            left,
                            ..
                        } => {
                            self.link_path.push(link);
                            link = left.as_ref();
                        }
                    }
                }

                Some(value)
            }
            _ => unreachable!() // because the vector is only populated with ColoredLinks
        }
    }
}

pub struct IntoIter<Item> {
    link_path: Vec<Link<Item>>
}

impl<Item> IntoIter<Item> {
    fn new(tree: Link<Item>) -> IntoIter<Item> {
        let mut link_path: Vec<Link<Item>> = vec!();
        let mut link = tree;
        let mut done = false;

        while !done {
            match link {
                End => {
                    done = true;
                },
                ColoredLink{
                    left_children_count,
                    value,
                    left,
                    right,
                    ..
                } => {
                    let vector_elem = ColoredLink {
                        color: Black, // irrelevant
                        left_children_count,
                        value,
                        left: Box::new(End), // we want to own the original left, so we put an End here instead
                        right
                    };
                    link_path.push(vector_elem);
                    link = *left;
                }
            }
        }

        IntoIter {
            link_path
        }
    }
}

impl<Item> Iterator for IntoIter<Item> {
    type Item = Item;

    fn next(&mut self) -> Option<Item> {
        let tail = self.link_path.pop();
        match tail {
            None => None,
            Some(ColoredLink {
                     value,
                     right,
                     .. // the color is irrelevant, and left will always be End per construction
                 }) => {
                let mut link = *right;

                let mut done = false;

                while !done {
                    match link {
                        End => {
                            done = true;
                        },
                        ColoredLink{
                            left_children_count,
                            value,
                            left,
                            right,
                            ..
                        } => {
                            let vector_elem = ColoredLink {
                                color: Black, // irrelevant
                                left_children_count,
                                value,
                                left: Box::new(End), // we want to own the original left, so we put an End here instead
                                right
                            };
                            self.link_path.push(vector_elem);
                            link = *left;
                        }
                    }
                }

                Some(value)
            }
            _ => unreachable!() // because the vector is only populated with ColoredLinks
        }
    }
}

#[cfg(test)]
mod tests {
    use super::{Tree, Link};

    #[test]
    fn test_simple_iterator() {
        let mut bst: Tree<i32> = Tree::new();
        bst.insert(0, 1); // 1
        bst.insert(1, 2); // 1, 2
        bst.insert(1, 3); // 1, 3, 2
        bst.insert(0, 4); // 4, 1, 3, 2

        let result: Vec<i32> = bst.into_iter().collect();

        assert_eq!(result, vec!(4, 1, 3, 2));
    }

    #[test]
    fn test_simple_index() {
        let mut bst: Tree<i32> = Tree::new();
        bst.insert(0, 1); // 1
        bst.insert(1, 2); // 1, 2
        bst.insert(1, 3); // 1, 3, 2
        bst.insert(0, 4); // 4, 1, 3, 2

        assert_eq!(bst[0], 4);
        assert_eq!(bst[1], 1);
        assert_eq!(bst[2], 3);
        assert_eq!(bst[3], 2);
    }

}
