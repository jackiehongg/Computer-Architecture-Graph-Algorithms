all: first

first: first.c
    gcc -Wall -Werror first.c -o first

clean:
    rm -f first