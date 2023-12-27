/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:08:20 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/27 16:46:04 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex.h>

#include "Server.hpp"
#include "User.hpp"
#include "Client.hpp"

#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define NONE	"\033[0m"

// COMMAND REPLIES
#define RPL_WELCOME(nick, user, host) (": 001 " + ":Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)

// ERROR REPLIES
#define ERR_ALREADYREGISTRED() (": 462 :Unauthorized command (already registered)")
#define ERR_NEEDMOREPARAMS(command) (": 461 " + command + " :Not enough parameters")
#define ERR_PASSWDMISMATCH() (": 464 :Password incorrect")

#define MAX_EVENTS 10

bool	RegExr(const char* pattern, const std::string& input);