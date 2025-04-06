unsigned int strlen(char* string) {
  unsigned int i = 0;
  while (string[i] != '\0') {
    i++;
  }
  return i;
}
