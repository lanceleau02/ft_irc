/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 11:18:58 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 09:44:09 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	signalHandler(int sig) {
	(void)sig;
	throw std::logic_error("server shutdown");
}

bool	RegExr(const char* pattern, const std::string& input) {
	regex_t		regex;
	regmatch_t	match;

	if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
        return false;
    if (regexec(&regex, input.c_str(), 1, &match, 0) == 0) {
		regfree(&regex);
		return true;
	}
	regfree(&regex);
	return false;
}

std::string	createNickList(Channel channel) {
	std::string nickList;

	for (std::map<int, Client>::const_iterator it = channel.getMap(USERS).begin(); it != channel.getMap(USERS).end(); it++) {
		if (it != channel.getMap(USERS).begin())
			nickList += " ";
		if (channel.isOperator(it->second.getSocket()))
			nickList += "@";
		else
			nickList += "+";
		nickList += it->second.getNickname();
	}
	return nickList;
}

bool	findClient(const std::map<int, Client>& map, std::string nickname) {
	for (std::map<int, Client>::const_iterator it = map.begin(); it != map.end(); it++)
		if (it->second.getNickname() == nickname)
			return true;
	return false;
}
