/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 13:51:06 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/16 13:04:07 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::bot(Client& client, const std::string& param) {
	std::istringstream	iss(param);
	std::string			quoi;
	
	iss >> quoi;
	if (client.getRegistration() && !param.empty())
		if (quoi == "quoi" || quoi == "QUOI")
			clientLog(client.getSocket(), "COUBEH\n");
}