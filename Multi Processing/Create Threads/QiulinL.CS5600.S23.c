#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int array[1000] = {0};

void *worker(void *data)
{
    char *name = (char*)data;

    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        printf("Hi from thread name = %s\n", name);
    }

    printf("Thread %s done!\n", name);
    return NULL;
}

void *worker2(void *data)
{
    char *name= (char*)data;
    for (int i = 0; i < sizeof(array); i++) {
        array[i] = i;
    }
    printf("Thread %s done!\n", name);
    return NULL;
}

void *worker3(void *data)
{
    char *name = (char*)data;
    for(int i = 0; i < sizeof(array); i++) {
        printf("array[%d] is %d\n", i, array[i]);
    }
    printf("Thread %s done!\n", name);
    return NULL;
}

int main(void)
{
    // The first example
    printf("This is the first example for threads th1 and th2:\n\n");
    sleep(2);
    pthread_t th1, th2;
    pthread_create(&th1, NULL, worker, "th1");
    pthread_create(&th2, NULL, worker, "th2");
    sleep(5);
    printf("Exiting from main program\n");
    
    sleep(2);
    // The second example
    printf("\n\nThis is the second example for threads th3 and th4:\n\n");
    sleep(2);
    pthread_t th3, th4;
    pthread_create(&th3, NULL, worker, "th3");
    pthread_create(&th4, NULL, worker, "th4");
    sleep(5);
    printf("Exiting from main program\n");
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);
    
    sleep(2);
    // The third example
    printf("\n\nThis is the third example for threads th5 and th6:\n\n");
    sleep(2);
    pthread_t th5, th6;
    printf("th5 keeps setting the index number to all elements, th6 keeps reading the array's .\n");
    sleep(2);
    pthread_create(&th6, NULL, worker3, "th6");
    pthread_create(&th5, NULL, worker2, "th5");
    //pthread_create(&th6, NULL, worker3, "th6");
    usleep(10000);
    pthread_cancel(th5);
    printf("====> Cancelling Thread th5!!\n");
    pthread_cancel(th6);
    printf("====> Cancelling Thread th6!\n");
    return 0;
}