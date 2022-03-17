NAME			= ircserv
 
CXX				= g++

FLAGS			= -Wall -Wextra -Werror -MMD -std=c++98
FLAGS			= -Wall -g  -MMD

RM				= rm -rf

INCLUDE			= -I inc

SRCS_FILES		= main \
				  utils/utils \
				  user/User \
				  server/Server \
				  server/ServerException \
				  handler/Handler \
				  commands/pass \

SRC_DIR 		= src/

SRCS			= $(addprefix $(SRC_DIR),$(addsuffix .cpp, $(SRCS_FILES)))
OBJS			= $(patsubst %.cpp,%.o,$(SRCS))

all:			$(NAME)

%.o:			%.cpp
				$(CXX) $(FLAGS) -c $< -o $@ $(INCLUDE) 

$(NAME):		$(OBJS)
				$(CXX) $(FLAGS) -o $(NAME) $(OBJS)		

clean:
				$(RM) $(OBJS) $(OBJS:.o=.d)

fclean:			clean
				$(RM) $(NAME) 

re:				fclean $(NAME)

.PHONY:			all clean fclean re bot
-include		$(OBJS:.o=.d)