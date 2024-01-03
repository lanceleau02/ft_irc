/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 11:18:58 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/03 11:33:42 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

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

std::string	createNickList(const std::map<int, User>& users, Channel channel) {
	std::string nickList;
	for (std::map<int, User>::const_iterator it = users.begin(); it != users.end(); it++) {
		if (it != users.begin())
			nickList += " ";
		if (it->second.isOperator(channel))
			nickList += "@";
		nickList += it->second.getNickname();
	}
	return nickList;
}
