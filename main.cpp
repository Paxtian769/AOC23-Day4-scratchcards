#include <iostream>
#include <fstream>
#include <list>
#include <string>


bool is_num(char c) {
    return ('0' <= c && c <= '9');
}


void parse_game_num(std::string line, int &pos) {
    while (line[pos] != ':') {
        pos++;
    }
    while (!is_num(line[pos])) {
        pos++;
    }
}

int parse_num(std::string line, int &pos) {
    int val = 0;

    while (is_num(line[pos]) && pos < line.length()) {
        val = (val * 10) + (line[pos] - '0');
        pos++;
    }

    return val;
}

void parse_numbers_had(std::string line, std::list<int> &numbers_had, int &pos) {
    while (line[pos] != '|' && pos < line.length()) {
        if (is_num(line[pos])) {
            int num = parse_num(line, pos);
            numbers_had.push_back(num);
            pos++;
        }
        else {
            pos++;
        }
    }
}


void parse_winning_numbers(std::string line, std::list<int> &winning_numbers, int &pos) {
    while (pos < line.length()) {
        if (is_num(line[pos])) {
            int num = parse_num(line, pos);
            winning_numbers.push_back(num);
            pos++;
        }
        else {
            pos++;
        }
    }
}


void parse_line(std::string line, std::list<int> &numbers_had, std::list<int> &winning_numbers) {
    int pos=0;
    parse_game_num(line, pos);
    parse_numbers_had(line, numbers_had, pos);
    parse_winning_numbers(line, winning_numbers, pos);
}

int calculate_matches(std::list<int> numbers_had, std::list<int> winning_numbers) {
    int num_matches = 0;

    numbers_had.sort();
    winning_numbers.sort();

    for (auto number_owned : numbers_had) {
        for (auto winning_number : winning_numbers) {
            if (number_owned == winning_number) {
                num_matches++;
            }
        }
    }

    return num_matches;
}

int calculate_score(int num_matches) {
    int score = 0;
    if (num_matches > 0) {
        score = 1;
        for (int i=1; i<num_matches; i++) {
            score *= 2;
        }
    }

    return score;
}

int main(int argc, char **argv) {
    std::fstream input_file;
    input_file.open("input.txt");
    std::string current_line;
    int num_scratchcards = 216;
    int scratchcard_copies[num_scratchcards];
    int scratchcard_matches[num_scratchcards];

    for (int i=0; i<num_scratchcards; i++) {
        scratchcard_copies[i]=1;
    }

    int total_score=0;

    int line=0;
    while (std::getline(input_file, current_line)) {
        std::list<int> numbers_had;
        std::list<int> winning_numbers;
        parse_line(current_line, numbers_had, winning_numbers);
        int num_matches = calculate_matches(numbers_had, winning_numbers);
        scratchcard_matches[line] = num_matches;
        line++;
        total_score += calculate_score(num_matches);
    }

    std::cout << "Total score = " << total_score << '\n';

    int total_scratchcards = 0;

    for (int cur_scratchcard = 0; cur_scratchcard < num_scratchcards; cur_scratchcard++) {
        if (scratchcard_matches[cur_scratchcard] > 0) {
            for (int i = 1; cur_scratchcard+i < num_scratchcards && i < scratchcard_matches[cur_scratchcard]+1; i++) {
                for (int j = 0; j < scratchcard_copies[cur_scratchcard]; j++) {
                    scratchcard_copies[cur_scratchcard+i] += 1;
                }
            }
        }
    }

    for (int i = 0; i < num_scratchcards; i++) {
        total_scratchcards += scratchcard_copies[i];
    }

    std::cout << "Total scratchcards = " << total_scratchcards << '\n';

    return 0;
}
