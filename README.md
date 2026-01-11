# Scanner-and-Printer-for-Cplusplus
They allow you to scan or print through many ways like `scanf`, `printf`, `cin`, `cout` with a very simple function you need to implement.

To use the class named scanner, you need to implement the function `read` with type `std::function<unsigned int(char*, unsigned int)>` and pass it to the constructor of the scanner class.

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

Then you can use the class like real scanf.
