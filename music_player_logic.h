//music_player_logic.h
#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <algorithm>
#include <deque>
#include <cstdio>
#include <fstream>
#include <map> // Still needed for std::map for now, but will be removed once user management completely switches
#include <iomanip>
#include <sstream>
#include <cctype> // For std::isalpha, std::isdigit

// (these are needed because MusicPlayerGUI uses MusicPlayerLogic)
class MusicPlayerLogic;

// Basic Hashing Function (NOT SECURE FOR PRODUCTION)
std::string simpleHash(const std::string& input);

// 1. Song Class
class Song {
public:
    std::string filePath;
    std::string title;
    std::string artist;
    sf::Time duration;

    Song(const std::string& path = "", const std::string& name = "", const std::string& art = "Unknown Artist", sf::Time dur = sf::Time::Zero);
    bool operator<(const Song& other) const;
    bool operator>(const Song& other) const;
    bool operator==(const Song& other) const;
    bool operator!=(const Song& other) const;
};

// 2. Doubly Linked List (for songs within a playlist)
template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(const T& val);
};

template <typename T>
class DoublyLinkedList {
public:
    Node<T>* head;
    Node<T>* tail;
    int size;

    DoublyLinkedList();
    ~DoublyLinkedList();
    void add(const T& val);
    bool remove(const T& val);
    T* get(int index);
    const T* get(int index) const;
    void clear();
};

// 3. Binary Search Tree (for searching songs and now for users)
template <typename T>
class TreeNode {
public:
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;

    TreeNode(const T& val);
};

template <typename T>
class BinarySearchTree {
private:
    TreeNode<T>* root;
    TreeNode<T>* insert(TreeNode<T>* node, const T& val);
    void inorderTraversal(TreeNode<T>* node, std::vector<T>& result) const;
    void clear(TreeNode<T>* node);
    void searchByPartialTitleRecursive(TreeNode<T>* node, const std::string& partialTitle, std::vector<T>& results) const;
    TreeNode<T>* searchRecursive(TreeNode<T>* node, const T& val) const; // New search recursive method

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void insert(const T& val);
    std::vector<T> searchByPartialTitle(const std::string& partialTitle) const; // For Songs only
    std::vector<T> getAllSongs() const; // For Songs only, will become getAllItems
    const T* search(const T& val) const; // New public search method (const)
    T* search(const T& val);             // New public search method (non-const)
    std::vector<T> getAllItems() const;  // Generic getAllItems for any type T
    bool isEmpty() const;
};

// 4. Stack (for recently played history)
template <typename T>
class Stack {
private:
    std::vector<T> elements;

public:
    Stack();
    void push(const T& item);
    T pop();
    T& top();
    bool isEmpty() const;
    size_t size() const;
    void clear();
};

// 5. Queue (generic implementation)
template <typename T>
class Queue {
private:
    std::deque<T> elements;

public:
    Queue() = default;
    void enqueue(const T& item) { elements.push_back(item); }
    T dequeue() {
        if (isEmpty()) throw std::out_of_range("Queue is empty");
        T item = elements.front();
        elements.pop_front();
        return item;
    }
    T& front() {
        if (isEmpty()) throw std::out_of_range("Queue is empty");
        return elements.front();
    }
    bool isEmpty() const { return elements.empty(); }
    size_t size() const { return elements.size(); }
    void clear() { elements.clear(); }
};

// 6. Playlist Class
class Playlist {
public:
    std::string name;
    DoublyLinkedList<Song> songsList;
    BinarySearchTree<Song> songsTree;

    Playlist(const std::string& n = "");
    void addSong(const Song& song);
    bool removeSong(const Song& song);
    std::vector<Song> searchSongs(const std::string& partialTitle) const;
    std::vector<Song> getAllSongs() const;
    bool isEmpty() const;
};

// User Class for Login/Signup
struct User {
    std::string username;
    std::string hashedPassword;
    std::vector<Playlist> playlists; // Vector of playlists for each user

    User(const std::string& user = "", const std::string& passHash = "");

    // Comparison operators for BinarySearchTree
    bool operator<(const User& other) const { return username < other.username; }
    bool operator>(const User& other) const { return username > other.username; }
    bool operator==(const User& other) const { return username == other.username; }
};

// App States
enum class AppState {
    Login,
    Signup,
    MainApp
};

// Enum for song prompt options
enum class AddSongPromptOption { NONE, ADD_TO_PLAYLIST, ADD_TO_QUEUE };

// MusicPlayerLogic Class
class MusicPlayerLogic {
private:
    sf::Music musicPlayer;
    sf::Clock messageTimer;
    const float messageDisplayDuration = 3.0f;

    AppState currentAppState;
    BinarySearchTree<User> usersTree; // Changed from std::map to BinarySearchTree
    std::string currentLoggedInUsername;

    Playlist allSongsGlobalPlaylist;
    Playlist queueDisplayPlaylist;
    Playlist* currentActivePlaylist;

    int currentSongIndex;
    bool isPlaying;

    Stack<Song> playHistory;
    std::deque<int> upcomingQueueIndices;
    Playlist* savedActivePlaylist;
    int savedSongIndex;

    std::string currentMessage;

    std::string formatTime(sf::Time time);
    void loadUsers();
    void saveUsers();
    void loadUserPlaylists(const std::string& username);
    void saveUserPlaylists(const std::string& username);
    // Helper to get a mutable User pointer from the tree
    User* getMutableUser(const std::string& username);


public:
    MusicPlayerLogic();
    ~MusicPlayerLogic();

    // Getters
    AppState getAppState() const;
    std::string getCurrentLoggedInUsername() const;
    const std::vector<Playlist>& getUserPlaylists() const;
    const Playlist& getAllSongsGlobalPlaylist() const;
    const Playlist* getCurrentActivePlaylist() const;
    int getCurrentSongIndex() const;
    bool getIsPlaying() const;
    std::string getCurrentSongDisplay() const;
    sf::Time getMusicPlayingOffset() const;
    sf::Time getMusicDuration() const;
    float getMusicVolume() const;
    const std::string& getMessageText() const;
    sf::Time getMessageElapsedTime() const;
    float getMessageDisplayDuration() const;
    size_t getQueueSize() const;
    const std::deque<int>& getUpcomingQueueIndices() const;

    // Setter for AppState
    void setAppState(AppState state);

    // Core functionalities
    void showMessage(const std::string& msg);
    void attemptLogin(const std::string& username, const std::string& password);
    void attemptSignup(const std::string& username, const std::string& password, const std::string& confirmPassword);
    void logoutUser();
    void selectPlaylist(const std::string& playlistName);
    void playSongByIndexInActivePlaylist(int index);
    void togglePlayPause();
    void playNextSong();
    void playPreviousSong();
    void setMusicPlayingOffset(float normalizedPosition); // For scrubber
    void seekMusic(float secondsDelta); // For arrow keys
    void adjustVolume(int delta);
    void createNewPlaylist(const std::string& name);
    std::vector<Song> searchSongsInActivePlaylist(const std::string& query) const;
    void addSongToQueueFromGlobalIndex(int globalSongIndex);
    void addSongToCustomPlaylistFromGlobalIndex(int globalSongIndex, const std::string& playlistName);
    void updateLogic();
}; 
