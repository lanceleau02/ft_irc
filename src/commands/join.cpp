/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:30:52 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:51:11 by laprieur         ###   ########.fr       */
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

static bool parsing(const Client& client, std::map<std::string, Channel> channels, std::string channel, std::string key) {
	if (channel.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
	else if (channel[0] != '#' && channel[0] != '&')
		Server::clientLog(client.getSocket(), ERR_NOSUCHCHANNEL(channel));
	else if (channels.find(channel) != channels.end()) {
		std::map<std::string, Channel>::iterator it = channels.find(channel);
		if (it->second.getIsUserLimit() && it->second.getNbUsers() >= it->second.getUserLimit())
			Server::clientLog(client.getSocket(), ERR_CHANNELISFULL(client.getNickname(), channel));
		else if ((it->second.getInviteMode()) && !it->second.isInvitee(client.getSocket()))
			Server::clientLog(client.getSocket(), ERR_INVITEONLYCHAN(client.getNickname(), channel));
		else if (!it->second.getKey().empty() && it->second.getKey() != key)
			Server::clientLog(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), channel));
		else if (it->second.isUser(client.getSocket()))
			Server::clientLog(client.getSocket(), ERR_USERONCHANNEL(client.getNickname(), channel));
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
	if (client.getRegistration() && parsing(client, _channels, channel, key)) {
		std::map<std::string, Channel>::iterator it = _channels.find(channel);
		if (it == _channels.end()) {
			_channels.insert(std::pair<std::string, Channel>(channel, Channel(client, channel)));
			_channels.at(channel).addOperator(client);
		}
		_channels.at(channel).addUser(client);
		_channels.at(channel).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_JOIN(client.getNickname(), channel));
		_channels.at(channel).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_NAMEREPLY(client.getNickname(), channel, createNickList(_channels.at(channel))));
		if ((_channels.at(channel).getTopic()).empty())
			clientLog(client.getSocket(), RPL_NOTOPIC(client.getNickname(), channel));
		else
			clientLog(client.getSocket(), RPL_TOPIC(client.getNickname(), channel, _channels.at(channel).getTopic()));
		serverLog(SUCCESS, "JOIN command success!");
	}
	else
		serverLog(FAILURE, "JOIN command failure!");
}