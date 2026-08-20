FROM golang:latest

ARG src=/usr/src/aoc

COPY . $src

WORKDIR $src/go

CMD ["go", "test", "-v", "main_test.go"]
