#include <unistd.h>
#include <stdlib.h>

int main() {
    char _string[256];
    int bytesRead;

    bytesRead = read(STDIN_FILENO, _string, sizeof(_string));
    if (bytesRead < 0){
        exit(-1);
    }
    
    if (_string[bytesRead - 2] == ';' || _string[bytesRead - 2] == '.') {
        // строка заканчивается на ";" или "."
        if((write(STDOUT_FILENO, _string, bytesRead - 1)) < 0){
            exit(-1);
        }
    } else {
        // строка не соответствует условию
        if((write(STDERR_FILENO, "Ошибка: строка не заканчивается на \";\" или \".\"\n", 80)) < 0){
            exit(-1);
        }
        exit(-1);
    }

    return 0;
}
