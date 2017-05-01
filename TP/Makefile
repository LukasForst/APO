OBJ=start.c color.c sets.c

program: ${OBJ}
	clang ${OBJ} -lm -o program.out

test: program
	./program.out && xdg-open pic.ppm

run: program
	./program.out

clean:
	rm -f *.o *.a *.out *.ppm
