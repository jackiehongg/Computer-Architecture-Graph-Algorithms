#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
    char vertex[20];
    struct node* next;
};

struct queue {
    struct node *front, *tail;
};

struct queue* allocateQ();
void EnQ(struct queue*, struct node*);
struct node* DeQ(struct queue*);
struct node** buildArray(FILE*, int); // Build array to store linked lists
void add(struct node**, char*, char*, int);  // Main build adjacencies
struct node* append(struct node*, char*);   // Helper for add method
struct node* appendList(struct node* , struct node*);
int isDisc(struct node*, char*);
struct node* seek(struct node**, char*, int);
struct node* bfs(struct node**, char*, int);
struct node* allocate(char*);
void freeAll(struct node**, int);
void freeList(struct node*);
void printAll(struct node**, int); // Debug
void printList(struct node*); // Debug
int isDisc(struct node*, char*);

struct queue* allocateQ() {
    struct queue* q = malloc(sizeof(struct queue));
    q->front = 0;
    q->tail = 0;
    return q;
}

void EnQ(struct queue* q, struct node* temp) {
    
    // First enqueue
    if (q->tail == 0) {
        q->front = temp;
        q->tail = temp;
        return;
    }

    // Not the first enqueue
    q->tail->next = temp;
    q->tail = temp;
}

struct node* DeQ(struct queue* q) {

    // Queue is empty
    if (q->front == 0) return 0;

    struct node* temp = q->front;
    q->front = q->front->next;

    // Dequeue'd last node
    if (q->front == 0) q->tail = 0;

    temp->next = 0;
    return temp;
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
        if (strcmp(ptr->vertex, v) > 0) {
            struct node* temp = allocate(v);
            temp->next = ptr;
            prev->next = temp;
            return head;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    prev->next = allocate(v);
    return head;
}

struct node* appendList(struct node* head, struct node* temp) {

    // First insert
    struct node* ptr = head;
    while (ptr->next != 0) {
        ptr = ptr->next;
    }
    ptr->next = temp;
    return head;
}


int isDisc(struct node* head, char* target) {
    struct node* ptr = head;
    while (ptr != 0) {
        // If target found
        if (strcmp(ptr->vertex, target) == 0) {
            return 1;
        }
        ptr = ptr->next;
    }

    // Not found
    return 0;
}

// Returns a list of adjacencies without the leading head
struct node* seek(struct node** arr, char* target, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, target) == 0) {
            return arr[i]->next;    // Don't include leading head
        }
    }
    return 0;
}


struct node* bfs(struct node** arr, char* start, int size) {
    struct node* visited = 0;
    struct node* lead = 0;
    struct node* v = 0;
    struct queue* q = allocateQ();
    // Find starting node
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, start) == 0) {
            lead = arr[i];
            break;
        }
    }

    // Q should only recieve a single list item
    visited = allocate(lead->vertex);
    EnQ(q, allocate(visited->vertex));

    // BFS
    while (q->front != 0) {
        v = DeQ(q);
        for (struct node* ptr = seek(arr, v->vertex, size); ptr != 0; ptr = ptr->next) {
            if (isDisc(visited, ptr->vertex) == 0) {
                visited = appendList(visited, allocate(ptr->vertex));
                EnQ(q, allocate(ptr->vertex));
            }
        }
        free(v);
    }
    free(q);
    return visited;
}

struct node* allocate(char* v) {
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, v);

    temp->next = 0;
    return temp;
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

void freeList(struct node* head) {
    struct node* ptr = head->next;
    struct node* prev = head;
    while (ptr != 0) {
        free(prev);
        prev = ptr;
        ptr = ptr->next;
    }
    free(prev);
}

// Debug
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

// Debug
void printList(struct node* head) {
    if (head == 0) printf("NULL");
    struct node* ptr = head;
    while (ptr != 0) {
        printf("%s ", ptr->vertex);
        ptr = ptr->next;
    }
    printf("\n");
}


int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    int size = 0;
    fscanf(fp, "%d\n", &size);

    // Builds initial array
    struct node** arr = buildArray(fp, size);
    arr = insertionSort(arr, size);
    // Builds connections
    char *v1 = malloc(20 * sizeof(char)), *v2 = malloc(20 * sizeof(char));
    while (fscanf(fp, "%s %s\n", v1, v2) != EOF) {
        add(arr, v1, v2, size);
    }
    fclose(fp);

    // Responds to queries
    struct node* temp = 0;
    fp = fopen(argv[2], "r");
    char *v = malloc(20 *sizeof(char));
    while (fscanf(fp, "%s\n", v) != EOF) {
        temp = bfs(arr, v, size);
        printList(temp);
        freeList(temp);
    }

    free(v);
    free(v1);
    free(v2);
    freeAll(arr, size);
    return EXIT_SUCCESS;
}