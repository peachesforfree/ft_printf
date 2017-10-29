#include "ft_printf.h"

int main(void)
{
	ft_printf("%      i", 42);
	write(1, "\n", 1);
	return (0);
}
