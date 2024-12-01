FROM golang:latest

ARG src=/usr/src/aoc

COPY . $src

WORKDIR $src/go

# RUN go mod init github.com/yury-fedorov/AoC/AoC23
RUN go get golang.org/x/exp/slices

CMD ["go", "test", "main_test.go"]
