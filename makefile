CXX := g++
# Mac users: add -I/path/to/SFML/include to CXXFLAGS
CXXFLAGS := -Wall -Wextra -pedantic -std=c++17 -g

sfml_flags := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main.exe: main.cpp model.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(sfml_flags)

tests.exe: tests.cpp model.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(sfml_flags)

.PHONY: clean all
clean:
	rm -vf *.exe *.out.txt