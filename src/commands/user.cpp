/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 09:51:01 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/28 11:03:36 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Error cases to handle:
ERR_NEEDMOREPARAMS		"<command>: Not enough parameters"
ERR_ALREADYREGISTRED	"<command>: Unauthorized command (already registered)" */

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
	if (parsing(user, username) && user.getAuthentication())
		user.setUsername(username);
}