
CXX       := g++
CXXSTD    := -std=c++20

CXXFLAGS  := $(CXXSTD) -O2 -ffunction-sections -fdata-sections -DNDEBUG \
             -Wall -Wextra -Wpedantic -MMD -MP

LDFLAGS   := -Wl,--gc-sections -Wl,--strip-all
LIBS      := -lsodium

SRC_DIR   := src
BUILD_DIR := build

TARGET    := $(BUILD_DIR)/pwgn

SRCS      := $(wildcard $(SRC_DIR)/*.cpp)
OBJS      := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS      := $(OBJS:.o=.d)

.PHONY: all clean run help

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "[LINK]   $@"
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
	@echo "[STRIP]  Final cleanup: removing all debug, symbol & metadata sections"
	@strip --strip-all --remove-section=.comment --remove-section=.note \
	       --remove-section=.note.gnu.build-id $@ 2>/dev/null || strip -s $@
	@echo "[OK]     Clean binary built: $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "[CXX]    $<"
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

run: $(TARGET)
	@./$(TARGET)

clean:
	@echo "[CLEAN]  Removing $(BUILD_DIR)/"
	@rm -rf $(BUILD_DIR)
	@echo "[OK]     Clean complete."

help:
	@echo "Usage: make [TARGET]"
	@echo ""
	@echo "Targets:"
	@echo "  all     - Build the project (default)"
	@echo "  run     - Build and execute the program"
	@echo "  clean   - Remove build artifacts"
	@echo "  help    - Show this help message"

-include $(DEPS)