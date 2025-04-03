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
#include <sys/stat.h>
#include <unistd.h>

#define CONTINUE 206
#define ENDP "\nreturn CONTINUE;}"
#define TOPMACRO "#top"

#define _INT_COLOR ""
//"\033[0;35m"
#define _RST_COLOR "" 
//"\033[0m" 

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

int print_longlong_uint(unsigned long long l) {
	return printf(_INT_COLOR "%llullu\n" _RST_COLOR, l); }
int print_uint(unsigned int i) { return printf(_INT_COLOR "%uu\n" _RST_COLOR, i); }
int print_short_uint(unsigned short s) { return printf(_INT_COLOR "%huhu\n" _RST_COLOR, s);}
int print_long_uint(unsigned long l) { return printf(_INT_COLOR "%lulu\n" _RST_COLOR, l); }
int print_long(long int l) {return printf(_INT_COLOR "%ldl\n" _RST_COLOR, l); }
int print_longlong(long long int l) {return printf(_INT_COLOR "%lldll\n" _RST_COLOR, l);}
int print_short(short int s) {return printf(_INT_COLOR "%hih\n" _RST_COLOR, s); }
int print_int(int i){ return printf(_INT_COLOR "%d\n" _RST_COLOR, i); }
int print_float(float f){ return printf(_INT_COLOR "%ff\n" _RST_COLOR, f); }
int print_str(char * s){ return printf(_INT_COLOR "\"%s\"\n" _RST_COLOR, s); }
int print_char(char c) { return printf(_INT_COLOR "'%c'\n" _RST_COLOR, c); }
int print_pointer(void * p){ return printf(_INT_COLOR "%p\n" _RST_COLOR, p); }
int print_unknown(void *err){ return fprintf(stderr, "ERROR: Unknown type\n"); exit(1); }

void quit() {
	return exit(0);
}

#endif
