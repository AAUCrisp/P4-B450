#ifndef SHM_WRITE_READ_H
#define SHM_WRITE_READ_H

#include "../Libraries.c"

/* Semaphore Names */
static sem_t SEM_READ;
static sem_t SEM_WRITE;

void shm_write(const char* message, const int SIZE, const char* name);

void* shm_read(const int SIZE, const char* name);

#endif