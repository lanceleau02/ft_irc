/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 12:51:16 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:51:22 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
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

static bool	parsing(const Server& server, const Client& client, const std::map<std::string, Channel> channels, std::string nickname, std::string channelName) {
	if (nickname.empty() || channelName.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
	else if (channels.find(channelName) == channels.end())
		return false;
	Channel channel = channels.at(channelName);
	if (!channel.isUser(client.getSocket()))
		Server::clientLog(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), channelName));
	else if (!channel.isOperator(client.getSocket()))
		Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
	else if (!findClient(server.getClients(), nickname))
		Server::clientLog(client.getSocket(), ERR_NOSUCHNICK(client.getNickname(), nickname));
	else if (findClient(channel.getMap(USERS), nickname))
		Server::clientLog(client.getSocket(), ERR_USERONCHANNEL(client.getNickname(), channelName));
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
	if (client.getRegistration() && parsing(*this, client, _channels, nickname, channel)) {
		for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
			if (it->second.getNickname() == nickname) {
				_channels.at(channel).addInvitee(it->second);
				clientLog(it->second.getSocket(), RPL_INVITERCVR(client.getNickname(), nickname, channel));
			}
		}
		clientLog(client.getSocket(), RPL_INVITESNDR(client.getNickname(), nickname, channel));
		serverLog(SUCCESS, "INVITE command success!");
	}
	else
		serverLog(FAILURE, "INVITE command failure!");
}
