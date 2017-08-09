/* http://www.qnx.com/developers/docs/660/index.jsp?topic=%2Fcom.qnx.doc.neutrino.getting_started%2Ftopic%2Fs1_procs_condvar.html
 */

/*
 * cp1.c
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "unit.h"

int count = 0;
int data_ready = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar;

extern unsigned int msleep(unsigned int msec);

void *consumer(void *notused)
{
    (void) notused;

    printk("In consumer thread...\n");
    while (1) {
        count++;
        pthread_mutex_lock(&mutex);
        while (!data_ready) {
            pthread_cond_wait(&condvar, &mutex);
        }
        // process data
        printk("consumer:  got data from producer\n");
        data_ready = 0;
        pthread_cond_signal(&condvar);
        pthread_mutex_unlock(&mutex);
    }
}

void *producer(void *notused)
{
    (void) notused;

    printk("In producer thread...\n");
    while (1) {
        // get data from hardware
        // we'll simulate this with a sleep (1)
        msleep(180);
        printk("producer:  got data from h/w\n");
        pthread_mutex_lock(&mutex);
        while (data_ready) {
            pthread_cond_wait(&condvar, &mutex);
        }
        data_ready = 1;
        pthread_cond_signal(&condvar);
        pthread_mutex_unlock(&mutex);
    }
}

int main(void)
{
    printk("Starting consumer/producer example...\n");

    pthread_cond_init(&condvar, NULL);

    // create the producer and consumer threads
    pthread_create(NULL, NULL, producer, NULL);
    pthread_create(NULL, NULL, consumer, NULL);

    while (count < 5)
        pthread_yield();
    printk("Bye-bye!\n");

    TEST_EXIT(0);
}
