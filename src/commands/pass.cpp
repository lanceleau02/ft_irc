/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/29 16:12:52 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors replies:
461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"
462 ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"
464	ERR_PASSWDMISMATCH		":Password incorrect" */

static bool parsing(const User& user, const std::string& userPass, const std::string& serverPass) {
	std::string command = "PASS";
	if (user.getAuthentication())
		Server::clientLog(user.getSocket(), ERR_ALREADYREGISTRED());
	else if (userPass.empty())
		Server::clientLog(user.getSocket(), ERR_NEEDMOREPARAMS(command));
	else if (userPass != serverPass)
		Server::clientLog(user.getSocket(), ERR_PASSWDMISMATCH());
	else
		return true;
	return false;
}

void	Server::pass(User& user, const std::string& password) {
	std::cout << "\t**2**" << std::endl;
	if (parsing(user, password, _password) && !user.getAuthentication() && password == _password) {
		user.setAuthentication();
		serverLog(0, "Client successfully authentified!");
	}
}