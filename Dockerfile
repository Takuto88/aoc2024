FROM debian:12

RUN apt update && apt-get install -y cmake build-essential valgrind pkg-config libcriterion-dev gdbserver

RUN mkdir -p /src/aoc_2024/build
WORKDIR /src/aoc_2024/build

COPY . /src/aoc_2024

RUN cmake .. && make aoc_2024

ENTRYPOINT ["valgrind", "--leak-check=full"]
#ENTRYPOINT ["gdbserver", "--remote-debug", "0.0.0.0:1234" ]
CMD ["/src/aoc_2024/build/aoc_2024"]