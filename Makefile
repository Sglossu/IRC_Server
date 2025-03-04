NAME			= ircserv
 
CXX				= g++

FLAGS			= -Wall -Wextra -Werror -MMD -std=c++98
FLAGS			= -Wall -g -MMD

RM				= rm  -rf

INCLUDE			= -I  inc

SRCS_FILES		= main \
				  utils/utils \
				  user/User \
				  user/Channel \
				  server/Server \
				  server/ServerException \
				  server/UserKey \
				  handler/Handler \
				  handler/commands/commands \
				  handler/error_msg \
				  handler/commands/system \
				  handler/commands/cmd_mode \
				  handler/commands/registration \
				  handler/commands/user_cmds \
				  handler/commands/channels \
				  handler/sha256/Sha256 \
				  handler/yaml_parser/YamlParser \
				  message/Message

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