#define concat(x,y)  x ## y

concat(foo, __LINE__)

#define STRING2(x) #x
#define STRING(x) STRING2(x)
concat(foo, __LINE__)

STRING(__LINE__)
