/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 13:37:20 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:02:47 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 324	RPL_CHANNELMODEIS		"<channel> <mode> <mode params>"              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 441	ERR_USERNOTINCHANNEL	"<nick> <chan> :They aren't on that channel"  */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 467	ERR_KEYSET				"<channel> :Channel key already set"          */
/* 477	ERR_NOCHANMODES			"<channel> :Channel doesn't support modes"    */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */

static bool parsing (const Client& client, std::map<std::string, Channel> _channels, std::string mode, std::string channel, std::string param) {
	if (mode.empty() || (mode.find_first_of("ko") != std::string::npos && param.empty()))
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
	else if (mode.size() > 2 || (mode[0] != '+' && mode[0] != '-'))
		return false;
	else if (_channels.find(channel) == _channels.end())
		Server::clientLog(client.getSocket(), ERR_NOSUCHCHANNEL(channel));
	else if (mode.find_first_not_of("+-itkol") != std::string::npos)
		Server::clientLog(client.getSocket(), ERR_NOCHANMODES(channel));
	else if (mode[1] == 'o' && !findClient(_channels.at(channel).getMap(USERS), param))
		Server::clientLog(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), param, channel));
	else if (mode[1] == 'k' && mode[0] == '+' && !_channels.at(channel).getKey().empty())
		Server::clientLog(client.getSocket(), ERR_KEYSET(channel));
	else if (mode[1] == 'k' && mode[0] == '-' && _channels.at(channel).getKey() != param)
		return false;
	else if (_channels.at(channel).getMap(OPERATORS).find(client.getSocket()) == _channels.at(channel).getMap(OPERATORS).end())
		Server::clientLog(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), channel));
	else
		return true;
	return false;
}

void Server::mode(Client& client, const std::string& args) {
	std::istringstream	iss(args);
	std::string			channel;
	std::string			mode;
	std::string			param;

	iss >> channel;
	iss >> mode;
	iss >> param;
	if (client.getRegistration() && parsing(client, _channels, mode, channel, param)) {
		if (mode[1] == 'i')
    		_channels.at(channel).setInviteMode((mode[0] == '+') ? INVITE_MODE : NON_INVITE_MODE);
		else if (mode[1] == 't')
			_channels.at(channel).setTopicRestrictions((mode[0] == '+') ? EVERYONE : OP_ONLY);
		else if (mode[1] == 'k')
			_channels.at(channel).setKey((mode[0] == '+' ? ADD_KEY : REMOVE_KEY), param);
		else if (mode[1] == 'o')
			_channels.at(channel).addOrRemove((mode[0] == '+' ? ADD_OPERATOR : REMOVE_OPERATOR), _channels.at(channel).getClient(param));
		else if (mode[1] == 'l' && mode[0] == '+' && !param.empty())
			_channels.at(channel).setUserLimit(CHANGE_USER_LIMIT, std::atoi(param.c_str()));
		else if (mode[1] == 'l' && mode[0] == '+' && param.empty())
			_channels.at(channel).setUserLimit(SET_USER_LIMIT, 0);
		else if (mode[1] == 'l' && mode[0] == '-' && param.empty())
			_channels.at(channel).setUserLimit(UNSET_USER_LIMIT, 0);
		Server::clientLog(client.getSocket(), RPL_MODE(client.getNickname(), channel, mode, param));
		serverLog(SUCCESS, "MODE command successful!");
	}
}