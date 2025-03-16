#include "ladder.h"
#include <algorithm>

void error(const string& word1, const string& word2, const string& msg) {
    cerr << "Error with words [\"" << word1 << "\", \"" 
         << word2 << "\"]: " << msg << "\n";
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = (int)str1.size();
    int len2 = (int)str2.size();
    if (abs(len1 - len2) > d) return false;
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (str1[i] != str2[i]) {
                diffCount++;
                if (diffCount > d) return false;
            }
        }
        return (diffCount <= d);
    } else {
        const string& sShort = (len1 < len2) ? str1 : str2;
        const string& sLong  = (len1 < len2) ? str2 : str1;
        int i = 0, j = 0;
        int mismatchCount = 0;
        while (i < (int)sShort.size() && j < (int)sLong.size()) {
            if (sShort[i] == sLong[j]) {
                i++; 
                j++;
            } else {
                mismatchCount++;
                if (mismatchCount > d) return false;
                j++;
            }
        }
        mismatchCount += (sLong.size() - j);
        return (mismatchCount <= d);
    }
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}


void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in.is_open()) {
        throw runtime_error("Could not open file: " + file_name);
    }
    string w;
    while (in >> w) {
        for (char& c : w) {
            c = tolower((unsigned char)c);
        }
        word_list.insert(w);
    }
    in.close();
}


vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list)
{
    if (begin_word == end_word) {
        cerr << "Start word and end word must differ!\n";
        return {};
    }
    if (word_list.find(end_word) == word_list.end()) {
        cerr << "End word '" << end_word 
             << "' is not in dictionary. No ladder possible.\n";
        return {};
    }

    queue<vector<string>> ladder_queue;
    set<string> visited;
    vector<string> start_ladder = { begin_word };
    ladder_queue.push(start_ladder);
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        const string& last_word = ladder.back();
        for (auto const & dict_word : word_list) {
            if (is_adjacent(last_word, dict_word) && !visited.count(dict_word)) {
                visited.insert(dict_word);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(dict_word);
                if (dict_word == end_word) {
                    return new_ladder;
                }
                ladder_queue.push(new_ladder);
            }
        }
    }
    return {};
}


void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found.\n";
        return;
    }
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << " ";
    }
    cout << "\n";
}

void verify_word_ladder(){ {
     set<string> dict = {"cat", "cot", "cog", "dog"};
        vector<string> ladder = generate_word_ladder("cat", "cat", dict);
        // Expect empty ladder.
        assert(ladder.empty());
    }

    // Test Case 2: End word not in the dictionary should yield an empty ladder.
    {
        set<string> dict = {"cat", "cot", "cog", "dog"};
        vector<string> ladder = generate_word_ladder("cat", "bat", dict);
        // "bat" is not in dict.
        assert(ladder.empty());
    }

    // Test Case 3: Known valid ladder (all words same length)
    // Expected: "cat" -> "cot" -> "cog" -> "dog"
    {
        set<string> dict = {"cat", "cot", "cog", "dog"};
        vector<string> ladder = generate_word_ladder("cat", "dog", dict);
        // The ladder must not be empty.
        assert(!ladder.empty());
        // First word should be "cat" and last word "dog"
        assert(ladder.front() == "cat");
        assert(ladder.back() == "dog");
        // Check that every consecutive pair of words is adjacent.
        for (size_t i = 1; i < ladder.size(); i++) {
            assert(is_adjacent(ladder[i - 1], ladder[i]));
        }
    }

    // Test Case 4: Ladder with words of different lengths.
    // For example, one possible ladder from "car" to "cheat" could be:
    // "car" -> "cat" -> "chat" -> "cheat"
    {
        // Notice: Only intermediate and end words must be in the dictionary.
        set<string> dict = {"cat", "chat", "cheat"};
        vector<string> ladder = generate_word_ladder("car", "cheat", dict);
        // Ensure that a ladder was found.
        assert(!ladder.empty());
        // Verify the ladder starts with the begin word and ends with the target.
        assert(ladder.front() == "car");
        assert(ladder.back() == "cheat");
        // Verify that each consecutive pair is adjacent.
        for (size_t i = 1; i < ladder.size(); i++) {
            assert(is_adjacent(ladder[i - 1], ladder[i]));
        }
    }

    cout << "[verify_word_ladder] All test cases passed.\n";
}