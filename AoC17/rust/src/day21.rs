use std::collections::HashMap;
extern crate regex;
use regex::Regex;

type Point = (i32, i32);
type Image = HashMap<Point, char>;
type Image2 = HashMap<Point, Image>; // segment

fn inverse(v: i32, n: i32) -> i32 {
    n - v - 1
}
fn original(p: Point, _: i32) -> Point {
    p
}
fn rotate90(p: Point, n: i32) -> Point {
    (inverse(p.1, n), p.0)
}
fn rotate180(p: Point, n: i32) -> Point {
    (inverse(p.0, n), inverse(p.1, n))
}
fn rotate270(p: Point, n: i32) -> Point {
    (p.1, inverse(p.0, n))
}
fn hor_flip(p: Point, n: i32) -> Point {
    (p.0, inverse(p.1, n))
}
fn ver_flip(p: Point, n: i32) -> Point {
    (inverse(p.0, n), p.1)
}
fn r180_hf(p: Point, n: i32) -> Point {
    hor_flip(rotate180(p, n), n)
}
fn r180_vf(p: Point, n: i32) -> Point {
    ver_flip(rotate180(p, n), n)
}
fn r270_hf(p: Point, n: i32) -> Point {
    hor_flip(rotate270(p, n), n)
}
fn r270_vf(p: Point, n: i32) -> Point {
    ver_flip(rotate270(p, n), n)
}

type Transformer = fn(Point, i32) -> Point;

fn to_side(n: usize) -> i32 {
    (n as f64).sqrt().round() as i32
}

fn to_image(data: &str) -> Image {
    let lines: Vec<&str> = data.split_terminator('/').collect();
    let mut result: Image = Image::new();
    let n = lines.len();
    for x in 0..n {
        for y in 0..n {
            let c = lines[y].chars().nth(x).unwrap();
            result.insert((x as i32, y as i32), c);
        }
    }
    result
}

fn from_image(image: &Image) -> String {
    let n = image.len();
    let n_side = to_side(n);
    let mut result = String::new();
    for y in 0..n_side {
        if result.len() > 0 {
            result.push('/');
        }
        for x in 0..n_side {
            result.push(*image.get(&(x, y)).unwrap());
        }
    }
    result
}

fn split(image: &Image, size: i32) -> Image2 {
    let mut result = Image2::new();
    let n = image.len();
    let piece_size = size * size;
    let n_pieces = n / piece_size as usize;
    let n_pieces_side = to_side(n_pieces);
    for x in 0..n_pieces_side {
        for y in 0..n_pieces_side {
            let mut piece = Image::new();
            for px in 0..size {
                for py in 0..size {
                    let global_x = x * size + px;
                    let global_y = y * size + py;
                    piece.insert((px, py), *image.get(&(global_x, global_y)).unwrap());
                }
            }
            result.insert((x, y), piece);
        }
    }
    result
}

fn transform(image: &Image, map: &HashMap<String, String>) -> Image {
    let key = from_image(image);
    let value = map.get(&key);
    if value.is_none() {
        panic!("No value found for: {}", key);
    }
    to_image(value.unwrap())
}

fn unify(big_image: &Image2) -> Image {
    let n = big_image.len();
    let n_side = to_side(n); // side of the image
    let m = big_image.iter().next().unwrap().1.len();
    let m_side = to_side(m); // piece side size (ie. 2 or 3)
    let mut image = Image::new();
    for y in 0..n_side {
        for x in 0..n_side {
            let piece = big_image.get(&(x, y)).unwrap();
            for py in 0..m_side {
                for px in 0..m_side {
                    let global_x = x * m_side + px;
                    let global_y = y * m_side + py;
                    image.insert((global_x, global_y), *piece.get(&(px, py)).unwrap());
                }
            }
        }
    }
    image
}

fn tx_image(image: &String, tx_fx: &Transformer) -> String {
    let i = to_image(image);
    let n = to_side(i.len());
    let mut result = Image::new();
    for x in 0..n {
        for y in 0..n {
            let p0 = (x, y);
            let p1: Point = tx_fx(p0, n);
            result.insert(p1, *i.get(&p0).unwrap());
        }
    }
    from_image(&result)
}

fn create_map(data: &str) -> HashMap<String, String> {
    // ../.. => .##/##./.#.
    lazy_static! {
        static ref RE_LINE: Regex = Regex::new(r"^(\S+) => (\S+)$").unwrap();
    }
    let map: HashMap<String, String> = data
        .lines()
        .into_iter()
        .flat_map(|line| RE_LINE.captures_iter(line))
        .map(|cap| (cap[1].to_string(), cap[2].to_string()))
        .collect();

    // enhance with rotations
    let tx_fn: Vec<Transformer> = vec![
        original, rotate90, rotate180, rotate270, hor_flip, ver_flip, r180_hf, r180_vf, r270_hf,
        r270_vf,
    ];

    map.iter()
        .flat_map(|i| tx_fn.iter().map(move |f| (tx_image(i.0, f), i.1.clone())))
        .collect()
}

pub fn task(data: &str, iter_count: i32) -> i32 {
    let map = create_map(data);
    let init = ".#./..#/###";
    let init_image = to_image(init);
    let mut image = init_image;
    for _ in 0..iter_count {
        let n = to_side(image.len());
        let is_by_2 = n % 2 == 0;
        let is_by_3 = n % 3 == 0;
        let size = if is_by_2 {
            2
        } else if is_by_3 {
            3
        } else {
            panic!("not expacted")
        };
        let image2: Image2 = split(&image, size);
        let new_image2: Image2 = image2
            .iter()
            .map(|i| (*i.0, transform(i.1, &map)))
            .collect();
        image = unify(&new_image2);
    }
    image.iter().filter(|i| *i.1 == '#').count() as i32
}
