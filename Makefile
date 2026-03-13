CXX ?= g++
TARGET ?= blockchain

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

OPENMP_FLAGS := -fopenmp
OPENSSL_LIBS := -lssl -lcrypto
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Wpedantic -I$(INC_DIR) $(OPENMP_FLAGS)
LDFLAGS ?=
LDLIBS ?= $(OPENMP_FLAGS) $(OPENSSL_LIBS)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)