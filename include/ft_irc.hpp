/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:08:20 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/29 15:51:23 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <map>
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
#include "Channel.hpp"

#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define NONE	"\033[0m"

// CUSTOM COMMAND REPLIES
#define RPL_JOIN(nick, channelName)			(":" + nick + " JOIN " + channelName + "\r\n")

// COMMAND REPLIES
#define RPL_WELCOME(nick, user, host)		("001 :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
#define	RPL_TOPIC(channel, topic)			("332 " + channel + " :" + topic + "\r\n")

// ERROR REPLIES
#define ERR_NOSUCHCHANNEL(channelName)		("403 " + channelName + " :No such channel\r\n")
#define ERR_NONICKNAMEGIVEN()				("431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick)			("432 " + nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick)				("433 " + nick + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nick, channel)	("441 " + nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_USERONCHANNEL(user, channel)	("443 " + user + " " + channel + " :is already on channel\r\n")
#define ERR_NEEDMOREPARAMS(command)			("461 " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED()				("462 :Unauthorized command (already registered)\r\n")
#define ERR_PASSWDMISMATCH()				("464 :Password incorrect\r\n")
#define ERR_CHANNELISFULL(channel)			("471 " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(channel)			("473 " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BANNEDFROMCHAN(channel)			("474 " + channel + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANNELKEY(channel)			("475 " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_BADCHANMASK(channel)			("476 " + channel + " :Bad Channel Mask\r\n")

#define MAX_EVENTS 100

bool	RegExr(const char* pattern, const std::string& input);