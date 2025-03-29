#ifndef REPLHEADER
#define REPLHEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

#define repl_print(X) _Generic((X), int: print_int, \
                              default: print_unknown, \
															char *: print_str, \
															double: print_float, \
															float: print_float)(X) 
int print_int(int i){
  return printf("%d\n", i);
}

int print_float(float f){
  return printf("%f\n", f);
}

int print_str(char * s) {
	return printf("%s\n", s);
}

int print_unknown(void *err){
  return printf("ERROR: Unknown type\n");
}
#endif
