/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 09:51:01 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 13:49:18 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 462	ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"  */
/* ************************************************************************** */

static bool parsing(const Client& client, std::string username) {
	if (username.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
	else if (!(client.getUsername()).empty())
		Server::clientLog(client.getSocket(), ERR_ALREADYREGISTRED(client.getNickname()));
	else
		return true;
	return false;
}

void	Server::user(Client& client, const std::string& user) {
	std::istringstream	iss(user);
	std::string			username;
	
	iss >> username;
	if (client.getAuthentication() && parsing(client, username)) {
		client.setUsername(username);
		serverLog(SUCCESS, "USER command success!");
	}
	else
		serverLog(FAILURE, "USER command failure!");
}
