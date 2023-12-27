/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/23 16:39:39 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors cases to handle:
ERR_NONICKNAMEGIVEN		"<nick>: No nickname given"
ERR_ERRONEUSNICKNAME	"<nick>: Erroneous nickname"
ERR_NICKNAMEINUSE		"<nick>: Nickname is already in use"
ERR_UNAVAILRESOURCE		"<nick/channel>: Nick/channel is temporarily unavailable"
ERR_USERNOTINCHANNEL	"<nick> <channel>: They aren't on that channel" */

static bool	parsing(const User& user, const std::string& nickname) {
	if (nickname.empty())
		Server::clientLog(user, user.getSocket(), ERR_NONICKNAMEGIVEN, "NICK");
	else if (!RegExr("^[a-zA-Z^{}|`-]{1}[a-zA-Z0-9^{}|`-]{0,8}\n$", nickname))
		Server::clientLog(user, user.getSocket(), ERR_ERRONEUSNICKNAME, "NICK");
	/* else if (ERR_NICKNAMEINUSE)
		Server::clientLog(user, user.getSocket(), ERR_NICKNAMEINUSE, "NICK");
	else if (ERR_UNAVAILRESOURCE)
		Server::clientLog(user, user.getSocket(), ERR_UNAVAILRESOURCE, "NICK");
	else if (ERR_USERNOTINCHANNEL)
		Server::clientLog(user, user.getSocket(), ERR_USERNOTINCHANNEL, "NICK"); */
	else
		return true;
	return false;
}

void	Server::nick(User& user, const std::string& nickname) {
	if (parsing(user, nickname) && user.getAuthentification())
		user.setNickname(nickname);
}