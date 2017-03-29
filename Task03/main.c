#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 255

void environment_variable_detected(char *line) {
    char *name[1];
    char *value;
    line++;

    name[0] = strtok(line, " \n");
    line = NULL;

    if ((value = strtok(line, " \n")) != NULL) {
        printf("setting environment variable = %s on value = %s\n", name[0], value);
        setenv(name[0], value, 1);
    } else {
        printf("unsetting environment variable = %s\n", name[0]);
        unsetenv(name[0]);
    }
}

void command_detected(char *line) {
    char *command;
    int status;
    int i = 1;
    char *args[SIZE];

    command = strtok(line, " \n");
    line = NULL;
    args[0] = (char *) malloc(strlen("/bin/") + strlen(command));
    if(command[0] != '.') {
        strcpy(args[0], "/bin/");
        strcat(args[0], command);
    }
    else {
        strcpy(args[0],command);
    }

    while ((args[i] = strtok(line, " \n")) != NULL) {
        line = NULL;
        i++;
    }
    args[i] = NULL;

    pid_t pid;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Can't exec command ");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("Can't make new procces");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Wrong arguments! The only argument should be name of the file");
        exit(-1);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Can't open file");
        return 1;
    }

    char *line;
    size_t len = 0;
    while (!feof(file)) {
        getline(&line, &len, file);

        printf("\nLINE = %s\n", line);

        if (line[0] == '#') {
            environment_variable_detected(line);
        } else {
            command_detected(line);
        }
    }


    fclose(file);

    return 0;
}
