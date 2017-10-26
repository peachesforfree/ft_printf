#include "ft_printf.h"

/*
**	This prints out a signed number
*/

void			ft_putnbr_mem(t_flags *flags, intmax_t nbr, int base)
{
	int			isneg;
	uintmax_t	power;

	isneg = (nbr < 0) ? -1 : 1;
	power = ft_power_ll(base, ft_numlen_ll(nbr, base));//why is this power shit here ?
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

/*
**	This prints out an unsigned number
*/

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
		if (flags->precision != 0 || (flags->flag & HASH
					&& flags->data_type == 3))
			ft_putchar_mem(flags, '0');
		power = power / base;
	}
}

/*
**	Sub functions to assist with conversion functions
**	this is also for printing out stuff from argv()
*/

void			ft_putchar_mem(t_flags *flags, char c)
{
	write(1, &c, 1);
	flags->written_chars++;
}