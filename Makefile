.PHONY: all 

all: compile run clean


compile:
	@echo "========== Compiling source ..."


run:
	@echo "========== Running binary ..."
	./finitefield.out

clean:
	@echo "========== Cleaning local directory"
	rm *.out
