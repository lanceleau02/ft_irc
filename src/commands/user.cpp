/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 09:51:01 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/11 14:08:08 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 462	ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"  */
/* ************************************************************************** */

static bool parsing(const Client& client, std::string cmd, const std::string& username) {
	if (!(client.getUsername()).empty())
		Server::clientLog(client.getSocket(), ERR_ALREADYREGISTRED(client.getUsername()));
	else if (username.empty())
		Server::clientLog(client.getSocket(), ERR_NEEDMOREPARAMS(client.getUsername(), cmd));
	else
		return true;
	return false;
}

void	Server::user(Client& client, const std::string& user) {
	std::istringstream	iss(user);
	std::string			username;
	
	iss >> username;
	if (client.getAuthentication() && parsing(client, "USER", username) && !client.getNickname().empty()) {
		client.setUsername(username);
	}
}
