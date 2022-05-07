#ifndef SHM_WRITE_READ_H
#define SHM_WRITE_READ_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>  //IPC thing
#include <sys/shm.h>  //SHM thing
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void shm_write(void* message, const int SIZE, const char* name);

char* shm_read(const int SIZE, const char* name);

#endif