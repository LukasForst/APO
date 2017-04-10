program: main.cpp
	g++ -mssse3 -g -O1 -Wall -Werror -lm -o main main.cpp

run: program
	./main

test: program
	./main data/vit_small.ppm && cmp output.txt correct_result/output.txt

zip: program
	zip hw02.zip main.cpp

clean:
	rm main output.txt output.ppm hw02.zip