# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-Wall -pedantic 

default: build

clean: 
	rm ./out/func_zero_calc
	

build: 
	$(CC) ./main.c ./functions.c $(CFLAGS) -o ./out/func_zero_calc -lm

run: 
	./out/func_zero_calc

	