#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

struct BestGuess {
  string word;
  double percent;
  BestGuess(string word, double percent) : word(word), percent(percent) {}
};

map<char, vector<int>> calculateFrequencies(const vector<string> &words) {
  map<char, vector<int>> frequencies;
  for (const string &word : words) {
    for (size_t i = 0; i < word.length(); ++i) {
      if (!frequencies.count(word[i])) {
        vector<int> temp(5);
        frequencies[word[i]] = temp;
      }
      frequencies[word[i]][i]++;
    }
  }
  return frequencies;
}

BestGuess findBestStartingWord(const vector<string> &words,
                               const vector<char> &green,
                               const map<int, string> &yellow,
                               const string &black) {
  map<char, vector<int>> frequencies = calculateFrequencies(words);
  string bestWord;
  int bestScore = -1000000; // Start with a low score
  bool debug;
  for (const string &word : words) {
    debug = false;
    int score = 0;
    bool invalidWord = false;
    map<char, int> yellowLettersCount;

    /*if (word == "filet") {
      cout << "word is " << word << endl;
      debug = true;
    }*/

    // Count the occurrences of each yellow letter in the word
    for (auto const &yellowPair : yellow) {
      if (word.find(yellowPair.second) != string::npos) {
        yellowLettersCount[yellowPair.second[0]]++;
      }
    }

    for (size_t i = 0; i < word.length(); ++i) {
      char currentChar = word[i];

      // Check for greens
      if (green[i] != '\0' && green[i] != currentChar) {
        invalidWord = true;
        break;
      }

      // Check for yellows
      if (yellow.find(i) != yellow.end() &&
          yellow.at(i).find(currentChar) != string::npos) {
        // cout << i << endl;
        // cout << "yellow" << endl;
        invalidWord = true;
        break;
      }

      // Check for blacks
      if (black.find(currentChar) != string::npos) {
        // cout << "black" << endl;
        invalidWord = true;
        break;
      }

      score += frequencies[currentChar][i];
    }

    if (debug == true) {
      cout << "Score: " << score << endl;
    }
    for (auto const &yellowPair : yellow) {
      for (char character : yellowPair.second) {
        if (word.find(character) == string::npos) {
          if (debug == true) {
            cout << word << ".find(" << character << ") => "
                 << word.find(character) << endl;
          }
          invalidWord = true;
          break;
        }
      }
    }

    if (!invalidWord && score > bestScore) {
      bestScore = score;
      bestWord = word;
    }
  }

  return BestGuess(bestWord, bestScore);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Wrong argument count" << endl;
    return -1;
  }
  string line;
  ifstream inputFile(argv[1]);
  int count = 0;

  json data;

  if (inputFile.is_open()) {
    while (getline(inputFile, line)) {
      data = nlohmann::json::parse(line);
    }
    inputFile.close();
  } else {
    cout << "Could not open file " << argv[1] << endl;
  }

  vector<string> wordList;
  ifstream wordListFile("wordList.txt");
  string delimiter = " ";

  if (wordListFile.is_open()) {
    while (getline(wordListFile, line)) {
      size_t pos = 0;
      string token;
      while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        wordList.push_back(token);
        line.erase(0, pos + delimiter.length());
      }
    }
    wordListFile.close();
  } else {
    cout << "Could not open file wordList.txt" << endl;
  }

  /*for (string word : wordList) {
    cout << word << endl;
  }*/
  for (int i = 0; i < data["words"].size(); i++) {
    vector<char> green(5);
    map<int, string> yellow;
    string groupedYellow = "";
    string black = "";
    auto words = data["words"][i];
    auto grades = data["grades"][i];
    cout << words << endl;
    cout << grades << endl;
    for (int j = 0; j < words.size(); j++) {
      string tmp_word = words[j];
      string tmp_grade = grades[j];
      for (int k = 0; k < tmp_word.length(); k++) {
        if (tmp_grade[k] == 'K' && black.find(tmp_word[k]) == string::npos &&
            /*(find(green.begin(), green.end(), tmp_word[k]) !=
                 green.end() ||*/
            groupedYellow.find(tmp_word[k]) == string::npos) {
          black += tmp_word[k];
        }
        if (tmp_grade[k] == 'G') {
          green[k] = tmp_word[k];
        }
        if (tmp_grade[k] == 'Y' &&
            yellow[k].find(tmp_word[k]) == string::npos) {
          if (groupedYellow.find(tmp_word[k]) == string::npos) {
            groupedYellow += tmp_word[k];
          }
          yellow[k] += tmp_word[k];
        }
      }
    }
    cout << "===================================" << endl;
    cout << "Green: ";
    for (auto g : green) {
      cout << g << " ";
    }
    cout << endl;
    cout << "Yellow: " << endl;
    for (const auto &[k, v] : yellow) {
      cout << "yellow[" << k << "] = " << v << endl;
    }
    cout << "Black: " << black << endl;
    cout << "===================================" << endl;
    BestGuess guess = findBestStartingWord(wordList, green, yellow, black);
    cout << guess.word << " => " << guess.percent << endl;
    // break;
  }
  return 0;
}
