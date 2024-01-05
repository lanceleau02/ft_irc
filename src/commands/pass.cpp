/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 13:25:49 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 462	ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"  */
/* 464	ERR_PASSWDMISMATCH		":Password incorrect"                         */
/* ************************************************************************** */

static bool parsing(const Client& client, const std::string& userPass, const std::string& serverPass) {
	std::string command = "PASS";
	if (client.getAuthentication())
		Server::clientLog(client.getSocket(), ERR_ALREADYREGISTRED(client.getUsername()));
	else if (userPass.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), command));
	else if (userPass != serverPass)
		Server::clientLog(client.getSocket(), ERR_PASSWDMISMATCH(client.getUsername()));
	else
		return true;
	return false;
}

void	Server::pass(Client& client, const std::string& password) {
	if (parsing(client, password, _password) && !client.getAuthentication() && password == _password) {
		client.setAuthentication(true);
		serverLog(0, "Client successfully authentified!");
	}
}