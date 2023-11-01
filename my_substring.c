#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024

int total = 0;              
int n1, n2;                 
char *s1, *s2;              
FILE *fp;                   
pthread_mutex_t lock;  

// Function to find the minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Function to read input strings from the "strings.txt" file
int readf() {
    // Attempt to open and read the input file
    if ((fp = fopen("strings.txt", "r")) == NULL) {
        printf("ERROR: can't open string.txt!\n");
        return 0;
    }
    // Allocate memory for the first string
    s1 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL) {
        printf("ERROR: Out of memory!\n");
        return -1;
    }
    // Allocate memory for the second string
    s2 = (char *)malloc(sizeof(char) * MAX);
    if (s2 == NULL) {
        printf("ERROR: Out of memory\n");
        return -1;
    }
    // Read the first and second strings from the file
    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);

    // Handle errors if the strings couldn't be read
    if (s1 == NULL || s2 == NULL) {
        if (s1 == NULL) free(s1);
        if (s2 == NULL) free(s2);
        fclose(fp);
        return -1;
    }

    // Calculate the lengths of the strings
    n1 = strlen(s1) - 1;
    n2 = strlen(s2) - 1;

    // Check if the first string is shorter than the second
    if (n1 < n2) {
        free(s1);
        free(s2);
        fclose(fp);
        return -1;
    }
    return 1;
}

// Function for substring search performed by each thread
void *substring_search(void *arg) {
    // Extract the thread ID
    int thread_id = *((int *)arg); 
    // Divide the search space into four equal chunks 
    int chunk_size = n1 / 4;        
    // Define the starting index for this thread's chunk
    int start = thread_id * chunk_size; 
    int end = (thread_id == 3) ? n1 : min(start + chunk_size + n2 - 1, n1);
    // Define the ending index for this thread's chunk, ensuring it doesn't go out of bounds

    // Count of substrings found by this thread
    int local_count = 0;  

    // Loop through the chunk of the string assigned to this thread
    for (int i = start; i <= (end - n2); i++) {
        int count = 0;
        // Check for substring matches
        for (int j = i, k = 0; k < n2; j++, k++) {
            if (s1[j] != s2[k]) {
                break;  
            } else {
                // Increment the count if characters match
                count++;  
            }
            if (count == n2) {
                // If a full substring match is found, increment local count
                local_count++;  
            }
        }
    }

    // Lock the mutex to update the shared 'total' count
    pthread_mutex_lock(&lock);
    // Update the shared total count with local count
    total += local_count; 
    // Release the lock
    pthread_mutex_unlock(&lock); 
    // Exit the thread
    pthread_exit(NULL);  
}

int main(int argc, char *argv[]) {
    // Read and process the input strings
    int retVal = readf(fp);
    if (retVal == -1) {
        printf("Error reading from file or processing strings.\n");
        return 1;
    }
    // Create an array to hold thread IDs
    pthread_t threads[4];
    // Initialize the mutex for thread synchronization
    pthread_mutex_init(&lock, NULL);  

    // Create an array to hold thread IDs
    int thread_ids[4];  
    for (int i = 0; i < 4; i++) {
        // Assign unique IDs to each thread
        thread_ids[i] = i;  
        // Create four threads, each responsible for searching a portion of the string
        pthread_create(&threads[i], NULL, substring_search, &thread_ids[i]);
    }

    for (int i = 0; i < 4; i++) {
        // Wait for all threads to finish
        pthread_join(threads[i], NULL);
    }

    printf("The number of substrings is: %d\n", total);
     // Clean up the mutex
    pthread_mutex_destroy(&lock); 
    // Free allocated memory for s1
    free(s1); 
    // Free allocated memory for s2
    free(s2); 
    // Close the file
    fclose(fp);
    return 0;
}

