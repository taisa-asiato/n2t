build:
	gcc -c main.c
	gcc -c JackAnalyzers.c 
	gcc -c JackToknizer.c  
	gcc -c CompliationEngine.c 
	gcc -o JackAnalyzer main.o JackAnalyzers.o JackToknizer.o CompliationEngine.o

clean:
	rm *.o

run:
	./JackAnalyzer ./ArrayTest/Main.jack
