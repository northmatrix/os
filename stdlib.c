int atoi(const char* str) {
  int result = 0;
  int sign = 0;
  while(*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') {
      str++;
  }
  if(*str == '-') {
    sign = -1;
    str++;
  }
  else if(*str == '+') {
    str++;
  }
  
  while(*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return sign * result;
}


void itoa(unsigned int num, char* str, int base) {
  int i = 0;
  int isNegative = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  if ((int)num < 0 && base == 10) {
    isNegative = 1;
    num = (unsigned int)(-(int)num);
  }

  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10 + 'a') : (rem + '0');
    num /= base;
  }

  if (isNegative) str[i++] = '-';

  str[i] = '\0';

  for (int start = 0, end = i - 1; start < end; start++, end--) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
  }
}
