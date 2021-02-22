use crate::common;

/// For each coordinate x, y, z, the position p(t) as a function of time is calculated as follows:
///
/// p(0) = p0
/// p(1) = p0 + v(1)
/// p(2) = p0 + v(1) + v(2)
/// p(t) = p0 + sum(1..t+1)(v(t))
///
/// v(0) = v0
/// v(1) = v0 + a
/// v(2) = v0 + a + a
/// v(t) = v0 + a*t
///
/// p(t) = p0 + sum(1..t+1)(v0 + at)
///      = p0 + v0*t + a*sum(1..t+1)(t)
///      = p0 + v0*t + a*(t^2 + t)/2  (see [https://de.wikipedia.org/wiki/Gaußsche_Summenformel])
///      = p0 + (v0 + a/2)*t + (a/2)*t^2
///
/// This is a polynomial, so for large times t, the term of the highest order will always be the
/// most significant one. Therefore, when I compare the position of two particles, I can compare the
/// coefficients of the exponential terms lexicographically, starting with the highest order term.
/// I.e. I first compare the absolute values of (a/2) of two particles (because that is the
/// coefficient of t^2). Only if they are equal, I then compare the absolute values of (v0 + a/2)
/// of those two particles (because that is the coefficient of t). And only if these are
/// equal as well, I compare the absolute values of p0 of the two particles (the coefficient of t^0).
///
/// Since I am given three dimensions and I have to compare the Manhatten sum, when I say p0 above,
/// I mean |px0| + |py0| + |pz0|. And likewise for v and a.
///
/// On top of that, I will all values (this does not matter for comparison), so that I can work with
/// integers, even when dividing by 2

mod parsing;

// a vector in the mathematical or geometrical sense, modeling x, y and z parts of position,
// velocity or acceleration
#[derive(Copy, Clone, Debug, PartialEq)]
struct Vector {
    x: i32,
    y: i32,
    z: i32,
}

#[derive(Copy, Clone, Debug, PartialEq)]
struct Particle {
    p: Vector,
    v: Vector,
    a: Vector,
}

// The documentation of Ord says:
// When derived on structs, it will produce a lexicographic ordering based on the top-to-bottom
// declaration order of the struct's members
#[derive(Ord, PartialOrd, PartialEq, Eq, Copy, Clone)]
struct ParticleCoefficients {
    t_hoch_2: i32,
    t_hoch_1: i32,
    t_hoch_0: i32,
}

impl ParticleCoefficients {
    fn from_particle(p: Particle) -> ParticleCoefficients {
        let t_hoch_2 = p.a.x.abs() + p.a.y.abs() + p.a.z.abs();
        let t_hoch_1 = (2*p.v.x + p.a.x).abs() + (2*p.v.y + p.a.y).abs() + (2*p.v.z + p.a.z).abs();
        let t_hoch_0 = 2*p.p.x + 2*p.p.y + 2*p.p.z;

        ParticleCoefficients {
            t_hoch_2,
            t_hoch_1,
            t_hoch_0,
        }
    }
}

fn closest_particle<I>(i: I) -> Option<usize>
where I: IntoIterator<Item=Particle> {
    i
        .into_iter()
        .zip(0..)
        .map(|(p, i)| (ParticleCoefficients::from_particle(p), i))
        .min_by_key(|(c, _)| *c)
        .map(|(_, i)| i)
}

pub fn task1(path: &str) -> usize {
    let text = common::input(path);
    let particles = text
        .lines()
        .into_iter()
        .map(|line| parsing::parse_particle().parse(line.as_bytes()).unwrap());

    closest_particle(particles).unwrap()
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_example() {
        let p1 = Particle {
            p: Vector{ x: 3, y: 0, z: 0 },
            v: Vector{ x: 2, y: 0, z: 0 },
            a: Vector{ x: -1, y: 0, z: 0 },
        };

        let p2 = Particle {
            p: Vector{ x: 4, y: 0, z: 0 },
            v: Vector{ x: 0, y: 0, z: 0 },
            a: Vector{ x: -2, y: 0, z: 0 },
        };

        let m = closest_particle(vec!(p1, p2));

        assert_eq!(m, Some(0));
    }

    #[test]
    fn test_task1_input_david() {
        let result = task1("20/input_david.txt");

        assert_eq!(result, 364)
    }
}
