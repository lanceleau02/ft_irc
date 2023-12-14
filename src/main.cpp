/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:04:57 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/14 14:12:28 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int	main(int argc, char **argv) {
	(void)argv;
	if (argc != 3) {
		std::cerr << RED << "Error: wrong number of arguments." << NONE << std::endl;
		std::cerr << GREEN << "Usage: ./ircserv <port> <password>" << NONE << std::endl;
		return 1;
	}
	return 0;
}