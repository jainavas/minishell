/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jainavas <jainavas@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 02:10:56 by jainavas          #+#    #+#             */
/*   Updated: 2024/08/12 02:10:56 by jainavas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

static int	spaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	minus;

	i = spaces(str);
	minus = 0;
	res = 0;
	if (str[i] == '-')
	{
		minus++;
		i++;
	}
	else
		if (str[i] == '+')
			i++;
	while (ft_isdigit((int)str[i]) == 1)
	{
		res *= 10;
		res += str[i] - '0';
		i++;
	}
	if (minus % 2 == 1)
		res = -res;
	return (res);
}

int	checkovrfandchar(const char *str)
{
	int		i;
	long	res;
	int		minus;

	i = spaces(str);
	minus = 1;
	res = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '-')
		minus = -minus;
	if (ft_isdigit((int)str[i]) == 0)
		return (0);
	while (ft_isdigit((int)str[i]) == 1)
	{
		res *= 10;
		res += str[i] - '0';
		i++;
		if (res * minus > INT_MAX || res * minus < -2147483648)
			return (0);
	}
	return (1);
}

// int main(int argc, char **argv)
// {
// 	if(argc != 2)
// 	{
// 		printf("faltan argumentos\n");
// 		return (-1);
// 	}
// 	printf("real: %d\n",atoi(argv[1]));
// 	printf("mio : %d\n",ft_atoi(argv[1]));
// }
