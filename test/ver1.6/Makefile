# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC= gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS= -Wall -pedantic 

default: build



clean:
	rm ./out/function_zeros_calculator
	

build:
	$(CC) ./main.c ./functions.c $(CFLAGS) -o ./out/function_zeros_calculator -lm

run: 
	./out/function_zeros_calculator
	