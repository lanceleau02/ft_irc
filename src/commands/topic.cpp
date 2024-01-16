/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:31:04 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:50:37 by laprieur         ###   ########.fr       */
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
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */

static bool	parsing(const Client& client, std::map<std::string, Channel> channels, std::string channelName, std::string topic) {
	if (channelName.empty()) {
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
		return false;
	} else if (channels.find(channelName) == channels.end())
		return false;
	Channel channel = channels.at(channelName);
	if (!(channel.isUser(client.getSocket())))
		Server::clientLog(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), channelName));
	else if (channel.getTopicRestrictions() && !channel.isOperator(client.getSocket()) && !topic.empty())
		Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
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
	if (client.getRegistration() && parsing(client, _channels, channelName, topic)) {
		if (topic[0] == ':')
			topic = topic.substr(1);
		if (topic.empty() && (_channels.at(channelName).getTopic()).empty())
			clientLog(client.getSocket(), RPL_NOTOPIC(client.getNickname(), channelName));
		else if (topic.empty() && !(_channels.at(channelName).getTopic()).empty())
			clientLog(client.getSocket(), RPL_SEETOPIC(client.getNickname(), channelName, _channels.at(channelName).getTopic()));
		else if (!topic.empty()) {
			_channels.at(channelName).setTopic(topic);
			_channels.at(channelName).sendMessage(SEND_TO_ALL, client.getSocket(), RPL_TOPIC(client.getNickname(), channelName, _channels.at(channelName).getTopic()));
		}
		serverLog(SUCCESS, "TOPIC command success!");
	}
	else
		serverLog(FAILURE, "TOPIC command failure!");
}