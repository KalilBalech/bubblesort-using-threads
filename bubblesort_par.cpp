#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "timeit.h"


#define MAX 20
#define NUM_THREADS 4

int arr[MAX];
int n;
int chunk_size;
sem_t sem;

void *bubble_sort(void *arg) {
    int thread_part = *(int *)arg;
    int start = thread_part * chunk_size;
    int end = (thread_part + 1) * chunk_size;

    for (int i = start; i < end; i++) {
        for (int j = start; j < end - i + start - 1; j++) {
            sem_wait(&sem);
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
            sem_post(&sem);
        }
    }

    pthread_exit(NULL);
}

// faz o merge dos 2 primeiros chunks. Depois do resultante com o proximo chunk e assim vai.
void merge(int low, int mid, int high) { // 0 1 3
    int n1 = mid - low + 1; // 2
    int n2 = high - mid; // 2
    int left[n1], right[n2];
    // carrega a parte left
    for (int i = 0; i < n1; i++) {
        left[i] = arr[low + i];
    }
    // carrega a parte right
    for (int i = 0; i < n2; i++) {
        right[i] = arr[mid + 1 + i];
    }
    // faz o merge
    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < n1) {
        arr[k++] = left[i++];
    }

    while (j < n2) {
        arr[k++] = right[j++];
    }
    // printf("Os elementos mergiados foram:\n");
    // for (i = low; i <= high; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");
}

// chama o merge em cada chunk menor e vai duplicando o tamanho dos chunks a serem ordenados
void parallel_merge_sort() {
    for (int size = chunk_size; size < n; size = 2 * size) {
        for (int left_start = 0; left_start < n - 1; left_start += 2 * size) {
            int mid = left_start + size - 1;
            int right_end = ((left_start + 2 * size - 1) < (n - 1)) ? (left_start + 2 * size - 1) : (n - 1);
            merge(left_start, mid, right_end);
        }
    }
}

int main(){
    int i;
    printf("Digite o número de elementos no vetor (máximo %d): ", MAX);
    scanf("%d", &n);

    if (n > MAX) {
        printf("O número máximo de elementos é %d.\n", MAX);
        return 1;
    }
    // printf("O número de elementos registrado é %d.\n", n);
    printf("Digite os elementos do vetor:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    // printf("Os elementos registrados foram:\n");
    // for (i = 0; i < n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    pthread_t threads[NUM_THREADS];
    int thread_parts[NUM_THREADS];
    chunk_size = n / NUM_THREADS;

    sem_init(&sem, 0, 1);
    TIME_IT(
        // Criação das threads
        for (int i = 0; i < NUM_THREADS; i++) {
            thread_parts[i] = i;
            pthread_create(&threads[i], NULL, bubble_sort, (void *)&thread_parts[i]);
        }

        // Espera pelas threads terminarem
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
        // Mesclando os segmentos ordenados
        parallel_merge_sort();

    );

    // printf("Os elementos ordenados dentro de cada chunk foram:\n");
    // for (i = 0; i < n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");


    // Exibindo o array ordenado
    printf("Array ordenado: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    sem_destroy(&sem);

    return 0;
}