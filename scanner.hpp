/*

**It is not finished yet**

This is a code for scanner class.

The scanner class allows you to read input from the simple function @c read
you implement in 3 ways:

- using @c scanf and @c operator()
> Now it allows %(*)(number)[hh / h /  / l / ll][d / i / u / x / X / o], %%,
> %(*)(number)[c / s], %(*)(number)p and %n.

- using the function named the format string
> Now it allows (*)(number)[hh / h /  / l / ll / t / z][d / i / u / x / o],
> (*)(number)[ / l / L]f, (*)(number)[c / s], (*)(number)p,
> (*)[hh / h /  / l / ll / t / z]n

- using @c operator>>
> Now it allows nothing.

To use the scanner class, you need to implement the function @c read which
can be called through @c read(buffer, count) and pass it to the constructor
of the scanner class.

The function @c read receives `char* buffer` and `unsigned int count`, which
means that the function puts the next n characters into the buffer and
returns n, where 0 < n <= count. Specially, if there is no more input, the
function returns 0.

For more information, see
<https://github.com/XiangXunyi/Scanner-and-Printer-for-Cplusplus>.

*/

#ifndef _SCANNER_HPP

#define _SCANNER_HPP

#include <stdarg.h>

template<unsigned int bufferLength = 1048576U>
class scanner
{
private:
	typedef decltype(sizeof(0)) size_t;
	typedef decltype((int*)(0) - (int*)(0)) ptrdiff_t;

	class functionBase
	{
	public:
		virtual unsigned int call(char* buffer, unsigned int count) = 0;
	} *read;

	template<typename valueType>
	class functionImpl : public functionBase
	{
	public:
		valueType value;
		functionImpl(valueType value) : value(value) {}
		unsigned int call(char* buffer, unsigned int count)
		{
			return value(buffer, count);
		}
	};

	char buffer[bufferLength];
	unsigned int begin, end;
	bool eof;

	void _nextchar(void)
	{
		if (eof)
			return;
		if (begin == end)
		{
			end = read->call(buffer, bufferLength);
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
			end = read->call(buffer, bufferLength);
			begin = 0;
			if (end == 0)
			{
				eof = true;
				return -1;
			}
		}
		return buffer[begin];
	}

	void _skipspace(unsigned long long& charsRead)
	{
		while (true)
		{
			int c = _readchar();
			if (c == -1 || c != ' ' && c != '\t' && c != '\n' && c != '\v' && c != '\f' && c != '\r')
				break;
			_nextchar();
			++charsRead;
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
			sc->_skipspace(charsRead);
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
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_d_withnumber(unsigned long long number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
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
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_i(void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
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
			if (c == '0')
			{
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
				if (c == 'x' || c == 'X')
				{
					sc->_nextchar();
					++charsRead;
					c = sc->_readchar();
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
				}
				else if ('0' <= c && c <= '7')
					while ('0' <= c && c <= '7')
					{
						success = true;
						val <<= 3;
						val += c - '0';
						sc->_nextchar();
						++charsRead;
						c = sc->_readchar();
					}
				else
				{
					success = true;
					sc->_nextchar();
					++charsRead;
					c = sc->_readchar();
				}
			}
			else
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
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_i_withnumber(unsigned long long number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
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
			if (c == '0' && number)
			{
				sc->_nextchar();
				++charsRead;
				--number;
				c = sc->_readchar();
				if (number && (c == 'x' || c == 'X'))
				{
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
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
				}
				else if (number && '0' <= c && c <= '7')
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
				else
				{
					success = true;
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
				}
			}
			else
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
			{
				if (value)
					++res;
			}
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
			sc->_skipspace(charsRead);
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
			unsigned char state = 0;
			while ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F' || c == 'x' || c == 'X')
			{
				if (c == 'x' || c == 'X')
					if (state == 48)
						success = false;
					else
						break;
				else
				{
					success = true;
					val <<= 4;
					if ('0' <= c && c <= '9')
						val += c - '0';
					else if ('a' <= c && c <= 'f')
						val += c - 'a' + 10;
					else
						val += c - 'A' + 10;
				}
				(state <<= 1) |= c;
				sc->_nextchar();
				++charsRead;
				c = sc->_readchar();
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_x_withnumber(unsigned long long number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
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
			unsigned char state = 0;
			while (number && ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F' || c == 'x' || c == 'X'))
			{
				if (c == 'x' || c == 'X')
					if (state == 48)
						success = false;
					else
						break;
				else
				{
					success = true;
					val <<= 4;
					if ('0' <= c && c <= '9')
						val += c - '0';
					else if ('a' <= c && c <= 'f')
						val += c - 'a' + 10;
					else
						val += c - 'A' + 10;
				}
				(state <<= 1) |= c;
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
			{
				if (value)
					++res;
			}
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
			sc->_skipspace(charsRead);
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
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_o_withnumber(unsigned long long number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
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
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_f(void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
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
			if (c == 'i' || c == 'I')
			{
				const char* infinityString1 = "inf";
				int pos = 0;
				while (pos != 3)
					if (c == infinityString1[pos] || c == infinityString1[pos] - 32)
					{
						sc->_nextchar();
						++charsRead;
						c = sc->_readchar();
						++pos;
					}
					else
						break;
				if (pos == 3)
				{
					if (c == 'i' || c == 'I')
					{
						const char* infinityString2 = "inity";
						pos = 0;
						while (pos != 5)
							if (c == infinityString2[pos] || c == infinityString2[pos] - 32)
							{
								sc->_nextchar();
								++charsRead;
								c = sc->_readchar();
								++pos;
							}
							else
								break;
						if (pos == 5)
						{
							success = true;
							val = (T)(1) / (T)(0);
						}
						else
						{
							sc->_nextchar();
							++charsRead;
						}
					}
					else
					{
						success = true;
						val = (T)(1) / (T)(0);
					}
				}
				else
				{
					sc->_nextchar();
					++charsRead;
				}
			}
			else if (c == 'n' || c == 'N')
			{
				const char* nanString = "nan";
				int pos = 0;
				while (pos != 3)
					if (c == nanString[pos] || c == nanString[pos] - 32)
					{
						sc->_nextchar();
						++charsRead;
						c = sc->_readchar();
						++pos;
					}
					else
						break;
				if (pos == 3)
				{
					success = true;
					val = (T)(0) / (T)(0);
				}
				else
				{
					sc->_nextchar();
					++charsRead;
				}
			}
			else if ('0' <= c && c <= '9' || c == '.')
			{
				unsigned int state = 0;
				if (c == '0')
				{
					sc->_nextchar();
					++charsRead;
					c = sc->_readchar();
					if (c == 'x' || c == 'X')
					{
						state = 4U;
						sc->_nextchar();
						++charsRead;
						c = sc->_readchar();
						T exp = 1;
						bool goon = false;
						while ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F' || c == '.' || c == 'p' || c == 'P')
						{
							success = true;
							if (c == 'p' || c == 'P')
							{
								goon = true;
								break;
							}
							if (c == '.')
							{
								sc->_nextchar();
								++charsRead;
								c = sc->_readchar();
								goon = true;
								break;
							}
							val *= 16;
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
						if (goon)
						{
							goon = false;
							while ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F' || c == 'p' || c == 'P')
							{
								if (c == 'p' || c == 'P')
								{
									goon = true;
									sc->_nextchar();
									++charsRead;
									c = sc->_readchar();
									break;
								}
								exp *= 0x1p-4;
								if ('0' <= c && c <= '9')
									val += (c - '0') * exp;
								else if ('a' <= c && c <= 'f')
									exp += (c - 'a' + 10) * exp;
								else
									exp += (c - 'A' + 10) * exp;
								sc->_nextchar();
								++charsRead;
								c = sc->_readchar();
							}
						}
						if (goon)
						{
							T base = 2;
							if (c == '-')
							{
								base = 0.5;
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
							T pow2[10];
							pow2[0] = 1;
							for (int i = 1; i < 10; ++i)
								pow2[i] = pow2[i - 1] * base;
							T times = 1;
							while ('0' <= c && c <= '9')
							{
								T tpow2 = times * times;
								T tpow4 = tpow2 * tpow2;
								times = tpow4 * tpow4 * tpow2;
								times *= pow2[c - '0'];
								sc->_nextchar();
								++charsRead;
								c = sc->_readchar();
							}
							val *= times;
						}
					}
					else
						state = 1U;
				}
				if (state != 4U)
				{
					T exp = 1;
					bool goon = false;
					while ('0' <= c && c <= '9' || c == '.' || c == 'e' || c == 'E')
					{
						if (c == 'e' || c == 'E')
						{
							goon = true;
							break;
						}
						if (c == '.')
						{
							sc->_nextchar();
							++charsRead;
							c = sc->_readchar();
							goon = true;
							break;
						}
						state |= 1U;
						val *= 10;
						val += c - '0';
						sc->_nextchar();
						++charsRead;
						c = sc->_readchar();
					}
					if (goon)
					{
						goon = false;
						while ('0' <= c && c <= '9' || c == 'e' || c == 'E')
						{
							if (c == 'e' || c == 'E')
							{
								goon = true;
								sc->_nextchar();
								++charsRead;
								c = sc->_readchar();
								break;
							}
							state |= 2U;
							exp *= 0.1;
							val += (c - '0') * exp;
							sc->_nextchar();
							++charsRead;
							c = sc->_readchar();
						}
					}
					if (goon)
					{
						T base = 10;
						if (c == '-')
						{
							base = 0.1;
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
						T pow2[10];
						pow2[0] = 1;
						for (int i = 1; i < 10; ++i)
							pow2[i] = pow2[i - 1] * base;
						T times = 1;
						while ('0' <= c && c <= '9')
						{
							T tpow2 = times * times;
							T tpow4 = tpow2 * tpow2;
							times = tpow4 * tpow4 * tpow2;
							times *= pow2[c - '0'];
							sc->_nextchar();
							++charsRead;
							c = sc->_readchar();
						}
						val *= times;
					}
					if (state)
						success = true;
					else
						val = 0;
				}
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_f_withnumber(unsigned long long number, void* value)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
			T val = 0;
			bool negative = false;
			int c = sc->_readchar();
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
			if (number && (c == 'i' || c == 'I'))
			{
				const char* infinityString1 = "inf";
				int pos = 0;
				while (pos != 3)
					if (number && (c == infinityString1[pos] || c == infinityString1[pos] - 32))
					{
						sc->_nextchar();
						++charsRead;
						--number;
						c = sc->_readchar();
						++pos;
					}
					else
						break;
				if (pos == 3)
				{
					if (number && (c == 'i' || c == 'I'))
					{
						const char* infinityString2 = "inity";
						pos = 0;
						while (pos != 5)
							if (number && (c == infinityString2[pos] || c == infinityString2[pos] - 32))
							{
								sc->_nextchar();
								++charsRead;
								--number;
								c = sc->_readchar();
								++pos;
							}
							else
								break;
						if (pos == 5)
						{
							success = true;
							val = (T)(1) / (T)(0);
						}
						else
						{
							sc->_nextchar();
							++charsRead;
							--number;
						}
					}
					else
					{
						success = true;
						val = (T)(1) / (T)(0);
					}
				}
				else
				{
					sc->_nextchar();
					++charsRead;
					--number;
				}
			}
			else if (number && (c == 'n' || c == 'N'))
			{
				const char* nanString = "nan";
				int pos = 0;
				while (pos != 3)
					if (number && (c == nanString[pos] || c == nanString[pos] - 32))
					{
						sc->_nextchar();
						++charsRead;
						--number;
						c = sc->_readchar();
						++pos;
					}
					else
						break;
				if (pos == 3)
				{
					success = true;
					val = (T)(0) / (T)(0);
				}
				else
				{
					sc->_nextchar();
					++charsRead;
					--number;
				}
			}
			else if (number && ('0' <= c && c <= '9' || c == '.'))
			{
				unsigned int state = 0;
				if (c == '0')
				{
					sc->_nextchar();
					++charsRead;
					--number;
					c = sc->_readchar();
					if (number && (c == 'x' || c == 'X'))
					{
						state = 4U;
						sc->_nextchar();
						++charsRead;
						--number;
						c = sc->_readchar();
						T exp = 1;
						bool goon = false;
						while (number && ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F' || c == '.' || c == 'p' || c == 'P'))
						{
							success = true;
							if (c == 'p' || c == 'P')
							{
								goon = true;
								break;
							}
							if (c == '.')
							{
								sc->_nextchar();
								++charsRead;
								--number;
								c = sc->_readchar();
								goon = true;
								break;
							}
							val *= 16;
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
						if (goon)
						{
							goon = false;
							while (number && ('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F' || c == 'p' || c == 'P'))
							{
								if (c == 'p' || c == 'P')
								{
									goon = true;
									sc->_nextchar();
									++charsRead;
									--number;
									c = sc->_readchar();
									break;
								}
								exp *= 0x1p-4;
								if ('0' <= c && c <= '9')
									val += (c - '0') * exp;
								else if ('a' <= c && c <= 'f')
									exp += (c - 'a' + 10) * exp;
								else
									exp += (c - 'A' + 10) * exp;
								sc->_nextchar();
								++charsRead;
								--number;
								c = sc->_readchar();
							}
						}
						if (goon)
						{
							T base = 2;
							if (number)
								if (c == '-')
								{
									base = 0.5;
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
							T pow2[10];
							pow2[0] = 1;
							for (int i = 1; i < 10; ++i)
								pow2[i] = pow2[i - 1] * base;
							T times = 1;
							while (number && '0' <= c && c <= '9')
							{
								T tpow2 = times * times;
								T tpow4 = tpow2 * tpow2;
								times = tpow4 * tpow4 * tpow2;
								times *= pow2[c - '0'];
								sc->_nextchar();
								++charsRead;
								--number;
								c = sc->_readchar();
							}
							val *= times;
						}
					}
					else
						state = 1U;
				}
				if (state != 4U)
				{
					T exp = 1;
					bool goon = false;
					while (number && ('0' <= c && c <= '9' || c == '.' || c == 'e' || c == 'E'))
					{
						if (c == 'e' || c == 'E')
						{
							goon = true;
							break;
						}
						if (c == '.')
						{
							sc->_nextchar();
							++charsRead;
							--number;
							c = sc->_readchar();
							goon = true;
							break;
						}
						state |= 1U;
						val *= 10;
						val += c - '0';
						sc->_nextchar();
						++charsRead;
						--number;
						c = sc->_readchar();
					}
					if (goon)
					{
						goon = false;
						while (number && ('0' <= c && c <= '9' || c == 'e' || c == 'E'))
						{
							if (c == 'e' || c == 'E')
							{
								goon = true;
								sc->_nextchar();
								++charsRead;
								--number;
								c = sc->_readchar();
								break;
							}
							state |= 2U;
							exp *= 0.1;
							val += (c - '0') * exp;
							sc->_nextchar();
							++charsRead;
							--number;
							c = sc->_readchar();
						}
					}
					if (goon)
					{
						T base = 10;
						if (number)
							if (c == '-')
							{
								base = 0.1;
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
						T pow2[10];
						pow2[0] = 1;
						for (int i = 1; i < 10; ++i)
							pow2[i] = pow2[i - 1] * base;
						T times = 1;
						while (number && '0' <= c && c <= '9')
						{
							T tpow2 = times * times;
							T tpow4 = tpow2 * tpow2;
							times = tpow4 * tpow4 * tpow2;
							times *= pow2[c - '0'];
							sc->_nextchar();
							++charsRead;
							--number;
							c = sc->_readchar();
						}
						val *= times;
					}
					if (state)
						success = true;
					else
						val = 0;
				}
			}
			if (negative)
				val = -val;
			if (value)
				*(T*)(value) = val;
			if (success)
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		template<typename T>
		scanner_stream& get_n(void* value)
		{
			if (!success)
				return *this;
			if (value)
			{
				*(T*)(value) = charsRead;
				++res;
			}
			return *this;
		}

	public:
		scanner_stream(scanner* s) : sc(s), charsRead(0ULL), success(true), res(0) {}

		operator int() const { return res; }

		/**
		 * @brief like @c "%hhd" in scanf
		 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhd(void* value = nullptr) { return get_d<signed char>(value); }

		/**
		 * @brief like @c "%[number]hhd" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhd(unsigned long long number, void* value = nullptr) { return get_d_withnumber<signed char>(number, value); }

		/**
		 * @brief like @c "%hd" in scanf
		 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hd(void* value = nullptr) { return get_d<signed short>(value); }

		/**
		 * @brief like @c "%[number]hd" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hd(unsigned long long number, void* value = nullptr) { return get_d_withnumber<signed short>(number, value); }

		/**
		 * @brief like @c "%d" in scanf
		 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& d(void* value = nullptr) { return get_d<signed int>(value); }

		/**
		 * @brief like @c "%[number]d" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& d(unsigned long long number, void* value = nullptr) { return get_d_withnumber<signed int>(number, value); }

		/**
		 * @brief like @c "%ld" in scanf
		 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ld(void* value = nullptr) { return get_d<signed long>(value); }

		/**
		 * @brief like @c "%[number]ld" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ld(unsigned long long number, void* value = nullptr) { return get_d_withnumber<signed long>(number, value); }

		/**
		 * @brief like @c "%lld" in scanf
		 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lld(void* value = nullptr) { return get_d<signed long long>(value); }

		/**
		 * @brief like @c "%[number]lld" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lld(unsigned long long number, void* value = nullptr) { return get_d_withnumber<signed long long>(number, value); }

		/**
		 * @brief like @c "%td" in scanf
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& td(void* value = nullptr) { return get_d<ptrdiff_t>(value); }

		/**
		 * @brief like @c "%[number]td" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& td(unsigned long long number, void* value = nullptr) { return get_d_withnumber<ptrdiff_t>(number, value); }

		/**
		 * @brief like @c "%zd" in scanf
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zd(void* value = nullptr) { return get_d<size_t>(value); }

		/**
		 * @brief like @c "%[number]zd" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zd(unsigned long long number, void* value = nullptr) { return get_d_withnumber<size_t>(number, value); }

		/**
		 * @brief like @c "%hhi" in scanf
		 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhi(void* value = nullptr) { return get_i<signed char>(value); }

		/**
		 * @brief like @c "%[number]hhi" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhi(unsigned long long number, void* value = nullptr) { return get_i_withnumber<signed char>(number, value); }

		/**
		 * @brief like @c "%hi" in scanf
		 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hi(void* value = nullptr) { return get_i<signed short>(value); }

		/**
		 * @brief like @c "%[number]hi" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hi(unsigned long long number, void* value = nullptr) { return get_i_withnumber<signed short>(number, value); }

		/**
		 * @brief like @c "%i" in scanf
		 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& i(void* value = nullptr) { return get_i<signed int>(value); }

		/**
		 * @brief like @c "%[number]i" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& i(unsigned long long number, void* value = nullptr) { return get_i_withnumber<signed int>(number, value); }

		/**
		 * @brief like @c "%li" in scanf
		 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& li(void* value = nullptr) { return get_i<signed long>(value); }

		/**
		 * @brief like @c "%[number]li" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& li(unsigned long long number, void* value = nullptr) { return get_i_withnumber<signed long>(number, value); }

		/**
		 * @brief like @c "%lli" in scanf
		 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lli(void* value = nullptr) { return get_i<signed long long>(value); }

		/**
		 * @brief like @c "%[number]lli" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lli(unsigned long long number, void* value = nullptr) { return get_i_withnumber<signed long long>(number, value); }

		/**
		 * @brief like @c "%ti" in scanf
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ti(void* value = nullptr) { return get_i<ptrdiff_t>(value); }

		/**
		 * @brief like @c "%[number]ti" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ti(unsigned long long number, void* value = nullptr) { return get_i_withnumber<ptrdiff_t>(number, value); }

		/**
		 * @brief like @c "%zi" in scanf
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zi(void* value = nullptr) { return get_i<size_t>(value); }

		/**
		 * @brief like @c "%[number]zi" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zi(unsigned long long number, void* value = nullptr) { return get_i_withnumber<size_t>(number, value); }

		/**
		 * @brief like @c "%hhu" in scanf
		 * @param value Pointer to a @c unsigned char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhu(void* value = nullptr) { return get_d<unsigned char>(value); }

		/**
		 * @brief like @c "%[number]hhu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c unsigned char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhu(unsigned long long number, void* value = nullptr) { return get_d_withnumber<unsigned char>(number, value); }

		/**
		 * @brief like @c "%hu" in scanf
		 * @param value Pointer to a @c unsigned short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hu(void* value = nullptr) { return get_d<unsigned short>(value); }

		/**
		 * @brief like @c "%[number]hu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c unsigned short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hu(unsigned long long number, void* value = nullptr) { return get_d_withnumber<unsigned short>(number, value); }

		/**
		 * @brief like @c "%u" in scanf
		 * @param value Pointer to a @c unsigned int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& u(void* value = nullptr) { return get_d<unsigned int>(value); }

		/**
		 * @brief like @c "%[number]u" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c unsigned int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& u(unsigned long long number, void* value = nullptr) { return get_d_withnumber<unsigned int>(number, value); }

		/**
		 * @brief like @c "%lu" in scanf
		 * @param value Pointer to a @c unsigned long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lu(void* value = nullptr) { return get_d<unsigned long>(value); }

		/**
		 * @brief like @c "%[number]lu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c unsigned long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lu(unsigned long long number, void* value = nullptr) { return get_d_withnumber<unsigned long>(number, value); }

		/**
		 * @brief like @c "%llu" in scanf
		 * @param value Pointer to a @c unsigned long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& llu(void* value = nullptr) { return get_d<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]llu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c unsigned long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& llu(unsigned long long number, void* value = nullptr) { return get_d_withnumber<unsigned long long>(number, value); }

		/**
		 * @brief like @c "%tu" in scanf
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& tu(void* value = nullptr) { return get_d<ptrdiff_t>(value); }

		/**
		 * @brief like @c "%[number]tu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& tu(unsigned long long number, void* value = nullptr) { return get_d_withnumber<ptrdiff_t>(number, value); }

		/**
		 * @brief like @c "%zu" in scanf
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zu(void* value = nullptr) { return get_d<size_t>(value); }

		/**
		 * @brief like @c "%[number]zu" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zu(unsigned long long number, void* value = nullptr) { return get_d_withnumber<size_t>(number, value); }

		/**
		 * @brief like @c "%hhx" in scanf
		 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhx(void* value = nullptr) { return get_x<unsigned char>(value); }

		/**
		 * @brief like @c "%[number]hhx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhx(unsigned long long number, void* value = nullptr) { return get_x_withnumber<unsigned char>(number, value); }

		/**
		 * @brief like @c "%hx" in scanf
		 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hx(void* value = nullptr) { return get_x<unsigned short>(value); }

		/**
		 * @brief like @c "%[number]hx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hx(unsigned long long number, void* value = nullptr) { return get_x_withnumber<unsigned short>(number, value); }

		/**
		 * @brief like @c "%x" in scanf
		 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& x(void* value = nullptr) { return get_x<unsigned int>(value); }

		/**
		 * @brief like @c "%[number]x" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& x(unsigned long long number, void* value = nullptr) { return get_x_withnumber<unsigned int>(number, value); }

		/**
		 * @brief like @c "%lx" in scanf
		 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lx(void* value = nullptr) { return get_x<unsigned long>(value); }

		/**
		 * @brief like @c "%[number]lx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lx(unsigned long long number, void* value = nullptr) { return get_x_withnumber<unsigned long>(number, value); }

		/**
		 * @brief like @c "%llx" in scanf
		 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& llx(void* value = nullptr) { return get_x<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]llx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& llx(unsigned long long number, void* value = nullptr) { return get_x_withnumber<unsigned long long>(number, value); }

		/**
		 * @brief like @c "%tx" in scanf
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& tx(void* value = nullptr) { return get_x<ptrdiff_t>(value); }

		/**
		 * @brief like @c "%[number]tx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& tx(unsigned long long number, void* value = nullptr) { return get_x_withnumber<ptrdiff_t>(number, value); }

		/**
		 * @brief like @c "%zx" in scanf
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zx(void* value = nullptr) { return get_x<size_t>(value); }

		/**
		 * @brief like @c "%[number]zx" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zx(unsigned long long number, void* value = nullptr) { return get_x_withnumber<size_t>(number, value); }

		/**
		 * @brief like @c "%hho" in scanf
		 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hho(void* value = nullptr) { return get_o<unsigned char>(value); }

		/**
		 * @brief like @c "%[number]hho" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hho(unsigned long long number, void* value = nullptr) { return get_o_withnumber<unsigned char>(number, value); }

		/**
		 * @brief like @c "%ho" in scanf
		 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ho(void* value = nullptr) { return get_o<unsigned short>(value); }

		/**
		 * @brief like @c "%[number]ho" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ho(unsigned long long number, void* value = nullptr) { return get_o_withnumber<unsigned short>(number, value); }

		/**
		 * @brief like @c "%o" in scanf
		 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& o(void* value = nullptr) { return get_o<unsigned int>(value); }

		/**
		 * @brief like @c "%[number]o" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& o(unsigned long long number, void* value = nullptr) { return get_o_withnumber<unsigned int>(number, value); }

		/**
		 * @brief like @c "%lo" in scanf
		 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lo(void* value = nullptr) { return get_o<unsigned long>(value); }

		/**
		 * @brief like @c "%[number]lo" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lo(unsigned long long number, void* value = nullptr) { return get_o_withnumber<unsigned long>(number, value); }

		/**
		 * @brief like @c "%llo" in scanf
		 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& llo(void* value = nullptr) { return get_o<unsigned long long>(value); }

		/**
		 * @brief like @c "%[number]llo" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& llo(unsigned long long number, void* value = nullptr) { return get_o_withnumber<unsigned long long>(number, value); }

		/**
		 * @brief like @c "%to" in scanf
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& to(void* value = nullptr) { return get_o<ptrdiff_t>(value); }

		/**
		 * @brief like @c "%[number]to" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& to(unsigned long long number, void* value = nullptr) { return get_o_withnumber<ptrdiff_t>(number, value); }

		/**
		 * @brief like @c "%zo" in scanf
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zo(void* value = nullptr) { return get_o<size_t>(value); }

		/**
		 * @brief like @c "%[number]zo" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zo(unsigned long long number, void* value = nullptr) { return get_o_withnumber<size_t>(number, value); }

		/**
		 * @brief like @c "%f" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c float variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& f(void* value = nullptr) { return get_f<float>(value); }

		/**
		 * @brief like @c "%[number]f" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c float variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& f(unsigned long long number, void* value = nullptr) { return get_f_withnumber<float>(number, value); }

		/**
		 * @brief like @c "%lf" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c double variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lf(void* value = nullptr) { return get_f<double>(value); }

		/**
		 * @brief like @c "%[number]lf" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c double variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lf(unsigned long long number, void* value = nullptr) { return get_f_withnumber<double>(number, value); }

		/**
		 * @brief like @c "%Lf" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c long double variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& Lf(void* value = nullptr) { return get_f<long double>(value); }

		/**
		 * @brief like @c "%[number]Lf" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c long double variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& Lf(unsigned long long number, void* value = nullptr) { return get_f_withnumber<long double>(number, value); }

		/**
		 * @brief like @c "%c" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a @c char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
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
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[number]c" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& c(unsigned long long number, void* value = nullptr)
		{
			if (!success)
				return *this;
			success = true;
			while (number--)
			{
				int val = sc->_readchar();
				if (val == -1)
				{
					success = false;
					break;
				}
				sc->_nextchar();
				++charsRead;
				if (value)
					*(((char*&)(value))++) = val;
			}
			if (success)
			{
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%s" in scanf
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& s(void* value = nullptr)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
			success = false;
			while (true)
			{
				int val = sc->_readchar();
				if (val == -1 || val == ' ' || val == '\t' || val == '\n' || val == '\v' || val == '\f' || val == '\r')
					break;
				sc->_nextchar();
				++charsRead;
				if (value)
					*(((char*&)(value))++) = val;
				success = true;
			}
			if (success)
			{
				*(((char*&)(value))++) = 0;
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[number]s" in scanf
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& s(unsigned long long number, void* value = nullptr)
		{
			if (!success)
				return *this;
			sc->_skipspace(charsRead);
			success = false;
			while (number)
			{
				int val = sc->_readchar();
				if (val == -1 || val == ' ' || val == '\t' || val == '\n' || val == '\v' || val == '\f' || val == '\r')
					break;
				sc->_nextchar();
				++charsRead;
				--number;
				if (value)
					*(((char*&)(value))++) = val;
				success = true;
			}
			if (success)
			{
				*(((char*&)(value))++) = 0;
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[characters]" in scanf
		 * @param characters Pointer to a string that describe a scanset
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& scanlist(const char* characters, void* value = nullptr)
		{
			if (!success)
				return *this;
			bool scanset[256];
			bool rev = false;
			if (*characters == '^')
			{
				++characters;
				rev = true;
			}
			for (int i = 0; i < 256; ++i)
				scanset[i] = rev;
			int last = -1;
			bool flag = false;
			while (true)
			{
				if (*characters == 0)
				{
					if (last != -1)
						scanset[last] = !rev;
					if (flag)
						scanset['-'] = !rev;
					break;
				}
				int val = (unsigned char)(*characters);
				if (flag)
				{
					for (int i = last; i <= val; ++i)
						scanset[i] = !rev;
					flag = false;
					last = -1;
				}
				else
					if (val == '-' && last != -1)
						flag = true;
					else
					{
						if (last != -1)
							scanset[last] = !rev;
						last = val;
					}
				++characters;
			}
			success = false;
			while (true)
			{
				int val = sc->_readchar();
				if (!scanset[val] || val == -1)
					break;
				sc->_nextchar();
				++charsRead;
				if (value)
					*(((char*&)(value))++) = val;
				success = true;
			}
			if (success)
			{
				*(((char*&)(value))++) = 0;
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[number][characters]" in scanf
		 * @param characters Pointer to a string that describe a scanset
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& scanlist(unsigned long long number, const char* characters, void* value = nullptr)
		{
			if (!success)
				return *this;
			bool scanset[256];
			bool rev = false;
			if (*characters == '^')
			{
				++characters;
				rev = true;
			}
			for (int i = 0; i < 256; ++i)
				scanset[i] = rev;
			int last = -1;
			bool flag = false;
			while (true)
			{
				if (*characters == 0)
				{
					if (last != -1)
						scanset[last] = !rev;
					if (flag)
						scanset['-'] = !rev;
					break;
				}
				int val = (unsigned char)(*characters);
				if (flag)
				{
					for (int i = last; i <= val; ++i)
						scanset[i] = !rev;
					flag = false;
					last = -1;
				}
				else
					if (val == '-' && last != -1)
						flag = true;
					else
					{
						if (last != -1)
							scanset[last] = !rev;
						last = val;
					}
				++characters;
			}
			success = false;
			while (number)
			{
				int val = sc->_readchar();
				if (!scanset[val] || val == -1)
					break;
				sc->_nextchar();
				++charsRead;
				--number;
				if (value)
					*(((char*&)(value))++) = val;
				success = true;
			}
			if (success)
			{
				*(((char*&)(value))++) = 0;
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[characters]" in scanf
		 * @param scanset Pointer to a bool array that describe a scanset
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& scanlist(const bool* scanset, void* value = nullptr)
		{
			if (!success)
				return *this;
			success = false;
			while (true)
			{
				int val = sc->_readchar();
				if (!scanset[val] || val == -1)
					break;
				sc->_nextchar();
				++charsRead;
				if (value)
					*(((char*&)(value))++) = val;
				success = true;
			}
			if (success)
			{
				*(((char*&)(value))++) = 0;
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%[number][characters]" in scanf
		 * @param scanset Pointer to a bool array that describe a scanset
		 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& scanlist(unsigned long long number, const bool* scanset, void* value = nullptr)
		{
			if (!success)
				return *this;
			success = false;
			while (number)
			{
				int val = sc->_readchar();
				if (!scanset[val] || val == -1)
					break;
				sc->_nextchar();
				++charsRead;
				--number;
				if (value)
					*(((char*&)(value))++) = val;
				success = true;
			}
			if (success)
			{
				*(((char*&)(value))++) = 0;
				if (value)
					++res;
			}
			else
				if (sc->eof && res == 0)
					res = -1;
			return *this;
		}

		/**
		 * @brief like @c "%p" in scanf
		 * @param value Pointer to a void pointer variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& p(void* value = nullptr) { return get_x<size_t>(value); }

		/**
		 * @brief like @c "%[number]p" in scanf
		 * @param number The maximum number of characters to read
		 * @param value Pointer to a void pointer variable or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& p(unsigned long long number, void* value = nullptr) { return get_x_withnumber<size_t>(number, value); }

		/**
		 * @brief like @c "%hhn" in scanf
		 * @param value Pointer to an @c signed char variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hhn(void* value = nullptr) { return get_n<signed char>(value); }

		/**
		 * @brief like @c "%hn" in scanf
		 * @param value Pointer to an @c signed short variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& hn(void* value = nullptr) { return get_n<signed short>(value); }

		/**
		 * @brief like @c "%n" in scanf
		 * @param value Pointer to an @c signed int variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& n(void* value = nullptr) { return get_n<signed int>(value); }

		/**
		 * @brief like @c "%ln" in scanf
		 * @param value Pointer to an @c signed long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& ln(void* value = nullptr) { return get_n<signed long>(value); }

		/**
		 * @brief like @c "%lln" in scanf
		 * @param value Pointer to an @c signed long long variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& lln(void* value = nullptr) { return get_n<signed long long>(value); }

		/**
		 * @brief like @c "%tn" in scanf
		 * @param value Pointer to an @c signed ptrdiff_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& tn(void* value = nullptr) { return get_n<ptrdiff_t>(value); }

		/**
		 * @brief like @c "%zn" in scanf
		 * @param value Pointer to an @c signed size_t variables or @c nullptr if you don't want to read the value
		 * @return a scanner stream
		 */
		scanner_stream& zn(void* value = nullptr) { return get_n<size_t>(value); }
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
	template<typename readType>
	scanner(readType read) : read(new functionImpl<readType>(read)), begin(0), end(0), eof(false) {}

	~scanner()
	{
		delete read;
	}

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
	 * @return Number of successful matches or -1 if end of file is reached before any matches
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
	 * @return Number of successful matches or -1 if end of file is reached before any matches
	 * @note This function is provided for convenience and is equivalent to calling @c scanf with
	 */
	int operator()(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		return _scanf(format, args);
	}

	/**
	 * @brief like @c "%hhd" in scanf
	 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhd(void* value = nullptr)
	{
		return scanner_stream(this).hhd(value);
	}

	/**
	 * @brief like @c "%[number]hhd" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhd(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hhd(number, value);
	}

	/**
	 * @brief like @c "%hd" in scanf
	 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hd(void* value = nullptr)
	{
		return scanner_stream(this).hd(value);
	}

	/**
	 * @brief like @c "%[number]hd" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hd(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hd(number, value);
	}

	/**
	 * @brief like @c "%d" in scanf
	 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream d(void* value = nullptr)
	{
		return scanner_stream(this).d(value);
	}

	/**
	 * @brief like @c "%[number]d" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream d(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).d(number, value);
	}

	/**
	 * @brief like @c "%ld" in scanf
	 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream ld(void* value = nullptr)
	{
		return scanner_stream(this).ld(value);
	}

	/**
	 * @brief like @c "%[number]ld" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream ld(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).ld(number, value);
	}

	/**
	 * @brief like @c "%lld" in scanf
	 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lld(void* value = nullptr)
	{
		return scanner_stream(this).lld(value);
	}

	/**
	 * @brief like @c "%[number]lld" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lld(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).lld(number, value);
	}

	/**
	 * @brief like @c "%td" in scanf
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream td(void* value = nullptr)
	{
		return scanner_stream(this).td(value);
	}

	/**
	 * @brief like @c "%[number]td" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream td(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).td(number, value);
	}

	/**
	 * @brief like @c "%zd" in scanf
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zd(void* value = nullptr)
	{
		return scanner_stream(this).zd(value);
	}

	/**
	 * @brief like @c "%[number]zd" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zd(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).zd(number, value);
	}

	/**
	 * @brief like @c "%hhi" in scanf
	 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhi(void* value = nullptr)
	{
		return scanner_stream(this).hhi(value);
	}

	/**
	 * @brief like @c "%[number]hhi" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhi(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hhi(number, value);
	}

	/**
	 * @brief like @c "%hi" in scanf
	 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hi(void* value = nullptr)
	{
		return scanner_stream(this).hi(value);
	}

	/**
	 * @brief like @c "%[number]hi" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hi(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hi(number, value);
	}

	/**
	 * @brief like @c "%i" in scanf
	 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream i(void* value = nullptr)
	{
		return scanner_stream(this).i(value);
	}

	/**
	 * @brief like @c "%[number]i" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream i(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).i(number, value);
	}

	/**
	 * @brief like @c "%li" in scanf
	 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream li(void* value = nullptr)
	{
		return scanner_stream(this).li(value);
	}

	/**
	 * @brief like @c "%[number]li" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream li(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).li(number, value);
	}

	/**
	 * @brief like @c "%lli" in scanf
	 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lli(void* value = nullptr)
	{
		return scanner_stream(this).lli(value);
	}

	/**
	 * @brief like @c "%[number]lli" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lli(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).lli(number, value);
	}

	/**
	 * @brief like @c "%ti" in scanf
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream ti(void* value = nullptr)
	{
		return scanner_stream(this).ti(value);
	}

	/**
	 * @brief like @c "%[number]ti" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream ti(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).ti(number, value);
	}

	/**
	 * @brief like @c "%zi" in scanf
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zi(void* value = nullptr)
	{
		return scanner_stream(this).zi(value);
	}

	/**
	 * @brief like @c "%[number]zi" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zi(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).zi(number, value);
	}

	/**
	 * @brief like @c "%hhu" in scanf
	 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhu(void* value = nullptr)
	{
		return scanner_stream(this).hhu(value);
	}

	/**
	 * @brief like @c "%[number]hhu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhu(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hhu(number, value);
	}

	/**
	 * @brief like @c "%hu" in scanf
	 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hu(void* value = nullptr)
	{
		return scanner_stream(this).hu(value);
	}

	/**
	 * @brief like @c "%[number]hu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hu(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hu(number, value);
	}

	/**
	 * @brief like @c "%u" in scanf
	 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream u(void* value = nullptr)
	{
		return scanner_stream(this).u(value);
	}

	/**
	 * @brief like @c "%[number]u" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream u(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).u(number, value);
	}

	/**
	 * @brief like @c "%lu" in scanf
	 * @param value Pointer to a @c unsigned long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lu(void* value = nullptr)
	{
		return scanner_stream(this).lu(value);
	}

	/**
	 * @brief like @c "%[number]lu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c unsigned long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lu(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).lu(number, value);
	}

	/**
	 * @brief like @c "%llu" in scanf
	 * @param value Pointer to a @c unsigned long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream llu(void* value = nullptr)
	{
		return scanner_stream(this).llu(value);
	}

	/**
	 * @brief like @c "%[number]llu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c unsigned long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream llu(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).llu(number, value);
	}

	/**
	 * @brief like @c "%tu" in scanf
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream tu(void* value = nullptr)
	{
		return scanner_stream(this).tu(value);
	}

	/**
	 * @brief like @c "%[number]tu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream tu(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).tu(number, value);
	}

	/**
	 * @brief like @c "%zu" in scanf
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zu(void* value = nullptr)
	{
		return scanner_stream(this).zu(value);
	}

	/**
	 * @brief like @c "%[number]zu" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zu(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).zu(number, value);
	}

	/**
	 * @brief like @c "%hhx" in scanf
	 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhx(void* value = nullptr)
	{
		return scanner_stream(this).hhx(value);
	}

	/**
	 * @brief like @c "%[number]hhx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hhx(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hhx(number, value);
	}

	/**
	 * @brief like @c "%hx" in scanf
	 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hx(void* value = nullptr)
	{
		return scanner_stream(this).hx(value);
	}

	/**
	 * @brief like @c "%[number]hx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hx(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hx(number, value);
	}

	/**
	 * @brief like @c "%x" in scanf
	 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream x(void* value = nullptr)
	{
		return scanner_stream(this).x(value);
	}

	/**
	 * @brief like @c "%[number]x" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream x(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).x(number, value);
	}

	/**
	 * @brief like @c "%lx" in scanf
	 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lx(void* value = nullptr)
	{
		return scanner_stream(this).lx(value);
	}

	/**
	 * @brief like @c "%[number]lx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lx(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).lx(number, value);
	}

	/**
	 * @brief like @c "%llx" in scanf
	 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream llx(void* value = nullptr)
	{
		return scanner_stream(this).llx(value);
	}

	/**
	 * @brief like @c "%[number]llx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream llx(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).llx(number, value);
	}

	/**
	 * @brief like @c "%tx" in scanf
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream tx(void* value = nullptr)
	{
		return scanner_stream(this).tx(value);
	}

	/**
	 * @brief like @c "%[number]tx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream tx(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).tx(number, value);
	}

	/**
	 * @brief like @c "%zx" in scanf
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zx(void* value = nullptr)
	{
		return scanner_stream(this).zx(value);
	}

	/**
	 * @brief like @c "%[number]zx" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zx(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).zx(number, value);
	}

	/**
	 * @brief like @c "%hho" in scanf
	 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hho(void* value = nullptr)
	{
		return scanner_stream(this).hho(value);
	}

	/**
	 * @brief like @c "%[number]hho" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream hho(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).hho(number, value);
	}

	/**
	 * @brief like @c "%ho" in scanf
	 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream ho(void* value = nullptr)
	{
		return scanner_stream(this).ho(value);
	}

	/**
	 * @brief like @c "%[number]ho" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream ho(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).ho(number, value);
	}

	/**
	 * @brief like @c "%o" in scanf
	 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream o(void* value = nullptr)
	{
		return scanner_stream(this).o(value);
	}

	/**
	 * @brief like @c "%[number]o" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream o(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).o(number, value);
	}

	/**
	 * @brief like @c "%lo" in scanf
	 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lo(void* value = nullptr)
	{
		return scanner_stream(this).lo(value);
	}

	/**
	 * @brief like @c "%[number]lo" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lo(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).lo(number, value);
	}

	/**
	 * @brief like @c "%llo" in scanf
	 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream llo(void* value = nullptr)
	{
		return scanner_stream(this).llo(value);
	}

	/**
	 * @brief like @c "%[number]llo" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to an @c unsigned long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream llo(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).llo(number, value);
	}

	/**
	 * @brief like @c "%to" in scanf
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream to(void* value = nullptr)
	{
		return scanner_stream(this).to(value);
	}

	/**
	 * @brief like @c "%[number]to" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream to(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).to(number, value);
	}

	/**
	 * @brief like @c "%zo" in scanf
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zo(void* value = nullptr)
	{
		return scanner_stream(this).zo(value);
	}

	/**
	 * @brief like @c "%[number]zo" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream zo(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).zo(number, value);
	}

	/**
	 * @brief like @c "%f" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c float variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream f(void* value = nullptr)
	{
		return scanner_stream(this).f(value);
	}

	/**
	 * @brief like @c "%[number]f" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c float variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream f(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).f(number, value);
	}

	/**
	 * @brief like @c "%lf" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c double variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lf(void* value = nullptr)
	{
		return scanner_stream(this).lf(value);
	}

	/**
	 * @brief like @c "%[number]lf" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c double variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream lf(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).lf(number, value);
	}

	/**
	 * @brief like @c "%Lf" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c long double variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream Lf(void* value = nullptr)
	{
		return scanner_stream(this).Lf(value);
	}

	/**
	 * @brief like @c "%[number]Lf" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c long double variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream Lf(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).Lf(number, value);
	}

	/**
	 * @brief like @c "%c" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a @c char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream c(void* value = nullptr)
	{
		return scanner_stream(this).c(value);
	}

	/**
	 * @brief like @c "%[number]c" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream c(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).c(number, value);
	}

	/**
	 * @brief like @c "%s" in scanf
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream s(void* value = nullptr)
	{
		return scanner_stream(this).s(value);
	}

	/**
	 * @brief like @c "%[number]s" in scanf
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream s(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).s(number, value);
	}

	/**
	 * @brief like @c "%[characters]" in scanf
	 * @param characters Pointer to a string that describe a scanset
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream scanlist(const char* characters, void* value = nullptr)
	{
		return scanner_stream(this).scanlist(characters, value);
	}

	/**
	 * @brief like @c "%[number][characters]" in scanf
	 * @param characters Pointer to a string that describe a scanset
	 * @param value Pointer to a string variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream scanlist(unsigned long long number, const char* characters, void* value = nullptr)
	{
		return scanner_stream(this).scanlist(number, characters, value);
	}

	/**
	 * @brief like @c "%p" in scanf
	 * @param value Pointer to a void pointer variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream p(void* value = nullptr)
	{
		return scanner_stream(this).p(value);
	}

	/**
	 * @brief like @c "%[number]p" in scanf
	 * @param number The maximum number of characters to read
	 * @param value Pointer to a void pointer variable or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream p(unsigned long long number, void* value = nullptr)
	{
		return scanner_stream(this).p(number, value);
	}

	/**
	 * @brief like @c "%hhn" in scanf
	 * @param value Pointer to a @c signed char variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& hhn(void* value = nullptr)
	{
		return scanner_stream(this).hhn(value);
	}

	/**
	 * @brief like @c "%hn" in scanf
	 * @param value Pointer to a @c signed short variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& hn(void* value = nullptr)
	{
		return scanner_stream(this).hn(value);
	}

	/**
	 * @brief like @c "%n" in scanf
	 * @param value Pointer to a @c signed int variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& n(void* value = nullptr)
	{
		return scanner_stream(this).n(value);
	}

	/**
	 * @brief like @c "%ln" in scanf
	 * @param value Pointer to a @c signed long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& ln(void* value = nullptr)
	{
		return scanner_stream(this).ln(value);
	}

	/**
	 * @brief like @c "%lln" in scanf
	 * @param value Pointer to a @c signed long long variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& lln(void* value = nullptr)
	{
		return scanner_stream(this).lln(value);
	}

	/**
	 * @brief like @c "%tn" in scanf
	 * @param value Pointer to a @c ptrdiff_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& tn(void* value = nullptr)
	{
		return scanner_stream(this).tn(value);
	}

	/**
	 * @brief like @c "%zn" in scanf
	 * @param value Pointer to a @c size_t variables or @c nullptr if you don't want to read the value
	 * @return a scanner stream
	 */
	scanner_stream& zn(void* value = nullptr)
	{
		return scanner_stream(this).zn(value);
	}
};

#endif