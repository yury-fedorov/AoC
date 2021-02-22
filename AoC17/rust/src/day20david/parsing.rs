use pom::parser::*;
use pom::char_class;
use std::str::{from_utf8, FromStr};
use super::{Vector, Particle};
use pom::char_class::space;

fn parse_i32<'a>() -> Parser<'a, u8, i32> {
    (sym(b'-').opt() + is_a(char_class::digit).repeat(1..))
        .collect()
        .map(|slice| i32::from_str(from_utf8(slice).unwrap()).unwrap())
}

fn parse_vector<'a>() -> Parser<'a, u8, Vector> {
    (sym(b'<') + parse_i32() + sym(b',') + parse_i32() + sym(b',') + parse_i32() + sym(b'>'))
        .map(|((((((_, x), _) , y), _), z), _)| Vector{x, y, z})
}

pub(super) fn parse_particle<'a>() -> Parser<'a, u8, Particle> {
    (seq(b"p=") + parse_vector() + seq(b", v=") + parse_vector() + seq(b", a=") + parse_vector())
        .map(|(((((_, p), _), v), _), a)| Particle{p, v, a})
}

#[cfg(test)]
mod test {
    use super::*;
    use super::super::{Vector, Particle};

    #[test]
    fn test_parse_vector() {
        let result = parse_vector().parse(b"<10,-25,-14>").unwrap();
        assert_eq!(result, Vector{x: 10, y: -25, z: -14})
    }

    #[test]
    fn test_parse_particle() {
        let result = parse_particle().parse(b"p=<-2068,-1106,1593>, v=<-297,-159,225>, a=<21,11,-13>").unwrap();
        let p = Particle {
            p: Vector{x: -2068, y: -1106, z: 1593},
            v: Vector{x: -297, y: -159, z: 225},
            a: Vector{x: 21, y: 11, z: -13}
        };
        assert_eq!(result, p)
    }
}
