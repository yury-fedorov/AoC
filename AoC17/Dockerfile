FROM rust:latest
ARG src=/usr/src/aoc
COPY . $src
WORKDIR /usr/src/aoc/rust
RUN cargo build
# RUN cargo install --path .
CMD cargo test
