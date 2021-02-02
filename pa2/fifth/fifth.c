#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

struct node {
    char vertex[20];
    int weight;
    struct node* next;
};

struct stack {
    struct node* top;
    int count;
};

struct node* pop(struct stack*);
void push(struct stack*, struct node*);
int isEmpty(struct stack*);
struct stack* allocateStack();
struct node** buildArray(FILE*, int);   // Build array to store linked lists
void add(struct node**, char*, char*, int, int);
struct node* append(struct node* head, char* v, int);    // Add helper method
struct node* allocate(char*);
void freeAll(struct node**, int);
void inDegree(struct node**, char*, int);   // Respond to Queries
void outDegree(struct node**, char*, int);
void adjacencies(struct node**, char*, int);
void printAll(struct node**, int);  // Debug
struct node* dfs(struct node**, struct node*, struct node*, int);
struct node* appendList(struct node* , struct node*);


void push(struct stack* s, struct node* temp) {
    if (isEmpty(s) == 1) {
        s->top = temp;
        s->count++;
    } else {
        temp->next = s->top;
        s->top = temp;
        s->count++;
    }
}

struct node* pop(struct stack* s) {
    if (isEmpty(s) == 1) { 
        return 0;
    } else {
        struct node* temp = s->top;
        s->top = s->top->next;
        return temp;
    }
}

int isEmpty(struct stack* s) {
    if (s->count == 0) return 1;
    else return 0;
}

struct stack* allocateStack() {
    struct stack* temp = malloc(sizeof(struct stack));
    temp->top = 0;
    temp->count = 0;
    return temp;
}

// Insertion Sort
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

    // Insertion sort 
    return arr;
}

void add(struct node** arr, char* v1, char* v2, int size, int wt) {

    // Find correct position to insert v2 onto v1
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, v1) == 0) {
            arr[i] = append(arr[i], v2, wt);
            break;
        }

    }

}

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

struct node* allocate(char* v) {
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->vertex, v);
    temp->weight = 0;
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

struct node* appendList(struct node* head, struct node* temp) {

    // First insert
    if (head == 0) return temp;

    struct node* ptr = head;
    while (ptr->next != 0) {
        ptr = ptr->next;
    }
    ptr->next = temp;
    return head;
}

struct node* removeNode(struct node* head, char* target) {
    struct node *prev = head;
    // If first node
    if (strcmp(prev->vertex, target) == 0) {
        prev->next = head->next;
        free(head);
        return prev;
    }

    // Not first node
    for (struct node* ptr = head->next; ptr != 0; ptr = ptr->next) {
        if (strcmp(ptr->vertex, target) == 0) {
            prev->next = ptr->next;
            free(ptr);
            return head;
        }
        prev = ptr;
    }

    // Not found
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

// V2 RETURNS WITHOUT HEAD
struct node* seek(struct node** arr, char* target, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, target) == 0) {
            return arr[i]->next;
        }
    }
    return 0;
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

void visit(struct node** arr, struct node* node, struct node** pPerm, 
    struct node** pTemp, struct node** pList, int size, int* pCycle) {
    if (isDisc(*pPerm, node->vertex) == 1 || *pCycle == 1) return;
    if (isDisc(*pTemp, node->vertex) == 1) {
        *pCycle += 1;
        return;
    }
    *pTemp = appendList(*pTemp, allocate(node->vertex));

    for (struct node* ptr = seek(arr, node->vertex, size); ptr != 0; ptr = ptr->next) {
        visit(arr, ptr, pPerm, pTemp, pList, size, pCycle);
    }
    *pTemp = removeNode(*pTemp, node->vertex);
    *pPerm = appendList(*pPerm, allocate(node->vertex));
    struct node* temp = allocate(node->vertex);
    if (*pList == 0) {
        *pList = temp;
    } else {
        temp->next = *pList;
        *pList = temp;
    }
}

int getIndex(struct node** arr, int size, char* target) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i]->vertex, target) == 0) {
            return i;
        }
    }
    return -1;
}

int getWeight(struct node** arr, int size, char* src, char* dst) {
    for (int i = 0; i < size; i++) {

        // Find source vertex
        if (strcmp(arr[i]->vertex, src) == 0) {
            for (struct node* ptr = arr[i]; ptr != 0; ptr = ptr->next) {
                if (strcmp(ptr->vertex, dst) == 0) {
                    return ptr->weight;
                }
            }
        }
    }
    return -1;
}

void shortestPath(struct node** arr, int size, char* source, struct node** pList) {
    int* distance = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        distance[i] = INT_MAX;
        // Source vertex
        if (strcmp(source, arr[i]->vertex) == 0) distance[i] = 0;
    }


    for (struct node* u = *pList; u != 0; u = u->next) {
        for (struct node* v = seek(arr,  u->vertex, size); v != 0; v = v->next) {
            if (distance[getIndex(arr, size, v->vertex)] > distance[getIndex(arr, size, u->vertex)] + getWeight(arr, size, u->vertex, v->vertex)) {
                if (distance[getIndex(arr, size, u->vertex)] == INT_MAX) continue;
                distance[getIndex(arr, size, v->vertex)] = distance[getIndex(arr, size, u->vertex)] + getWeight(arr, size, u->vertex, v->vertex);
            }
        }
    }
    for (int i = 0; i < size; i++) {
        if (distance[i] == INT_MAX) printf("%s INF\n", arr[i]->vertex);
        else printf("%s %d\n", arr[i]->vertex, distance[i]);
    }
    free(distance);
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

    // Top sort
    int cycle = 0;
    int* pCycle = &cycle;
    struct node* perm = 0, *temp = 0, *list = 0;
    struct node** pPerm = &perm, **pTemp = &temp, **pList = &list;
    for (int i = 0; i < size; i++) {
        if (isDisc(perm, arr[i]->vertex) == 0) {
            visit(arr, arr[i], pPerm, pTemp, pList, size, pCycle);
        }
    }
    if (cycle > 0) {
        printf("CYCLE\n");
        return EXIT_SUCCESS;
    }

    // Respond to queries
    fp = fopen(argv[2], "r");
    char* v3 = malloc(20 * sizeof(char));
    while (fscanf(fp, "%s\n", v3) != EOF) {
        printf("\n");
        shortestPath(arr, size, v3, pList);
    }




    free(v1);
    free(v2);
    freeList(*pList);
    freeList(*pPerm);
    freeAll(arr, size);
    return EXIT_SUCCESS;
}

