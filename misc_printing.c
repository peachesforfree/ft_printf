#include "ft_printf.h"

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
		if ((flags->flag & HASH && flags->data_type == 6)
				|| flags->data_type == 7)
			zeroes = zeroes - 2;
		else if (flags->flag & HASH && flags->data_type == 3)
			zeroes--;
	}
	return (zeroes);
}

void		print_prefix(t_flags *flags, uintmax_t nbr)
{
	if ((flags->flag & HASH && nbr != 0) || flags->data_type == 7)
	{
		if (flags->data_type == 3 || flags->data_type == 6
				|| flags->data_type == 7)
			ft_putchar_mem(flags, '0');
		if (flags->data_type == 6 || flags->data_type == 7)
			(flags->caps == true) ?
				ft_putchar_mem(flags, 'X') : ft_putchar_mem(flags, 'x');
	}
}