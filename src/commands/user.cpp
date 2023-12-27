/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 16:32:19 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/27 09:49:01 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Error cases to handle:
ERR_NEEDMOREPARAMS		"<command>: Not enough parameters"
ERR_ALREADYREGISTRED	"<command>: Unauthorized command (already registered)" */

static bool parsing(const User& user, const std::string& username) {
	if (user.getAuthentification())
		Server::clientLog(user, user.getSocket(), ERR_ALREADYREGISTRED, "USER");
	else if (username.empty())
		Server::clientLog(user, user.getSocket(), ERR_NEEDMOREPARAMS, "USER");
	else
		return true;
	return false;
}

void	Server::user(User& user, const std::string& username) {
	if (parsing(user, username) && user.getAuthentification())
		user.setUsername(username);
}