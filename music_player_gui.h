#pragma once
//music_player_gui.h
#pragma once
#include <SFML/Graphics.hpp>
#include "music_player_logic.h" // Include the logic header

// MusicPlayerGUI Class
class MusicPlayerGUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    MusicPlayerLogic* logic; // Pointer to the logic class

    // UI elements for common display (panels, message text handled by logic, but drawn here)
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;
    sf::RectangleShape bottomPanel;

    // UI elements for Login Page
    sf::RectangleShape loginBackground;
    sf::Text loginHeader;
    sf::RectangleShape loginUsernameInputRect;
    sf::Text loginUsernameInputText;
    std::string loginUsernameString;
    sf::RectangleShape loginPasswordInputRect;
    sf::Text loginPasswordInputText;
    std::string loginPasswordString;
    bool isLoginUsernameActive;
    bool isLoginPasswordActive;
    sf::RectangleShape loginButton;
    sf::Text loginButtonText;
    sf::Text goToSignupText;

    // UI elements for Signup Page
    sf::RectangleShape signupBackground;
    sf::Text signupHeader;
    sf::RectangleShape signupUsernameInputRect;
    sf::Text signupUsernameInputText;
    std::string signupUsernameString;
    sf::RectangleShape signupPasswordInputRect;
    sf::Text signupPasswordInputText;
    std::string signupPasswordString;
    sf::RectangleShape signupConfirmPasswordInputRect;
    sf::Text signupConfirmPasswordInputText;
    std::string signupConfirmPasswordString;
    bool isSignupUsernameActive;
    bool isSignupPasswordActive;
    bool isSignupConfirmPasswordActive;
    sf::RectangleShape signupButton;
    sf::Text signupButtonText;
    sf::Text goToLoginText;

    // UI elements for Main Music Player App
    sf::Text homeText;
    sf::Text playlistsHeader;
    std::vector<sf::Text> playlistsDisplayTexts;
    sf::RectangleShape newPlaylistInputRect;
    sf::Text newPlaylistInputText;
    std::string newPlaylistInputString;
    bool isCreatingPlaylist;
    sf::RectangleShape createPlaylistButton;
    sf::Text createPlaylistButtonText;
    sf::RectangleShape logoutButton;
    sf::Text logoutButtonText;

    sf::Text libraryHeader;
    sf::Text titleColumnHeader;
    sf::Text artistColumnHeader;
    std::vector<std::pair<sf::Text, sf::Text>> currentPlaylistSongDisplayTexts;
    sf::RectangleShape searchBarRectRight;
    sf::Text searchBarTextRight;
    std::string searchInputStringRight;
    bool isSearchingRight;

    sf::Text currentSongDisplayBottom;
    sf::RectangleShape progressBar;
    sf::CircleShape scrubberCircle;
    bool isScrubbing;
    sf::Text currentTimeText;
    sf::Text totalTimeText;

    sf::RectangleShape playButton;
    sf::Text playButtonText;
    sf::RectangleShape nextButton;
    sf::Text nextButtonText;
    sf::RectangleShape prevButton;
    sf::Text prevButtonText;
    sf::RectangleShape volumeUpButton;
    sf::Text volumeUpButtonText;
    sf::RectangleShape volumeDownButton;
    sf::Text volumeDownButtonText;
    sf::RectangleShape volumeBar;
    sf::RectangleShape volumeLevel;

    // Prompt for adding to playlist
    sf::RectangleShape promptBackground;
    sf::Text promptText;
    sf::Text promptInstructionText;
    bool showAddSongPrompt;
    int songToAddGlobalIndexForPrompt; // Global index of song clicked for prompt

    // New Prompt for Add Song Options (Add to Playlist / Add to Queue)
    sf::RectangleShape addOptionsBackground;
    sf::Text addOptionsHeader;
    sf::RectangleShape addOptionPlaylistButton;
    sf::Text addOptionPlaylistButtonText;
    sf::RectangleShape addOptionQueueButton;
    sf::Text addOptionQueueButtonText;
    bool showAddSongOptionsPrompt; // Controls visibility of the initial prompt

    int playlistsScrollOffset;
    int currentPlaylistSongsScrollOffset;
    const int maxDisplayItems = 12;

    // Private helper functions for GUI - DECLARATIONS ADDED HERE
    void setupCommonUI();
    void setupLoginPageUI();
    void setupSignupPageUI();
    void setupMainAppUI();
    void setupAddSongOptionsPromptUI(); // Also added this setup function declaration

    void setupInputField(sf::RectangleShape& rect, sf::Text& text, const std::string& placeholder, float yPos);
    void setupButton(sf::RectangleShape& rect, sf::Text& text, const std::string& str, float yPos); // Overloaded if needed, or single flexible
    void handleInputString(std::string& targetString, sf::Text& targetText, const sf::Event::TextEvent& textEvent, bool isPassword = false);
    std::string formatTime(sf::Time time);


public:
    MusicPlayerGUI(MusicPlayerLogic* appLogic);

    bool isWindowOpen() const { return window.isOpen(); }
    void handleEvents();
    void render();

    void updatePlaylistsDisplay();
    void updateCurrentPlaylistDisplay();

};