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

void itoa(int num,char* str,int base) {
  int i = 0;
  int isNegative = 0;

  if(num == 0) {
     str[i++] =  '0';
     str[i] = '\0';
     return;
  }

  if(num < 0 && base == 10) {
    isNegative = 1;
    num = -num;
  }

  while (num != 0) {
    int rem = num % base;
    if ( rem > 9 ) {
      str[i++] = rem - 10 + 'a';
    }
    else {
      str[i++] = rem + '0'; 
    }
  }

  if(isNegative) {
    str[i++] = '-';
  }

  str[i] = '\0';

  int start = 0;
  int end = i - 1;
  while ( start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}
