/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 13:37:20 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/10 13:46:18 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 331	RPL_NOTOPIC				"<channel> :No topic is set"                  */
/* 332	RPL_TOPIC				"<channel> :<topic>"                          */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 441	ERR_USERNOTINCHANNEL	"<nick> <chan> :They aren't on that channel"  */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 467	ERR_KEYSET				"<channel> :Channel key already set"          */
/* 472	ERR_UNKNOWNMODE			"<char> :is unknown mode char to me for <channel>"*/
/* 477	ERR_NOCHANMODES			"<channel> :Channel doesn't support modes"    */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */