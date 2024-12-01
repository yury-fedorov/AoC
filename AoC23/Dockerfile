FROM golang:latest

RUN go version

RUN git config --global http.sslverify false

ARG src=/usr/src/aoc

COPY . $src

WORKDIR $src/go

# RUN go mod init github.com/yury-fedorov/AoC/AoC23
RUN go get github.com/yury-fedorov/AoC/AoC23/day07

CMD ["go", "test", "main_test.go"]
