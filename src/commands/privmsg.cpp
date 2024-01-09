/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:26:34 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/09 10:45:02 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 301	RPL_AWAY				"<nick> :<away message>"                      */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 401	ERR_NOSUCHNICK			"<nickname> :No such nick/channel"            */
/* 404	ERR_CANNOTSENDTOCHAN	"<channel name> :Cannot send to channel"      */
/* 411	ERR_NORECIPIENT			":No recipient given (<command>)"             */
/* 412	ERR_NOTEXTTOSEND		":No text to send"                            */
/* ************************************************************************** */

static bool	parsing(const Client& client, std::string msgtarget, std::string msg) {
	(void)msgtarget;
	(void)msg;
	if (msg.empty())
		Server::clientLog(client.getSocket(), ERR_NOTEXTTOSEND(client.getUsername()));
	else
		return true;
	return false;
}

void	Server::privmsg(Client& client, const std::string& args)
{
	std::istringstream iss(args);
	std::string msgtarget;
	std::string msg;
	iss >> msgtarget;
	iss >> msg;
	if (parsing(client, msgtarget, msg)) {
		if (_channels.find(msgtarget) != _channels.end()) {
			Channel& final_target = _channels.at(msgtarget);
			if (final_target.getMap(OPERATORS).find(client.getSocket()) != final_target.getMap(OPERATORS).end())
				final_target.sendMessage(msg);
		}
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			if (it->second.getNickname() == msgtarget)
				Server::clientLog(it->second.getSocket(), msg + '\n');
		}		
	}
}