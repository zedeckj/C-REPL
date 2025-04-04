#include "repl.h"
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE 256
#define PROG_SIZE 5000
#define HEADER_SIZE 5000
#define MAX_SIZE 1100
#define loop for(;;)
#define PROLOGUE "#include \"repl.h\" \n%s \n\nint main(){\n#line 1 \"crepl\"\n"

bool should_print(char * line) {
	size_t l = strlen(line);
	return (line[0] == '(' && line[l - 2] == ')');
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
			remove("./a.out");
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

bool format_prog(char * new_prog, 
								char * header,
								char * body,
								char * last_line, 
								bool print) {
		
	bool added;
	if ((added = is_macro(last_line))) {
			handle_top_macro(&last_line);
			strcpy(header + strlen(header), last_line);
			sprintf(new_prog, PROLOGUE "%s" ENDP, header, body);
	}
	else if (print) 
	sprintf(new_prog, PROLOGUE "%srepl_print%s" ENDP, 
									header, body, last_line);
	else
	sprintf(new_prog, PROLOGUE "%s%s" ENDP, 
									header, body, last_line);
	return added;
}


void process_line(char * header, char * body, char * last_line) {
	char new_prog[MAX_SIZE];
	bool rerun;	
	rerun = should_print(last_line);
	bool added = format_prog(new_prog, header,body, last_line, false);
	if (compile(new_prog)) {
		if (rerun) {
			format_prog(new_prog, header, body, last_line, true);
			if (compile(new_prog)) run();	
		}
		if (!added) {
			strcat(last_line, "\n");
			strcpy(body + strlen(body), last_line);
		}
		else run();
		return;
	}
}	

void move_down(int x, int y) {
	return;
}

int main(int argc, char **argv) {	
	char body[PROG_SIZE] = {0};
	char header[HEADER_SIZE] = {0};
	using_history();	
	loop {
		char *line = readline("\033[38;5;87m>>\033[0m ");
		if (line && *line) {
			process_line(header, body, line);
		}
		add_history(line);
	}
	return 0;
}


