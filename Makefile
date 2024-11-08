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
TEST_SOURCES = io_test.cc tokenizer_test.cc
TEST_OBJS    = $(TEST_SOURCES:%.cc=$(TEST_OBJDIR)/%.o)
TEST_DEPS    = $(TEST_OBJDIR)/io.o $(TEST_OBJDIR)/tokenizer.o
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

# Special rules for source files when building tests
$(TEST_OBJDIR)/io.o: $(SRCDIR)/io.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJDIR)/tokenizer.o: $(SRCDIR)/tokenizer.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Test target
$(TEST_TARGET): $(TEST_OBJS) $(TEST_DEPS)
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
