import common as c

def read_input(data:str):
    left = []
    right = []
    with c.open_file(data) as f:
        for line in f:
            x = line.split()
            left.append(int(x[0]))
            right.append(int(x[1]))
    left.sort()
    right.sort()
    return left, right

def answer1(left, right) -> int:
    diff = 0
    for i in range(0, len(left)):
        diff += abs(right[i] - left[i])
    return diff

def answer2(left, right) -> int:
    score = 0  # similarity score
    for i in range(0, len(left)):
        score += (left[i] * right.count(left[i]))
    return score

left, right = read_input("01")
assert answer1(left, right) == 2344935, "answer 1"
assert answer2(left, right) == 27647262, "answer 2"