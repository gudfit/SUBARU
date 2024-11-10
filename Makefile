#############################################################
CXX        = g++
NAME       = vvtbi
SRCDIR     = src
OBJDIR     = obj
TESTDIR    = tests/unit
TEST_OBJDIR = $(OBJDIR)/test
CXXFLAGS   = -Wall -Werror -O2 -Wextra -pedantic -std=c++20 -DNDEBUG
DEBUGFLAGS = -DDEBUG_MODE
#############################################################
#### DO NOT EDIT BELOW THIS LINE ############################
VERSION    = 3.0
SOURCES    = io.cc tokenizer.cc vvtbi.cc main.cc
OBJS       = $(SOURCES:%.cc=$(OBJDIR)/%.o)

# Test related variables
TEST_SOURCES = io_test.cc tokenizer_test.cc 
TEST_OBJS    = $(TEST_SOURCES:%.cc=$(TEST_OBJDIR)/%.o)
TEST_DEPS    = $(TEST_OBJDIR)/io.o $(TEST_OBJDIR)/tokenizer.o $(TEST_OBJDIR)/vvtbi.o
TEST_TARGET  = run_tests

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
$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create the obj directory
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Debug build target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean $(NAME)_debug

$(NAME)_debug: $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)_debug
	@echo "Debug build completed."

# Test object files
$(TEST_OBJDIR)/%.o: $(TESTDIR)/%.cc | $(TEST_OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rules for source files when building tests
$(TEST_OBJDIR)/io.o: $(SRCDIR)/io.cc | $(TEST_OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJDIR)/tokenizer.o: $(SRCDIR)/tokenizer.cc | $(TEST_OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJDIR)/vvtbi.o: $(SRCDIR)/vvtbi.cc | $(TEST_OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create the test_obj directory
$(TEST_OBJDIR):
	@mkdir -p $(TEST_OBJDIR)

# Test target
$(TEST_TARGET): $(TEST_OBJS) $(TEST_DEPS) | $(TEST_TARGET)
	@$(CXX) $(CXXFLAGS) $^ -o $@ -lCatch2Main -lCatch2
	@echo "Test binary compiled successfully!"

# Test target without debug logs
test: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Test target with debug logs
test_debug: CXXFLAGS += $(DEBUGFLAGS)
test_debug: $(TEST_TARGET)
	@./$(TEST_TARGET)

.PHONY: clean test test_debug debug all
clean:
	@rm -rf $(OBJDIR) $(NAME) $(NAME)_debug $(TEST_TARGET)

all: clean $(NAME)
#############################################################
