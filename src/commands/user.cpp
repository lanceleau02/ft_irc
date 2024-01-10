/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 09:51:01 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/10 11:51:22 by laprieur         ###   ########.fr       */
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
	if (parsing(client, "USER", username) && client.getAuthentication() && !client.getNickname().empty())
		client.setUsername(username);
}
