/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:26:34 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/10 13:13:05 by hsebille         ###   ########.fr       */
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
/* 412	ERR_NOTEXTTOSEND		":No text to send"                            */
/* ************************************************************************** */

static bool	parsing(const Server& server, std::map<std::string, Channel> _channels, const Client& client, std::string msgtarget, std::string msg) {
	if (msg.empty())
		Server::clientLog(client.getSocket(), ERR_NOTEXTTOSEND(client.getUsername()));
	else if (msgtarget[0] != '#' && !server.findClientByNick(msgtarget))
		Server::clientLog(client.getSocket(), ERR_NOSUCHNICK(client.getUsername(), msgtarget));
	else if (msgtarget[0] == '#' && _channels.find(msgtarget) == _channels.end())
		Server::clientLog(client.getSocket(), ERR_CANNOTSENDTOCHAN(client.getUsername(), msgtarget));
	else
		return true;
	return false;
}

void	Server::privmsg(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			msgtarget;
	std::string			msg;
	
	iss >> msgtarget;
	iss >> msg;
	if (parsing(*this, _channels, client, msgtarget, msg)) {
		if (_channels.find(msgtarget) != _channels.end()) {
			Channel& final_target = _channels.at(msgtarget);
			if (final_target.getMap(OPERATORS).find(client.getSocket()) != final_target.getMap(OPERATORS).end())
				_channels.at(msgtarget).sendMessage(RPL_AWAY(client.getNickname(), msg));
		}
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			if (it->second.getNickname() == msgtarget)
				RPL_AWAY(client.getNickname(), msg);
		}		
	}
}