import common as c
import unittest


def _count_xmas(data: []) -> int:
    return sum([x.count("XMAS") for x in data])


def _invert_y(data: []) -> []:
    return _rotate(data)


def _invert_x(data: []) -> []:
    return [x[::-1] for x in data]


def _invert_xy(data: []) -> []:
    return _invert_x(_invert_y(data))


def _diagonals(input: []) -> []:
    result = []
    xn = len(input[0])
    yn = len(input)

    def is_in(p: (int, int)) -> bool:
        x, y = p
        if x < 0 or y < 0:
            return False
        if x >= xn or y >= yn:
            return False
        return True

    def diagonal(x: int, y: int, dx: int, dy: int) -> str:
        result = ""
        for _ in iter(int, 1):
            result += input[y][x]
            x += dx
            y += dy
            if not is_in((x, y)):
                return result

    # Right and down
    dx, dy = 1, 1
    x0, y0 = xn, 0
    for i in range(yn + xn):
        if x0 > 0:
            x0 -= 1
        else:
            y0 += 1
        if not is_in((x0, y0)):
            break
        result.append(diagonal(x0, y0, dx, dy))

    # Right and up
    dx, dy = 1, -1
    x0, y0 = 0, 0
    for i in range(yn + xn):
        # (0,0), (0,1), ...
        if i < (yn - 1):
            y0 += 1
        else:
            x0 += 1
        if not is_in((x0, y0)):
            break
        result.append(diagonal(x0, y0, dx, dy))

    return result


def _diagonals_all(input: []) -> []:
    dl = _diagonals(input)
    di = [i[::-1] for i in dl]  # inverted diagonals
    return dl + di


def _rotate(data: []) -> []:
    result = []
    xn = len(data[0])
    yn = len(data)
    m = {}
    for y in range(yn):
        for x in range(xn):
            m[(y, x)] = data[y][x]
    for y in range(xn):
        line = ""
        for x in range(yn):
            line += m[(x, y)]
        result.append(line)
    return result


def _answer1(input: []) -> int:
    i0 = input
    ix = _invert_x(i0)
    iy = _invert_y(i0)
    ixy = _invert_xy(i0)
    d = _diagonals_all(i0)
    return _count_xmas(i0 + ix + iy + ixy + d)


# Part 2

def _answer2(input: []) -> int:
    xn = len(input[0])
    yn = len(input)

    def is_x(x:int,y:int) -> bool:
        # Is the position is suitable for X?
        if x < 1 or y < 1:
            return False
        if x >= (xn-1) or y >= (yn-1):
            return False

        # Is the center is 'A'?
        center = input[y][x]
        if center != 'A':
            return False

        a = input[y-1][x-1] + 'A' + input[y+1][x+1]
        b = input[y+1][x-1] + 'A' + input[y-1][x+1]
        ai = a[::-1]
        bi = b[::-1]

        return (a + b + ai + bi).count("MAS") == 2

    result = 0
    for x in range(xn):
        for y in range(yn):
            if is_x(x, y):
                result += 1
    return result

class Day04(unittest.TestCase):
    def __solution(self, data: str, a1: int, a2: int):
        input = c.read_lines(data)
        self.assertEqual(a1, _answer1(input), "answer 1")
        self.assertEqual(a2, _answer2(input), "answer 2")

    def __answer1(self, data:str) -> int:
        return _answer1(c.read_lines(data))

    def test_rotate(self):
        input = c.read_lines("04-1")
        right = c.read_lines("04-1-r90")
        self.assertEqual(right, _rotate(input), "wrong 90 degree rotation")

    def test_diagonals(self):
        self.assertEqual(1, _count_xmas(_diagonals_all(c.read_lines("04-1"))), "check diagonals 04-1")

    def test_sample2(self):
        self.assertEqual(18, self.__answer1("04-2"), "sample 2")

    def test_sample_part2(self):
        self.assertEqual(9, _answer2(c.read_lines("04-3")), "part 2 sample")

    def test_sample(self):
        self.assertEqual(4, self.__answer1("04-1"), "04-1")

    def test_sample_a(self):
        self.assertEqual(5, self.__answer1("04-1a"), "04-1a")

    def test_day(self):
        self.__solution("04", 2454, 1858)
