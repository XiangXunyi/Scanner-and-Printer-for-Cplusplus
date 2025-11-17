#include <stdarg.h>
template<unsigned int bufferLength = 1048576U>
class scanner
{
private:
	void* read;
	char buffer[bufferLength];
	unsigned int begin, end;
	bool eof;
	void* moreInformation;
	bool useMoreInformation;
	void _nextchar(void)
	{
		if (eof)
			return;
		if (begin == end)
		{
			if (useMoreInformation)
				end = (*(unsigned int(*)(char*, unsigned int, void*))(read))(buffer, bufferLength, moreInformation);
			else
				end = (*(unsigned int(*)(char*, unsigned int))(read))(buffer, bufferLength);
			begin = 0;
			if (end == 0)
			{
				eof = true;
				return;
			}
		}
		++begin;
	}
	int _readchar(void)
	{
		if (eof)
			return -1;
		if (begin == end)
		{
			if (useMoreInformation)
				end = (*(unsigned int(*)(char*, unsigned int, void*))(read))(buffer, bufferLength, moreInformation);
			else
				end = (*(unsigned int(*)(char*, unsigned int))(read))(buffer, bufferLength);
			begin = 0;
			if (end == 0)
			{
				eof = true;
				return -1;
			}
		}
		return buffer[begin];
	}
	int _scanf(const char* format, va_list args)
	{
		int res = 0;
		unsigned long long charsRead = 0;
		while (*format)
		{
			if (*format == ' ' || *format == '\t' || *format == '\n' || *format == '\v' || *format == '\f' || *format == '\r')
			{
				while (true)
				{
					int c = _readchar();
					if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
					{
						_nextchar();
						++charsRead;
					}
					else
						break;
				}
			}
			else if (*format != '%')
			{
				int c = _readchar();
				if (c != *format)
					break;
				_nextchar();
				++charsRead;
			}
			else
			{
				++format;

				void* p = nullptr;
				if (*format == '*')
					++format;
				else
					p = va_arg(args, void*);

				unsigned int width = 0;
				if ('0' <= *format && *format <= '9')
				{
					while ('0' <= *format && *format <= '9')
					{
						width = width * 10 + (*format ^ '0');
						++format;
					}
				}

				unsigned int modifier = 2;
				if (*format == 'h')
				{
					++format;
					if (*format == 'h')
					{
						++format;
						modifier = 0;
					}
					else
						modifier = 1;
				}
				else if (*format == 'l')
				{
					++format;
					if (*format == 'l')
					{
						++format;
						modifier = 4;
					}
					else
						modifier = 3;
				}
				else if (*format == 'L')
				{
					++format;
					modifier = 4;
				}

				if (*format == '%')
				{
					int c = _readchar();
					if (c != '%')
						break;
					--res;
					_nextchar();
					++charsRead;
				}
				else if (*format == 'c')
				{
					if (width == 0)
					{
						int c = _readchar();
						_nextchar();
						++charsRead;
						if (c == -1)
							break;
						if (p)
							*(char*)p = c;
					}
					else
					{
						for (unsigned int i = 0; i < width; ++i)
						{
							int c = _readchar();
							_nextchar();
							++charsRead;
							if (c == -1)
								break;
							if (p)
								((char*)p)[i] = c;
						}
					}
				}
				else if (*format == '[')
				{
					// Not implemented yet
				}
				else if (*format == 'n')
				{
					if (p)
						if (modifier == 0)
							*(unsigned char*)p = charsRead;
						else if (modifier == 1)
							*(unsigned short*)p = charsRead;
						else if (modifier == 2)
							*(unsigned int*)p = charsRead;
						else if (modifier == 3)
							*(unsigned long*)p = charsRead;
						else
							*(unsigned long long*)p = charsRead;
				}
				else
				{
					while (true)
					{
						int c = _readchar();
						if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
						{
							_nextchar();
							++charsRead;
						}
						else
							break;
					}
					if (*format == 'd' || *format == 'i' || *format == 'u' || *format == 'x' || *format == 'X' || *format == 'o')
					{
						int base;
						int l1, r1, l2, r2;
						if (*format == 'd' || *format == 'i' || *format == 'u')
						{
							base = 10;
							l1 = '0';
							r1 = '9';
							l2 = 0;
							r2 = -1;
						}
						else if (*format == 'x' || *format == 'X')
						{
							base = 16;
							l1 = '0';
							r1 = '9';
							l2 = *format == 'X' ? 'A' : 'a';
							r2 = *format == 'X' ? 'F' : 'f';
						}
						else
						{
							base = 8;
							l1 = '0';
							r1 = '7';
							l2 = 0;
							r2 = -1;
						}
						if (modifier == 0)
						{
							char value = 0;
							bool negative = false, success = false;
							int c = _readchar();
							if (width == 0)
								width = -1;
							if (c == '-')
							{
								negative = true;
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							else if (c == '+')
							{
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							while (width != 0 && (l1 <= c && c <= r1 || l2 <= c && c <= r2))
							{
								success = true;
								value *= base;
								if (l1 <= c && c <= r1)
									value += c - l1;
								else
									value += c - l2 + 10;
								_nextchar();
								++charsRead;
								--width;
								c = _readchar();
							}
							if (!success)
								break;
							if (negative)
								value = -value;
							if (p)
								*(char*)p = value;
						}
						if (modifier == 1)
						{
							short value = 0;
							bool negative = false, success = false;
							int c = _readchar();
							if (width == 0)
								width = -1;
							if (c == '-')
							{
								negative = true;
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							else if (c == '+')
							{
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							while (width != 0 && (l1 <= c && c <= r1 || l2 <= c && c <= r2))
							{
								success = true;
								value *= base;
								if (l1 <= c && c <= r1)
									value += c - l1;
								else
									value += c - l2 + 10;
								_nextchar();
								++charsRead;
								--width;
								c = _readchar();
							}
							if (!success)
								break;
							if (negative)
								value = -value;
							if (p)
								*(short*)p = value;
						}
						if (modifier == 2)
						{
							int value = 0;
							bool negative = false, success = false;
							int c = _readchar();
							if (width == 0)
								width = -1;
							if (c == '-')
							{
								negative = true;
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							else if (c == '+')
							{
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							while (width != 0 && (l1 <= c && c <= r1 || l2 <= c && c <= r2))
							{
								success = true;
								value *= base;
								if (l1 <= c && c <= r1)
									value += c - l1;
								else
									value += c - l2 + 10;
								_nextchar();
								++charsRead;
								--width;
								c = _readchar();
							}
							if (!success)
								break;
							if (negative)
								value = -value;
							if (p)
								*(int*)p = value;
						}
						if (modifier == 3)
						{
							long value = 0;
							bool negative = false, success = false;
							int c = _readchar();
							if (width == 0)
								width = -1;
							if (c == '-')
							{
								negative = true;
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							else if (c == '+')
							{
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							while (width != 0 && (l1 <= c && c <= r1 || l2 <= c && c <= r2))
							{
								success = true;
								value *= base;
								if (l1 <= c && c <= r1)
									value += c - l1;
								else
									value += c - l2 + 10;
								_nextchar();
								++charsRead;
								--width;
								c = _readchar();
							}
							if (!success)
								break;
							if (negative)
								value = -value;
							if (p)
								*(long*)p = value;
						}
						if (modifier == 4)
						{
							long long value = 0;
							bool negative = false, success = false;
							int c = _readchar();
							if (width == 0)
								width = -1;
							if (c == '-')
							{
								negative = true;
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							else if (c == '+')
							{
								_nextchar();
								++charsRead;
								c = _readchar();
								--width;
							}
							while (width != 0 && (l1 <= c && c <= r1 || l2 <= c && c <= r2))
							{
								success = true;
								value *= base;
								if (l1 <= c && c <= r1)
									value += c - l1;
								else
									value += c - l2 + 10;
								_nextchar();
								++charsRead;
								--width;
								c = _readchar();
							}
							if (!success)
								break;
							if (negative)
								value = -value;
							if (p)
								*(long long*)p = value;
						}
					}
					else if (*format == 's')
					{
						unsigned int len = 0;
						while (true)
						{
							int c = _readchar();
							if (c == -1 || c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
								break;
							_nextchar();
							++charsRead;
							if (p)
								((char*)p)[len++] = c;
							else
								++len;
							if (len == width)
								break;
						}
						if (p)
							((char*)p)[len] = 0;
						if (len == 0)
							break;
					}
					else if (*format == 'p')
					{
						void* value = nullptr;
						bool success = false;
						int c = _readchar();
						if (width == 0)
							width = -1;
						while (width && ('0' <= c && c <= '9' || 'A' <= c && c <= 'F'))
						{
							success = true;
							if ('0' <= c && c <= '9')
								value = (void*)((long long)(value) << 4 | (c - '0'));
							else
								value = (void*)((long long)(value) << 4 | (c - 'A' + 10));
							_nextchar();
							++charsRead;
							--width;
							c = _readchar();
						}
						if (!success)
							break;
						if (p)
							*(void**)p = value;
					}
				}
				++res;
			}
			++format;
		}
		va_end(args);
		if (res == 0 && eof)
			return -1;
		return res;
	}
public:
	scanner(unsigned int (*read)(char*, unsigned int, void*), void* moreInformation) : read((void*)(read)), begin(0), end(0), eof(false), moreInformation(moreInformation), useMoreInformation(true) {}
	scanner(unsigned int (*read)(char*, unsigned int)) : read((void*)(read)), begin(0), end(0), eof(false), moreInformation(nullptr), useMoreInformation(false) {}
	int getchar(void)
	{
		return _nextchar();
	}
	char* gets(char* s, unsigned int maxlen)
	{
		char* res = s, * r = s + maxlen - 1;
		while (true)
		{
			if (s == r)
			{
				*s = 0;
				break;
			}
			int c = _nextchar();
			if (c == -1)
			{
				*s = 0;
				break;
			}
			if (c == '\n')
			{
				*s = '\n';
				*(++s) = 0;
				break;
			}
		}
		if (res == s)
			return nullptr;
		return res;
	}
	int scanf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		return _scanf(format, args);
	}
	int operator()(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		return _scanf(format, args);
	}
};
