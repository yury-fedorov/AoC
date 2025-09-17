FROM maven:latest

RUN mvn --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src/java
CMD mvn test

