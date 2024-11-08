#############################################################
CXX        = g++
NAME       = vvtbi
SRCDIR     = src
OBJDIR     = obj
TESTDIR    = tests/unit
TEST_OBJDIR = $(OBJDIR)/test
CXXFLAGS   = -Wall -Werror -O2 -Wextra -pedantic -std=c++20 -DNDEBUG
#############################################################
#### DO NOT EDIT BELOW THIS LINE ############################
VERSION    = 3.0
SOURCES    = io.cc tokenizer.cc vvtbi.cc main.cc
OBJS       = $(SOURCES:%.cc=$(OBJDIR)/%.o)

# Test related variables
TEST_SOURCES = io_test.cc
TEST_OBJS    = $(TEST_SOURCES:%.cc=$(TEST_OBJDIR)/%.o)
TEST_TARGET  = run_tests

# Create object directories if they don't exist
$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(TEST_OBJDIR))

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
	@echo "Idea by: <jahan[at]jahanaddison[dot]com>"
	@echo "**************************************************"

# Pattern rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Test object files
$(TEST_OBJDIR)/%.o: $(TESTDIR)/%.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for io.o when building tests
$(TEST_OBJDIR)/io.o: $(SRCDIR)/io.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Test target (only using io.o)
$(TEST_TARGET): $(TEST_OBJDIR)/io_test.o $(TEST_OBJDIR)/io.o
	@$(CXX) $(CXXFLAGS) $^ -o $@ -lCatch2Main -lCatch2
	@echo "Test binary compiled successfully!"

.PHONY: clean test
clean:
	@rm -rf $(OBJDIR) $(NAME) $(TEST_TARGET)

test: $(TEST_TARGET)
	@./$(TEST_TARGET)

.PHONY: all
all: clean $(NAME)
#############################################################
