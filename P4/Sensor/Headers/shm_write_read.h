#ifndef SHM_WRITE_READ_H
#define SHM_WRITE_READ_H

/* Semaphore Names */
#define SEM_READ_FNAME "../SEM_READ"
#define SEM_WRITE_FNAME "../SEM_WRITE"

void shm_write(const char *message, const int SIZE, const char* name);

void* shm_read(const int SIZE, const char* name);

#endif