unsigned int strlen(char* string) {
  unsigned int i = 0;
  while (string[i] != '\0') {
    i++;
  }
  return i;
}

void *memcpy(void *d, const void *s, unsigned long n) {
    unsigned char *dst = d;
    const unsigned char *src = s;
    while (n--) *dst++ = *src++;
    return d;
}

void *memmove(void *d, const void *s, unsigned long n) {
    unsigned char *dst = d;
    const unsigned char *src = s;
    if (dst < src) while (n--) *dst++ = *src++;
    else {
        dst += n; src += n;
        while (n--) *--dst = *--src;
    }
    return d;
}

void *memset(void *s, int c, unsigned long n) {
    unsigned char *p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

int memcmp(const void *s1, const void *s2, unsigned long n) {
    const unsigned char *a = s1, *b = s2;
    while (n--) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return 0;
}

void abort(void) {
    while (1);
}

