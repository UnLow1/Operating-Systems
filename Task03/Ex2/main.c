#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define SIZE 255

void timeCheckpoint(const char *message) {
    // Getting time values
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    struct timeval user_time = usage.ru_utime;
    struct timeval system_time = usage.ru_stime;
    // Show info
    printf("user time: %ld | system time: %ld  ---  %s\n",
           user_time.tv_sec*1000000+user_time.tv_usec,
           system_time.tv_sec*1000000+system_time.tv_usec,
           message);
}

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

void command_detected(char *line, struct rlimit time_limit, struct rlimit memory_limit) {
    char *command;
    int status;
    int i = 1;
    char *args[SIZE];
    char *message = line;


    command = strtok(line, " \n");
    line = NULL;
    args[0] = (char *) malloc(strlen("/bin/") + strlen(command));
    if (command[0] != '.') {
        strcpy(args[0], "/bin/");
        strcat(args[0], command);
    } else {
        strcpy(args[0], command);
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

        time_limit.rlim_cur = 3;
        memory_limit.rlim_cur = 20 * 1024 * 1024;

        if (setrlimit(RLIMIT_CPU, &time_limit) == -1) {
            perror("setrlimit(RLIMIT_CPU) failed");
            exit(1);
        }
        if (setrlimit(RLIMIT_AS, &memory_limit)) {
            perror("setrlimit(RLIMIT_AS) failed");
            exit(1);
        }

//        getrlimit (RLIMIT_CPU, &time_limit);
//        printf("time_limit: soft=%lld; max=%ld\n",
//               (long long) time_limit.rlim_cur,time_limit.rlim_max);
//        getrlimit (RLIMIT_AS, &memory_limit);
//        printf("memory_limit: soft=%lld; max=%ld\n",
//               (long long) memory_limit.rlim_cur/1024/1024, memory_limit.rlim_max/1024/1024);

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
            timeCheckpoint(message);
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Wrong arguments! Arguments: name of the file, limit in seconds and limit in MB");
        exit(-1);
    }

    struct rlimit time_limit;
    struct rlimit memory_limit;

    time_limit.rlim_max = (rlim_t) atoi(argv[2]);
    time_limit.rlim_cur = time_limit.rlim_max;


    memory_limit.rlim_max = (rlim_t) atoi(argv[3]) * 1024 * 1024;
    memory_limit.rlim_cur = memory_limit.rlim_max;


//    printf("time_limit: soft=%lld; hard=%lld\n",
//           (long long) time_limit.rlim_cur, (long long) time_limit.rlim_max);
//    printf("memory_limit: soft=%lld; hard=%lld\n",
//           (long long) memory_limit.rlim_cur, (long long) memory_limit.rlim_max/1024/1024);


    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Can't open file");
        return 1;
    }

    timeCheckpoint("Starting program");

    char *line;
    size_t len = 0;
    while (!feof(file)) {
        getline(&line, &len, file);

//        printf("\nLINE = %s\n", line);

        if (line[0] == '#') {
            environment_variable_detected(line);
        } else {
            command_detected(line, time_limit, memory_limit);
        }

        printf("\n");
    }


    fclose(file);

    return 0;
}
