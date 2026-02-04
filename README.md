# Scanner-and-Printer-for-Cplusplus
They allow you to scan or print through many ways like `scanf`, `printf`, `cin`, `cout` with a very simple reading function you need to implement.

## Scanner

### Constructor
To use the class named scanner, you need to implement the function `read` which can be called through `read(buffer, count)`, and then pass it to the constructor of the scanner class.

The function `read` receives `char* buffer` and `unsigned int count`, which means that the function puts the next $n$ characters into the buffer and returns $n$, where $1 \le n \le \text{count}$. Specially, if there is no more input, the function returns $0$.

For example, if you want to scan from response from a server, the function can be:
``` cpp
unsigned int read(char* buffer, unsigned int length)
{
	return recv(sock, buffer, length, 0);
}
```
Or if you want to scan from stdin, the function can be:
``` cpp
[&] (char* buffer, unsigned int maxSize) -> unsigned int { return fread(buffer, 1, maxSize, stdin); }
```

### Function

#### Scanf-like

We provide two functions `int scanf(const char* format, ...)` and `int operator(const char* format, ...)`, which do the same thing as `scanf`. (But now there are some bugs in the two functions, we will fix it later.)

If you want to reduce the time spent on decoding the format string, you can use the functions named the format specifiers where the pointer is passed as a parameter. For example:

- `d(void* p)` means `%d`
- `llu(unsigned long long number, void* p)` means `%[number]llu`
- `Scanner.d(&n).s(10, s)` is equivalent to `Scanner.scanf("%d%10s", &n, s)`

If you want to get the return value of this round of scanning, you can use the `operator int()`. For example, `Scanner.d(&n).s(10, s).operator int()` returns the same value as `Scanner.scanf("%d%10s", &n, s)`.

Those functions follow the rules in [https://man7.org/linux/man-pages/man3/scanf.3.html](https://man7.org/linux/man-pages/man3/scanf.3.html).

#### Cin-like

We will provide some functions like `operator>>(T& t)`. But now we provide nothing.

#### Others

We provide some other functions which are `int getchar()` and `char* gets(char* s, unsigned int maxlen)`.

## Printer

It has not been implemented yet.