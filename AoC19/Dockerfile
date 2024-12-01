FROM ubuntu:24.10
# FROM openjdk:latest

RUN apt-get update \
    && apt-get --no-install-recommends -y install openjdk-17-jdk maven
RUN mvn --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src/java
CMD mvn test

