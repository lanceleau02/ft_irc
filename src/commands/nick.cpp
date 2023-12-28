/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/28 11:14:03 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors cases to handle:
431	ERR_NONICKNAMEGIVEN		":No nickname given"
432	ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"
433	ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"
437	ERR_UNAVAILRESOURCE		"<nick/channel> :Nick/channel is temporarily unavailable"
441	ERR_USERNOTINCHANNEL	"<nick> <channel> :They aren't on that channel" */

static bool	parsing(const User& user, const std::string& nickname) {
	if (nickname.empty())
		Server::clientLog(user.getSocket(), ERR_NONICKNAMEGIVEN());
	else if (!RegExr("^[a-zA-Z^{}|`-]{1}[a-zA-Z0-9^{}|`-]{0,8}$", nickname))
		Server::clientLog(user.getSocket(), ERR_ERRONEUSNICKNAME(nickname));
	/* else if (ERR_NICKNAMEINUSE)
		Server::clientLog(user.getSocket(), ERR_NICKNAMEINUSE);
	else if (ERR_UNAVAILRESOURCE)
		Server::clientLog(user.getSocket(), ERR_UNAVAILRESOURCE);
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