#ifndef SHM_WRITE_READ_H
#define SHM_WRITE_READ_H

void* shm_create_writer(const int SIZE, const char* name);

char* shm_read(const int SIZE, const char* name);

#endif