import common as c
import unittest


def _count_xmas(data: []) -> int:
    return sum([x.count("XMAS") for x in data])

def _invert_y(data: []) -> []:
    return _rotate(data)
    # data[::-1]

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

    # Right and down
    dx, dy = 1, 1
    for i in range(yn+xn):
        if i < yn:
            p = (0,yn-i-1)
        else:
            p = (i-yn, 0)
        line = ""
        for _ in iter(int, 1):
            x, y = p
            line += input[y][x]
            p = (x+dx, y+dy)
            if not is_in(p):
                break
        result.append(line)

    # Right and up
    dx, dy = 1, -1
    for i in range(yn+xn):
        if i < yn:
            p = (0,i)
        else:
            p = (i-yn, yn-1)
        line = ""
        for _ in iter(int, 1):
            x, y = p
            line += input[y][x]
            p = (x+dx, y+dy)
            if not is_in(p):
                break
        result.append(line)

    return result

def _diagonals_all(input: []) -> []:
    dl = _diagonals(input)
    di =  [i[::-1] for i in dl] # inverted diagonals
    return dl + di

def _rotate(data: []) -> [] :
    result = []
    xn = len(data[0])
    yn = len(data)
    m = {}
    for y in range(yn):
        for x in range(xn):
            m[(y,x)] = data[y][x]
    for y in range(xn):
        line = ""
        for x in range(yn):
            line += m[(x,y)]
        result.append(line)
    return result

def _answer1(input: []) -> int:
    i0 = input
    ix = _invert_x(i0)
    iy = _invert_y(i0)
    ixy = _invert_xy(i0)
    d = _diagonals_all(i0)
    cx = _count_xmas(ix)
    cy = _count_xmas(iy)
    cxy = _count_xmas(ixy)
    return _count_xmas(i0 + d) + cx + cy + cxy #+ (cxy - cx - cy)


def _answer2(input: []) -> int:
    return 0


class Day04(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        input = c.read_lines(data)
        self.assertEqual(a1, _answer1(input), "answer 1")
        # self.assertEqual(a2, _answer2(input), "answer 2")

    def test_rotate(self):
        input = c.read_lines("04-1")
        right = c.read_lines("04-1-r90")
        self.assertEqual(right, _rotate(input),"wrong 90 degree rotation")

    def test_diagonals(self):
        self.assertEqual(1, _count_xmas(_diagonals_all(c.read_lines("04-1"))), "check diagonals 04-1")

    def test_sample2(self):
        self.assertEqual(18,_answer1(c.read_lines("04-2")),"sample 2")

    def test_sample(self):
        self.__solution("04-1", 4, 0)

    def test_sample_a(self):
        self.__solution("04-1a", 5, 0)

    def test_day(self):
        # 2463 -- too high, 2057 -- too low, 2271 - not right, 2252 - not right
        self.__solution("04", 0, 0)

