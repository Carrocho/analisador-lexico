all:
	gcc analisador.c -o analisador
	./analisador teste.txt

run:
	./analisador teste.txt

clean:
	rm analisador