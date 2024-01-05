/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 15:08:15 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 11:35:52 by hsebille         ###   ########.fr       */
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

static bool parsing(const User& user, const std::string& channelName, std::map<std::string, Channel>& channels) {
	std::string command = "JOIN";
	if (channelName.empty())
		Server::clientLog(user.getSocket(), ERR_NEEDMOREPARAMS(user.getUsername(), command));
	else if (channels.find(channelName) != channels.end()) {
		std::map<std::string, Channel>::iterator it = channels.find(channelName);
		if (it->second.getNbUsers() >= it->second.getUserLimit())
			Server::clientLog(user.getSocket(), ERR_CHANNELISFULL(user.getUsername(), channelName));
		else if ((it->second.getInviteMode())/* and user hasn't been invited */)
			Server::clientLog(user.getSocket(), ERR_INVITEONLYCHAN(user.getUsername(), channelName));
		else if (it->second.getPasswordMode()/* && it->second.getPassword() != password */)
			Server::clientLog(user.getSocket(), ERR_BADCHANNELKEY(user.getUsername(), channelName));
		else if (it->second.isOnChannel(user.getSocket()))
			Server::clientLog(user.getSocket(), ERR_USERONCHANNEL(user.getUsername(), user.getNickname(), channelName));
	}
	else
		return true;
	return false;
}

void	Server::join(User& user, const std::string& channelName) {
	if (parsing(user, channelName, _channels) && user.getRegistration()) {
		std::map<std::string, Channel>::iterator it = _channels.find(channelName);
		if (it == _channels.end()) {
			user.setType(OPERATOR);
			_channels.insert(std::pair<std::string, Channel>(channelName, Channel(user, channelName)));
			_channels.at(channelName).addOperator(user);
		}
		_channels.at(channelName).addUser(user);
		Server::clientLog(user.getSocket(), RPL_JOIN(user.getNickname(), user.getUsername(), channelName));
		Server::clientLog(user.getSocket(), RPL_NAMEREPLY(user.getNickname(), channelName, createNickList(_channels.at(channelName))));
	}
}