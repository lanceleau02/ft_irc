/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 14:42:59 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/12 13:11:59 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 431	ERR_NONICKNAMEGIVEN		":No nickname given"                          */
/* 432	ERR_ERRONEUSNICKNAME	"<nick> :Erroneous nickname"                  */
/* 433	ERR_NICKNAMEINUSE		"<nick> :Nickname is already in use"          */
/* ************************************************************************** */

static bool	parsing(const Server& server, const Client& client, const std::string& nickname) {
	if (nickname.empty())
		Server::clientLog(client.getSocket(), ERR_NONICKNAMEGIVEN(client.getUsername(), nickname));
	else if (!RegExr("^[a-zA-Z^{}|`-]{1}[a-zA-Z0-9^{}|`-]{0,8}[\n\r]?$", nickname))
		Server::clientLog(client.getSocket(), ERR_ERRONEUSNICKNAME(client.getUsername(), nickname));
	else if (server.findClientByNick(nickname))
		Server::clientLog(client.getSocket(), ERR_NICKNAMEINUSE(client.getUsername(), nickname));
	else
		return true;
	return false;
}

void	Server::nick(Client& client, const std::string& nick) {
	std::istringstream	iss(nick);
	std::string			nickname;
	
	iss >> nickname;
	if (client.getAuthentication() && parsing(*this, client, nickname)) {
		client.setNickname(nickname);
		serverLog(0, "NICK command successful!");
	}
}