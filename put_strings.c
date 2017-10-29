#include "ft_printf.h"

void			print_char(t_flags *flags, va_list *arg, int base)
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
	while (!(flags->flag & MINUS) && (spaces-- > 0))
		ft_putchar_mem(flags, ' ');
	print_sign(flags, nbr);
	while (zeroes-- > 0)
		ft_putchar_mem(flags, '0');
	(flags->data_type == 8) ? ft_putchar_mem(flags, nbr) :
		ft_putnbr_mem(flags, nbr, base);
	while (flags->flag & MINUS && (spaces-- > 0))
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
