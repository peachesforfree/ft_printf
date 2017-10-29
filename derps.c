#include "ft_printf.h"

int main(void)
{
	ft_printf("% ++d", 42);
	write(1, "\n", 1);
	return (0);
}
