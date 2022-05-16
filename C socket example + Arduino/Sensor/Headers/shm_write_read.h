#ifndef SHM_WRITE_READ_H
#define SHM_WRITE_READ_H

void shm_write(int message, const int SIZE, const char* name);

char* shm_read(const int SIZE, const char* name);

#endif