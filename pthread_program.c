#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 12

char buffer[BUFFER_SIZE];
// Index to insert a character into the buffer
int in = 0;  
// Index to remove a character from the buffer    
int out = 0;  
// Number of characters currently in the buffer   
int count = 0;   

// Mutex for synchronizing access to the buffer
pthread_mutex_t mutex;     
// Condition variables for signaling buffer status
pthread_cond_t empty, full; 

// Producer function to read characters from a file and add them to the buffer
void *producer(void *arg) {
    FILE *file = fopen("message.txt", "r");
    if (file == NULL) {
        perror("Error opening message.txt");
        exit(1);
    }

    char ch;
    while (1) {
        ch = fgetc(file);

        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            // Wait if the buffer is full
            pthread_cond_wait(&empty, &mutex);
        }
        // Add a character to the buffer
        buffer[in] = ch;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        // Signal that the buffer is not empty
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);

        if (ch == EOF) {
            // End of file, stop producing
            break; 
        }
    }

    fclose(file);
    return NULL;
}

// Consumer function to remove characters from the buffer and print them
void *consumer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            // Wait if the buffer is empty
            pthread_cond_wait(&full, &mutex);
        }
        // Remove a character from the buffer
        char ch = buffer[out];    
        out = (out + 1) % BUFFER_SIZE;
        count--;

        // Signal that the buffer is not full
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

        if (ch == EOF) {
            // End of file, stop consuming
            break;
        }
        // Print the consumed character
        printf("%c", ch);   
        fflush(stdout);
    }

    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);

    return 0;
}

