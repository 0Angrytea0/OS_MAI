#include <stdio.h>  //gcc -o main main.c -pthread
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 4

// Структура для передачи аргументов в поток
typedef struct {
    int* array;
    int start;
    int end;
} ThreadArgs;

// Функция сортировки слиянием
void merge(int* array, int start, int mid, int end) {
    int leftLength = mid - start + 1;
    int rightLength = end - mid;

    int* leftArray = (int*)malloc(leftLength * sizeof(int));
    int* rightArray = (int*)malloc(rightLength * sizeof(int));

    for (int i = 0; i < leftLength; i++) {
        leftArray[i] = array[start + i];
    }

    for (int i = 0; i < rightLength; i++) {
        rightArray[i] = array[mid + 1 + i];
    }

    int i = 0, j = 0, k = start;

    while (i < leftLength && j < rightLength) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < leftLength) {
        array[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < rightLength) {
        array[k] = rightArray[j];
        j++;
        k++;
    }

    free(leftArray);
    free(rightArray);
}

// Функция сортировки слиянием в отдельном потоке
void* mergeSort(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int* array = args->array;
    int start = args->start;
    int end = args->end;

    if (start < end) {
        int mid = start + (end - start) / 2;

        // Рекурсивно сортируем левую и правую части массива
        ThreadArgs leftArgs = {array, start, mid};
        ThreadArgs rightArgs = {array, mid + 1, end};

        pthread_t leftThread, rightThread;

        pthread_create(&leftThread, NULL, mergeSort, &leftArgs);
        pthread_create(&rightThread, NULL, mergeSort, &rightArgs);

        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        // Объединяем отсортированные части
        merge(array, start, mid, end);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int arraySize = atoi(argv[1]);

    if (arraySize <= 0) {
        printf("Array size must be a positive integer\n");
        return 1;
    }

    int* array = (int*)malloc(arraySize * sizeof(int));

    // Заполняем массив случайными значениями
    for (int i = 0; i < arraySize; i++) {
        int a;
        scanf("%d", &a);
        array[i] = a;
    }

    // Выводим исходный массив
    printf("Original array: ");
    for (int i = 0; i < arraySize; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Создаем структуру для аргументов потока
    ThreadArgs args = {array, 0, arraySize - 1};

    // Создаем поток для сортировки
    pthread_t sortingThread;
    pthread_create(&sortingThread, NULL, mergeSort, &args);

    // Ожидаем завершения потока
    pthread_join(sortingThread, NULL);

    // Выводим отсортированный массив
    printf("Sorted array: ");
    for (int i = 0; i < arraySize; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);

    return 0;
}