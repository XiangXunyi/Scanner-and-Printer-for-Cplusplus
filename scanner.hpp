/*

**It is not finished yet**

This is a code for scanner class.

The scanner class alows you to read input from the function @c read you implement in 3 ways:

- using @c scanf and @c operator()
> Now it allows %(*)(number)[hh / h /  / l / ll][d / i / u / x / X / o], %%, %(*)(number)[c / s], %(*)(number)p and %n.

- using the function named the format string
> Now it allows (*)(number)[hh / h /  / l / ll][d / u / x / o], %(*)(number)[c / s], %(*)(number)p

- using @c operator>>
> Now it allows nothing.

To use the scanner class, you need to implement the function @c read with type @c std::function<unsigned int(char*, unsigned int)>
and pass it to the constructor of the scanner class.

The function @c read `receives char* buffer` and `unsigned int bufferLength` which means that the function puts the next n characters
into the buffer and returns n, where 0 < n <= bufferLength. Specially, if there is no more input, the function returns 0.
*/

#ifndef _SCANNER_HPP

#define _SCANNER_HPP

#include <stdarg.h>
#include <functional>

template<unsigned int bufferLength = 1048576U>
class scanner
{
private:
	std::function<unsigned int(char*, unsigned int)> read;
	char buffer[bufferLength];
	unsigned int begin, end;
	bool eof;

	void _nextchar(void)
	{
		if (eof)
			return;
		if (begin == end)
		{
			end = read(buffer, bufferLength);
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
			end = read(buffer, bufferLength);
			begin = 0;
			if (end == 0)
			{
				eof = true;
				return -1;
			}
		}
		return buffer[begin];
	}

	void _skipspace(void)
	{
		while (true)
		{
			int c = _readchar();
			if (c == -1 || c != ' ' && c != '\t' && c != '\n' && c != '\v' && c != '\f' && c != '\r')
				break;
			_nextchar();
		}
	}

	class scanner_stream
	{
	private:
		scanner* sc;
		unsigned long long charsRead;
		bool success;
		int res;

		template<typename T>
		scanner_stream& get_d(void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
			if (c == '-')
			{
				negative = true;
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			else if (c == '+')
			{
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			success = false;
			while ('0' <= c && c <= '9')
			{
				success = true;
				val *= 10;
				val += c - '0';
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_d_withnumber(unsigned int number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
			if (number)
				if (c == '-')
				{
					negative = true;
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
				else if (c == '+')
				{
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
			success = false;
			while (number && '0' <= c && c <= '9')
			{
				success = true;
				val *= 10;
				val += c - '0';
				sc->_nextchar();
				++charsRead;
				--number;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_x(void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
			if (c == '-')
			{
				negative = true;
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			else if (c == '+')
			{
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			success = false;
			while ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F')
			{
				success = true;
				val <<= 4;
				if ('0' <= c && c <= '9')
					val += c - '0';
				else if ('a' <= c && c <= 'f')
					val += c - 'a' + 10;
				else
					val += c - 'A' + 10;
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_x_withnumber(unsigned int number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
			if (number)
				if (c == '-')
				{
					negative = true;
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
				else if (c == '+')
				{
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
			success = false;
			while (number && ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F'))
			{
				success = true;
				val <<= 4;
				if ('0' <= c && c <= '9')
					val += c - '0';
				else if ('a' <= c && c <= 'f')
					val += c - 'a' + 10;
				else
					val += c - 'A' + 10;
				sc->_nextchar();
				++charsRead;
				--number;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_o(void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
			if (c == '-')
			{
				negative = true;
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			else if (c == '+')
			{
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			success = false;
			while ('0' <= c && c <= '7')
			{
				success = true;
				val <<= 3;
				val += c - '0';
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_o_withnumber(unsigned int number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
			if (number)
				if (c == '-')
				{
					negative = true;
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
				else if (c == '+')
				{
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
			success = false;
			while (number && '0' <= c && c <= '7')
			{
				success = true;
				val <<= 3;
				val += c - '0';
				sc->_nextchar();
				++charsRead;
				--number;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}
	public:
		scanner_stream(scanner* s) : sc(s), charsRead(0ULL), success(true), res(0) {}

		operator int() const { return res; }

		/**
		 * @brief like @c "%hhd" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hhd(void* value = nullptr) { return get_d<signed char>(value); }

		/**
		 * @brief like @c "%[number]hhd" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hhd(unsigned int number, void* value = nullptr) { return get_d_withnumber<signed char>(number, value); }

		/**
		 * @brief like @c "%hd" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hd(void* value = nullptr) { return get_d<signed short>(value); }

		/**
		 * @brief like @c "%[number]hd" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hd(unsigned int number, void* value = nullptr) { return get_d_withnumber<signed short>(number, value); }

		/**
		 * @brief like @c "%d" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& d(void* value = nullptr) { return get_d<signed int>(value); }

		/**
		 * @brief like @c "%[number]d" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& d(unsigned int number, void* value = nullptr) { return get_d_withnumber<signed int>(number, value); }

		/**
		 * @brief like @c "%ld" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& ld(void* value = nullptr) { return get_d<signed long>(value); }

		/**
		 * @brief like @c "%[number]ld" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& ld(unsigned int number, void* value = nullptr) { return get_d_withnumber<signed long>(number, value); }

		/**
		 * @brief like @c "%lld" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lld(void* value = nullptr) { return get_d<signed long long>(value); }

		/**
		 * @brief like @c "%[number]lld" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lld(unsigned int number, void* value = nullptr) { return get_d_withnumber<signed long long>(number, value); }

		/**
		 * @brief like @c "%hhu" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hhu(void* value = nullptr) { return get_d<unsigned char>(value); }

		/**
		 * @brief like @c "%[number]hhu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hhu(unsigned int number, void* value = nullptr) { return get_d_withnumber<unsigned char>(number, value); }

		/**
		 * @brief like @c "%hu" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hu(void* value = nullptr) { return get_d<unsigned short>(value); }

		/**
		 * @brief like @c "%[number]hu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hu(unsigned int number, void* value = nullptr) { return get_d_withnumber<unsigned short>(number, value); }

		/**
		 * @brief like @c "%u" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& u(void* value = nullptr) { return get_d<unsigned int>(value); }

		/**
		 * @brief like @c "%[number]u" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& u(unsigned int number, void* value = nullptr) { return get_d_withnumber<unsigned int>(number, value); }

		/**
		 * @brief like @c "%lu" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lu(void* value = nullptr) { return get_d<unsigned long>(value); }

		/**
		 * @brief like @c "%[number]lu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lu(unsigned int number, void* value = nullptr) { return get_d_withnumber<unsigned long>(number, value); }

		/**
		 * @brief like @c "%llu" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& llu(void* value = nullptr) { return get_d<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]llu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& llu(unsigned int number, void* value = nullptr) { return get_d_withnumber<unsigned long long>(number, value); }

		/**
		 * @brief like @c "%hhx" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hhx(void* value = nullptr) { return get_x<unsigned char>(value); }

		/**
		 * @brief like @c "%[number]hhx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hhx(unsigned int number, void* value = nullptr) { return get_x_withnumber<unsigned char>(number, value); }

		/**
		 * @brief like @c "%hx" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hx(void* value = nullptr) { return get_x<unsigned short>(value); }

		/**
		 * @brief like @c "%[number]hx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hx(unsigned int number, void* value = nullptr) { return get_x_withnumber<unsigned short>(number, value); }

		/**
		 * @brief like @c "%x" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& x(void* value = nullptr) { return get_x<unsigned int>(value); }

		/**
		 * @brief like @c "%[number]x" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& x(unsigned int number, void* value = nullptr) { return get_x_withnumber<unsigned int>(number, value); }

		/**
		 * @brief like @c "%lx" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lx(void* value = nullptr) { return get_x<unsigned long>(value); }

		/**
		 * @brief like @c "%[number]lx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lx(unsigned int number, void* value = nullptr) { return get_x_withnumber<unsigned long>(number, value); }

		/**
		 * @brief like @c "%llx" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& llx(void* value = nullptr) { return get_x<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]llx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& llx(unsigned int number, void* value = nullptr) { return get_x_withnumber<unsigned long long>(number, value); }

		/**
		 * @brief like @c "%hho" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hho(void* value = nullptr) { return get_o<unsigned char>(value); }

		/**
		 * @brief like @c "%[number]hho" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& hho(unsigned int number, void* value = nullptr) { return get_o_withnumber<unsigned char>(number, value); }

		/**
		 * @brief like @c "%ho" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& ho(void* value = nullptr) { return get_o<unsigned short>(value); }

		/**
		 * @brief like @c "%[number]ho" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& ho(unsigned int number, void* value = nullptr) { return get_o_withnumber<unsigned short>(number, value); }

		/**
		 * @brief like @c "%o" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& o(void* value = nullptr) { return get_o<unsigned int>(value); }

		/**
		 * @brief like @c "%[number]o" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& o(unsigned int number, void* value = nullptr) { return get_o_withnumber<unsigned int>(number, value); }

		/**
		 * @brief like @c "%lo" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lo(void* value = nullptr) { return get_o<unsigned long>(value); }

		/**
		 * @brief like @c "%[number]lo" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& lo(unsigned int number, void* value = nullptr) { return get_o_withnumber<unsigned long>(number, value); }

		/**
		 * @brief like @c "%llo" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& llo(void* value = nullptr) { return get_o<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]llo" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& llo(unsigned int number, void* value = nullptr) { return get_o_withnumber<unsigned long long>(number, value); }

		/**
		 * @brief like @c "%c" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& c(void* value = nullptr)
		{
			if (!success)
				return *this;
			int val = sc->_readchar();
			if (val == -1)
			{
				success = false;
				val = 0;
			}
			else
			{
				sc->_nextchar();
				++charsRead;
			}
			if (value)
				*(char*)(value) = val;
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[number]c" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& c(unsigned int number, void* value = nullptr)
		{
			if (!success)
				return *this;
			success = false;
			for (unsigned int i = 0; i != number; ++i)
			{
				int val = sc->_readchar();
				if (val == -1)
					break;
				sc->_nextchar();
				++charsRead;
				if (value)
					*((char*)(value++)) = val;
				success = true;
			}
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%s" in scanf
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& s(void* value = nullptr)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			while (true)
			{
				int val = sc->_readchar();
				if (val == -1 || val == ' ' || val == '\t' || val == '\n' || val == '\v' || val == '\f' || val == '\r')
					break;
				sc->_nextchar();
				++charsRead;
				if (value)
					*((char*)(value++)) = val;
				success = true;
			}
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[number]s" in scanf
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& s(unsigned int number, void* value = nullptr)
		{
			if (!success)
				return *this;
			sc->_skipspace();
			while (number)
			{
				int val = sc->_readchar();
				if (val == -1 || val == ' ' || val == '\t' || val == '\n' || val == '\v' || val == '\f' || val == '\r')
					break;
				sc->_nextchar();
				++charsRead;
				--number;
				if (value)
					*((char*)(value++)) = val;
				success = true;
			}
			if (success)
				++res;
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%p" in scanf
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& p(void* value = nullptr) { return get_x<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]p" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
		 * @return the class itself
		 */
		scanner_stream& p(unsigned int number, void* value = nullptr) { return get_x_withnumber<unsigned long long>(number, value); }
	};

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
	/**
	 * @brief Constructor
	 * @param read A function that reads data
	 */
	scanner(std::function<unsigned int(char*, unsigned int)> read) : read(read) {}

	/**
	 * @brief Read a character from the input stream
	 * @return The next character or -1 if end of file is reached
	 */
	int getchar(void)
	{
		int c = _readchar();
		_nextchar();
		return c;
	}

	/**
	 * @brief Read a character from the input stream
	 * @param s Pointer to a character buffer
	 * @param maxlen Maximum length of the buffer
	 * @return Pointer to the end of the buffer or nullptr if end of file is reached
	 */
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
			int c = _readchar();
			_nextchar();
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

	/**
	 * @brief scanf function
	 * @param format Format string
	 * @param ... Variable arguments
	 * @return Number of successful matches or -1 if end of file is reached
	 */
	int scanf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		return _scanf(format, args);
	}

	/**
	 * @brief scanf function
	 * @param format Format string
	 * @param ... Variable arguments
	 * @return Number of successful matches or -1 if end of file is reached
	 */
	int operator()(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		return _scanf(format, args);
	}

	/**
	 * @brief like @c "%hhd" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hhd(void* value = nullptr)
	{
		return scanner_stream(this).hhd(value);
	}

	/**
	 * @brief like @c "%[number]hhd" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hhd(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hhd(number, value);
	}

	/**
	 * @brief like @c "%hd" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hd(void* value = nullptr)
	{
		return scanner_stream(this).hd(value);
	}

	/**
	 * @brief like @c "%[number]hd" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hd(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hd(number, value);
	}

	/**
	 * @brief like @c "%d" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream d(void* value = nullptr)
	{
		return scanner_stream(this).d(value);
	}

	/**
	 * @brief like @c "%[number]d" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream d(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).d(number, value);
	}

	/**
	 * @brief like @c "%ld" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream ld(void* value = nullptr)
	{
		return scanner_stream(this).ld(value);
	}

	/**
	 * @brief like @c "%[number]ld" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream ld(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).ld(number, value);
	}

	/**
	 * @brief like @c "%lld" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lld(void* value = nullptr)
	{
		return scanner_stream(this).lld(value);
	}

	/**
	 * @brief like @c "%[number]lld" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lld(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).lld(number, value);
	}

	/**
	 * @brief like @c "%hhu" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hhu(void* value = nullptr)
	{
		return scanner_stream(this).hhu(value);
	}

	/**
	 * @brief like @c "%[number]hhu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hhu(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hhu(number, value);
	}

	/**
	 * @brief like @c "%hu" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hu(void* value = nullptr)
	{
		return scanner_stream(this).hu(value);
	}

	/**
	 * @brief like @c "%[number]hu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hu(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hu(number, value);
	}

	/**
	 * @brief like @c "%u" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream u(void* value = nullptr)
	{
		return scanner_stream(this).u(value);
	}

	/**
	 * @brief like @c "%[number]u" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream u(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).u(number, value);
	}

	/**
	 * @brief like @c "%lu" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lu(void* value = nullptr)
	{
		return scanner_stream(this).lu(value);
	}

	/**
	 * @brief like @c "%[number]lu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lu(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).lu(number, value);
	}

	/**
	 * @brief like @c "%llu" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream llu(void* value = nullptr)
	{
		return scanner_stream(this).llu(value);
	}

	/**
	 * @brief like @c "%[number]llu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream llu(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).llu(number, value);
	}

	/**
	 * @brief like @c "%hhx" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hhx(void* value = nullptr)
	{
		return scanner_stream(this).hhx(value);
	}

	/**
	 * @brief like @c "%[number]hhx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hhx(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hhx(number, value);
	}

	/**
	 * @brief like @c "%hx" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hx(void* value = nullptr)
	{
		return scanner_stream(this).hx(value);
	}

	/**
	 * @brief like @c "%[number]hx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hx(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hx(number, value);
	}

	/**
	 * @brief like @c "%x" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream x(void* value = nullptr)
	{
		return scanner_stream(this).x(value);
	}

	/**
	 * @brief like @c "%[number]x" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream x(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).x(number, value);
	}

	/**
	 * @brief like @c "%lx" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lx(void* value = nullptr)
	{
		return scanner_stream(this).lx(value);
	}

	/**
	 * @brief like @c "%[number]lx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lx(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).lx(number, value);
	}

	/**
	 * @brief like @c "%llx" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream llx(void* value = nullptr)
	{
		return scanner_stream(this).llx(value);
	}

	/**
	 * @brief like @c "%[number]llx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream llx(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).llx(number, value);
	}

	/**
	 * @brief like @c "%hho" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hho(void* value = nullptr)
	{
		return scanner_stream(this).hho(value);
	}

	/**
	 * @brief like @c "%[number]hho" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream hho(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).hho(number, value);
	}

	/**
	 * @brief like @c "%ho" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream ho(void* value = nullptr)
	{
		return scanner_stream(this).ho(value);
	}

	/**
	 * @brief like @c "%[number]ho" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream ho(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).ho(number, value);
	}

	/**
	 * @brief like @c "%o" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream o(void* value = nullptr)
	{
		return scanner_stream(this).o(value);
	}

	/**
	 * @brief like @c "%[number]o" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream o(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).o(number, value);
	}

	/**
	 * @brief like @c "%lo" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lo(void* value = nullptr)
	{
		return scanner_stream(this).lo(value);
	}

	/**
	 * @brief like @c "%[number]lo" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream lo(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).lo(number, value);
	}

	/**
	 * @brief like @c "%llo" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream llo(void* value = nullptr)
	{
		return scanner_stream(this).llo(value);
	}

	/**
	 * @brief like @c "%[number]llo" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream llo(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).llo(number, value);
	}

	/**
	 * @brief like @c "%c" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream c(void* value = nullptr)
	{
		return scanner_stream(this).c(value);
	}

	/**
	 * @brief like @c "%[number]c" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream c(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).c(number, value);
	}

	/**
	 * @brief like @c "%s" in scanf
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream s(void* value = nullptr)
	{
		return scanner_stream(this).s(value);
	}

	/**
	 * @brief like @c "%[number]s" in scanf
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream s(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).s(number, value);
	}

	/**
	 * @brief like @c "%p" in scanf
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream p(void* value = nullptr)
	{
		return scanner_stream(this).p(value);
	}

	/**
	 * @brief like @c "%[number]p" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an integer variable or @c nullptr if you don't want to read the value
	 * @return the class itself
	 */
	scanner_stream p(unsigned int number, void* value = nullptr)
	{
		return scanner_stream(this).p(number, value);
	}
};

#endif
