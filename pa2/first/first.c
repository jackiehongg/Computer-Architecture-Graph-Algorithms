#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
    char vertex[8];
    struct node* next;
};

struct node** buildArray(FILE*, int); // Build array to store linked lists
void add(struct node**, char*, char*, int);  // Main build adjacencies
struct node* append(struct node*, char*);   // Helper for add method
void degree(struct node**, char*, int); // Print degree 
void adjacencies(struct node**, char*, int);    // Print adjacencies
struct node* allocate(char*);
void freeAll(struct node**, int);
void printAll(struct node**, int); // Debug

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

struct node** buildArray(FILE* fp, int size) {
    // Constructs arr to store LLs
    // Sets each array element to vertex
    struct node** arr = malloc(size * sizeof(struct node*));
    char v[5];
    for (int i = 0; i < size; i++) {
        fscanf(fp, "%s", v);
        arr[i] = allocate(v);
        for (int j = 0; j < i; j++) {
        }
    }
    return arr;
}

struct node* allocate(char* v) {
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, v);
    temp->next = 0;
    return temp;
}

void add(struct node** arr, char* v1, char* v2, int size) {
    // Find correct index for v1, then append v2
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v1) == 0) {
            arr[i] = append(arr[i], v2);
            break;
        }
    }

    // Find correct index for v2, then append v1
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v2) == 0) {
            arr[i] = append(arr[i], v1);
            break;
        }
    }
}

struct node* append(struct node* head, char* v) {
    struct node* ptr = head->next; // Skip leading head
    struct node* prev = head;

    // If head was only the leading node
    if (ptr == 0) {
        head->next = allocate(v);
        return head;
    }

    // Check if it can be inserted to the middle
    while (ptr != 0) {
        // Find if pointer is larger than v
        if (strcmp(ptr->vertex, v) > 0) {
            struct node* temp = allocate(v);
            temp->next = ptr;
            prev->next = temp;
            return head;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    // Add to end
    prev->next = allocate(v);
    return head;
}

void degree(struct node** arr, char* v, int size) {
    int count = 0;
    // Locate the vertex
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v) == 0) {
            // Traverse and count
            struct node* ptr = arr[i];
            while (ptr->next != 0) {
                count++;
                ptr = ptr->next;
            }
            printf("%d\n", count);
            return;
        }
    }
}

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

// For debug purposes
void printAll(struct node** arr, int size) {
    printf("START PRINT\n");
    for (int i = 0; i < size; i++) {
        struct node* ptr = arr[i];
        while (ptr != 0) {
            printf("%s ", ptr->vertex);
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

    // Reads the next x lines after reading the initial integer
    struct node** arr = buildArray(fp, size);
    arr = insertionSort(arr, size);

    // Reads the rest of the file
    char *v1 = malloc(8 * sizeof(char)), *v2 = malloc(8 * sizeof(char));
    while (fscanf(fp, "%s %s\n", v1, v2) != EOF) {
        add(arr, v1, v2, size);
    }
    fclose(fp);
    
    // Responds to queries
    fp = fopen(argv[2], "r");
    char *op = malloc(8 * sizeof(char)), *start = malloc(8 * sizeof(char));
    while (fscanf(fp, "%s %s\n", op, start) != EOF) {
        if (strcmp(op, "d") == 0) degree(arr, start, size);
        else adjacencies(arr, start, size);
    }

    free(v1);
    free(v2);
    free(op);
    free(start);
    fclose(fp);
    freeAll(arr, size);


    return EXIT_SUCCESS;
}

/* 
Undirected Graph

FORMAT:
Build graph:
5
A
B
C
D
E
A B
A C
...

Queries:
"d" degree
"a" adjacencies
d A
a C

Linked List representation

 */


