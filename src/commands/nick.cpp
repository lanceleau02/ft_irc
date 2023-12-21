/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 10:18:41 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors cases to handle:
ERR_NONICKNAMEGIVEN		":No nickname given"
ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"
ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"
ERR_NICKCOLLISION		"<nick> :Nickname collision KILL from <user>@<host>"
ERR_UNAVAILRESOURCE		"<nick/channel> :Nick/channel is temporarily unavailable"
ERR_USERNOTINCHANNEL	"<nick> <channel> :They aren't on that channel"
*/

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