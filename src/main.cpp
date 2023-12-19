/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:04:57 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/19 15:35:17 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int	main(int argc, char **argv) {
	try {
		if (argc != 3)
			throw std::invalid_argument("invalid number of arguments.");
		Server	server(argv);
	}
	catch (std::exception& e) {
		std::cerr << RED << "Error: " << e.what() << NONE << std::endl;
		return 1;
	}
	return 0;
}