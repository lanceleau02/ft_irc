/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 15:08:15 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 15:32:33 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 332	RPL_TOPIC			"<channel> :<topic>"                              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 443	ERR_USERONCHANNEL	"<user> <channel> :is already on channel"         */
/* 461	ERR_NEEDMOREPARAMS	"<command> :Not enough parameters"                */
/* 471	ERR_CHANNELISFULL	"<channel> :Cannot join channel (+l)"             */
/* 473	ERR_INVITEONLYCHAN	"<channel> :Cannot join channel (+i)"             */
/* 475	ERR_BADCHANNELKEY	"<channel> :Cannot join channel (+k)"             */
/* ************************************************************************** */

static bool parsing(const Client& client, const std::string& channelName, std::map<std::string, Channel>& channels) {
	std::string command = "JOIN";
	if (channelName.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), command));
	else if (channels.find(channelName) != channels.end()) {
		std::map<std::string, Channel>::iterator it = channels.find(channelName);
		if (it->second.getNbUsers() >= it->second.getUserLimit())
			Server::clientLog(client.getSocket(), ERR_CHANNELISFULL(client.getUsername(), channelName));
		else if ((it->second.getInviteMode())/* and client hasn't been invited */)
			Server::clientLog(client.getSocket(), ERR_INVITEONLYCHAN(client.getUsername(), channelName));
		else if (it->second.getPasswordMode()/* && it->second.getPassword() != password */)
			Server::clientLog(client.getSocket(), ERR_BADCHANNELKEY(client.getUsername(), channelName));
		else if (it->second.isOnChannel(client.getSocket()))
			Server::clientLog(client.getSocket(), ERR_USERONCHANNEL(client.getUsername(), client.getNickname(), channelName));
		else
			return true;
	}
	else
		return true;
	return false;
}

void	Server::join(Client& client, const std::string& channelName) {
	if (parsing(client, channelName, _channels) && client.getRegistration()) {
		std::map<std::string, Channel>::iterator it = _channels.find(channelName);
		if (it == _channels.end()) {
			_channels.insert(std::pair<std::string, Channel>(channelName, Channel(client, channelName)));
			_channels.at(channelName).addOperator(client);
		}
		_channels.at(channelName).addUser(client);
		_channels.at(channelName).sendMessage(RPL_JOIN(client.getNickname(), client.getUsername(), channelName));
		_channels.at(channelName).sendMessage(RPL_NAMEREPLY(client.getNickname(), channelName, createNickList(_channels.at(channelName))));
	}
}