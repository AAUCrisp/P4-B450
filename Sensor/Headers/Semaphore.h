#ifndef SEMAPHORE
#define SEMAPHORE

static const char *SemLockWrite = "SemLockWrite";
static const char *SemLockRead = "SemLockRead";

static sem_t *SemRead;
static sem_t *SemWrite;

#endif