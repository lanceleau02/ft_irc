/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 17:55:31 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
Errors cases to handle:
ERR_PASSWDMISMATCH		"<command>: Password incorrect"
ERR_NEEDMOREPARAMS		"<command>: Not enough parameters"
ERR_ALREADYREGISTRED	"<command>: Unauthorized command (already registered)"
*/

static bool parsing(User& user, const std::string& userPass, const std::string& serverPass) {
	if (user.getAuthentification() == true)
		Server::clientLog(user, user.getSocket(), ERR_ALREADYREGISTRED, "PASS");
	else if (userPass.empty())
		Server::clientLog(user, user.getSocket(), ERR_NEEDMOREPARAMS, "PASS");
	else if (userPass != serverPass)
		Server::clientLog(user, user.getSocket(), ERR_PASSWDMISMATCH, "PASS");
	else
		return true;
	return false;
}

void	Server::pass(User& user, const std::string& password) {
	if (parsing(user, password, _password) && !user.getAuthentification() && password == _password) {
		user.setAuthentification();
		char	buffer[1024];
		std::string buf(buffer);
		while (!nick(user, buf)) {
			send(user.getSocket(), "NICK ", 5, 0);
			recv(user.getSocket(), buffer, sizeof(buffer), 0);
		}
		std::cout << GREEN << "Client successfully authentified!" << NONE << std::endl;
	}
}