FROM sbtscala/scala-sbt:eclipse-temurin-17.0.4_1.7.1_3.2.0

RUN sbt --version

ARG src=/usr/src/aoc
COPY . $src
WORKDIR $src
CMD sbt test

