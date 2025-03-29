#ifndef REPLHEADER
#define REPLHEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h> 

#define CONTINUE 206

#define repl_print(X) _Generic((X), int: print_int, \
                              default: print_pointer, \
															char *: print_str, \
															char: print_char, \
															unsigned char: print_char, \
															signed char: print_char, \
															bool: print_int, \
															long int: print_long, \
															long long int: print_longlong, \
															short int: print_short, \
															unsigned int: print_uint, \
															unsigned long int: print_long_uint, \
															unsigned short int: print_short_uint, \
															unsigned long long int: print_longlong_uint, \
															double: print_float, \
															float: print_float)(X) 

int print_longlong_uint(unsigned long long l) {return printf("%llu\n", l); }
int print_uint(unsigned int i) { return printf("%u\n", i); }
int print_short_uint(unsigned short s) { return printf("%hu\n", s);}
int print_long_uint(unsigned long l) { return printf("%lu\n", l); }
int print_long(long int l) {return printf("%ld\n", l); }
int print_longlong(long long int l) {return printf("%lld\n", l);}
int print_short(short int s) {return printf("%hi\n", s); }
int print_int(int i){ return printf("%d\n", i); }
int print_float(float f){ return printf("%f\n", f); }
int print_str(char * s){ return printf("%s\n", s); }
int print_char(char c) { return printf("%c\n", c); }
int print_pointer(void * p){ return printf("%p\n", p); }
int print_unknown(void *err){ return fprintf(stderr, "ERROR: Unknown type\n"); exit(1); }

void quit() {
	return exit(0);
}

#endif
