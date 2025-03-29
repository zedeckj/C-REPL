#include "repl.h"

#define BUFFER_SIZE 256
#define PROG_SIZE 5000


#define PROLOGUE "#include \"repl.h\"\n\nint main(){\n"
#define run_it() (system("./a.out"), system("rm a.out"))

bool should_print(char * line) {
	size_t l = strlen(line);
	return (line[0] == '(' && line[l - 3] == ')');
}

bool compile(char * prog) {
	FILE * stream = popen("cc -x c - -Wno-unused-value", "w");
	fprintf(stream, "%s", prog);
	return !pclose(stream);
}

void write_file(char * prog, char * last_line) {
	char new_prog[PROG_SIZE];
	bool rerun;	
	rerun = should_print(last_line);
	sprintf(new_prog, PROLOGUE "\n%s%s}", prog, last_line);
	if (compile(new_prog)) {
		if (rerun) {
			sprintf(new_prog, PROLOGUE "\n%srepl_print%s}", prog, last_line);
			if (compile(new_prog)) run_it();	
			else goto err;
		}
		else run_it();
		strcpy(prog + strlen(prog), last_line);
		return;
	}
	err:
		exit(1);
}	

int main(int argc, char **argv) {
	
	char buffer[BUFFER_SIZE] = {0};
	char prog[PROG_SIZE] = {0};
	do {
		printf(">> ");
		memset(buffer, 0, BUFFER_SIZE);
		if (fgets(buffer, BUFFER_SIZE, stdin)){
			write_file(prog, buffer);
		}
	} while (!feof(stdin));
	return 0;
}

