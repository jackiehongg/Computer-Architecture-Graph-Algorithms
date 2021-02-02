#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
    char vertex[20];
    int weight;
    struct node* next;
};

struct node** buildArray(FILE*, int);   // Build array to store linked lists
void add(struct node**, char*, char*, int, int);
struct node* append(struct node* head, char* v, int);    // Add helper method
struct node* allocate(char*);
void freeAll(struct node**, int);
void inDegree(struct node**, char*, int);   // Respond to Queries
void outDegree(struct node**, char*, int);
void adjacencies(struct node**, char*, int);
void printAll(struct node**, int);  // Debug

struct node** insertionSort(struct node** arr, int size) {
    if (size < 2) return arr;
    int temp = 0;
    struct node* swap;
    for (int i = 1; i < size; i++) {
        temp = i;
        while (strcmp(arr[temp-1]->vertex, arr[temp]->vertex) > 0) {
            swap = arr[temp];
            arr[temp] = arr[temp-1];
            arr[temp-1] = swap;
            temp--;
            if (temp == 0) break;
        }
    } 
    return arr;
}

// Copied from first
struct node** buildArray(FILE* fp, int size) {

    // Constructs arr to store LLs
    // Sets each array element to vertex
    struct node** arr = malloc(size * sizeof(struct node*));
    char v[20];
    // Leading heads do not recieve weight
    for (int i = 0; i < size; i++) {
        fscanf(fp, "%s", v);
        arr[i] = allocate(v);
        for (int j = 0; j < i; j++) {
        }
    }
    return arr;
}

void add(struct node** arr, char* v1, char* v2, int size, int wt) {

    // Find correct position to insert v2 onto v1
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v2) == 0) arr[i]->weight++;
        if (strcmp(arr[i]->vertex, v1) == 0) {
            arr[i] = append(arr[i], v2, wt);
        }
        // Previous two conditions fulfilled
        // Was going to make this more efficient but unable due to time constraints
    }

}

// Modified from first
struct node* append(struct node* head, char* v, int wt) {
    struct node* ptr = head->next; // Skip leading head
    struct node* prev = head;

    // If head was only the leading node
    if (ptr == 0) {
        head->next = allocate(v);
        head->next->weight = wt;
        return head;
    }

    // Check if it can be inserted to the middle
    while (ptr != 0) {
        // Find if pointer is larger than v
        if (strcmp(ptr->vertex, v) > 0) {
            struct node* temp = allocate(v);
            temp->weight = wt;
            temp->next = ptr;
            prev->next = temp;
            return head;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    // Add to end
    prev->next = allocate(v);
    prev->next->weight = wt;
    return head;
}

// Modified from first
struct node* allocate(char* v) {
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, v);
    temp->weight = 0;
    temp->next = 0;
    return temp;
}

// Copied from first
void freeAll(struct node** arr, int size) {
    struct node* ptr;
    struct node* prev = 0;
    for (int i = 0; i < size; i++) {
        ptr = arr[i];
        while (ptr != 0) {
            prev = ptr;
            ptr = ptr->next;
            free(prev);
        }
        prev = 0;
    }
    free(arr);
}

// Copied from first
void adjacencies(struct node** arr, char* v, int size) {

    // Find the vertex in arr
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v) == 0) {
            // Traverse and print vertex
            struct node* ptr = arr[i]->next;
            while (ptr != 0) {
                printf("%s ", ptr->vertex);
                ptr = ptr->next;
            }
            printf("\n");
        }
    }
}

void inDegree(struct node** arr, char*v, int size) {
    
    // Leading head's weight is used as in degree
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v) == 0) {
            printf("%d\n", arr[i]->weight);
            break;
        }
    }
}

void outDegree(struct node** arr, char*v, int size) {
    struct node* ptr;
    // Locate where v head is
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v) == 0) {
            ptr = arr[i]->next;
            break;
        }
    }

    // Count number of nodes after head
    int count = 0;
    while (ptr != 0) {
        count++;
        ptr = ptr->next;
    }
    
    printf("%d\n", count);
}

// Debug
void printAll(struct node** arr, int size) {
    printf("START PRINT\n");
    for (int i = 0; i < size; i++) {
        struct node* ptr = arr[i];
        while (ptr != 0) {
            printf("%s %d ", ptr->vertex, ptr->weight);
            ptr = ptr->next;
        }
        printf("\n");
    }
    printf("END PRINT\n");
}

int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    int size;
    fscanf(fp, "%d\n", &size);

    // Build initial array for following lines to build connections onto
    struct node** arr = buildArray(fp, size);
    arr = insertionSort(arr, size);
    // Build connections
    char *v1 = malloc(20 * sizeof(char)), *v2 = malloc(20 * sizeof(char));
    int wt;
    while (fscanf(fp, "%s %s %d\n", v1, v2, &wt) != EOF) {
        add(arr, v1, v2, size, wt);
    } 
    fclose(fp);

    // Respond to queries
    fp = fopen(argv[2], "r");
    char *op = malloc(20 * sizeof(char)), *v3 = malloc(20 * sizeof(char));
    while (fscanf(fp, "%s %s\n", op, v3) != EOF) {
        if (strcmp(op, "o") == 0) outDegree(arr, v3, size);
        else if (strcmp(op, "i") == 0) inDegree(arr, v3, size);
        else adjacencies(arr, v3, size);
    }

    free(v1);
    free(v2);
    free(v3);
    free(op);
    freeAll(arr, size);
    return EXIT_SUCCESS;
}