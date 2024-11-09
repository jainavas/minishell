/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 00:54:06 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 00:54:06 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define BASE10 "0123456789"
# define BASE16 "0123456789abcdef"

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdarg.h>

size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t				ft_strlen(const char *s);
int					ft_intputchar_fd(char c, int fd);
int					ft_intputnbr_fd(int n, int fd);
int					ft_intputstr_fd(char *s, int fd);
char				*ft_strtolower(char *str);
char				*ft_strtoupper(char *str);
char				*print_number(char *buffer, unsigned long unbr, char *base,
						int nbase);
char				*ft_putnbr_base(unsigned long nbr, char *base);
char				*ft_putlong_base(unsigned long lnbr, char *base);
char				*ft_ultoa(unsigned long n);
int					ft_printf(char const *s, ...);

#endif