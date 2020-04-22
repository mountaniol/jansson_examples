EX1_S=example1_basic.c
EX1_T=example1_basic.out

EX2_S=example2_optimised.c
EX2_T=example2_optimised.out

all: ex1 ex2
clean:
	rm -f $(EX1_T) $(EX2_T)
ex1:
	gcc -Wall -Wextra -o $(EX1_T) $(EX1_S) -ljansson

ex2:
	gcc -Wall -Wextra -o $(EX2_T) $(EX2_S) -ljansson

