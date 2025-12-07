FROM ubuntu:latest

RUN apt-get update \
    && apt-get --no-install-recommends -y install cmake make clang
RUN clang --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src
RUN cmake .
RUN make all
CMD ["./aoc17"]
