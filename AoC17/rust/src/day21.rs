use std::collections::HashMap;

type Point = (i32,i32);
type Point2 = (Point,Point); // segment, within segment
type Image = HashMap<Point,char>;
type Image2 = HashMap<Point2,char>;

fn rotate_left( image : Image ) -> Image { image } // TODO
fn flip_vertically( image : Image ) -> Image { image } // TODO
fn flip_horizontally( image : Image ) -> Image { image } // TODO

fn equal( a : &Image, b : &Image) -> bool { false }  // TODO

fn to_image( data : &str ) -> Image {
    let lines : Vec<&str> = data.split_terminator( '/' ).collect();
    let mut result : Image = Image::new();
    let n = lines.len();
    for x in 0 .. n {
        for y in 0 .. n {
            let c = lines[y].chars().nth(x).unwrap();
            result.insert( (x as i32,y as i32), c );
        }
    }
    result
}

pub fn task12( data : &str ) -> (i32,i32) {
    let init = ".#./..#/###";
    let init_image = to_image( init );
    let mut image = init_image;
    for _ in 0 .. 2 { //  TODO 5 iterations
        let n = ( image.len() as f64 ).sqrt() as usize;
        let is_by_2 = n % 2 == 0;
    }
    (0,0)
}