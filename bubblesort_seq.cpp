#include <stdio.h>
#include "timeit.h"

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Troca arr[j] e arr[j+1]
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    int n, i;
    
    printf("Digite a quantidade de elementos do vetor: ");
    scanf("%d", &n);
    
    int arr[n];
    
    printf("Digite os elementos do vetor:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    TIME_IT(
        bubbleSort(arr, n);
    );
    
    printf("Vetor ordenado:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}
