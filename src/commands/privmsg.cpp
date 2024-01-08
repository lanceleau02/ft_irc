/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:26:34 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/08 15:11:49 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::privmsg(Client& client, std::string args)
{
	std::istringstream iss(args);
	std::string msgtarget;
	std::string msg;
	iss >> msgtarget;
	iss >> msg;

	if (_channels.find(msgtarget) != _channels.end()) {
		Channel& final_target = _channels.at(msgtarget);
		
	}
}