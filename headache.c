#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<inttypes.h>
#define MEM_SIZE 30000
#define CELL_SIZE 1

/*
 * Copyright 2022 © GG
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the “Software”), to deal 
 * in the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

/*
 * Brainfuck interpreter
 * ...
 * yes
 */

char *ip;
char *text;
uint8_t data[MEM_SIZE] = { 0 };
uint8_t *dp;

/*
 * author GG weebcyberpunk@gmail.com
 * version 1.1.0
 * since Jun 19, 2022
 */

/*
 * initialize Brainfuck virtual machine (such an elegant name for it lol)
 *
 * returns the program size
 */
int init(char *filename) {

    // open program file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(ENOENT);
    }


    // get program size, ignoring non-instructions, then push program into text
    char c;
    unsigned long text_size = 0;
    short push = 0;
    for (;;) {
        c = getc(fp);
        if (
            (c == '>') ||
            (c == '<') ||
            (c == '+') ||
            (c == '-') ||
            (c == '.') ||
            (c == ',') ||
            (c == '[') ||
            (c == ']')

        ) {
        if (push)
            text[text_size] = c;
        text_size++;
        }
        
        if (c == EOF) {
            if (push)
                break;
            else {
                text = malloc(text_size);
                fseek(fp, 0, SEEK_SET);
                push = 1;
                text_size = 0;
                continue;
            }
        }
    }
    text[text_size] = EOF;

    // initialize pointers
    ip = text;
    dp = data;

    return(text_size);
}

int loop(char this) {

    unsigned long pad = 0;
    uint8_t forward = 0;
    char other;

    if (this == '[') {
        other = ']';
        forward = 1;
        ip++;
    } else {
        other = '[';
        ip--;
    }

    for (;;) {
        if ((*ip == other) && (!pad))
            return(errno);
        if ((*ip == other) && (pad))
            pad--;

        if (*ip == this)
            pad++;

        if (forward) ip++;
        else ip--;
    }
}

int run() {

    while (*ip != EOF) {
        if (*ip == '>')
            dp++;
        else if (*ip == '<')
            dp--;
        else if (*ip == '+')
            ++*dp;
        else if (*ip == '-')
            --*dp;
        else if (*ip == '.')
            putchar(*dp);
        else if (*ip == ',')
            *dp = getchar();
        else if (*ip == '[') {
            if (*dp == 0)
                loop(*ip);
        } else if (*ip == ']') {
            if (*dp != 0)
                loop(*ip);
        }

        ip++;
    }

    return(errno);
}

int main(int argv, char *argc[]) {
    if (argv < 2) {
        fprintf(stderr, "Please specify Brainfuck script to run!\n");
        return(EBADF);
    }

    int p_size = init(argc[1]);
    run();

#ifdef DEBUG
    printf("IP: %p\n", ip);
    printf("Text: %p\n", text);
    printf("Text size: %i\n", p_size);
    printf("DP: %p\n", dp);
    printf("Data: %p\n", data);

    for (int c = 0; c < MEM_SIZE; c++)
        printf("%i ", data[c]);

    for (int c = 0; c < p_size; c++)
        printf("%c ", text[c]);
#endif

    return(errno);
}
