#include "ft_printf.h"

/*
**Dispactch table use to start printing process of each
**variable to be pulled from va_list and printed.
*/

void				(*g_func_sort[127]) (t_flags *flags, va_list *arg, int base) =
{
	print_char,
	print_signed,
	print_unsigned,
	print_string,
	print_unsigned,
	print_unsigned,
	print_unsigned,
	print_char
};

/*
**	This is ft_printf, dont confuse this with printf.
**	This function first runs through the format string and prints it
**	Once % appears it will check for . #0-+ flags
**	Checks for '.' and a number, looking for width precision
**	Checks for h, hh, l, ll, j, z  length specifiers
**	checks for sSpdDioOuUxXcC for incoming datatype
**		result is input to dispatch table for pulling & printing the variable
*/

int			ft_printf(const char *format, ...)
{
	t_flags flags;
	va_list arg;

	va_start(arg, format);
	flags.written_chars = 0;
	flags.index = 0;
	while (format[flags.index])
	{
		print_format_string(format, &flags);
		check_flags(format, &flags);
		check_width_precision(format, &flags);
		check_length(format, &flags);
		if ((flags.data_type = check_conversion(format[flags.index], &flags)))
			g_func_sort[flags.data_type](&flags, &arg, flags.base);
		if (format[flags.index])
			flags.index++;
	}
	va_end(arg);
	return (flags.written_chars);
}

/*
** Prints formats string leading up to & handles %%
*/

void		print_format_string(const char *format, t_flags *flags)
{
	while (format[flags->index] != '%' && format[flags->index])
		ft_putchar_mem(flags, format[flags->index++]);
	if (format[flags->index] && format[flags->index] == '%')
		flags->index++;
}

/*
**	Handle precision
**	- justify left
**	+ places and forces a - + sign
**	' ' blank space padding
**	# with types oxX will place 0x infront
**	0 left padds number with zeros
**	for precision
*/

void		check_flags(const char *format, t_flags *flags)
{
	flags->flag = 0b0000000;
	while (format[flags->index] == '-' || format[flags->index] == '+'
			|| format[flags->index] == ' ' || format[flags->index] == '#'
			|| format[flags->index] == '0')
	{
		if (format[flags->index] == '-' &&
				check_dubs(&format[flags->index], '-', flags))
			flags->flag += MINUS;
		else if (format[flags->index] == '+' &&
				check_dubs(&format[flags->index], '+', flags))
			flags->flag += PLUS;
		else if (format[flags->index] == ' ' &&
				check_dubs(&format[flags->index], ' ', flags))
			flags->flag += SPACE;
		else if (format[flags->index] == '#'
				&& check_dubs(&format[flags->index], '#', flags))
			flags->flag += HASH;
		else if (format[flags->index] == '0' &&
				check_dubs(&format[flags->index], '0', flags))
			flags->flag += ZERO;
		flags->index++;
	}
}

/*
**This is to make sure the flag is accounted forces
**as well all occurences of the same flag are forwarded past
*/

int			check_dubs(const char *format, char c, t_flags *flags)
{
	while (format[flags->index] == c)
	{
		flags->index++;
		if (format[flags->index] == c)
			return (0);
	}
	return (1);
}