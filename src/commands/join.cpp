/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 15:08:15 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/28 15:57:22 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Command replies:
332	RPL_TOPIC			"<channel> :<topic>" */

/* Error replies:
403	ERR_NOSUCHCHANNEL	"<channel name> :No such channel"
405	ERR_TOOMANYCHANNELS	"<channel name> :You have joined too many channels"			-> not handled
407	ERR_TOOMANYTARGETS	"<target> :<error code> recipients. <abort message>"		-> not handled
437	ERR_UNAVAILRESOURCE	"<nick/channel> :Nick/channel is temporarily unavailable"	-> not handled
461	ERR_NEEDMOREPARAMS	"<command> :Not enough parameters"
471	ERR_CHANNELISFULL	"<channel> :Cannot join channel (+l)"
473	ERR_INVITEONLYCHAN	"<channel> :Cannot join channel (+i)"
474	ERR_BANNEDFROMCHAN	"<channel> :Cannot join channel (+b)"						-> not handled
475	ERR_BADCHANNELKEY	"<channel> :Cannot join channel (+k)"
476	ERR_BADCHANMASK		"<channel> :Bad Channel Mask" 								-> not handled */

static bool parsing(const Server& server, const User& user, const std::string& channelName) {
	std::string command = "JOIN";
	if (channelName.empty())
		Server::clientLog(user.getSocket(), ERR_NEEDMOREPARAMS(command));
	else if ((server._channels).find(channelName) == (server._channels).end())
		Server::clientLog(user.getSocket(), ERR_NOSUCHCHANNEL(channelName));
	/* else if (ERR_CHANNELISFULL)
		clientLog(user.getSocket(), ERR_CHANNELISFULL(channelName));
	else if (ERR_INVITEONLYCHAN)
		clientLog(user.getSocket(), ERR_INVITEONLYCHAN(channelName));
	else if (ERR_BADCHANNELKEY)
		clientLog(user.getSocket(), ERR_BADCHANNELKEY(channelName));*/
	else
		return true;
	return false;
}

void	Server::join(User& user, const std::string& channelName) {
	if (parsing(*this, user, username) && user.getRegistration())
		user.setUsername(username);
}