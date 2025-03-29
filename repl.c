#include "repl.h"

#define BUFFER_SIZE 256
#define PROG_SIZE 5000
#define HEADER_SIZE 5000
#define MAX_SIZE 1100

#define PROLOGUE "#include \"repl.h\" \n%s \n\nint main(){\n"
#define run_it() (system("./a.out"), system("rm a.out"))

bool should_print(char * line) {
	size_t l = strlen(line);
	return (line[0] == '(' && line[l - 3] == ')');
}

bool compile(char * prog) {
	FILE * stream = popen("cc -x c - -Wno-unused-value -Wno-int-conversion", "w");
	fprintf(stream, "%s", prog);
	return !pclose(stream);
}

bool is_macro(char * line) {
	return line[0] == '#';
}

bool handle_top_macro(char ** line) {
	char scratch[BUFFER_SIZE];
	strcpy(scratch, *line);
	char * first = strtok(scratch, " \t");
	if (!strcmp(first, TOPMACRO)){
		*line += strlen(TOPMACRO);
		return true;
	}
	return false;
}



void run() {
	pid_t pid = fork();
	if (pid == 0) {
		char *args[] = {"./a.out", 0};
		execv(args[0], args);
	}
	else if (pid > 0) {
		int status;
		if (waitpid(pid, &status, 0) > 0) {
			switch (WTERMSIG(status)) {
				case 0:
				break;
				case SIGSEGV:
				fprintf(stderr, "Segmentation fault\n");
				default:
				goto leave;			
			}
			if (WIFEXITED(status) && CONTINUE == WEXITSTATUS(status)) return; 
		}
	}
	leave:
	exit(1);	
}			

void format_prog(char * new_prog, 
								char * header,
								char * body,
								char * last_line, 
								bool print) {
		
	bool added;
	if ((added = is_macro(last_line))) {
			handle_top_macro(&last_line);
			strcpy(header + strlen(header), last_line);
			sprintf(new_prog, PROLOGUE "\n%s" ENDP, header, body);
	}
	else if (print) 
	sprintf(new_prog, PROLOGUE "\n%srepl_print%s" ENDP, 
									header, body, last_line);
	else
	sprintf(new_prog, PROLOGUE "\n%s%s" ENDP, 
									header, body, last_line);
	if (!added) {
		strcpy(body + strlen(body), last_line);
	}
}

void process_line(char * header, char * body, char * last_line) {
	char new_prog[MAX_SIZE];
	bool rerun;	
	rerun = should_print(last_line);
	format_prog(new_prog, header,body, last_line, false);
	if (compile(new_prog)) {
		if (rerun) {
			format_prog(new_prog, header, body, last_line, true);
			if (compile(new_prog)) run();	
			else goto err;
		}
		else run();
		return;
	}

	err:
		exit(1);
}	

int main(int argc, char **argv) {
	
	char buffer[BUFFER_SIZE] = {0};
	char body[PROG_SIZE] = {0};
	char header[HEADER_SIZE] = {0};
	do {
		printf(">> ");
		memset(buffer, 0, BUFFER_SIZE);
		if (fgets(buffer, BUFFER_SIZE, stdin)){
			process_line(header, body, buffer);
		}
	} while (!feof(stdin));
	return 0;
}

