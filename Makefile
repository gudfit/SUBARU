#############################################################
CXX        = g++
NAME       = vvtbi
SRCDIR     = src
OBJDIR     = obj
CXXFLAGS   = -Wall -Werror -O2 -Wextra -pedantic -std=c++20
#############################################################
#### DO NOT EDIT BELOW THIS LINE ############################
VERSION    = 3.0
SOURCES    = io.cc tokenizer.cc vvtbi.cc main.cc
OBJS       = $(SOURCES:%.cc=$(OBJDIR)/%.o)

# Create object directory if it doesn't exist
$(shell mkdir -p $(OBJDIR))

# Main target
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo ""
	@echo "**************************************************"
	@echo " Vvtbi: \"Very, Very, Tiny, Basic\" Interpreter"
	@echo " Version: "$(VERSION)
	@echo " Vvtbi (the \"software\") is distributed under"
	@echo " the terms of the Apache License, Version 2.0."
	@echo " For more information, see README and CHANGELOG."
	@echo "         <nnamdidaniel114[at]gmail[dot]com>"
	@echo "Idea by: <jahan.addison[at]jacata[dot]me>"
	@echo "**************************************************"

# Pattern rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf $(OBJDIR) $(NAME)

.PHONY: all
all: clean $(NAME)
#############################################################
