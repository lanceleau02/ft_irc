/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 16:55:09 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors cases to handle:
ERR_NEEDMOREPARAMS		"<command>: Not enough parameters"
ERR_ALREADYREGISTRED	"<command>: Unauthorized command (already registered)"
*/

void	Server::pass(User& user, const std::string& password) {
	if (user.getAuthentification() == true)
		clientLog(user, user.getSocket(), ERR_ALREADYREGISTRED, "PASS");
	else if (password.empty())
		clientLog(user, user.getSocket(), ERR_NEEDMOREPARAMS, "PASS");
	else if (user.getAuthentification() == false && password == _password) {
		user.setAuthentification();
	
		std::string nickname = "";
		
		while (nick(user, nickname) == false) {
			send(user.getSocket(), "NICK ", 5, 0);
			recv(user.getSocket(), const_cast<char *>(nickname.c_str()), sizeof(nickname), 0);
		}
	
		std::cout << GREEN << "Client successfully authentified!" << NONE << std::endl;
		//clientLog(user, user.getSocket(), )
	}
	else if (password != _password)
		std::cout << RED << "Wrong password submitted." << NONE << std::endl;
}