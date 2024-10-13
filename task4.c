#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100

typedef struct {
    char name[50];
    int severity;
} Patient;

typedef struct {
    Patient patients[MAX_PATIENTS];
    int size;
} MaxHeap;

void swap(Patient* a, Patient* b) {
    Patient temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MaxHeap* heap, int index) {
    int parent = (index - 1) / 2;
    if (index > 0 && heap->patients[index].severity > heap->patients[parent].severity) {
        swap(&heap->patients[index], &heap->patients[parent]);
        heapifyUp(heap, parent);
    }
}

void heapifyDown(MaxHeap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->patients[left].severity > heap->patients[largest].severity)
        largest = left;

    if (right < heap->size && heap->patients[right].severity > heap->patients[largest].severity)
        largest = right;

    if (largest != index) {
        swap(&heap->patients[index], &heap->patients[largest]);
        heapifyDown(heap, largest);
    }
}

void addPatient(MaxHeap* heap, const char* name, int severity) {
    if (heap->size >= MAX_PATIENTS) {
        printf("Error: Emergency room is full\n");
        return;
    }

    Patient newPatient;
    strcpy(newPatient.name, name);
    newPatient.severity = severity;

    heap->patients[heap->size] = newPatient;
    heapifyUp(heap, heap->size);
    heap->size++;
}

Patient* extractMax(MaxHeap* heap) {
    if (heap->size == 0) return NULL;

    Patient* max = &heap->patients[0];
    heap->patients[0] = heap->patients[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);

    return max;
}

void updateSeverity(MaxHeap* heap, const char* name, int newSeverity) {
    for (int i = 0; i < heap->size; i++) {
        if (strcmp(heap->patients[i].name, name) == 0) {
            heap->patients[i].severity = newSeverity;
            heapifyUp(heap, i);
            heapifyDown(heap, i);
            return;
        }
    }
    printf("Patient not found\n");
}

void displayQueue(MaxHeap* heap) {
    printf("Current patient queue:\n");
    for (int i = 0; i < heap->size; i++) {
        printf("%s (Severity: %d)\n", heap->patients[i].name, heap->patients[i].severity);
    }
}

int main() {
    MaxHeap heap = {0};
    int choice, severity;
    char name[50];

    while (1) {
        printf("\n1. Add patient\n2. Treat next patient\n3. Update patient severity\n");
        printf("4. Display patient queue\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter patient name: ");
                scanf("%s", name);
                printf("Enter severity (1-10): ");
                scanf("%d", &severity);
                addPatient(&heap, name, severity);
                break;
            case 2:
                {
                    Patient* nextPatient = extractMax(&heap);
                    if (nextPatient) {
                        printf("Treating patient: %s (Severity: %d)\n", nextPatient->name, nextPatient->severity);
                    } else {
                        printf("No patients in queue.\n");
                    }
                }
                break;
            case 3:
                printf("Enter patient name: ");
                scanf("%s", name);
                printf("Enter new severity (1-10): ");
                scanf("%d", &severity);
                updateSeverity(&heap, name, severity);
                break;
            case 4:
                displayQueue(&heap);
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
