/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/04 13:38:04 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 462	ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"  */
/* 464	ERR_PASSWDMISMATCH		":Password incorrect"                         */
/* ************************************************************************** */

static bool parsing(const User& user, const std::string& userPass, const std::string& serverPass) {
	std::string command = "PASS";
	if (user.getAuthentication())
		Server::clientLog(user.getSocket(), ERR_ALREADYREGISTRED(user.getUsername()));
	else if (userPass.empty())
		Server::clientLog(user.getSocket(), ERR_NEEDMOREPARAMS(user.getUsername(), command));
	else if (userPass != serverPass)
		Server::clientLog(user.getSocket(), ERR_PASSWDMISMATCH(user.getUsername()));
	else
		return true;
	return false;
}

void	Server::pass(User& user, const std::string& password) {
	if (parsing(user, password, _password) && !user.getAuthentication() && password == _password) {
		user.setAuthentication();
		serverLog(0, "Client successfully authentified!");
	}
}