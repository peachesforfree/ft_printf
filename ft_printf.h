#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdint.h>

# define MINUS				0b1
# define PLUS				0b10
# define SPACE				0b100
# define HASH				0b1000
# define ZERO				0b10000

typedef	struct		s_flags
{
	unsigned char	flag;
	int				width;
	int				precision;
	int				length;
	int				data_type;				// this keeps track of the conversion indicator
	char			invalid_spec;
	bool			caps;
	int				written_chars;		//The number of written total written_chars
	int				index;
	int				base;
	int				fd;
}					t_flags;

int					ft_printf(const char *format, ...);
void				print_format_string(const char *format, t_flags *flags);
void				check_flags(const char *format, t_flags *flags);
int					check_dubs(const char *format, char c, t_flags *flags);
void				check_width_precision(const char *format, t_flags *flags);
void				check_length(const char *format, t_flags *flags);
int					check_conversion(char format, t_flags *flags);
void				parse_args(t_flags *flags, va_list *arg, int conv);
void				print_char(t_flags *flags, va_list *arg, int base);
void				print_signed(t_flags *flags, va_list *arg, int base);
void				print_unsigned(t_flags *flags, va_list *arg, int base);
void				print_string(t_flags *flags, va_list *arg, int base);

void				ft_putchar_mem(t_flags *flags, char c);
void				ft_putnbr_mem(t_flags *flags, intmax_t nbr, int base);
void				ft_putunbr_mem(t_flags *flags, uintmax_t nbr, int base);
intmax_t			typecast_num(t_flags *flags, intmax_t nbr);
uintmax_t			typecast_unum(t_flags *flags, uintmax_t nbr);
int					zeroes_signed(t_flags *flags, intmax_t nbr, int base);
int					ft_numlen_ull(uintmax_t value, unsigned int base);
int					ft_numlen_ll(intmax_t value, int base);
intmax_t			ft_power_ll(intmax_t nbr, intmax_t power);
void				print_sign(t_flags *flags, intmax_t nbr);
void				print_prefix(t_flags *flags, uintmax_t nbr);
void            	ft_putchar(char c);
size_t          	ft_strlen(const char *s);
int					ft_isdigit(int c);
int					ft_atoi(const char *str);
uintmax_t			ft_power_ull(uintmax_t nbr, uintmax_t power);
int					zeroes_unsigned(t_flags *flags, uintmax_t nbr, int base);


void				ft_putchar_fd(char c, int fd);
int					ft_isspace(const char *s);

#endif
