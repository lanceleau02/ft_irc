/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:31:04 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/11 14:31:05 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 324	RPL_CHANNELMODEIS		"<channel> <mode> <mode params>"              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 442	ERR_NOTONCHANNEL		"<channel> :You're not on that channel"       */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 477	ERR_NOCHANMODES			"<channel> :Channel doesn't support modes"    */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */

static bool	parsing(const Client& client, std::map<std::string, Channel> channels, std::string cmd, std::string channelName, std::string topic) {
	if (channelName.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), cmd));
	else if (channels.find(channelName) != channels.end()) {
		Channel channel = channels.at(channelName);
		if (!(channel.isOnChannel(client.getSocket())))
			Server::clientLog(client.getSocket(), ERR_NOTONCHANNEL(client.getUsername(), channelName));
		else if (channel.getTopicRestrictions())
			Server::clientLog(client.getSocket(), ERR_NOCHANMODES(channelName));
		else if (!client.isOperator(channel) && !topic.empty())
			Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getUsername(), channelName));
		else
			return true;
	}
	else
		return true;
	return false;
}

void	Server::topic(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			channelName;
	std::string			topic;
	
	std::cout << "Je suis au debut de TOPIC" << std::endl;	
	iss >> channelName;
	iss >> topic;
	if (client.getRegistration() && parsing(client, _channels, "TOPIC", channelName, topic)) {
		Channel channel = _channels.at(channelName);
		std::cout << "J'ai passe le parsing" << std::endl;
		std::cout << "channel name: " << channelName << std::endl;
		std::cout << "channel topic: " << channel.getTopic() << std::endl;
		std::cout << "topic: " << topic << std::endl;
		if (topic.empty() && (channel.getTopic()).empty())
			Server::clientLog(client.getSocket(), RPL_NOTOPIC(client.getUsername(), channelName));
		else if (topic.empty() && !(channel.getTopic()).empty())
			Server::clientLog(client.getSocket(), RPL_SEETOPIC(client.getUsername(), channelName, topic));
		else if (!topic.empty()) {
			channel.setTopic(topic);
			Server::clientLog(client.getSocket(), RPL_TOPIC(client.getUsername(), channelName, channel.getTopic()));
		}
		std::cout << "topic a la fin de topic: " << channel.getTopic() << std::endl;
	}
}