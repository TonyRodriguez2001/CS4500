#include <stdio.h>           
#include <stdlib.h>          
#include <unistd.h>          
#include <pthread.h>         
#include <sys/time.h>        
#include <sys/param.h>       
#include <sched.h>          

#define K 200 

struct Node
{
    int data;
    struct Node* next;
};

struct list
{
    struct Node *header;
    struct Node *tail;
};

// Mutex lock for synchronization
pthread_mutex_t mutex_lock; 

// Global linked list
struct list *List; 

// Function to bind a thread to a specific CPU core
void bind_thread_to_cpu(int cpuid) {
    cpu_set_t mask;
    // Initialize CPU mask
    CPU_ZERO(&mask); 

    // Set the CPU mask to the specified core
    CPU_SET(cpuid, &mask); 
    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
        // Print an error message if affinity setting fails
        fprintf(stderr, "sched_setaffinity"); 
        // Exit the program with an error code
        exit(EXIT_FAILURE); 
    }
}

// Function to generate a data node
struct Node *generate_data_node() {
    struct Node *ptr;
    // Allocate memory for a new node
    ptr = (struct Node *)malloc(sizeof(struct Node)); 

    if (NULL != ptr) {
        // Initialize the 'next' pointer of the node
        ptr->next = NULL; 
    } else {
        // Print an error message if node allocation fails
        printf("Node allocation failed!\n"); 
    }
    // Return a pointer to the new node
    return ptr; 
}

// Producer thread function
void *producer_thread(void *arg) {
    // Bind this thread to a specific CPU core
    bind_thread_to_cpu(*((int *)arg)); 

    struct Node *ptr, tmp;
    int counter = 0;

    // Allocate memory for a local linked list
    struct list *localList = (struct list *)malloc(sizeof(struct list)); 
    // Initialize the local linked list
    localList->header = localList->tail = NULL; 

    /* Generate and attach K nodes to the local list */
    while (counter < K) {
        // Generate a new data node
        ptr = generate_data_node(); 

        if (NULL != ptr) {
            // Set the data in the node (generating data)
            ptr->data = 1; 
            /* Attach the generated node to the local list */
            if (localList->header == NULL) {
                localList->header = localList->tail = ptr;
            } else {
                localList->tail->next = ptr;
                localList->tail = ptr;
            }
        }
        ++counter;
    }

    while (1) {
        /* Access the critical region and merge the local list into the global list */
        if (!pthread_mutex_trylock(&mutex_lock)) { 
            if (List->tail == NULL) {
                // Merge local list into the global list
                List->header = localList->header; 
                List->tail = localList->tail;
            } else if (localList->header != NULL) {
                List->tail->next = localList->header;
                List->tail = localList->tail;
            }
            // Release the mutex lock
            pthread_mutex_unlock(&mutex_lock); 
            break;
        }
    }
    // Free the thread-local list
    free(localList); 
}

int main(int argc, char *argv[]) {
    int i, num_threads;

    // Number of CPUs
    int NUM_PROCS; 
    // Array to hold CPU core assignments
    int *cpu_array = NULL; 

    struct Node *tmp, *next;
    struct timeval starttime, endtime;

    if (argc == 1) {
        // Print an error message if no command-line argument is provided
        printf("ERROR: please provide an input arg (the number of threads)\n"); 
        // Exit the program with an error code
        exit(1); 
    }
    // Read the number of threads from the command line
    num_threads = atoi(argv[1]); 
    // Create an array of thread IDs
    pthread_t producer[num_threads]; 
    // Get the number of available CPU cores
    NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF); 

    if (NUM_PROCS > 0) {
        // Allocate memory for CPU core array
        cpu_array = (int *)malloc(NUM_PROCS * sizeof(int)); 
        if (cpu_array == NULL) {
            // Print an error message if memory allocation fails
            printf("Allocation failed!\n"); 
            // Exit the program
            exit(0); 
        } else {
            for (i = 0; i < NUM_PROCS; i++)
            // Assign CPU core numbers to the array
                cpu_array[i] = i; 
        }
    }

    // Initialize the mutex lock for synchronization
    pthread_mutex_init(&mutex_lock, NULL); 

    // Allocate memory for the global linked list
    List = (struct list *)malloc(sizeof(struct list)); 
    if (NULL == List) {
        // Print a message if memory allocation fails
        printf("End here\n"); 
        exit(0); 
    }
    // Initialize the global linked list
    List->header = List->tail = NULL; 

    // Get the program start time
    gettimeofday(&starttime, NULL); 
    for (i = 0; i < num_threads; i++) {
        // Create producer threads
        pthread_create(&(producer[i]), NULL, (void *)producer_thread, &cpu_array[i % NUM_PROCS]); 
    }

    for (i = 0; i < num_threads; i++) {
        if (producer[i] != 0) {
            // Wait for producer threads to finish
            pthread_join(producer[i], NULL); 
        }
    }
    // Get the program finish time
    gettimeofday(&endtime, NULL);

    if (List->header != NULL) {
        next = tmp = List->header;
        while (tmp != NULL) {
            next = tmp->next;
            // Free memory of the linked list nodes
            free(tmp); 
            tmp = next;
        }
    }
    if (cpu_array != NULL)
        // Free memory allocated for CPU core array
        free(cpu_array); 

    /* Calculate program runtime */
    printf("Total run time is %ld microseconds.\n", (endtime.tv_sec - starttime.tv_sec) * 1000000 + (endtime.tv_usec - starttime.tv_usec));
    return 0;
}








