/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/03 15:42:42 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors replies:
431	ERR_NONICKNAMEGIVEN		":No nickname given"
432	ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"
433	ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"
437	ERR_UNAVAILRESOURCE		"<nick/channel> :Nick/channel is temporarily unavailable"	-> not handled
441	ERR_USERNOTINCHANNEL	"<nick> <channel> :They aren't on that channel"
484	ERR_RESTRICTED			":Your connection is restricted!" 							-> not handled */

static bool	parsing(const User& user, const std::string& nickname) {
	std::cout << "\'" << nickname << "\'" << std::endl;
	if (nickname.empty())
		Server::clientLog(user.getSocket(), ERR_NONICKNAMEGIVEN(user.getUsername(), nickname));
	else if (!RegExr("^[a-zA-Z^{}|`-]{1}[a-zA-Z0-9^{}|`-]{0,8}[\n\r]?$", nickname))
		Server::clientLog(user.getSocket(), ERR_ERRONEUSNICKNAME(user.getUsername(), nickname));
	/*else if (ERR_NICKNAMEINUSE)
		Server::clientLog(user.getSocket(), ERR_NICKNAMEINUSE);
	else if (ERR_USERNOTINCHANNEL)
		Server::clientLog(user.getSocket(), ERR_USERNOTINCHANNEL); */
	else
		return true;
	return false;
}

void	Server::nick(User& user, const std::string& nickname) {
	if (parsing(user, nickname) && user.getAuthentication())
		user.setNickname(nickname);
}