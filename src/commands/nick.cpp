/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/20 16:32:23 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::nick(User& user, std::string nickname) {
	if (user.getAuthentification() == false)
		std::cerr << RED << "Not authentified yet." << NONE << std::endl;
	else if (!nickname.empty()) {
		user.setNickname(nickname);
		std::cout << GREEN << "Nickname successfully changed for " << nickname << "!" << NONE << std::endl;
	}
	else
		std::cerr << RED << "Impossible to change the nickname!" << NONE << std::endl;
}