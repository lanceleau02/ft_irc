/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 16:16:13 by laprieur         ###   ########.fr       */
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
		nick(user, )
		std::cout << GREEN << "Client successfully authentified!" << NONE << std::endl;
		clientLog()
	}
	else if (password != _password)
		std::cout << RED << "Wrong password submitted." << NONE << std::endl;
}