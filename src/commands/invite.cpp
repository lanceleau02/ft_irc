/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 12:51:16 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/15 14:33:53 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 301	RPL_AWAY				"<nick> :<away message>"                      */
/* 341	RPL_INVITING			"<channel> <nick>"                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 401	ERR_NOSUCHNICK			"<nickname> :No such nick/channel"            */
/* 442	ERR_NOTONCHANNEL		"<channel> :You're not on that channel"       */
/* 443	ERR_USERONCHANNEL		"<user> <channel> :is already on channel"     */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */

static bool	parsing(Server& server, const Client& client, const std::map<std::string, Channel>& channels, std::string cmd, std::string nickname, std::string channelName) {
	if (nickname.empty() || channelName.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), cmd));
	else if (channels.find(channelName) == channels.end()) {
		std::cout << "Le channel" << channelName << " n'existe pas fdp" << std::endl;
		return false;
	}
	Channel channel = channels.at(channelName);
	if (!channel.isUser(client.getSocket()))
		Server::clientLog(client.getSocket(), ERR_NOTONCHANNEL(client.getUsername(), channelName));
	else if (!channel.isOperator(client.getSocket()))
		Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getUsername(), channelName));
	else if (!findClient(server.getClients(), nickname))
		Server::clientLog(client.getSocket(), ERR_NOSUCHNICK(client.getUsername(), nickname));
	else if (findClient(channel.getMap(USERS), nickname))
		Server::clientLog(client.getSocket(), ERR_USERONCHANNEL(client.getUsername(), client.getNickname(), channelName));
	else
		return true;
	return false;
}

void	Server::invite(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			nickname;
	std::string			channel;
	
	iss >> nickname;
	iss >> channel;
	if (client.getRegistration() && parsing(*this, client, _channels, "INVITE", nickname, channel)) {
		for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
			if (it->second.getNickname() == nickname)
				_channels.at(channel).addInvitee(it->second);
		serverLog(0, "INVITE command successful!");
	}
}
