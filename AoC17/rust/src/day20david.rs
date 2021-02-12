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
/// I mean px0 + py0 + pz0. And likewise for v and a.
///

