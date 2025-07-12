#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

unsigned int strlen(const char* data);
void *memcpy(void *dest, const void *src, unsigned long n);
void *memmove(void *dest, const void *src, unsigned long n);
void *memset(void *s, int c, unsigned long n);
int memcmp(const void *s1, const void *s2, unsigned long n);
void abort(void);
#endif
