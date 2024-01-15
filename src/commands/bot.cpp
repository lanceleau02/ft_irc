/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 13:51:06 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/15 14:44:24 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::bot(Client& client, const std::string& param)
{
	std::istringstream	iss(param);
	std::string			quoi;
	
	iss >> quoi;
	if (client.getRegistration() && !param.empty()) {
		if (quoi == "quoi" || quoi == "QUOI")
			Server::clientLog(client.getSocket(), "COUBEH\n");
	}
}