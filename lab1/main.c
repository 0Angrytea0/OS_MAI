#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    char FileName[256];
    write(STDOUT_FILENO, "Введите имя файла для записи: ", 55);
    read(STDIN_FILENO, FileName, 256);
    FileName[strlen(FileName) - 1] = '\0'; 
    int file = open(FileName, O_WRONLY); 
    if(file < 0) {
        perror("Error file");
        exit(-1);
    }

    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    pid_t child_pid = fork();
    
    if (child_pid == -1)
    {
        write(STDERR_FILENO, "fork", 5);
        exit(-1);
    }

    if (child_pid == 0) {
        // Дочерний процесс
        close(pipe1[1]); //na zapis
        close(pipe2[0]); //na chtenie

        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        execlp("./child", "./child", NULL);
        _exit(1);
    } else if(child_pid > 0) {
        // Родительский процесс
        close(pipe1[0]);
        close(pipe2[1]);

        char _String[256];

        write(STDOUT_FILENO, "Введите строку: ", 30);
        int bytesRead = read(STDIN_FILENO, _String, sizeof(_String));

        write(pipe1[1], _String, bytesRead);
            
        int bytesReadChild = read(pipe2[0], _String, sizeof(_String));
        
        int status;
        waitpid(0, &status, 0);

        if(status != 0)
            return -1;
        else {   
            write(STDOUT_FILENO, "Строка удовлетворяет условию: ", 57);
            write(STDOUT_FILENO, _String, bytesReadChild);
            write(STDOUT_FILENO, "\n", 2);
        }

        close(pipe1[1]);
        close(pipe2[0]);

        
        close(file);
    }

    return 0;
}
