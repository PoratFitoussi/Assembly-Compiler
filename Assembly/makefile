assembler: main.o convertText.o input.o checkText.o symbol.o data.o order.o addOrder.o addEx.o lineOutput.o creatFile.o
	gcc -g -ansi -Wall -pedantic main.o convertText.o input.o checkText.o symbol.o data.o order.o addOrder.o addEx.o lineOutput.o creatFile.o -o assembler -lm
main.o : main.c input.o symbol.h checkText.h  convertText.h order.h lineOutput.h addEx.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
convertText.o: convertText.c convertText.h
	gcc -c -ansi -Wall -pedantic convertText.c -o convertText.o -lm
input.o: input.c input.h 
	gcc -c -ansi -Wall -pedantic input.c -o input.o
checkText.o: checkText.c checkText.h 
	gcc -c -ansi -Wall -pedantic checkText.c -o checkText.o
symbol.o: symbol.c input.h symbol.h checkText.h
	gcc -c -ansi -Wall -pedantic symbol.c -o symbol.o
data.o: data.c data.h
	gcc -c -ansi -Wall -pedantic data.c -o data.o
order.o: order.c order.h 
	gcc -c -ansi -Wall -pedantic order.c -o order.o
addOrder.o: addOrder.c order.h 
	gcc -c -ansi -Wall -pedantic addOrder.c -o addOrder.o
addEx.o: addEx.c addEx.h
	gcc -c -ansi -Wall -pedantic addEx.c -o addEx.o
lineOutput.o: lineOutput.c lineOutput.h symbol.h data.h addEx.h
	gcc -c -ansi -Wall -pedantic lineOutput.c -o lineOutput.o
creatFile.o: creatFile.c 
	gcc -c -ansi -Wall -pedantic creatFile.c -o creatFile.o