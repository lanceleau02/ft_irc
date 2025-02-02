# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/14 13:54:59 by laprieur          #+#    #+#              #
#    Updated: 2024/01/15 14:01:54 by hsebille         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= ircserv

# **************************************************************************** #
#                                 INGREDIENTS                                  #
# **************************************************************************** #

SRC				:=	src/main.cpp				\
					src/Server.cpp				\
					src/Client.cpp				\
					src/Channel.cpp				\
					src/utils.cpp				\
					src/commands/pass.cpp		\
					src/commands/nick.cpp		\
					src/commands/user.cpp		\
					src/commands/join.cpp		\
					src/commands/privmsg.cpp	\
					src/commands/kick.cpp		\
					src/commands/invite.cpp		\
					src/commands/topic.cpp		\
					src/commands/mode.cpp		\
					src/commands/bot.cpp		\
					
SRC_OBJS		:=	$(SRC:%.cpp=.build/%.o)
DEPS			:=	$(SRC_OBJS:%.o=%.d)

CXX				:=	c++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 -g
CPPFLAGS		:=	-MP -MMD -Iinclude
LDFLAGS			:=

# **************************************************************************** #
#                                    TOOLS                                     #
# **************************************************************************** #

MAKEFLAGS		+= --silent --no-print-directory

# **************************************************************************** #
#                                   RECIPES                                    #
# **************************************************************************** #

all: header $(NAME)

$(NAME): $(SRC_OBJS)
	$(CXX) $(CXXFLAGS) $(SRC_OBJS) $(LDFLAGS) -o $(NAME)
	@printf "%b" "$(BLUE)CREATED $(CYAN)$(NAME)\n"

.build/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $(CPPFLAGS) $< -o $@
	@printf "%b" "$(BLUE)CREATED $(CYAN)$@\n"

-include $(DEPS)

clean:
	rm -rf .build

fclean: clean
	rm -rf $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

# **************************************************************************** #
#                                    STYLE                                     #
# **************************************************************************** #

GREEN			:= \033[0;32m
YELLOW			:= \033[0;33m
BLUE			:= \033[0;34m
CYAN			:= \033[0;36m
OFF				:= \033[m

header:
	@printf "%b" "$(GREEN)"
	@echo "	  __ _       _          "
	@echo "	 / _| |_    (_)____ ___ "
	@echo "	| |_| __|   | |  __/ __|"
	@echo "	|  _| |_    | | | | (__ "
	@echo "	|_|  \__|___|_|_|  \___|"
	@echo "		|____|			"
	@echo "	by hsebille and laprieur"
	@echo
	@printf "%b" "$(CYAN)CXX:      $(YELLOW)$(CXX)\n"
	@printf "%b" "$(CYAN)CXXFlags: $(YELLOW)$(CXXFLAGS)\n"
	@printf "%b" "$(OFF)"
	@echo

# **************************************************************************** #
#                                   SPECIAL                                    #
# **************************************************************************** #

.PHONY: all clean fclean re
.DELETE_ON_ERROR: