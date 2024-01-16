/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:54:49 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:50:56 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 403	ERR_NOSUCHCHANNEL		"<channel name> :No such channel"             */
/* 441	ERR_USERNOTINCHANNEL	"<nick> <chan> :They aren't on that channel"  */
/* 442	ERR_NOTONCHANNEL		"<channel> :You're not on that channel"       */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */

static bool parsing (const Client& client, std::map<std::string, Channel> _channels, std::string channel, std::string user) {
	if (user.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
	else if (_channels.find(channel) == _channels.end())
		Server::clientLog(client.getSocket(), ERR_NOSUCHCHANNEL(channel));	
	else if (_channels.at(channel).getMap(USERS).find(client.getSocket()) == _channels.at(channel).getMap(USERS).end())
		Server::clientLog(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), channel));	
	else if (!findClient(_channels.at(channel).getMap(USERS), user))
		Server::clientLog(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), user, channel));
	else if (_channels.at(channel).getMap(OPERATORS).find(client.getSocket()) == _channels.at(channel).getMap(OPERATORS).end())
		Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
	else
		return true;
	return (false);
}

void Server::kick(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			channel;
	std::string			user;

	iss >> channel;
	iss >> user;
	if (client.getRegistration() && parsing(client, _channels, channel, user)) {
		_channels.at(channel).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_KICK(client.getNickname(), channel, user));
		_channels.at(channel).deleteUser(user);
		if (_channels.at(channel).getMap(USERS).empty())
			_channels.erase(channel);
		serverLog(SUCCESS, "KICK command success!");
	}
	else
		serverLog(FAILURE, "KICK command failure!");
}