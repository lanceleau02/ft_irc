/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:40:35 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/11 14:09:52 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 332	RPL_TOPIC			"<channel> :<topic>"                              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 403	ERR_NOSUCHCHANNEL	"<channel name> :No such channel"                 */
/* 443	ERR_USERONCHANNEL	"<user> <channel> :is already on channel"         */
/* 461	ERR_NEEDMOREPARAMS	"<command> :Not enough parameters"                */
/* 471	ERR_CHANNELISFULL	"<channel> :Cannot join channel (+l)"             */
/* 473	ERR_INVITEONLYCHAN	"<channel> :Cannot join channel (+i)"             */
/* 475	ERR_BADCHANNELKEY	"<channel> :Cannot join channel (+k)"             */
/* ************************************************************************** */

static bool parsing(const Client& client, std::map<std::string, Channel>& channels, std::string cmd, std::string channel, std::string key) {
	if (channel.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), cmd));
	else if (channel[0] != '#' && channel[0] != '&')
		Server::clientLog(client.getSocket(), ERR_NOSUCHCHANNEL(channel));
	else if (channels.find(channel) != channels.end()) {
		std::map<std::string, Channel>::iterator it = channels.find(channel);
		if (it->second.getNbUsers() >= it->second.getUserLimit())
			Server::clientLog(client.getSocket(), ERR_CHANNELISFULL(client.getUsername(), channel));
		else if ((it->second.getInviteMode()) && !it->second.isInvited(client.getSocket()))
			Server::clientLog(client.getSocket(), ERR_INVITEONLYCHAN(client.getUsername(), channel));
		else if (it->second.getKeyMode() && !key.empty() && it->second.getKey() != key)
			Server::clientLog(client.getSocket(), ERR_BADCHANNELKEY(client.getUsername(), channel));
		else if (it->second.isOnChannel(client.getSocket()))
			Server::clientLog(client.getSocket(), ERR_USERONCHANNEL(client.getUsername(), client.getNickname(), channel));
		else
			return true;
	}
	else
		return true;
	return false;
}

void	Server::join(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			channel;
	std::string			key;
	
	iss >> channel;
	iss >> key;
	if (client.getRegistration() && parsing(client, _channels, "JOIN", channel, key)) {
		std::map<std::string, Channel>::iterator it = _channels.find(channel);
		if (it == _channels.end()) {
			_channels.insert(std::pair<std::string, Channel>(channel, Channel(client, channel)));
			_channels.at(channel).addOperator(client);
		}
		_channels.at(channel).addUser(client);
		_channels.at(channel).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_JOIN(client.getNickname(), client.getUsername(), channel));
		_channels.at(channel).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_NAMEREPLY(client.getNickname(), channel, createNickList(_channels.at(channel))));
		_channels.at(channel).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_TOPIC(client.getUsername(), channel, _channels.at(channel).getTopic()));
	}
}