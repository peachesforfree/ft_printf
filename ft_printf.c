#include "ft_printf.h"


void			(*func_sort[127])(t_flags *flags, va_list *arg, int base) =
{
	[1] = print_char,
	[2] = print_signed,
	[3] = print_unsigned,
	[4] = print_string,
	[5] = print_unsigned,
	[6] = print_unsigned,
	[7] = print_unsigned,
	[8] = print_char
};


int					ft_printf(const char *format, ...)
{
	t_flags flags;
	va_list arg;

	va_start(arg, format);
	flags.written_chars = 0;
	flags.index = 0;
	while (format[flags.index])
	{
		print_format_string(format, &flags);		//this prints the format string until the first %
		check_flags(format, &flags);				//reads the format string for justification flags '. #0-+'
		check_width_precision(format, &flags);		// This checks for a '.' and or a number.
		check_length(format, &flags);				//this checks length modifiers h, hh, l, ll, j, z
		//this checks the conversion indicators (sSpdDioOuUxXcC)
		if ((flags.data_type = check_conversion(format[flags.index], &flags)))	//if there is a conversion indicator we parse_args()
			(func_sort[flags.data_type])(&flags, &arg, flags.base);
			//parse_args(&flags, &arg, conv);						// this parses the va_list for the data we need
		if (format[flags.index])										//if still existing, iterate again to finish off the format string
			flags.index++;
	}
	va_end(arg);											//va_list must be closed
	return (flags.written_chars);							// return number of chars printed
}

void		print_format_string(const char *format, t_flags *flags)	// this will print the format string up to '%'
{
	while (format[flags->index] != '%' && format[flags->index])
		ft_putchar_mem(flags, format[flags->index++]);
	if (format[flags->index] && format[flags->index] == '%')							//in the event we have %% a single % is printed, ther first % is an escape char
		flags->index++;
}

void			check_flags(const char *format, t_flags *flags) // this keeps track of the justification flags
{
	flags->flag = 0b0000000;
	while (format[flags->index] == '-' || format[flags->index] == '+' || format[flags->index] == ' ' ||
		format[flags->index] == '#' || format[flags->index] == '0')
	{
		if (format[flags->index] == '-' && check_dubs(&format[flags->index], '-', flags))                  // keeping track of flag with binary descriptors.
			flags->flag += 0b00000001;
		else if (format[flags->index] == '+' && check_dubs(&format[flags->index], '+', flags))
			flags->flag += 0b00000010;
		else if (format[flags->index] == ' ' && check_dubs(&format[flags->index], ' ', flags))
			flags->flag += 0b00000100;
		else if (format[flags->index] == '#' && check_dubs(&format[flags->index], '#', flags))
			flags->flag += 0b00001000;
		else if (format[flags->index] == '0' && check_dubs(&format[flags->index], '0', flags))
			flags->flag += 0b00010000;
		flags->index++;
	}
}

int					check_dubs(const char *format, char c, t_flags *flags)          // make this a defines "-+ #0" and a ft_strchr statement when called.
{
	while (format[flags->index] == '-' || format[flags->index] == '+' || format[flags->index] == ' ' ||
		format[flags->index] == '#' || format[flags->index] == '0')
	{
		flags->index++;
		if (format[flags->index] == c)
			return (0);
	}
	return (1);
}

void				check_width_precision(const char *format, t_flags *flags) //this handles '.' precision and numberic width
{
	flags->width = 0;
	flags->precision = -1;
	if (ft_isdigit(format[flags->index]))												// here width is handled
		flags->width = ft_atoi(&format[flags->index]);
	while (flags->width != 0 && ft_isdigit(format[flags->index]))						//this skips through the numbers that set width
		flags->index++;
	if (format[flags->index] == '.')															//handle precision 
	{
		flags->precision = 0;
		flags->index++;
	}
	if (ft_isdigit(format[flags->index]))												//reading the number for precision
		flags->precision = ft_atoi(&format[flags->index]);
	while (flags->precision != -1 && ft_isdigit(format[flags->index]))					//forwards thru the numbers in format string
		flags->index++;													//returns the new place in format string
}

void			check_length(const char *format, t_flags *flags)               //sets numeric number code for each length specifier "hh, h, l, ll, j, z"
{	
	char *str;
	str = (char*)format;																				// maybe make this a jump table?
	flags->length = 0;
	while (str[flags->index] == 'h' || str[flags->index] == 'l' || str[flags->index] == 'j' || str[flags->index] == 'z')
	{
		if (str[flags->index] == 'h' && str[flags->index + 1] == 'h')
			flags->length = 1;
		else if (str[flags->index] == 'h' && str[flags->index + 1] != 'h')
			flags->length = 2;
		else if (str[flags->index] == 'l' && str[flags->index + 1] == 'l')
			flags->length = 3;
		else if (str[flags->index] == 'l' && str[flags->index + 1] != 'l')
			flags->length = 4;
		else if (str[flags->index] == 'j')
			flags->length = 5;
		else if (str[flags->index] == 'z')
			flags->length = 6;
		if (flags->length == 1 || flags->length == 3)
			flags->index++;
		if (flags->length != 0)
			flags->index++;
	}
}

int				check_conversion(char format, t_flags *flags) // checks for conversion flag "sSpdDioOuUxXcC" then sets number code for each one
{ 
	int i;
	flags->base = 10;                                       // maybe this can be made into a jump table ?
	i = 0;
	if (format == 'c' || format == 'C')
		i = 1;
	else if (format == 'd' || format == 'i' || format == 'D')
		i = 2;
	else if (format == 'o' || format == 'O')
	{
		flags->base = 8;
		i = 3;
	}
	else if (format == 's' || format == 'S')
		i = 4;
	else if (format == 'u' || format == 'U')
		i = 5;
	else if (format == 'x' || format == 'X')
	{
		flags->base = 16;
		i = 6;
	}
	else if (format == 'p')
	{
		flags->base = 16;
		i = 7;
	}
	else if (format)
	{
		flags->invalid_spec = format;
		i = 8;
	}
	flags->caps = (format >= 'A' && format <= 'Z') ? true : false;
	return (i);
}

void			print_unsigned(t_flags *flags, va_list *arg, int base)
{
	uintmax_t	nbr;
	int			zeroes;
	int			spaces;

	spaces = 0;
	nbr = va_arg(*(arg), uintmax_t);
	nbr = typecast_unum(flags, nbr);
	zeroes = zeroes_unsigned(flags, nbr, base);
	spaces = flags->width - ft_numlen_ull(nbr, base) - zeroes;
	if (flags->flag & 1000 && flags->data_type == 3)
		zeroes--;
	if (nbr == 0 && flags->precision == 0)
		spaces++;
	if ((flags->flag & HASH && flags->data_type == 6) || flags->data_type == 7)
		spaces = spaces - 2;
	else if (flags->flag & HASH && flags->data_type == 3)
		spaces--;
	while (!(flags->flag & MINUS) && spaces-- > 0)
		ft_putchar_mem(flags, ' ');
	print_prefix(flags, nbr);
	while (zeroes-- > 0)
		ft_putchar_mem(flags, '0');
	ft_putunbr_mem(flags, nbr, base);
	while (flags->flag & MINUS && spaces-- > 0)
		ft_putchar_mem(flags, ' ');
}
/*
void				parse_args(t_flags *flags, va_list *arg, int conv)	// reads conversion flag and sends it to proper va_list function
{																// make this a jump table print_char(flags,arg, 0) for uniformity
	if (conv == 1 || conv == 8)
		print_char(flags, arg);
	else if (conv == 2)
		print_signed(flags, arg, 10);
	else if (conv == 3)
		print_unsigned(flags, arg, 8);
	else if (conv == 4)
		print_string(flags, arg);
	else if (conv == 6 || conv == 7)
		print_unsigned(flags, arg, 16);
	else if (conv == 5)
		print_unsigned(flags, arg, 10);
}
*/

/*
Below
all conversions are made from va_list
*/

void			print_char(t_flags *flags, va_list *arg, int base)			//prints chars
{
	char	c;
	char	buffer;

	(void)base;
	buffer = (flags->data_type == 8 && flags->flag & ZERO) ? '0' : ' ';
	if (flags->data_type != 8)
		c = va_arg(*(arg), int);
	else
		c = flags->invalid_spec;
	if (flags->flag & MINUS)
		ft_putchar_mem(flags, c);
	while (flags->width-- > 1)
		ft_putchar_mem(flags, buffer);
	if (!(flags->flag & MINUS))
		ft_putchar_mem(flags, c);
}

void			print_signed(t_flags *flags, va_list *arg, int base)
{
	intmax_t	nbr;
	int			zeroes;
	int			spaces;

	spaces = 0;
	nbr = va_arg(*(arg), intmax_t);
	nbr = typecast_num(flags, nbr);
	zeroes = zeroes_signed(flags, nbr, base);
	spaces = flags->width - ft_numlen_ll(nbr, base) - zeroes;
	if (nbr == 0 && flags->precision == 0)
		spaces++;
	if (nbr < 0 || flags->flag & PLUS || flags->flag & SPACE)
		spaces--;
	while (!(flags->flag & MINUS) && spaces-- > 0)
		ft_putchar_mem(flags, ' ');
	print_sign(flags, nbr);
	while (zeroes-- > 0)
		ft_putchar_mem(flags, '0');
	(flags->data_type == 8) ? ft_putchar_mem(flags, nbr) :
		ft_putnbr_mem(flags, nbr, base);
	while (flags->flag & MINUS && spaces-- > 0)
		ft_putchar_mem(flags, ' ');
}

void			print_string(t_flags *flags, va_list *arg, int base)
{
	int		i;
	char	*string;
	int		spaces;
	int		length;

	(void)base;
	i = 0;
	string = va_arg(*(arg), char *);
	if (!string)
		string = "(null)";
	length = (int)ft_strlen(string);
	spaces = flags->width - length;
	if (flags->precision < length && flags->precision != -1)
		spaces = spaces - (flags->precision - length);
	while (!(flags->flag & MINUS) && spaces-- > 0)
		ft_putchar_mem(flags, ' ');
	while (string[i] != '\0')
	{
		if (flags->precision != -1)
			if ((i + 1) > flags->precision)
				break ;
		ft_putchar_mem(flags, string[i++]);
	}
	while (flags->flag & MINUS && spaces-- > 0)
		ft_putchar_mem(flags, ' ');
}

/*

Sub functions to assist with conversion functions
this is also for printing out stuff from argv()

*/

void			ft_putchar_mem(t_flags *flags, char c)
{
	write(1, &c, 1);
	flags->written_chars++;
}

void			ft_putnbr_mem(t_flags *flags, intmax_t nbr, int base)
{
	int			isneg;
	uintmax_t	power;

	isneg = (nbr < 0) ? -1 : 1;
	power = ft_power_ll(base, ft_numlen_ll(nbr, base));
	power = power / base;
	while (nbr != 0 && power != 0)
	{
		ft_putchar_mem(flags, ((nbr * isneg) / power) + '0');
		nbr = nbr - isneg * ((nbr * isneg) / power) * power;
		power = power / base;
	}
	while (nbr == 0 && power > 0)
	{
		if (flags->precision != 0)
			ft_putchar_mem(flags, '0');
		power = power / base;
	}
}

void			ft_putunbr_mem(t_flags *flags, uintmax_t nbr, int base)
{
	int			alpha;
	uintmax_t	power;

	alpha = (flags->caps == true) ? '7' : 'W';
	power = ft_power_ull(base, (ft_numlen_ull(nbr, base) - 1));
	while (nbr != 0 && power != 0)
	{
		if ((nbr / power) > 9)
			ft_putchar_mem(flags, (nbr / power) + alpha);
		else
			ft_putchar_mem(flags, (nbr / power) + '0');
		nbr = nbr - (nbr / power) * power;
		power = power / base;
	}
	while (nbr == 0 && power > 0)
	{
		if (flags->precision != 0 || (flags->flag & HASH && flags->data_type == 3))
			ft_putchar_mem(flags, '0');
		power = power / base;
	}
}

intmax_t			typecast_num(t_flags *flags, intmax_t nbr)
{
	if (flags->length == 4 || (flags->caps == true && flags->data_type == 2))
		nbr = (long int)nbr;
	else if (flags->length == 0)
		nbr = (int)nbr;
	else if (flags->length == 1)
		nbr = (signed char)nbr;
	else if (flags->length == 2)
		nbr = (short int)nbr;
	else if (flags->length == 6)
		nbr = (size_t)nbr;
	return (nbr);
}

uintmax_t			typecast_unum(t_flags *flags, uintmax_t nbr)
{
	if (flags->data_type == 7)
		return (nbr);
	if (flags->length == 4 || (flags->caps == true && flags->data_type != 6))
		nbr = (unsigned long)nbr;
	else if (flags->length == 0)
		nbr = (unsigned int)nbr;
	else if (flags->length == 1)
		nbr = (unsigned char)nbr;
	else if (flags->length == 2)
		nbr = (unsigned short)nbr;
	else if (flags->length == 6)
		nbr = (size_t)nbr;
	return (nbr);
}


int				zeroes_signed(t_flags *flags, intmax_t nbr, int base)
{
	int zeroes;

	zeroes = 0;
	if (flags->precision >= ft_numlen_ll(nbr, base))
		zeroes = flags->precision - ft_numlen_ll(nbr, base);
	else if (flags->flag & ZERO && flags->width > 0 && !(flags->flag & MINUS))
	{
		zeroes = flags->width - ft_numlen_ll(nbr, base);
		if (nbr < 0 || flags->flag & PLUS || flags->flag & SPACE)
			zeroes--;
	}
	return (zeroes);
}

int				zeroes_unsigned(t_flags *flags, uintmax_t nbr, int base)
{
	int zeroes;

	zeroes = 0;
	if (flags->precision >= ft_numlen_ull(nbr, base))
		zeroes = flags->precision - ft_numlen_ull(nbr, base);
	else if (flags->flag & ZERO && flags->width > 0 && !(flags->flag & MINUS))
	{
		zeroes = flags->width - ft_numlen_ull(nbr, base);
		if ((flags->flag & HASH && flags->data_type == 6) || flags->data_type == 7)
			zeroes = zeroes - 2;
		else if (flags->flag & HASH && flags->data_type == 3)
			zeroes--;
	}
	return (zeroes);
}


int						ft_numlen_ull(uintmax_t value, unsigned int base)
{
	int		len;

	len = (value == 0) ? 1 : 0;
	while (value != 0)
	{
		value = value / base;
		len++;
	}
	return (len);
}

int						ft_numlen_ll(intmax_t value, int base)
{
	int		len;

	len = (value == 0) ? 1 : 0;
	while (value != 0)
	{
		value = value / base;
		len++;
	}
	return (len);
}

intmax_t				ft_power_ll(intmax_t nbr, intmax_t power)
{
	intmax_t	tmp_nbr;

	tmp_nbr = nbr;
	if (power == 0)
		return (1);
	while (power-- > 1)
		nbr *= tmp_nbr;
	return (nbr);
}

uintmax_t					ft_power_ull(uintmax_t nbr, uintmax_t power)
{
	uintmax_t tmp_nbr;

	tmp_nbr = nbr;
	if (power == 0)
		return (1);
	while (power-- > 1)
		nbr *= tmp_nbr;
	return (nbr);
}

void			print_sign(t_flags *flags, intmax_t nbr)
{
	if (nbr < 0)
	{
		ft_putchar('-');
		flags->written_chars++;
	}
	else if (flags->flag & PLUS && nbr >= 0)
	{
		ft_putchar('+');
		flags->written_chars++;
	}
	else if (!(flags->flag & PLUS) && flags->flag & SPACE && nbr >= 0)
	{
		ft_putchar(' ');
		flags->written_chars++;
	}
}

void			print_prefix(t_flags *flags, uintmax_t nbr)
{
	if ((flags->flag & HASH && nbr != 0) || flags->data_type == 7)
	{
		if (flags->data_type == 3 || flags->data_type == 6 || flags->data_type == 7)
			ft_putchar_mem(flags, '0');
		if (flags->data_type == 6 || flags->data_type == 7)
			(flags->caps == true) ?
				ft_putchar_mem(flags, 'X') : ft_putchar_mem(flags, 'x');
	}
}
