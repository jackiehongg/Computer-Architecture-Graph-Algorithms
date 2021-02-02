#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
    char* vertex;
    struct node* next;
};

struct node* allocate(char* v) {
    struct node* temp = malloc(sizeof(struct node));
    temp->vertex = v;
    temp->next = 0;
    return temp;
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
        for (int i = 0; i < 5; i++) printf("%s ", arr[i]->vertex);
        printf("\n");

    } 

    return arr;
}

void increment(int* arr) {
    for (int i = 0; i < 5; i++) {
        arr[i]++;
    }
    printf("%p", arr);
}

void incTop(int* ptr) {
    *ptr += 1;
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



int main(int argc, char* argv[]) {
    struct node* node1 = allocate("A");
    struct node* node2 = allocate("B");
    struct node* node3 = allocate("C");
    struct node* node4 = allocate("D");
    struct node* node5 = allocate("E");

    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;
    // printf("%s\n", node5->vertex);
    for (struct node* ptr = node1; ptr != 0; ptr = ptr->next) printf("%s\n", ptr->vertex);
    node1 = removeNode(node1, "E");
    node1 = removeNode(node1, "D");
    node1 = removeNode(node1, "E");

    for (struct node* ptr = node1; ptr != 0; ptr = ptr->next) printf("%s\n", ptr->vertex);

    return EXIT_SUCCESS;
}


