/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 17:55:22 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
Errors cases to handle:
ERR_NONICKNAMEGIVEN		":No nickname given"
ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"
ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"
ERR_NICKCOLLISION		"<nick> :Nickname collision KILL from <user>@<host>"
ERR_UNAVAILRESOURCE		"<nick/channel> :Nick/channel is temporarily unavailable"
ERR_USERNOTINCHANNEL	"<nick> <channel> :They aren't on that channel"
*/

bool	Server::nick(User& user, const std::string& nickname) {
	std::cout << nickname << std::endl;
	if (user.getAuthentification() == false)
		std::cerr << RED << "Not authentified yet." << NONE << std::endl;
	else if (!nickname.empty()) {
		user.setNickname(nickname);
		std::cout << GREEN << "Nickname successfully changed for " << nickname << "!" << NONE << std::endl;
		return true;
	}
	else
		std::cerr << RED << "Impossible to change the nickname!" << NONE << std::endl;
	return false;
}