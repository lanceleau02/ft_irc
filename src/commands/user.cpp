/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 09:51:01 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/29 16:12:43 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Error replies:
461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"
462	ERR_ALREADYREGISTRED	":Unauthorized command (already registered)" */

static bool parsing(const User& user, const std::string& username) {
	std::string command = "USER";
	if (!(user.getUsername()).empty())
		Server::clientLog(user.getSocket(), ERR_ALREADYREGISTRED());
	else if (username.empty())
		Server::clientLog(user.getSocket(), ERR_NEEDMOREPARAMS(command));
	else
		return true;
	return false;
}

void	Server::user(User& user, const std::string& username) {
	std::cout << "\t**4**" << std::endl;
	if (parsing(user, username) && user.getAuthentication())
		user.setUsername(username);
}
