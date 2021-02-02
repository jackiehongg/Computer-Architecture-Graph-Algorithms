#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

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
struct node** insertionSort(struct node**, int);

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

int getIndex(struct node** arr, int size, char* target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == 0) continue;
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

    // If only node
    if (strcmp(prev->vertex, target) == 0) {
        if (head->next == 0) {
        free(head);
        return 0;
        }
    }

    // If first node
    if (strcmp(prev->vertex, target) == 0) {
        prev = prev->next;
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

int getMin(struct node** arr, int* distance, int size, int threshold, struct node* vertexSet) {
    int num  = INT_MAX, index = -1;
    for (int i = 0; i < size; i++) {
        if (distance[i] < num && isDisc(vertexSet, arr[i]->vertex)) {
            num = distance[i];
            index = i;
        }
    }
    return index;
}

// Follows pseudocode from wiki
void djikstra(struct node** arr, int size, char* src) {
    int* distance = malloc(size * sizeof(int));
    struct node** prev = malloc(size * sizeof(struct node));
    struct node* vertexSet = 0;
    int index = 0;
    for (int i = 0; i < size; i++) {
        distance[i] = INT_MAX;
        prev[i] = 0;
        // vertexSet = appendList(vertexSet, allocate(arr[i]->vertex));
        if (strcmp(arr[i]->vertex, src) == 0) {
            distance[i] = 0;
            index = i;
            vertexSet = appendList(vertexSet, allocate(arr[i]->vertex));
        }
    }


    struct node* u = 0;
    int alt = 0;
    int threshold = 0;
    while (vertexSet != 0) {
        // getMin locates the minimum index of distance just greater than the current threshold.
        // The threshold is in place to retrieve the next smallest number to avoid infinite looping
        int minimum = getMin(arr, distance, size, threshold, vertexSet);
        index = minimum > 0 ? minimum: index;   // Take source index found when zeroing distances above
        threshold = distance[index];
        u = arr[index];
        vertexSet = removeNode(vertexSet, u->vertex);
        // This loop is only to add vertices that we can currently access.
        // Without this, starting on a terminating node will lead to an infinite loop
        for (struct node* ptr = u->next; ptr != 0; ptr = ptr->next) {
            // Add only if not in vertexSet already and not discovered
            if (isDisc(vertexSet, ptr->vertex) == 0 && distance[getIndex(arr, size, ptr->vertex)] == INT_MAX) {
                vertexSet = appendList(vertexSet, allocate(ptr->vertex));
            }
        }
        for (struct node* v = u->next; v != 0; v = v->next) {
            // Only for v that are still in vertexSet
            if (isDisc(vertexSet, v->vertex) == 0) continue;
            alt = distance[getIndex(arr, size, u->vertex)] + getWeight(arr, size, u->vertex, v->vertex);
            if (alt < distance[getIndex(arr, size, v->vertex)]) {
                distance[getIndex(arr, size, v->vertex)] = alt;
                prev[getIndex(arr, size, v->vertex)] = allocate(v->vertex);
            }
        }
    }

    for (int i = 0; i < size; i++) {
        int dist = distance[i];
        if (dist == INT_MAX) printf("%s %s\n", arr[i]->vertex, "INF");
        else printf("%s %d\n", arr[i]->vertex, distance[i]);
    }
    freeAll(prev, size);
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

    // Respond to queries
    fp = fopen(argv[2], "r");
    char *src = malloc(20 * sizeof(char));
    while (fscanf(fp, "%s\n", src) != EOF) {
        djikstra(arr, size, src);
        printf("\n");
    }

    free(v1);
    free(v2);
    free(src);
    freeAll(arr, size);
    return EXIT_SUCCESS;
}