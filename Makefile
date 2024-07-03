# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/13 11:42:41 by alvicina          #+#    #+#              #
#    Updated: 2024/06/27 10:50:21 by alvicina         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#SETUP

NAME            = webserv
CC              = c++
RM              = rm -rf
FLAGS           = -Wall -Wextra -Werror -std=c++98 -g3

#FILES AND PATH

HEADER_SRCS = \
	Utils.hpp FileChecker.hpp FileParser.hpp Server.hpp Location.hpp \
	ServerManager.hpp Socket.hpp Epoll.hpp Request.hpp Response.hpp \
		      
HEADER_DIR  = includes/
HEADER      = $(addprefix $(HEADER_DIR), $(HEADER_SRCS))

MPATH_SRCS  =  \
	Main.cpp Utils.cpp FileChecker.cpp FileParser.cpp Server.cpp Location.cpp \
	ServerManager.cpp Socket.cpp Epoll.cpp Request.cpp Response.cpp \

MPATH_DIR   = mandatory/
MPATH       = $(addprefix $(MPATH_DIR), $(MPATH_SRCS))
OBJ_M       = $(MPATH:.cpp=.o)

#COMMANDS
all:        $(NAME)

$(NAME):    $(OBJ_M)
		    @$(CC) $(FLAGS) $(OBJ_M) -o $(NAME) 

%.o: %.cpp $(HEADER) Makefile
		    @$(CC) $(FLAGS) -c $< -o $@

clean:
	        @$(RM) $(OBJ_M)

fclean:	    clean
		    @$(RM) $(NAME)

re:         fclean all

.PHONY: all execute clean fclean re 