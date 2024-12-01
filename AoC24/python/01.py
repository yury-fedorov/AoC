import common as c

left = []
right = []
with c.open_file("01") as f:
    for line in f:
        x = line.split()
        left.append(int(x[0]))
        right.append(int(x[1]))

left.sort()
right.sort()

n = len(left)
diff = []
for i in range(0, n):
    diff.append(abs(right[i] - left[i]))

assert sum(diff) == 2344935, "answer 1"

score = []  # similarity score
for i in range(0, n):
    score.append(left[i] * right.count(left[i]))

assert sum(score) == 27647262, "answer 2"
