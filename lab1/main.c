#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    char FileName[256];
    if(write(STDOUT_FILENO, "Введите имя файла для записи: ", 55) < 0){
        exit(-1);
    }
    if((read(STDIN_FILENO, FileName, 256)) < 0){
        exit(-1);
    }
    FileName[strlen(FileName) - 1] = '\0'; 
    int file = open(FileName, O_WRONLY); 
    if(file < 0) {
        printf("Opening error file\n");
        exit(-1);
    }
    int pipe1[2], pipe2[2];
    if((pipe(pipe1)) < 0 || (pipe(pipe2)) < 0){
        printf("Error pipe\n");
        exit(-1);
    }

    pid_t child_pid = fork();
    
    if (child_pid == -1)
    {
        printf("fork");
        exit(-1);
    }

    if (child_pid == 0) {
        // Дочерний процесс
        close(pipe1[1]); //na zapis
        close(pipe2[0]); //na chtenie

        if ((dup2(pipe1[0], STDIN_FILENO)) < 0){
            printf("error dup2\n");
            exit(-1);
        }
        if ((dup2(pipe2[1], STDOUT_FILENO)) < 0){
            printf("error dup2\n");
            exit(-1);
        }

        execlp("./child.out", "./child.out", NULL);
        perror("execlp\n");
        _exit(1);
    } else if(child_pid > 0) {
        // Родительский процесс
        close(pipe1[0]);
        close(pipe2[1]);

        char _String[256];

        if((write(STDOUT_FILENO, "Введите строку: ", 30)) < 0){
            exit(-1);
        }
        int bytesRead = read(STDIN_FILENO, _String, sizeof(_String));
        if (bytesRead < 0){
            exit(-1);
        }

        if((write(pipe1[1], _String, bytesRead)) < 0){
            exit(-1);
        }
            
        int bytesReadChild = read(pipe2[0], _String, sizeof(_String));
        if (bytesRead < 0) {
            exit(-1);
        }
        
        int status;
        waitpid(0, &status, 0);

        if(status != 0)
            return -1;
        else {   
            if((write(STDOUT_FILENO, "Строка удовлетворяет условию: ", 57)) < 0 || (write(STDOUT_FILENO, _String, bytesReadChild)) < 0 || (write(STDOUT_FILENO, "\n", 2)) < 0){
                exit(-1);
            }
        }

        close(pipe1[1]);
        close(pipe2[0]);

        
        close(file);
    }

    return 0;
}
