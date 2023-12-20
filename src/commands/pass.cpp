/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/20 14:40:47 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::pass(User& user, const std::string& password) {
	if (password == _password) {
		user.setAuthentification();
		std::cout << GREEN << "Client successfully authentified!" << NONE << std::endl;
	}
	else
		std::cout << RED << "Wrong password submitted." << NONE << std::endl;
}