/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:26:34 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/16 13:50:29 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 401	ERR_NOSUCHNICK			"<nickname> :No such nick/channel"            */
/* 404	ERR_CANNOTSENDTOCHAN	"<channel name> :Cannot send to channel"      */
/* 412	ERR_NOTEXTTOSEND		":No text to send"                            */
/* ************************************************************************** */

static bool	parsing(const Server& server, std::map<std::string, Channel> _channels, const Client& client, std::string msgtarget, std::string msg) {
	if (msg.empty())
		Server::clientLog(client.getSocket(), ERR_NOTEXTTOSEND(client.getNickname()));
	else if ((msgtarget[0] != '#' && msgtarget[0] != '&') && !findClient(server.getClients(), msgtarget))
		Server::clientLog(client.getSocket(), ERR_NOSUCHNICK(client.getNickname(), msgtarget));
	else if ((msgtarget[0] == '#' || msgtarget[0] == '&') && _channels.find(msgtarget) == _channels.end())
		Server::clientLog(client.getSocket(), ERR_CANNOTSENDTOCHAN(client.getNickname(), msgtarget));
	else if ((msgtarget[0] == '#' || msgtarget[0] == '&') && _channels.at(msgtarget).getMap(USERS).find(client.getSocket()) == _channels.at(msgtarget).getMap(USERS).end())
		Server::clientLog(client.getSocket(), ERR_CANNOTSENDTOCHAN(client.getNickname(), msgtarget));	
	else
		return true;
	return false;
}

void	Server::privmsg(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			msgtarget;
	std::string			msg;
	
	iss >> msgtarget;
	std::getline(iss, msg);
	if (client.getRegistration() && parsing(*this, _channels, client, msgtarget, msg)) {
		if (_channels.find(msgtarget) != _channels.end())
			_channels.at(msgtarget).sendMessage(EXCLUDE_SENDER, client.getSocket(), ":" + client.getNickname() + " PRIVMSG " + msgtarget + " :" + msg + "\r\n");
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
			if (it->second.getNickname() == msgtarget)
				clientLog(it->second.getSocket(), ":" + client.getNickname() + " PRIVMSG " + msgtarget + " " + msg.substr(1) + "\r\n");
		serverLog(SUCCESS, "PRIVMSG command success!");
	}
	else
		serverLog(FAILURE, "PRIVMSG command failure!");
}