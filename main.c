#include <stdio.h>
#include <stdlib.h>

// set how many processes
#define MAX_PROCESSES 10

typedef struct Node {
    int id;
    struct Node* next;
} Node;

typedef struct PCB {
    int parent;
    Node* children;
} PCB;

PCB* process;

// case 1
void initialize_process_hierarchy() {
    process = (PCB*) malloc(MAX_PROCESSES * sizeof(PCB));
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process[i].parent = -1;
        process[i].children = NULL;
    }
}

void create_child(int p) {
    // allocate memory for an unused PCB[q]
    int q;
    for (q = 0; q < MAX_PROCESSES; q++) {
        if (process[q].parent == -1) {
            break;
        }
    }
    
    // record the parent's index, p, in PCB[q]
    process[q].parent = p;
    
    // initialize the list of children of PCB[q] as empty (NULL)
    process[q].children = NULL;
    
    // create a new link containing the child's index q and append the link to the children field of process[p]
    if (q != 0) {
        Node* new_child = (Node*) malloc(sizeof(Node));
        new_child->id = q;
        new_child->next = process[p].children;
        process[p].children = new_child;
    }
}

void destroy_descendants(int p) {
    // base case: if process[p] does not exist
    if (process[p].parent == -1) {
        return;
    }
    
    // iterate through the list of children of process[p]
    Node* current = process[p].children;
    while (current != NULL) {
        // recursively destroy all descendants of the current child process
        destroy_descendants(current->id);
        
        // free memory utilized by PCB[q] and set it to NULL
        process[current->id].parent = -1;
        process[current->id].children = NULL;
        
        // free memory utilized by the node with id q and set it to NULL
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    
    // set the list of children of process[p] to NULL
    process[p].children = NULL;
}

void print_process() {
    
    printf("Process list:\n");
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        // if process[i] exist
        if (process[i].parent != -1) {
            printf("\nProcess id: %d\n", i);
            
            // print parent of process[i]
            if (process[i].parent == i)
                printf("    No parent process\n");
            else
                printf("    Parent process: %d\n", process[i].parent);
            
            // print children of process[i]
            Node* current = process[i].children;
            if (current == NULL)
                printf("    No child processes\n");
            else {
                while (current != NULL) {
                    printf("    Child processes: %d\n", current->id);
                    current = current->next;
                }
            }
        }
    }
}

int main() {
    int choice;
    while (1) {
        
        // print menu
        printf("\nProcess creation and destruction\n");
        printf("--------------------------------\n");
        printf("1) Initialize process hierarchy\n");
        printf("2) Create a new child process\n");
        printf("3) Destroy all descendants of a process\n");
        printf("4) Quit program and free memory\n");
        printf("Enter selection: ");
        scanf("%d", &choice);
        
        // 4 cases
        switch (choice) {
            case 1:
                initialize_process_hierarchy();
                create_child(0);
                print_process();
                break;
            case 2:
                {
                    int p;
                    printf("Enter the parent process id: ");
                    scanf("%d", &p);
                    if (process[p].parent != -1) {
                        create_child(p);
                        print_process();
                    } else {
                        printf("\nParent process does not exist.\n");
                    }
                    break;
                }
            case 3:
                {
                    int p;
                    printf("Enter the parent process id: ");
                    scanf("%d", &p);
                    if (process[p].parent != -1) {
                        destroy_descendants(p);
                        print_process();
                    } else {
                        printf("\nParent process does not exist.\n");
                    }
                    break;
                }
            case 4:
                {
                    for (int i = 0; i < MAX_PROCESSES; i++) {
                        if (process[i].parent != -1) {
                            destroy_descendants(i);
                        }
                    }
                    free(process);
                    return 0;
                }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}