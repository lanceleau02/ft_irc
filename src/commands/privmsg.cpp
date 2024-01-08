/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:26:34 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/08 15:45:05 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::privmsg(Client& client, const std::string& args)
{
	std::istringstream iss(args);
	std::string msgtarget;
	std::string msg;
	iss >> msgtarget;
	iss >> msg;

	if (msg.empty())
		Server::clientLog(client.getSocket(), ERR_NOTEXTTOSEND(client.getUsername()));


	if (_channels.find(msgtarget) != _channels.end()) {
		Channel& final_target = _channels.at(msgtarget);
		if (final_target.getMap(OPERATORS).find(client.getSocket()) != final_target.getMap(OPERATORS).end())
			final_target.sendMessage(msg);
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getNickname() == msgtarget)
			Server::clientLog(it->second.getSocket(), msg + '\n');
	}
}