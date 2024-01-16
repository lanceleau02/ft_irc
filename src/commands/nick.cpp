/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:50:08 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 431	ERR_NONICKNAMEGIVEN		":No nickname given"                          */
/* 432	ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"                  */
/* 433	ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"          */
/* ************************************************************************** */

static bool	parsing(const Server& server, const Client& client, std::string nickname) {
	if (nickname.empty())
		Server::clientLog(client.getSocket(), ERR_NONICKNAMEGIVEN(client.getNickname(), nickname));
	else if (!RegExr("^[a-zA-Z^{}|`-]{1}[a-zA-Z0-9^{}|`-]{0,8}[\n\r]?$", nickname))
		Server::clientLog(client.getSocket(), ERR_ERRONEUSNICKNAME(client.getNickname(), nickname));
	else if (findClient(server.getClients(), nickname))
		Server::clientLog(client.getSocket(), ERR_NICKNAMEINUSE(client.getNickname(), nickname));
	else
		return true;
	return false;
}

void	Server::nick(Client& client, const std::string& nick) {
	std::istringstream	iss(nick);
	std::string			nickname;
	
	iss >> nickname;
	if (client.getAuthentication() && parsing(*this, client, nickname)) {
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			if (findClient(it->second.getMap(USERS), _clients.at(client.getSocket()).getNickname()))
				const_cast<std::map<int, Client>&>(it->second.getMap(USERS)).at(client.getSocket()).setNickname(nickname);
		if (!client.getNickname().empty())
			clientLog(client.getSocket(), RPL_NICK(client.getNickname(), nickname));
		client.setNickname(nickname);
		serverLog(SUCCESS, "NICK command success!");
	}
	else
		serverLog(FAILURE, "NICK command failure!");
}