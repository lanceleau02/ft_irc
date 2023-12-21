/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:30:36 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 10:56:51 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Errors cases to handle:
ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"
ERR_ALREADYREGISTRED	":Unauthorized command (already registered)"
*/

void	Server::pass(User& user, const std::string& password) {
	if (password.empty())
		clientLog(user, );
	if (password == _password) {
		user.setAuthentification();
		std::cout << GREEN << "Client successfully authentified!" << NONE << std::endl;
		//clientLog()
	}
	else
		std::cout << RED << "Wrong password submitted." << NONE << std::endl;
}