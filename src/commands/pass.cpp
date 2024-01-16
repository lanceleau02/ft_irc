/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:50:01 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 462	ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"  */
/* 464	ERR_PASSWDMISMATCH		":Password incorrect"                         */
/* ************************************************************************** */

static bool parsing(const Client& client, std::string userPass, std::string serverPass) {
	if (client.getAuthentication())
		Server::clientLog(client.getSocket(), ERR_ALREADYREGISTRED(client.getNickname()));
	else if (userPass.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
	else if (userPass != serverPass)
		Server::clientLog(client.getSocket(), ERR_PASSWDMISMATCH(client.getNickname()));
	else
		return true;
	return false;
}

void	Server::pass(Client& client, const std::string& pass) {
	std::istringstream	iss(pass);
	std::string			password;
	
	iss >> password;
	if (!client.getAuthentication() && parsing(client, password, _password) && password == _password) {
		client.setAuthentication(true);
		serverLog(SUCCESS, "PASS command success!");
	}
	else
		serverLog(FAILURE, "PASS command failure!");
}