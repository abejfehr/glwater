CXX = g++
LIBS = -framework GLUT -framework OpenGL -framework CoreFoundation -Llib -lSOIL
CXXFLAGS = -Wall -c -std=c++11
LDFLAGS = $(LIBS)
SRC = .
BUILD = build
OUT = .
EXE = $(OUT)/app

OBJECTS = $(BUILD)/Main.o $(BUILD)/GLWater.o $(BUILD)/Water.o $(BUILD)/Ground.o

all: $(EXE)

$(EXE): $(OBJECTS) | $(OUT)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD)/%.o : $(SRC)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -d -o $@

$(BUILD):
	mkdir -p $(BUILD)

$(OUT):
	mkdir -p $(OUT)

clean:
	rm $(BUILD)/*.o && rm $(EXE)
