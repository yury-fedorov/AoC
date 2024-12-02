import common as c
import math
import copy

sign = lambda x: math.copysign(1, x)


def read_levels(data: str):
    levels = []
    with c.open_file(data) as f:
        for line in f:
            x = line.split()
            level = []
            for lv in x:
                level.append(int(lv))
            levels.append(level)
    return levels


def calc_diff(level):
    n = len(level)
    diff = []
    for i in range(1, n):
        diff.append(level[i] - level[i - 1])
    return diff


def is_safe(level) -> bool:
    diff = calc_diff(level)
    direction = None
    for d in diff:
        if not (1 <= abs(d) <= 3):
            return False
        direction_x = sign(d)
        if (direction is not None) and (direction_x != direction):
            return False
        direction = direction_x

    return True


def is_safe_2(level) -> bool:
    if is_safe(level):
        return True
    n = len(level)
    for i in range(0, n):
        m_level = copy.copy(level)
        m_level.pop(i)
        if is_safe(m_level):
            return True

    return False


def answer1(levels) -> int:
    safe_count = 0
    for level in levels:
        safe_count += 1 if is_safe(level) else 0
    return safe_count


def answer2(levels) -> int:
    safe_count = 0
    for level in levels:
        safe_count += 1 if is_safe_2(level) else 0
    return safe_count


levels = read_levels("02-1")
assert answer1(levels) == 2, "answer 1"
assert answer2(levels) == 4, "answer 2"

levels = read_levels("02")
assert answer1(levels) == 549, "answer 1"
assert answer2(levels) == 589, "answer 2"
