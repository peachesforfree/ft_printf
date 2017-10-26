#include "ft_printf.h"

/*
**From unsigned long long (unitmax_t) and base
**returns the number of digits to print
*/

int			ft_numlen_ull(uintmax_t value, unsigned int base)
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

/*
**From long long (intmax_t) and base given
**Returns number of digits to print  
*/

int			ft_numlen_ll(intmax_t value, int base)
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

/*
**From long long (intmax_t) and power
*/

intmax_t	ft_power_ll(intmax_t nbr, intmax_t power)
{
	intmax_t	tmp_nbr;

	tmp_nbr = nbr;
	if (power == 0)
		return (1);
	while (power-- > 1)
		nbr *= tmp_nbr;
	return (nbr);
}

/*
**
*/

uintmax_t	ft_power_ull(uintmax_t nbr, uintmax_t power)
{
	uintmax_t tmp_nbr;

	tmp_nbr = nbr;
	if (power == 0)
		return (1);
	while (power-- > 1)
		nbr *= tmp_nbr;
	return (nbr);
}

/*
**
*/

void		print_sign(t_flags *flags, intmax_t nbr)
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