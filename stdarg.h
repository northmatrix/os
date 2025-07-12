typedef char* va_list;

#define _VA_ALIGN(t) (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, last_arg) (ap = (va_list)(&(last_arg) + 1))
#define va_arg(ap, type) (*(type *)((ap += _VA_ALIGN(type)) - _VA_ALIGN(type)))
#define va_end(ap) (ap = (va_list)0)
