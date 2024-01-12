/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:31:04 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/12 13:35:06 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 331	RPL_NOTOPIC				"<channel> :No topic is set"                  */
/* 332	RPL_TOPIC				"<channel> :<topic>"                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 442	ERR_NOTONCHANNEL		"<channel> :You're not on that channel"       */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 477	ERR_NOCHANMODES			"<channel> :Channel doesn't support modes"    */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */

static bool	parsing(const Client& client, std::map<std::string, Channel> channels, std::string cmd, std::string channelName, std::string topic) {
	if (channelName.empty()) {
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), cmd));
		return false;
	} else if (channels.find(channelName) == channels.end())
		return false;
	Channel channel = channels.at(channelName);
	if (!(channel.isUser(client.getSocket())))
		Server::clientLog(client.getSocket(), ERR_NOTONCHANNEL(client.getUsername(), channelName));
	else if (channel.getTopicRestrictions())
		Server::clientLog(client.getSocket(), ERR_NOCHANMODES(channelName));
	else if (!channel.isOperator(client.getSocket()) && !topic.empty())
		Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getUsername(), channelName));
	else
		return true;
	return false;
}

void	Server::topic(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			channelName;
	std::string			topic;

	iss >> channelName;
	iss >> topic;
	if (client.getRegistration() && parsing(client, _channels, "TOPIC", channelName, topic)) {
		//Channel channel = _channels.at(channelName);
		if (topic.empty() && (_channels.at(channelName).getTopic()).empty())
			Server::clientLog(client.getSocket(), RPL_NOTOPIC(client.getUsername(), channelName));
		else if (topic.empty() && !(_channels.at(channelName).getTopic()).empty())
			Server::clientLog(client.getSocket(), RPL_SEETOPIC(client.getUsername(), channelName, _channels.at(channelName).getTopic()));
		else if (!topic.empty()) {
			_channels.at(channelName).setTopic(topic);
			Server::clientLog(client.getSocket(), RPL_TOPIC(client.getUsername(), channelName, _channels.at(channelName).getTopic()));
		}
		serverLog(0, "TOPIC command successful!");
	}
}