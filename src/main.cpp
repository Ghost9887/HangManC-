#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>

class Global {

    public:
        Global(int lives) {
            this->lives = lives;
        }
        void set_word(std::string word) {
            this->word = word;
        }
        int get_lives() {
            return this->lives;
        }
        std::string *get_word() {
            return &this->word;
        }
        void print_global() {
            std::cout << "Lives: " << lives << '\n';
            std::cout << "Word: " << word << '\n';
        }
        void decrease_lives() {
            this->lives -= 1;
        }
    private:
        std::string word;
        int lives;
};


std::string get_word(const std::string *file_name) {
    std::ifstream word_file(*file_name);

    if (!word_file) {
        throw std::runtime_error("Cannot open file");
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(word_file, line)) {
        lines.push_back(line);
    }

    if (lines.empty()) {
        throw std::runtime_error("Cannot read file");
    }

    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    int index = std::rand() % lines.size();
    return lines[index];
}

std::string print_guesses(std::vector<char> *guesses) {
    std::string str_guesses;
    str_guesses += '[';
    for (char c : *guesses) {
       str_guesses += c;
       str_guesses += ',';
    }
    str_guesses += ']';

    return str_guesses;
}

bool is_valid(char guess) {
    return 'a' <= guess && guess <= 'z';
}

void run_game(Global *global) {
    std::cout << "Welcome to hangman";
    
    std::string temp_word;
    for (int i = 0; i < global->get_word()->length(); i++) {
        temp_word += "*";
    }
    
    std::vector<char> guesses;
while (true) {
        std::cout << "Lives: " << global->get_lives() << '\n';
        std::cout << temp_word << '\n';
        std::cout << print_guesses(&guesses) << '\n'; 
        std::cout << "Type in your guess: " << '\n';

        std::string buff;
        std::cin >> buff;
        char guess = buff[0];
        
        if (std::find(guesses.begin(), guesses.end(), guess) != guesses.end()) {
            std::cout << "Already guessed that character try again!" << '\n';
            continue;
        }

        if (!is_valid(guess)) {
            std::cout << "Invalid character: Only lowercase a - z" << '\n';
            continue;
        }

        guesses.push_back(guess);
        std::string *word = global->get_word();
        bool found = false;
        for (int i = 0; i < word->length(); i++) {
            if (guess == word->at(i)) {
                temp_word.at(i) = guess;
                found = true;
            }
        }

        if (!found) global->decrease_lives();
        
        if (global->get_lives() <= 0) {
            std::cout << "You lose!" << '\n';
            std::cout << "The word was: " << *word << '\n';
            break;
        }
        if (temp_word == *word) {
            std::cout << "You win!" << '\n';
            break;
        }
    }
}


int main() {
    const int lives = 8;
    const std::string file_name = "../words.txt";
    Global global(lives);
    global.set_word(get_word(&file_name));

    run_game(&global);

    return 0;
}
