# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/14 13:54:59 by laprieur          #+#    #+#              #
#    Updated: 2023/12/21 15:30:26 by laprieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= ircserv

# **************************************************************************** #
#                                 INGREDIENTS                                  #
# **************************************************************************** #

SRC				:=	src/main.cpp			\
					src/Server.cpp			\
					src/Client.cpp			\
					src/User.cpp			\
					src/commands/pass.cpp	\
					src/commands/nick.cpp	\
					
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
	@echo "	 / _| |_    (_)_ __ ___ "
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