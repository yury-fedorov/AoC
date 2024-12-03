echo "Mini version of only not solved parts."
rm ./mini/aoc22-mini
cd mini
cmake .
make all
cd ..
./mini/aoc22-mini