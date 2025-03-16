#include <gtest/gtest.h>

#include "dijkstras.h"
#include "ladder.h"

TEST(EditDistanceWithinTest, SameLengthOneDifference) {
    EXPECT_TRUE(edit_distance_within("cat", "bat", 1));
    EXPECT_FALSE(edit_distance_within("cat", "dog", 1));
}

TEST(EditDistanceWithinTest, InsertionDeletion) {
    EXPECT_TRUE(edit_distance_within("chat", "cheat", 1));
    EXPECT_TRUE(edit_distance_within("car", "cat", 1));
}

TEST(IsAdjacentTest, Basic) {
    EXPECT_TRUE(is_adjacent("date", "data")); 
    EXPECT_TRUE(is_adjacent("chat", "cheat"));
    EXPECT_FALSE(is_adjacent("cat", "dog"));
}

TEST(LoadWordsTest, LoadsAndConvertsToLowercase) {
    string filename = "temp_test_words.txt";
    ofstream out(filename);
    out << "Apple\nBanana\nCherry\n";
    out.close();

    set<string> word_list;
    load_words(word_list, filename);
    EXPECT_EQ(word_list.size(), 3);
    EXPECT_TRUE(word_list.find("apple") != word_list.end());
    EXPECT_TRUE(word_list.find("banana") != word_list.end());
    EXPECT_TRUE(word_list.find("cherry") != word_list.end());

    remove(filename.c_str());
}

TEST(WordLadderTest, ValidLadder) {
    set<string> dict = {"cat", "cot", "cog", "dog"};
    vector<string> ladder = generate_word_ladder("cat", "dog", dict);
    vector<string> expected = {"cat", "cot", "cog", "dog"};
    EXPECT_EQ(ladder, expected);
}

TEST(WordLadderTest, SameStartEndReturnsEmpty) {
    set<string> dict = {"cat", "cot", "cog", "dog"};
    vector<string> ladder = generate_word_ladder("cat", "cat", dict);
    EXPECT_TRUE(ladder.empty());
}

TEST(WordLadderTest, EndWordNotInDictionaryReturnsEmpty) {
    set<string> dict = {"cat", "cot", "cog", "dog"};
    vector<string> ladder = generate_word_ladder("cat", "bat", dict);
    EXPECT_TRUE(ladder.empty());
}

TEST(PrintWordLadderTest, CapturesOutput) {
    vector<string> ladder = {"cat", "cot", "cog", "dog"};
    testing::internal::CaptureStdout();
    print_word_ladder(ladder);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Ladder found"), string::npos);
    EXPECT_NE(output.find("cat -> cot -> cog -> dog"), string::npos);
}

// -------------------------
// Tests for dijkstras.cpp
// -------------------------

TEST(Dijkstras, DijkstraShortestPath) {
    // Construct a graph with 20 vertices (so indices 0, 7, 15, 5, 14, 11, 12 are valid).
    // Build a chain: 0 -> 7 -> 15 -> 5 -> 14 -> 11 -> 12, each with weight 0.
    Graph G;
    G.numVertices = 20;
    G.resize(G.numVertices);
    G[0].push_back(Edge(0, 7, 0));
    G[7].push_back(Edge(7, 15, 0));
    G[15].push_back(Edge(15, 5, 0));
    G[5].push_back(Edge(5, 14, 0));
    G[14].push_back(Edge(14, 11, 0));
    G[11].push_back(Edge(11, 12, 0));

    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    // Extract the shortest path from 0 to 12.
    vector<int> path = extract_shortest_path(distances, previous, 12);
    vector<int> expected_path = {0, 7, 15, 5, 14, 11, 12};
    EXPECT_EQ(path, expected_path);
}

// Test extract_shortest_path on a small graph.
TEST(Dijkstras, ExtractShortestPath) {
    // Build a small graph: vertices 0, 1, 2.
    // Edges: 0 -> 1 (weight 0), 1 -> 2 (weight 15)
    Graph G;
    G.numVertices = 3;
    G.resize(3);
    G[0].push_back(Edge(0, 1, 0));
    G[1].push_back(Edge(1, 2, 15));

    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    // Expected distances: 0 for vertex 0, 0 for vertex 1, 15 for vertex 2.
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 0);
    EXPECT_EQ(distances[2], 15);

    // Extract shortest path from 0 to 2.
    vector<int> path = extract_shortest_path(distances, previous, 2);
    vector<int> expected_path = {0, 1, 2};
    EXPECT_EQ(path, expected_path);
}

// Test print_path() when no path is found.
// Expected output: (an empty line for the path) then a line "Total cost is 8\n"
TEST(Dijkstras, PrintPath_NoPath) {
    testing::internal::CaptureStdout();
    vector<int> emptyPath;  // No path
    print_path(emptyPath, 8);
    string output = testing::internal::GetCapturedStdout();
    // Expected: an empty line for the path (i.e. just "\n") then "Total cost is 8\n"
    string expected_output = "\nTotal cost is 8\n";
    EXPECT_EQ(output, expected_output);
}

// Test print_path() when a valid path is provided.
TEST(Dijkstras, PrintPath_WithPath) {
    testing::internal::CaptureStdout();
    // Test with path: {1, 5, 6, 2, 8} and total cost 7.
    vector<int> path = {1, 5, 6, 2, 8};
    print_path(path, 7);
    string output = testing::internal::GetCapturedStdout();
    // Expected output: vertices separated by a space, then newline, then "Total cost is 7\n"
    string expected_output = "1 5 6 2 8 \nTotal cost is 7\n";
    EXPECT_EQ(output, expected_output);
}

// Test print_path() with a longer path.
TEST(Dijkstras, PrintPath_WithLongPath) {
    testing::internal::CaptureStdout();
    // Test with path: {1, 5, 6, 2, 8, 10, 3} and total cost 0.
    vector<int> path = {1, 5, 6, 2, 8, 10, 3};
    print_path(path, 0);
    string output = testing::internal::GetCapturedStdout();
    string expected_output = "1 5 6 2 8 10 3 \nTotal cost is 0\n";
    EXPECT_EQ(output, expected_output);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



