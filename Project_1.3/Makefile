# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC= gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS= -c -Wall 

default: build

general: main.o functions.o 
	$(CC) ./out/main.o ./out/functions.o declare.h -o ./out/function_zeros_calculator -lm
	

main.o: main.c
	$(CC) $(CFLAGS) main.c -o ./out/main.o

functions.o: functions.c
	$(CC) $(CFLAGS) functions.c -o ./out/functions.o

clean:
	rm -rf ./out/main.o ./out/functions.o

build: general clean

run: 
	./out/function_zeros_calculator