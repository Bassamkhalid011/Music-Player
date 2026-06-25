// music_player_logic.cpp
#include "music_player_logic.h" // Include the logic header
#include <iomanip> // For std::quoted

// --- Basic Hashing Function (NOT SECURE FOR PRODUCTION) ---
std::string simpleHash(const std::string& input) {
    std::string hashed = "";
    for (char c : input) {
        hashed += std::to_string(static_cast<int>(c) * 7 % 100); // Very basic "hash"
    }
    return hashed;
}

// --- Song Class Implementation ---
Song::Song(const std::string& path, const std::string& name, const std::string& art, sf::Time dur)
    : filePath(path), title(name), artist(art), duration(dur) {
}

bool Song::operator<(const Song& other) const { return title < other.title; }
bool Song::operator>(const Song& other) const { return title > other.title; }
bool Song::operator==(const Song& other) const { return filePath == other.filePath; }
bool Song::operator!=(const Song& other) const { return !(*this == other); }

// --- Doubly Linked List Implementation ---
template <typename T>
Node<T>::Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template <typename T>
void DoublyLinkedList<T>::add(const T& val) {
    Node<T>* newNode = new Node<T>(val);
    if (!head) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

template <typename T>
bool DoublyLinkedList<T>::remove(const T& val) {
    Node<T>* current = head;
    while (current) {
        if (current->data == val) { // Requires T to have operator==
            if (current->prev) {
                current->prev->next = current->next;
            }
            else {
                head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            else {
                tail = current->prev;
            }
            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
T* DoublyLinkedList<T>::get(int index) {
    if (index < 0 || index >= size) {
        return nullptr;
    }
    Node<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return &(current->data);
}

template <typename T>
const T* DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= size) {
        return nullptr;
    }
    Node<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return &(current->data);
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    Node<T>* current = head;
    while (current) {
        Node<T>* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// --- Binary Search Tree Implementation ---
template <typename T>
TreeNode<T>::TreeNode(const T& val) : data(val), left(nullptr), right(nullptr) {}

template <typename T>
BinarySearchTree<T>::BinarySearchTree() : root(nullptr) {}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    clear(root);
}

template <typename T>
TreeNode<T>* BinarySearchTree<T>::insert(TreeNode<T>* node, const T& val) {
    if (!node) {
        return new TreeNode<T>(val);
    }
    if (val < node->data) {
        node->left = insert(node->left, val);
    }
    else if (val > node->data) {
        node->right = insert(node->right, val);
    }
    // If val == node->data, it's a duplicate, do nothing.
    return node;
}

template <typename T>
void BinarySearchTree<T>::insert(const T& val) {
    root = insert(root, val);
}

template <typename T>
void BinarySearchTree<T>::inorderTraversal(TreeNode<T>* node, std::vector<T>& result) const {
    if (node) {
        inorderTraversal(node->left, result);
        result.push_back(node->data);
        inorderTraversal(node->right, result);
    }
}

template <typename T>
void BinarySearchTree<T>::clear(TreeNode<T>* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        // Added logging for debugging the exception
        // std::cout << "Clearing node address: " << node << std::endl; 
        delete node;
    }
}

template <typename T>
void BinarySearchTree<T>::searchByPartialTitleRecursive(TreeNode<T>* node, const std::string& partialTitle, std::vector<T>& results) const {
    if (node) {
        // Only if T is Song
        if constexpr (std::is_same_v<T, Song>) {
            // Convert both to lowercase for case-insensitive search
            std::string nodeTitleLower = node->data.title;
            std::string partialTitleLower = partialTitle;
            std::transform(nodeTitleLower.begin(), nodeTitleLower.end(), nodeTitleLower.begin(), ::tolower);
            std::transform(partialTitleLower.begin(), partialTitleLower.end(), partialTitleLower.begin(), ::tolower);

            if (nodeTitleLower.find(partialTitleLower) != std::string::npos) {
                results.push_back(node->data);
            }
        }
        // Recursively search in both subtrees
        searchByPartialTitleRecursive(node->left, partialTitle, results);
        searchByPartialTitleRecursive(node->right, partialTitle, results);
    }
}


template <typename T>
TreeNode<T>* BinarySearchTree<T>::searchRecursive(TreeNode<T>* node, const T& val) const {
    if (!node || val == node->data) {
        return node;
    }
    if (val < node->data) {
        return searchRecursive(node->left, val);
    }
    else {
        return searchRecursive(node->right, val);
    }
}

// New public search method (const)
template <typename T>
const T* BinarySearchTree<T>::search(const T& val) const {
    TreeNode<T>* resultNode = searchRecursive(root, val);
    return resultNode ? &(resultNode->data) : nullptr;
}

// New public search method (non-const)
template <typename T>
T* BinarySearchTree<T>::search(const T& val) {
    TreeNode<T>* resultNode = searchRecursive(root, val);
    return resultNode ? &(resultNode->data) : nullptr;
}


template <typename T>
std::vector<T> BinarySearchTree<T>::searchByPartialTitle(const std::string& partialTitle) const {
    std::vector<T> results;
    searchByPartialTitleRecursive(root, partialTitle, results);
    return results;
}

// For Songs only
template <typename T>
std::vector<T> BinarySearchTree<T>::getAllSongs() const {
    std::vector<T> result;
    inorderTraversal(root, result);
    return result;
}

// Generic getAllItems for any type T
template <typename T>
std::vector<T> BinarySearchTree<T>::getAllItems() const {
    std::vector<T> result;
    inorderTraversal(root, result);
    return result;
}

template <typename T>
bool BinarySearchTree<T>::isEmpty() const {
    return root == nullptr;
}

// --- Stack Implementation ---
template <typename T>
Stack<T>::Stack() {}

template <typename T>
void Stack<T>::push(const T& item) {
    elements.push_back(item);
}

template <typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    T item = elements.back();
    elements.pop_back();
    return item;
}

template <typename T>
T& Stack<T>::top() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    return elements.back();
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return elements.empty();
}

template <typename T>
size_t Stack<T>::size() const {
    return elements.size();
}

template <typename T>
void Stack<T>::clear() {
    elements.clear();
}

// --- Queue Implementation ---
// Constructor and methods defined in header with default/inline definitions.
// Only explicit template instantiations or separate definitions needed here.

// --- Playlist Class Implementation ---
Playlist::Playlist(const std::string& n) : name(n) {}

void Playlist::addSong(const Song& song) {
    songsList.add(song); // Add to linked list for order
    songsTree.insert(song); // Add to BST for search
}

bool Playlist::removeSong(const Song& song) {
    // Note: This only removes from the list. If you want to remove from BST,
    // BST needs a remove method. For now, assuming only list removal needed for UI.
    return songsList.remove(song);
}

std::vector<Song> Playlist::searchSongs(const std::string& partialTitle) const {
    return songsTree.searchByPartialTitle(partialTitle);
}

std::vector<Song> Playlist::getAllSongs() const {
    return songsTree.getAllSongs(); // Uses BST's inorder traversal for all songs
}

bool Playlist::isEmpty() const {
    return songsList.size == 0;
}

// --- User Struct Implementation ---
User::User(const std::string& user, const std::string& passHash)
    : username(user), hashedPassword(passHash) {
}

// --- MusicPlayerLogic Class Implementation ---
MusicPlayerLogic::MusicPlayerLogic()
    : currentAppState(AppState::Login),
    currentSongIndex(-1),
    isPlaying(false),
    currentActivePlaylist(nullptr),
    savedActivePlaylist(nullptr),
    savedSongIndex(-1)
{
    // FIX 1: Name the global playlist so it is visible in the GUI
    allSongsGlobalPlaylist.name = "All Songs";

    loadUsers();

    // Initialize allSongsGlobalPlaylist by scanning a directory, for example
    // For now, let's just add a placeholder if no songs are loaded
    if (allSongsGlobalPlaylist.isEmpty()) {
        // Example: Add a dummy song if no real music is scanned
        // allSongsGlobalPlaylist.addSong(Song("path/to/dummy.mp3", "Dummy Song", "Dummy Artist"));
    }
    currentActivePlaylist = &allSongsGlobalPlaylist; // Default active playlist
}

MusicPlayerLogic::~MusicPlayerLogic() {
    // usersTree's destructor will call clear.
    // Ensure all data is saved
    if (!currentLoggedInUsername.empty()) {
        saveUserPlaylists(currentLoggedInUsername);
    }
    saveUsers();
}

// Getters
AppState MusicPlayerLogic::getAppState() const { return currentAppState; }
std::string MusicPlayerLogic::getCurrentLoggedInUsername() const { return currentLoggedInUsername; }

const std::vector<Playlist>& MusicPlayerLogic::getUserPlaylists() const {
    static const std::vector<Playlist> emptyPlaylists; // Return empty vector if no user logged in
    const User* currentUser = usersTree.search(User(currentLoggedInUsername));
    if (currentUser) {
        return currentUser->playlists;
    }
    return emptyPlaylists; // Return a const reference to an empty vector
}

const Playlist& MusicPlayerLogic::getAllSongsGlobalPlaylist() const { return allSongsGlobalPlaylist; }

const Playlist* MusicPlayerLogic::getCurrentActivePlaylist() const { return currentActivePlaylist; }

int MusicPlayerLogic::getCurrentSongIndex() const { return currentSongIndex; }
bool MusicPlayerLogic::getIsPlaying() const { return isPlaying; }

std::string MusicPlayerLogic::getCurrentSongDisplay() const {
    if (currentActivePlaylist && currentSongIndex >= 0) {
        const Song* song = currentActivePlaylist->songsList.get(currentSongIndex);
        if (song) {
            return song->title + " - " + song->artist;
        }
    }
    return "No song playing";
}

sf::Time MusicPlayerLogic::getMusicPlayingOffset() const {
    return musicPlayer.getPlayingOffset();
}

sf::Time MusicPlayerLogic::getMusicDuration() const {
    return musicPlayer.getDuration();
}

float MusicPlayerLogic::getMusicVolume() const {
    return musicPlayer.getVolume();
}

const std::string& MusicPlayerLogic::getMessageText() const {
    return currentMessage;
}

sf::Time MusicPlayerLogic::getMessageElapsedTime() const {
    return messageTimer.getElapsedTime();
}

float MusicPlayerLogic::getMessageDisplayDuration() const {
    return messageDisplayDuration;
}

size_t MusicPlayerLogic::getQueueSize() const {
    return upcomingQueueIndices.size();
}

const std::deque<int>& MusicPlayerLogic::getUpcomingQueueIndices() const {
    return upcomingQueueIndices;
}


// Setter for AppState
void MusicPlayerLogic::setAppState(AppState state) {
    currentAppState = state;
}

// Core functionalities
void MusicPlayerLogic::showMessage(const std::string& msg) {
    currentMessage = msg;
    messageTimer.restart();
}

void MusicPlayerLogic::attemptLogin(const std::string& username, const std::string& password) {
    User tempUser(username);
    const User* foundUser = usersTree.search(tempUser);

    if (foundUser && foundUser->hashedPassword == simpleHash(password)) {
        currentLoggedInUsername = username;
        showMessage("Login successful!");
        loadUserPlaylists(currentLoggedInUsername); // Load playlists after login
        setAppState(AppState::MainApp);
    }
    else {
        showMessage("Invalid username or password.");
    }
}

void MusicPlayerLogic::attemptSignup(const std::string& username, const std::string& password, const std::string& confirmPassword) {
    if (username.empty() || password.empty() || confirmPassword.empty()) {
        showMessage("All fields are required.");
        return;
    }

    // Check if username already exists using BST search
    User tempUser(username);
    if (usersTree.search(tempUser) != nullptr) {
        showMessage("Username already taken.");
        return;
    }

    // Basic username and password validity checks
    if (username.length() < 3) {
        showMessage("Username must be at least 3 characters.");
        return;
    }
    if (password.length() < 6) {
        showMessage("Password must be at least 6 characters.");
        return;
    }
    if (password != confirmPassword) {
        showMessage("Passwords do not match.");
        return;
    }

    std::string hashedPassword = simpleHash(password);
    usersTree.insert(User(username, hashedPassword)); // Insert new user into the BST
    saveUsers(); // Save all users (including the new one)
    showMessage("Signup successful! You can now login.");
    // Changed this line to explicitly set state to Login
    setAppState(AppState::Login);
}


void MusicPlayerLogic::logoutUser() {
    if (!currentLoggedInUsername.empty()) {
        saveUserPlaylists(currentLoggedInUsername); // Save current user's playlists
    }
    currentLoggedInUsername = "";
    currentActivePlaylist = &allSongsGlobalPlaylist; // Reset active playlist
    currentSongIndex = -1; // Reset current song
    musicPlayer.stop(); // Stop music
    isPlaying = false;
    playHistory.clear(); // Clear history
    upcomingQueueIndices.clear(); // Clear queue
    showMessage("Logged out successfully.");
    setAppState(AppState::Login);
}

void MusicPlayerLogic::selectPlaylist(const std::string& playlistName) {
    if (playlistName == "All Songs") {
        currentActivePlaylist = &allSongsGlobalPlaylist;
        showMessage("Switched to All Songs.");
    }
    else if (playlistName == "Queue") {
        currentActivePlaylist = &queueDisplayPlaylist;
        showMessage("Switched to Queue.");
    }
    else {
        User* currentUser = getMutableUser(currentLoggedInUsername);
        if (currentUser) {
            for (auto& playlist : currentUser->playlists) {
                if (playlist.name == playlistName) {
                    currentActivePlaylist = &playlist;
                    showMessage("Switched to playlist: " + playlistName);
                    return;
                }
            }
        }
        showMessage("Playlist not found: " + playlistName);
    }
    currentSongIndex = -1; // Reset song index when changing playlist
    musicPlayer.stop();
    isPlaying = false;
}

void MusicPlayerLogic::playSongByIndexInActivePlaylist(int index) {
    if (currentActivePlaylist) {
        const Song* song = currentActivePlaylist->songsList.get(index);
        if (song) {
            if (musicPlayer.openFromFile(song->filePath)) {
                musicPlayer.play();
                isPlaying = true;
                currentSongIndex = index;
                playHistory.push(*song); // Add to history
                showMessage("Now playing: " + song->title + " by " + song->artist);
            }
            else {
                showMessage("Could not open file: " + song->filePath);
            }
        }
        else {
            showMessage("Song index out of bounds or song not found.");
        }
    }
    else {
        showMessage("No active playlist selected.");
    }
}

void MusicPlayerLogic::togglePlayPause() {
    if (musicPlayer.getStatus() == sf::Music::Playing) {
        musicPlayer.pause();
        isPlaying = false;
        showMessage("Paused.");
    }
    else if (musicPlayer.getStatus() == sf::Music::Paused) {
        musicPlayer.play();
        isPlaying = true;
        showMessage("Resumed.");
    }
    else if (currentActivePlaylist && currentSongIndex != -1) {
        // If stopped but a song is selected, play it
        playSongByIndexInActivePlaylist(currentSongIndex);
    }
}

void MusicPlayerLogic::playNextSong() {
    if (upcomingQueueIndices.size() > 0) {
        int nextGlobalIndex = upcomingQueueIndices.front();
        upcomingQueueIndices.pop_front();
        // Temporarily set active playlist to global and play song by global index
        Playlist* tempSavedActivePlaylist = currentActivePlaylist;
        int tempSavedSongIndex = currentSongIndex;

        currentActivePlaylist = &allSongsGlobalPlaylist; // Assume queue indices refer to global songs
        playSongByIndexInActivePlaylist(nextGlobalIndex);

        // Restore previous active playlist and song index for display if needed
        currentActivePlaylist = tempSavedActivePlaylist;
        currentSongIndex = tempSavedSongIndex; // The playing song is now from global, this is for context
        return;
    }

    if (currentActivePlaylist && currentActivePlaylist->songsList.size > 0) {
        int nextIndex = (currentSongIndex + 1) % currentActivePlaylist->songsList.size;
        playSongByIndexInActivePlaylist(nextIndex);
    }
    else {
        showMessage("No more songs in playlist.");
    }
}

void MusicPlayerLogic::playPreviousSong() {
    if (playHistory.size() > 1) { // If there's more than the current song in history
        playHistory.pop(); // Remove current song
        const Song& prevSong = playHistory.top(); // Get previous song
        // Need to find the index of this song in the currentActivePlaylist
        // This is inefficient; a better history would store (playlist*, index)
        int prevIndex = -1;
        if (currentActivePlaylist) {
            for (int i = 0; i < currentActivePlaylist->songsList.size; ++i) {
                const Song* songInList = currentActivePlaylist->songsList.get(i);
                if (songInList && *songInList == prevSong) { // Requires Song::operator==
                    prevIndex = i;
                    break;
                }
            }
        }

        if (prevIndex != -1) {
            musicPlayer.stop(); // Stop current playback
            playSongByIndexInActivePlaylist(prevIndex); // Play the actual previous song
            playHistory.pop(); // Pop the song that was just played (it will be pushed again by playSongByIndexInActivePlaylist)
        }
        else {
            showMessage("Could not find previous song in current playlist.");
        }
    }
    else {
        showMessage("No previous songs in history.");
        // If only current song in history, restart current song
        if (currentActivePlaylist && currentSongIndex != -1) {
            musicPlayer.setPlayingOffset(sf::Time::Zero);
            musicPlayer.play();
            isPlaying = true;
            showMessage("Restarted current song.");
        }
    }
}


void MusicPlayerLogic::setMusicPlayingOffset(float normalizedPosition) {
    if (musicPlayer.getStatus() != sf::Music::Stopped) {
        sf::Time duration = musicPlayer.getDuration();
        musicPlayer.setPlayingOffset(sf::seconds(duration.asSeconds() * normalizedPosition));
    }
}

void MusicPlayerLogic::seekMusic(float secondsDelta) {
    if (musicPlayer.getStatus() != sf::Music::Stopped) {
        sf::Time currentOffset = musicPlayer.getPlayingOffset();
        sf::Time newOffset = currentOffset + sf::seconds(secondsDelta);
        // Fixed: Compare sf::Time with sf::Time::Zero
        if (newOffset < sf::Time::Zero) newOffset = sf::Time::Zero;
        if (newOffset > musicPlayer.getDuration()) newOffset = musicPlayer.getDuration();
        musicPlayer.setPlayingOffset(newOffset);
    }
}

void MusicPlayerLogic::adjustVolume(int delta) {
    float newVolume = musicPlayer.getVolume() + delta;
    if (newVolume < 0) newVolume = 0;
    if (newVolume > 100) newVolume = 100;
    musicPlayer.setVolume(newVolume);
    showMessage("Volume: " + std::to_string(static_cast<int>(newVolume)));
}


void MusicPlayerLogic::createNewPlaylist(const std::string& name) {
    if (currentLoggedInUsername.empty()) {
        showMessage("Please log in to create playlists.");
        return;
    }
    User* currentUser = getMutableUser(currentLoggedInUsername);
    if (currentUser) {
        // Check for duplicate playlist name
        for (const auto& p : currentUser->playlists) {
            if (p.name == name) {
                showMessage("Playlist with this name already exists.");
                return;
            }
        }
        currentUser->playlists.emplace_back(name); // Add new playlist
        saveUserPlaylists(currentLoggedInUsername); // Save updated playlists
        showMessage("Playlist '" + name + "' created.");
    }
    else {
        showMessage("Error: Current user not found.");
    }
}

std::vector<Song> MusicPlayerLogic::searchSongsInActivePlaylist(const std::string& query) const {
    if (currentActivePlaylist) {
        return currentActivePlaylist->searchSongs(query);
    }
    return {};
}

void MusicPlayerLogic::addSongToQueueFromGlobalIndex(int globalSongIndex) {
    const Song* song = allSongsGlobalPlaylist.songsList.get(globalSongIndex);
    if (song) {
        upcomingQueueIndices.push_back(globalSongIndex);
        queueDisplayPlaylist.addSong(*song); // Add to display playlist for queue
        showMessage("Added '" + song->title + "' to queue.");
    }
    else {
        showMessage("Error: Song not found in global list.");
    }
}

void MusicPlayerLogic::addSongToCustomPlaylistFromGlobalIndex(int globalSongIndex, const std::string& playlistName) {
    if (currentLoggedInUsername.empty()) {
        showMessage("Please log in to add songs to custom playlists.");
        return;
    }

    const Song* songToAdd = allSongsGlobalPlaylist.songsList.get(globalSongIndex);
    if (!songToAdd) {
        showMessage("Error: Song not found in global list.");
        return;
    }

    User* currentUser = getMutableUser(currentLoggedInUsername);
    if (currentUser) {
        bool playlistFound = false;
        for (auto& playlist : currentUser->playlists) {
            if (playlist.name == playlistName) {
                // Check if song already exists in playlist to avoid duplicates
                bool songExists = false;
                for (int i = 0; i < playlist.songsList.size; ++i) {
                    if (*playlist.songsList.get(i) == *songToAdd) {
                        songExists = true;
                        break;
                    }
                }
                if (!songExists) {
                    playlist.addSong(*songToAdd);
                    saveUserPlaylists(currentLoggedInUsername);
                    showMessage("Added '" + songToAdd->title + "' to '" + playlistName + "'.");
                }
                else {
                    showMessage("Song already exists in '" + playlistName + "'.");
                }
                playlistFound = true;
                break;
            }
        }
        if (!playlistFound) {
            showMessage("Playlist '" + playlistName + "' not found.");
        }
    }
    else {
        showMessage("Error: Current user not found.");
    }
}


void MusicPlayerLogic::updateLogic() {
    // FIX 2: Modified logic to trigger next song correctly
    // If logic says we are playing, but SFML says we are Stopped, the song ended naturally.
    if (isPlaying && musicPlayer.getStatus() == sf::Music::Stopped) {
        playNextSong(); // This will automatically handle Queue vs Normal playlist priority
    }

    // Update global playlist with any new music from a directory (conceptual)
    static bool initialScanDone = false;
    if (!initialScanDone) {
        std::string musicDirectory = "assets"; // Specify your assets directory
        namespace fs = std::filesystem; // Alias for brevity
        if (fs::exists(musicDirectory) && fs::is_directory(musicDirectory)) {
            showMessage("Scanning music directory: " + musicDirectory); // Debug message
            int songsAdded = 0;
            for (const auto& entry : fs::directory_iterator(musicDirectory)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    std::string lowerFilePath = filePath;
                    std::transform(lowerFilePath.begin(), lowerFilePath.end(), lowerFilePath.begin(), ::tolower);

                    if ((lowerFilePath.length() >= 4 && lowerFilePath.substr(lowerFilePath.length() - 4) == ".mp3") ||
                        (lowerFilePath.length() >= 4 && lowerFilePath.substr(lowerFilePath.length() - 4) == ".ogg") ||
                        (lowerFilePath.length() >= 5 && lowerFilePath.substr(lowerFilePath.length() - 5) == ".flac")) {

                        std::string title = entry.path().filename().string();
                        std::string artist = "Unknown Artist";

                        sf::Music tempMusic;
                        sf::Time duration = sf::Time::Zero;
                        if (tempMusic.openFromFile(filePath)) {
                            duration = tempMusic.getDuration();
                        }

                        if (allSongsGlobalPlaylist.songsTree.search(Song(filePath)) == nullptr) {
                            allSongsGlobalPlaylist.addSong(Song(filePath, title, artist, duration));
                            songsAdded++;
                        }
                    }
                }
            }
            showMessage("Finished scanning. Added " + std::to_string(songsAdded) + " songs to 'All Songs'.");
        }
        else {
            showMessage("Error: Music directory not found or is not a directory: " + musicDirectory);
        }
        initialScanDone = true;
    }
}

// --- File I/O for Users and Playlists ---
void MusicPlayerLogic::loadUsers() {
    std::ifstream inFile("users.txt");
    if (!inFile.is_open()) {
        std::cerr << "users.txt not found. Starting with no users." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string username, hashedPassword;
        if (std::getline(ss, username, ':') && std::getline(ss, hashedPassword)) {
            User newUser(username, hashedPassword);
            if (usersTree.search(newUser) == nullptr) {
                usersTree.insert(newUser);
                loadUserPlaylists(username);
            }
            else {
                std::cerr << "Warning: Duplicate user '" << username << "' found in users.txt. Skipping." << std::endl;
            }
        }
        else {
            std::cerr << "Error parsing user data. Line: " << line << std::endl;
        }
    }
    inFile.close();
}

void MusicPlayerLogic::saveUsers() {
    std::ofstream outFile("users.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open users.txt for writing." << std::endl;
        return;
    }

    std::vector<User> allUsers = usersTree.getAllItems();
    for (const auto& user : allUsers) {
        outFile << user.username << ":" << user.hashedPassword << "\n";
    }
    outFile.close();
}

void MusicPlayerLogic::loadUserPlaylists(const std::string& username) {
    User* currentUser = getMutableUser(username);
    if (!currentUser) {
        std::cerr << "Error: Attempted to load playlists for non-existent user '" << username << "'." << std::endl;
        return;
    }

    std::ifstream inFile(username + "_playlists.txt");
    if (!inFile.is_open()) {
        std::cerr << username << "_playlists.txt not found. Starting with no playlists for this user." << std::endl;
        return;
    }

    std::string line;
    Playlist* currentPlaylist = nullptr;

    while (std::getline(inFile, line)) {
        if (line.rfind("PLAYLIST:", 0) == 0) {
            std::string playlistName = line.substr(std::string("PLAYLIST:").length());
            bool found = false;
            for (auto& p : currentUser->playlists) {
                if (p.name == playlistName) {
                    currentPlaylist = &p;
                    found = true;
                    break;
                }
            }
            if (!found) {
                currentUser->playlists.emplace_back(playlistName);
                currentPlaylist = &currentUser->playlists.back();
            }
        }
        else if (line.rfind("SONG:", 0) == 0) {
            if (!currentPlaylist) {
                std::cerr << "Error: Song found before any playlist definition for user " << username << ". Skipping." << std::endl;
                continue;
            }
            std::string songData = line.substr(std::string("SONG:").length());
            std::stringstream ss(songData);
            std::string filePath, title, artist;

            ss >> std::quoted(filePath) >> std::quoted(title) >> std::quoted(artist);

            if (!filePath.empty() && !title.empty() && !artist.empty()) {
                currentPlaylist->addSong(Song(filePath, title, artist));
            }
            else {
                std::cerr << "Error parsing song data for user " << username << ". Line: " << line << std::endl;
            }
        }
        else {
            std::cerr << "Warning: Unrecognized line in " << username << "_playlists.txt: " << line << std::endl;
        }
    }
    inFile.close();
}

void MusicPlayerLogic::saveUserPlaylists(const std::string& username) {
    std::ofstream outFile(username + "_playlists.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open " << username << "_playlists.txt for writing." << std::endl;
        return;
    }

    const User* currentUser = usersTree.search(User(username));
    if (currentUser) {
        for (const auto& playlist : currentUser->playlists) {
            outFile << "PLAYLIST:" << playlist.name << "\n";
            for (const auto& song : playlist.getAllSongs()) {
                outFile << "SONG:" << std::quoted(song.filePath) << " " << std::quoted(song.title) << " " << std::quoted(song.artist) << "\n";
            }
        }
    }
    outFile.close();
}

User* MusicPlayerLogic::getMutableUser(const std::string& username) {
    return usersTree.search(User(username));
}

template class BinarySearchTree<Song>;
template class BinarySearchTree<User>;
template class DoublyLinkedList<Song>;
template class Stack<Song>;