/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/10 11:52:16 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 431	ERR_NONICKNAMEGIVEN		":No nickname given"                          */
/* 432	ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"                  */
/* 433	ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"          */
/* 441	ERR_USERNOTINCHANNEL	"<nick> <chan> :They aren't on that channel"  */
/* 484	ERR_RESTRICTED			":Your connection is restricted!"             */
/* ************************************************************************** */

static bool	parsing(const Server& server, const Client& client, const std::string& nickname) {
	if (nickname.empty())
		Server::clientLog(client.getSocket(), ERR_NONICKNAMEGIVEN(client.getUsername(), nickname));
	else if (!RegExr("^[a-zA-Z^{}|`-]{1}[a-zA-Z0-9^{}|`-]{0,8}[\n\r]?$", nickname))
		Server::clientLog(client.getSocket(), ERR_ERRONEUSNICKNAME(client.getUsername(), nickname));
	else if (server.findClientByNick(nickname))
		Server::clientLog(client.getSocket(), ERR_NICKNAMEINUSE(client.getUsername(), nickname));
	/* else if (ERR_USERNOTINCHANNEL)
		Server::clientLog(client.getSocket(), ERR_USERNOTINCHANNEL); */
	else
		return true;
	return false;
}

void	Server::nick(Client& client, const std::string& nick) {
	std::istringstream	iss(nick);
	std::string			nickname;
	
	iss >> nickname;
	if (parsing(*this, client, nickname) && client.getAuthentication())
		client.setNickname(nickname);
}