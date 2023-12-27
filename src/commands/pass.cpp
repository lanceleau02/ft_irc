/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/27 15:44:32 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors cases to handle:
ERR_PASSWDMISMATCH		"<command>: Password incorrect"
ERR_NEEDMOREPARAMS		"<command>: Not enough parameters"
ERR_ALREADYREGISTRED	"<command>: Unauthorized command (already registered)" */

static bool parsing(const User& user, const std::string& userPass, const std::string& serverPass) {
	if (user.getAuthentication())
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
	std::cerr << "password = " << password << std::endl;
	if (parsing(user, password, _password) && !user.getAuthentication() && password == _password) {
		user.setAuthentication();
		serverLog(0, "Client successfully authentified!");
	}
}