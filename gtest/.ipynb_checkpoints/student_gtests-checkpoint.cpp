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

TEST(DijkstraTest, BasicShortestPath) {
    // Create a simple directed graph.
    // Vertices: 0, 1, 2, 3.
    // Edges:
    //   0 -> 1 (weight 1)
    //   0 -> 2 (weight 4)
    //   1 -> 2 (weight 2)
    //   1 -> 3 (weight 5)
    //   2 -> 3 (weight 1)
    Graph G;
    G.numVertices = 4;
    G.resize(4);
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 5));
    G[2].push_back(Edge(2, 3, 1));

    vector<int> previous(4, -1);
    vector<int> dist = dijkstra_shortest_path(G, 0, previous);
    vector<int> expected_dist = {0, 1, 3, 4};
    EXPECT_EQ(dist, expected_dist);

    vector<int> path = extract_shortest_path(dist, previous, 3);
    vector<int> expected_path = {0, 1, 2, 3};
    EXPECT_EQ(path, expected_path);
}

TEST(DijkstraTest, NoPath) {
    Graph G;
    G.numVertices = 2;
    G.resize(2);
    vector<int> previous(2, -1);
    vector<int> dist = dijkstra_shortest_path(G, 0, previous);
    EXPECT_EQ(dist[1], INF);
    vector<int> path = extract_shortest_path(dist, previous, 1);
    EXPECT_TRUE(path.empty());
}

TEST(PrintPathTest, CapturesOutput) {
    vector<int> path = {0, 1, 2, 3};
    int total = 4;
    testing::internal::CaptureStdout();
    print_path(path, total);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Shortest path"), string::npos);
    EXPECT_NE(output.find("0 -> 1 -> 2 -> 3"), string::npos);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}