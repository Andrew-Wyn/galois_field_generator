.PHONY: all 

all: compile run


compile:
	@echo "========== Compiling source ..."
	gcc finitefield.c -lm -o finitefield.out
	@echo "========== DONE!"

run:
	./finitefield.out

clean:
	@echo "========== Cleaning local directory"
	rm *.out
	@echo "========== DONE!"
