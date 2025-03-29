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
	FILE * stream = popen("cc -x c - -Wno-unused-value -Wno-int-conversion", "w");
	fprintf(stream, "%s", prog);
	return !pclose(stream);
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


void process_line(char * prog, char * last_line) {
	char new_prog[PROG_SIZE];
	bool rerun;	
	rerun = should_print(last_line);
	sprintf(new_prog, PROLOGUE "\n%s%sreturn CONTINUE;}", prog, last_line);
	if (compile(new_prog)) {
		if (rerun) {
			sprintf(new_prog, PROLOGUE "\n%srepl_print%sreturn CONTINUE;}", prog, last_line);
			if (compile(new_prog)) run();	
			else goto err;
		}
		else run();
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
			process_line(prog, buffer);
		}
	} while (!feof(stdin));
	return 0;
}

