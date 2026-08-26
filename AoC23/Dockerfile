FROM golang:latest

ARG src=/usr/src/aoc

COPY . $src

WORKDIR $src/go

# Runs all slow tests. It takes 168.6 s. The slowest is day 25 (135 s).
ENV AOC_SLOW=1

CMD ["go", "test", "-v", "main_test.go"]