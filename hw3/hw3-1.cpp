#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

sem_t semaphore;

void count(int index) {
    
    int num = 1000000;
    while (num--) {}
    cout << "I'm thread " << index << ", local count: 1000000\n";
    sem_post(&semaphore);
}

int main(void) {

    sem_init(&semaphore, 0, 1);

    thread t[100];

    for (int i = 0; i < 100; i++) {
        sem_wait(&semaphore);
        t[i] = thread(count, i);
    }

    for (int i = 0; i < 100; i++)
        t[i].join();

    sem_destroy(&semaphore);

    return 0;
}