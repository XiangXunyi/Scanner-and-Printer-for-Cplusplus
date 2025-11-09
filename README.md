# Scanner-and-Printer-for-Cplusplus
They are like scanf and printf, but can scan from or print to what you want.

To use the class named scanner, you need to implement a function `unsigned int read(char* buffer, unsigned int length)` that put chars scanned less than or equal to `length` to `buffer` and return the number of chars scanned.

For example, if you want to scan from response from a server, the function may be:
``` cpp
unsigned int read(char* buffer, unsigned int length)
{
	return recv(sock, buffer, length, 0);
}
```
Or if you want to scan from stdin, the function may be:
``` cpp
unsigned int read(char* buffer, unsigned int length)
{
	return fread(buffer, 1, length, stdin);
}
```

Then you can use the class like real scanf.
