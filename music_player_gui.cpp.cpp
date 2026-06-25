#include "music_player_gui.h"

// --- MusicPlayerGUI Class Implementation ---
MusicPlayerGUI::MusicPlayerGUI(MusicPlayerLogic* appLogic)
    : window(sf::VideoMode(1200, 800), "SFML Music Player", sf::Style::Close | sf::Style::Resize),
    logic(appLogic), // Initialize the logic pointer
    isCreatingPlaylist(false),
    showAddSongPrompt(false),
    showAddSongOptionsPrompt(false),
    songToAddGlobalIndexForPrompt(-1),
    playlistsScrollOffset(0),
    currentPlaylistSongsScrollOffset(0),
    isScrubbing(false),
    isSearchingRight(false),
    isLoginUsernameActive(false),
    isLoginPasswordActive(false),
    isSignupUsernameActive(false),
    isSignupPasswordActive(false),
    isSignupConfirmPasswordActive(false)
{
    window.setFramerateLimit(60);
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error loading font: arial.ttf" << std::endl;
    }

    // Setup all UI components
    setupCommonUI();
    setupLoginPageUI();
    setupSignupPageUI();
    setupMainAppUI();
    setupAddSongOptionsPromptUI();
}

void MusicPlayerGUI::setupCommonUI() {
    leftPanel.setSize(sf::Vector2f(250, window.getSize().y));
    leftPanel.setPosition(0, 0);
    leftPanel.setFillColor(sf::Color(30, 30, 30));

    rightPanel.setSize(sf::Vector2f(window.getSize().x - 250, window.getSize().y - 120));
    rightPanel.setPosition(250, 0);
    rightPanel.setFillColor(sf::Color(18, 18, 18));

    bottomPanel.setSize(sf::Vector2f(window.getSize().x, 120));
    bottomPanel.setPosition(0, window.getSize().y - 120);
    bottomPanel.setFillColor(sf::Color(40, 40, 40));
}

void MusicPlayerGUI::setupLoginPageUI() {
    loginBackground.setSize(sf::Vector2f(400, 400));
    loginBackground.setFillColor(sf::Color(30, 30, 30, 220));
    loginBackground.setOutlineThickness(2);
    loginBackground.setOutlineColor(sf::Color::White);
    loginBackground.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2 - 200);

    loginHeader.setFont(font);
    loginHeader.setString("Login");
    loginHeader.setCharacterSize(36);
    loginHeader.setFillColor(sf::Color::White);
    sf::FloatRect headerBounds = loginHeader.getLocalBounds();
    loginHeader.setOrigin(headerBounds.left + headerBounds.width / 2.0f, headerBounds.top + headerBounds.height / 2.0f);
    loginHeader.setPosition(loginBackground.getPosition().x + loginBackground.getSize().x / 2.0f, loginBackground.getPosition().y + 50);

    setupInputField(loginUsernameInputRect, loginUsernameInputText, "Username", loginBackground.getPosition().y + 120);
    setupInputField(loginPasswordInputRect, loginPasswordInputText, "Password", loginBackground.getPosition().y + 180);

    // Buttons here are positioned relative to the loginBackground's center
    loginButton.setSize(sf::Vector2f(150, 45));
    loginButton.setFillColor(sf::Color(60, 60, 60));
    loginButton.setOutlineThickness(1);
    loginButton.setOutlineColor(sf::Color(100, 100, 100));
    loginButton.setPosition(loginBackground.getPosition().x + loginBackground.getSize().x / 2 - loginButton.getSize().x / 2, loginBackground.getPosition().y + 260);

    loginButtonText.setFont(font);
    loginButtonText.setString("Login");
    loginButtonText.setCharacterSize(22);
    loginButtonText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = loginButtonText.getLocalBounds();
    loginButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    loginButtonText.setPosition(loginButton.getPosition().x + loginButton.getSize().x / 2.0f, loginButton.getPosition().y + loginButton.getSize().y / 2.0f);


    goToSignupText.setFont(font);
    goToSignupText.setString("Don't have an account? Sign Up");
    goToSignupText.setCharacterSize(18);
    goToSignupText.setFillColor(sf::Color(100, 150, 255));
    sf::FloatRect signupTextBounds = goToSignupText.getLocalBounds();
    goToSignupText.setOrigin(signupTextBounds.left + signupTextBounds.width / 2.0f, signupTextBounds.top + signupTextBounds.height / 2.0f);
    goToSignupText.setPosition(loginBackground.getPosition().x + loginBackground.getSize().x / 2.0f, loginBackground.getPosition().y + 350);
}

void MusicPlayerGUI::setupSignupPageUI() {
    signupBackground.setSize(sf::Vector2f(400, 450));
    signupBackground.setFillColor(sf::Color(30, 30, 30, 220));
    signupBackground.setOutlineThickness(2);
    signupBackground.setOutlineColor(sf::Color::White);
    signupBackground.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2 - 225);

    signupHeader.setFont(font);
    signupHeader.setString("Sign Up");
    signupHeader.setCharacterSize(36);
    signupHeader.setFillColor(sf::Color::White);
    sf::FloatRect headerBounds = signupHeader.getLocalBounds();
    signupHeader.setOrigin(headerBounds.left + headerBounds.width / 2.0f, headerBounds.top + headerBounds.height / 2.0f);
    signupHeader.setPosition(signupBackground.getPosition().x + signupBackground.getSize().x / 2.0f, signupBackground.getPosition().y + 50);

    setupInputField(signupUsernameInputRect, signupUsernameInputText, "Username", signupBackground.getPosition().y + 120);
    setupInputField(signupPasswordInputRect, signupPasswordInputText, "Password", signupBackground.getPosition().y + 180);
    setupInputField(signupConfirmPasswordInputRect, signupConfirmPasswordInputText, "Confirm Password", signupBackground.getPosition().y + 240);

    // Buttons here are positioned relative to the signupBackground's center
    signupButton.setSize(sf::Vector2f(150, 45));
    signupButton.setFillColor(sf::Color(60, 60, 60));
    signupButton.setOutlineThickness(1);
    signupButton.setOutlineColor(sf::Color(100, 100, 100));
    signupButton.setPosition(signupBackground.getPosition().x + signupBackground.getSize().x / 2 - signupButton.getSize().x / 2, signupBackground.getPosition().y + 320);

    signupButtonText.setFont(font);
    signupButtonText.setString("Sign Up");
    signupButtonText.setCharacterSize(22);
    signupButtonText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = signupButtonText.getLocalBounds();
    signupButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    signupButtonText.setPosition(signupButton.getPosition().x + signupButton.getSize().x / 2.0f, signupButton.getPosition().y + signupButton.getSize().y / 2.0f);


    goToLoginText.setFont(font);
    goToLoginText.setString("Already have an account? Login");
    goToLoginText.setCharacterSize(18);
    goToLoginText.setFillColor(sf::Color(100, 150, 255));
    sf::FloatRect loginTextBounds = goToLoginText.getLocalBounds();
    goToLoginText.setOrigin(loginTextBounds.left + loginTextBounds.width / 2.0f, loginTextBounds.top + loginTextBounds.height / 2.0f);
    goToLoginText.setPosition(signupBackground.getPosition().x + signupBackground.getSize().x / 2.0f, signupBackground.getPosition().y + 410);
}

void MusicPlayerGUI::setupMainAppUI() {
    homeText.setFont(font);
    homeText.setString("Home");
    homeText.setCharacterSize(24);
    homeText.setFillColor(sf::Color::White);
    homeText.setPosition(20, 20);

    playlistsHeader.setFont(font);
    playlistsHeader.setString("Playlists");
    playlistsHeader.setCharacterSize(20);
    playlistsHeader.setFillColor(sf::Color::White);
    playlistsHeader.setPosition(20, 70);

    newPlaylistInputRect.setSize(sf::Vector2f(210, 30));
    newPlaylistInputRect.setPosition(20, window.getSize().y - 200);
    newPlaylistInputRect.setFillColor(sf::Color(50, 50, 50));
    newPlaylistInputRect.setOutlineThickness(1);
    newPlaylistInputRect.setOutlineColor(sf::Color(80, 80, 80));

    newPlaylistInputText.setFont(font);
    newPlaylistInputText.setCharacterSize(18);
    newPlaylistInputText.setFillColor(sf::Color(180, 180, 180));
    newPlaylistInputText.setPosition(newPlaylistInputRect.getPosition().x + 5, newPlaylistInputRect.getPosition().y + 5);
    newPlaylistInputText.setString("New playlist name...");

    createPlaylistButton.setSize(sf::Vector2f(210, 30));
    createPlaylistButton.setPosition(20, window.getSize().y - 160);
    createPlaylistButton.setFillColor(sf::Color(60, 60, 60));
    createPlaylistButton.setOutlineThickness(1);
    createPlaylistButton.setOutlineColor(sf::Color(100, 100, 100));

    createPlaylistButtonText.setFont(font);
    createPlaylistButtonText.setString("Create Playlist");
    createPlaylistButtonText.setCharacterSize(18);
    createPlaylistButtonText.setFillColor(sf::Color::White);
    sf::FloatRect createBtnTextBounds = createPlaylistButtonText.getLocalBounds();
    createPlaylistButtonText.setOrigin(createBtnTextBounds.left + createBtnTextBounds.width / 2.0f, createBtnTextBounds.top + createBtnTextBounds.height / 2.0f);
    createPlaylistButtonText.setPosition(createPlaylistButton.getPosition().x + createPlaylistButton.getSize().x / 2.0f,
        createPlaylistButton.getPosition().y + createPlaylistButton.getSize().y / 2.0f);

    logoutButton.setSize(sf::Vector2f(100, 30));
    logoutButton.setFillColor(sf::Color(150, 50, 50));
    logoutButton.setOutlineThickness(1);
    logoutButton.setOutlineColor(sf::Color(200, 100, 100));
    // Adjusted position: move it further up from the bottom panel
    logoutButton.setPosition(window.getSize().x - logoutButton.getSize().x - 20, window.getSize().y - logoutButton.getSize().y - bottomPanel.getSize().y - 20); // Increased padding from bottom

    logoutButtonText.setFont(font);
    logoutButtonText.setString("Logout");
    logoutButtonText.setCharacterSize(18);
    logoutButtonText.setFillColor(sf::Color::White);
    sf::FloatRect logoutBtnTextBounds = logoutButtonText.getLocalBounds();
    logoutButtonText.setOrigin(logoutBtnTextBounds.left + logoutBtnTextBounds.width / 2.0f, logoutBtnTextBounds.top + logoutBtnTextBounds.height / 2.0f);
    logoutButtonText.setPosition(logoutButton.getPosition().x + logoutButton.getSize().x / 2.0f,
        logoutButton.getPosition().y + logoutButton.getSize().y / 2.0f);


    libraryHeader.setFont(font);
    libraryHeader.setString("Library");
    libraryHeader.setCharacterSize(30);
    libraryHeader.setFillColor(sf::Color::White);
    libraryHeader.setPosition(250 + 20, 20);

    titleColumnHeader.setFont(font);
    titleColumnHeader.setString("Title");
    titleColumnHeader.setCharacterSize(20);
    titleColumnHeader.setFillColor(sf::Color(180, 180, 180));
    titleColumnHeader.setPosition(250 + 20, 80);

    artistColumnHeader.setFont(font);
    artistColumnHeader.setString("Artist");
    artistColumnHeader.setCharacterSize(20);
    artistColumnHeader.setFillColor(sf::Color(180, 180, 180));
    artistColumnHeader.setPosition(250 + 20 + 300, 80);

    searchBarRectRight.setSize(sf::Vector2f(250, 30));
    searchBarRectRight.setPosition(rightPanel.getPosition().x + rightPanel.getSize().x - 20 - 250, 30);
    searchBarRectRight.setFillColor(sf::Color(50, 50, 50));
    searchBarRectRight.setOutlineThickness(1);
    searchBarRectRight.setOutlineColor(sf::Color(80, 80, 80));

    searchBarTextRight.setFont(font);
    searchBarTextRight.setCharacterSize(18);
    searchBarTextRight.setFillColor(sf::Color(180, 180, 180));
    searchBarTextRight.setPosition(searchBarRectRight.getPosition().x + 5, searchBarRectRight.getPosition().y + 5);
    searchBarTextRight.setString("Search songs...");

    currentSongDisplayBottom.setFont(font);
    currentSongDisplayBottom.setCharacterSize(20);
    currentSongDisplayBottom.setFillColor(sf::Color::White);
    currentSongDisplayBottom.setPosition(20, bottomPanel.getPosition().y + 10);
    currentSongDisplayBottom.setString("No song playing");

    progressBar.setSize(sf::Vector2f(window.getSize().x - 40, 2));
    progressBar.setPosition(20, bottomPanel.getPosition().y + 40);
    progressBar.setFillColor(sf::Color(100, 100, 100));

    scrubberCircle.setRadius(7);
    scrubberCircle.setFillColor(sf::Color::Green);
    scrubberCircle.setOrigin(7, 7);
    scrubberCircle.setPosition(20, progressBar.getPosition().y);

    currentTimeText.setFont(font);
    currentTimeText.setCharacterSize(16);
    currentTimeText.setFillColor(sf::Color::White);
    currentTimeText.setPosition(20, bottomPanel.getPosition().y + 50);

    totalTimeText.setFont(font);
    totalTimeText.setCharacterSize(16);
    totalTimeText.setFillColor(sf::Color::White);
    totalTimeText.setPosition(progressBar.getPosition().x + progressBar.getSize().x - 50, bottomPanel.getPosition().y + 50);

    auto setupPlaybackButton = [&](sf::RectangleShape& shape, sf::Text& text, sf::Vector2f pos, sf::Vector2f size, const std::string& str, sf::Color fillColor = sf::Color(70, 70, 70)) {
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(fillColor);
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        text.setPosition(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
        };

    float controlButtonY = bottomPanel.getPosition().y + 70;
    float startX = (window.getSize().x / 2) - ((50 * 3) + (10 * 2)) / 2;

    setupPlaybackButton(prevButton, prevButtonText, sf::Vector2f(startX, controlButtonY), sf::Vector2f(50, 40), "|<<");
    setupPlaybackButton(playButton, playButtonText, sf::Vector2f(startX + 60, controlButtonY), sf::Vector2f(50, 40), "Play", sf::Color::Green);
    setupPlaybackButton(nextButton, nextButtonText, sf::Vector2f(startX + 120, controlButtonY), sf::Vector2f(50, 40), ">>|");

    float volumeControlX = window.getSize().x - 150;
    setupPlaybackButton(volumeDownButton, volumeDownButtonText, sf::Vector2f(volumeControlX, controlButtonY), sf::Vector2f(30, 30), "-", sf::Color(60, 60, 60));
    setupPlaybackButton(volumeUpButton, volumeUpButtonText, sf::Vector2f(volumeControlX + 110, controlButtonY), sf::Vector2f(30, 30), "+", sf::Color(60, 60, 60));

    volumeBar.setSize(sf::Vector2f(80, 5));
    volumeBar.setPosition(volumeControlX + 35, controlButtonY + 12.5f);
    volumeBar.setFillColor(sf::Color(100, 100, 100));

    volumeLevel.setSize(sf::Vector2f(logic->getMusicVolume() / 100.0f * 80, 5));
    volumeLevel.setPosition(volumeControlX + 35, controlButtonY + 12.5f);
    volumeLevel.setFillColor(sf::Color::Green);

    promptBackground.setSize(sf::Vector2f(400, 300));
    promptBackground.setFillColor(sf::Color(50, 50, 50, 200));
    promptBackground.setOutlineThickness(2);
    promptBackground.setOutlineColor(sf::Color::White);
    promptBackground.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2 - 150);

    promptText.setFont(font);
    promptText.setCharacterSize(22);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(promptBackground.getPosition().x + 20, promptBackground.getPosition().y + 20);
    promptText.setString("Add song to which playlist?");

    promptInstructionText.setFont(font);
    promptInstructionText.setCharacterSize(16);
    promptInstructionText.setFillColor(sf::Color::Yellow);
    promptInstructionText.setPosition(promptBackground.getPosition().x + 20, promptBackground.getPosition().y + 270);
    promptInstructionText.setString("Click on a playlist name or ESC to cancel");
}

void MusicPlayerGUI::setupAddSongOptionsPromptUI() {
    addOptionsBackground.setSize(sf::Vector2f(300, 180));
    addOptionsBackground.setFillColor(sf::Color(50, 50, 50, 220));
    addOptionsBackground.setOutlineThickness(2);
    addOptionsBackground.setOutlineColor(sf::Color::White);
    addOptionsBackground.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 90);

    addOptionsHeader.setFont(font);
    addOptionsHeader.setString("Add Song To:");
    addOptionsHeader.setCharacterSize(22);
    addOptionsHeader.setFillColor(sf::Color::White);
    sf::FloatRect headerBounds = addOptionsHeader.getLocalBounds();
    addOptionsHeader.setOrigin(headerBounds.left + headerBounds.width / 2.0f, headerBounds.top + headerBounds.height / 2.0f);
    addOptionsHeader.setPosition(addOptionsBackground.getPosition().x + addOptionsBackground.getSize().x / 2.0f, addOptionsBackground.getPosition().y + 20);

    // Use setupButton helper, ensuring it positions relative to the prompt background
    sf::Vector2f buttonSize(200, 40);
    float buttonX = addOptionsBackground.getPosition().x + addOptionsBackground.getSize().x / 2 - buttonSize.x / 2;

    addOptionPlaylistButton.setSize(buttonSize);
    addOptionPlaylistButton.setFillColor(sf::Color(60, 60, 60));
    addOptionPlaylistButton.setOutlineThickness(1);
    addOptionPlaylistButton.setOutlineColor(sf::Color(100, 100, 100));
    addOptionPlaylistButton.setPosition(buttonX, addOptionsBackground.getPosition().y + 70);
    setupButton(addOptionPlaylistButton, addOptionPlaylistButtonText, "Add to Playlist", addOptionPlaylistButton.getPosition().y); // Use its own position

    addOptionQueueButton.setSize(buttonSize);
    addOptionQueueButton.setFillColor(sf::Color(60, 60, 60));
    addOptionQueueButton.setOutlineThickness(1);
    addOptionQueueButton.setOutlineColor(sf::Color(100, 100, 100));
    addOptionQueueButton.setPosition(buttonX, addOptionsBackground.getPosition().y + 120);
    setupButton(addOptionQueueButton, addOptionQueueButtonText, "Add to Queue", addOptionQueueButton.getPosition().y); // Use its own position
}

void MusicPlayerGUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            if (logic->getAppState() == AppState::Login) {
                if (loginButton.getGlobalBounds().contains(mousePos)) {
                    logic->attemptLogin(loginUsernameString, loginPasswordString);
                }
                else if (goToSignupText.getGlobalBounds().contains(mousePos)) {
                    // Corrected: Directly set AppState for navigation
                    logic->setAppState(AppState::Signup);

                    // Clear input fields for both login and signup pages
                    loginUsernameString = "";
                    loginPasswordString = "";
                    loginUsernameInputText.setString("Username");
                    loginPasswordInputText.setString("Password");
                    isLoginUsernameActive = isLoginPasswordActive = false;

                    signupUsernameString = "";
                    signupPasswordString = "";
                    signupConfirmPasswordString = "";
                    signupUsernameInputText.setString("Username");
                    signupPasswordInputText.setString("Password");
                    signupConfirmPasswordInputText.setString("Confirm Password");
                    isSignupUsernameActive = isSignupPasswordActive = isSignupConfirmPasswordActive = false;
                }
                else if (loginUsernameInputRect.getGlobalBounds().contains(mousePos)) {
                    isLoginUsernameActive = true; isLoginPasswordActive = false;
                }
                else if (loginPasswordInputRect.getGlobalBounds().contains(mousePos)) {
                    isLoginPasswordActive = true; isLoginUsernameActive = false;
                }
                else {
                    isLoginUsernameActive = false; isLoginPasswordActive = false;
                }
            }
            else if (logic->getAppState() == AppState::Signup) {
                if (signupButton.getGlobalBounds().contains(mousePos)) {
                    logic->attemptSignup(signupUsernameString, signupPasswordString, signupConfirmPasswordString);
                }
                else if (goToLoginText.getGlobalBounds().contains(mousePos)) {
                    // Corrected: Directly set AppState for navigation
                    logic->setAppState(AppState::Login);

                    // Clear input fields for both signup and login pages
                    signupUsernameString = "";
                    signupPasswordString = "";
                    signupConfirmPasswordString = "";
                    signupUsernameInputText.setString("Username");
                    signupPasswordInputText.setString("Password");
                    signupConfirmPasswordInputText.setString("Confirm Password");
                    isSignupUsernameActive = isSignupPasswordActive = isSignupConfirmPasswordActive = false;

                    loginUsernameString = "";
                    loginPasswordString = "";
                    loginUsernameInputText.setString("Username");
                    loginPasswordInputText.setString("Password");
                    isLoginUsernameActive = isLoginPasswordActive = false;
                }
                else if (signupUsernameInputRect.getGlobalBounds().contains(mousePos)) {
                    isSignupUsernameActive = true; isSignupPasswordActive = isSignupConfirmPasswordActive = false;
                }
                else if (signupPasswordInputRect.getGlobalBounds().contains(mousePos)) {
                    isSignupPasswordActive = true; isSignupUsernameActive = isSignupConfirmPasswordActive = false;
                }
                else if (signupConfirmPasswordInputRect.getGlobalBounds().contains(mousePos)) {
                    isSignupConfirmPasswordActive = true; isSignupUsernameActive = isSignupPasswordActive = false;
                }
                else {
                    isSignupUsernameActive = false; isSignupPasswordActive = false; isSignupConfirmPasswordActive = false;
                }
            }
            else if (logic->getAppState() == AppState::MainApp) {
                if (showAddSongOptionsPrompt) {
                    if (addOptionPlaylistButton.getGlobalBounds().contains(mousePos)) {
                        showAddSongOptionsPrompt = false;
                        showAddSongPrompt = true;
                    }
                    else if (addOptionQueueButton.getGlobalBounds().contains(mousePos)) {
                        logic->addSongToQueueFromGlobalIndex(songToAddGlobalIndexForPrompt);
                        showAddSongOptionsPrompt = false;
                    }
                    else {
                        showAddSongOptionsPrompt = false;
                        logic->showMessage("Add song options cancelled.");
                    }
                    return;
                }

                if (showAddSongPrompt) {
                    float startY = promptBackground.getPosition().y + 50;
                    const auto& userPlaylists = logic->getUserPlaylists();
                    for (size_t i = 0; i < userPlaylists.size(); ++i) {
                        sf::FloatRect textBounds(promptBackground.getPosition().x + 20, startY + (i * 25), 360, 20);
                        if (textBounds.contains(mousePos)) {
                            logic->addSongToCustomPlaylistFromGlobalIndex(songToAddGlobalIndexForPrompt, userPlaylists[i].name);
                            showAddSongPrompt = false;
                            break;
                        }
                    }
                    return;
                }

                if (playButton.getGlobalBounds().contains(mousePos)) {
                    logic->togglePlayPause();
                }
                else if (nextButton.getGlobalBounds().contains(mousePos)) {
                    logic->playNextSong();
                }
                else if (prevButton.getGlobalBounds().contains(mousePos)) {
                    logic->playPreviousSong();
                }
                else if (volumeUpButton.getGlobalBounds().contains(mousePos)) {
                    logic->adjustVolume(5);
                }
                else if (volumeDownButton.getGlobalBounds().contains(mousePos)) {
                    logic->adjustVolume(-5);
                }
                else if (progressBar.getGlobalBounds().contains(mousePos) && event.mouseButton.button == sf::Mouse::Left) {
                    isScrubbing = true;
                }
                else if (newPlaylistInputRect.getGlobalBounds().contains(mousePos)) {
                    isCreatingPlaylist = true;
                    newPlaylistInputText.setString(newPlaylistInputString + "_"); // Show cursor
                    isSearchingRight = false;
                    searchBarTextRight.setString(searchInputStringRight); // Reset search bar text display
                }
                else if (createPlaylistButton.getGlobalBounds().contains(mousePos)) {
                    logic->createNewPlaylist(newPlaylistInputString);
                    newPlaylistInputString = "";
                    newPlaylistInputText.setString("New playlist name...");
                    isCreatingPlaylist = false;
                }
                else if (searchBarRectRight.getGlobalBounds().contains(mousePos)) {
                    isSearchingRight = true;
                    searchBarTextRight.setString(searchInputStringRight + "_"); // Show cursor
                    isCreatingPlaylist = false;
                    newPlaylistInputText.setString(newPlaylistInputString); // Reset new playlist text display
                }
                else if (logoutButton.getGlobalBounds().contains(mousePos)) {
                    logic->logoutUser();
                }
                else {
                    isCreatingPlaylist = false;
                    if (newPlaylistInputString.empty()) newPlaylistInputText.setString("New playlist name...");
                    else newPlaylistInputText.setString(newPlaylistInputString);

                    isSearchingRight = false;
                    if (searchInputStringRight.empty()) searchBarTextRight.setString("Search songs...");
                    else searchBarTextRight.setString(searchInputStringRight);

                    // Playlist selection in left panel
                    sf::FloatRect playlistsListArea(0, playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10, 250, leftPanel.getSize().y - (playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10) - (window.getSize().y - newPlaylistInputRect.getPosition().y) - 10);
                    if (playlistsListArea.contains(mousePos)) {
                        for (size_t i = 0; i < playlistsDisplayTexts.size(); ++i) {
                            if (playlistsDisplayTexts[i].getGlobalBounds().contains(mousePos)) {
                                if (i == 0) { // All Songs
                                    logic->selectPlaylist("All Songs");
                                }
                                else if (i == 1) { // Queue
                                    logic->selectPlaylist("Queue");
                                }
                                else { // Custom playlists
                                    int customPlaylistIndex = (playlistsScrollOffset + i) - 2;
                                    const auto& userPlaylists = logic->getUserPlaylists();
                                    if (customPlaylistIndex >= 0 && customPlaylistIndex < userPlaylists.size()) {
                                        logic->selectPlaylist(userPlaylists[customPlaylistIndex].name);
                                    }
                                }
                                break;
                            }
                        }
                    }

                    // Song selection in right panel
                    const auto* activePlaylist = logic->getCurrentActivePlaylist();
                    std::vector<Song> songsInDisplay;
                    if (activePlaylist->name == "Queue") { // Special handling for queue view
                        songsInDisplay = logic->searchSongsInActivePlaylist(searchInputStringRight); // Use logic's search for the actual active playlist
                    }
                    else { // All Songs or Custom playlist
                        songsInDisplay = isSearchingRight && !searchInputStringRight.empty()
                            ? activePlaylist->searchSongs(searchInputStringRight)
                            : activePlaylist->getAllSongs();
                    }


                    for (size_t i = 0; i < currentPlaylistSongDisplayTexts.size(); ++i) {
                        if ((currentPlaylistSongDisplayTexts[i].first.getGlobalBounds().contains(mousePos) ||
                            currentPlaylistSongDisplayTexts[i].second.getGlobalBounds().contains(mousePos)) &&
                            (currentPlaylistSongsScrollOffset + i < songsInDisplay.size())) {

                            Song clickedSong = songsInDisplay[currentPlaylistSongsScrollOffset + i];
                            // Find the original index of the clicked song in the global 'All Songs' playlist
                            int allSongsOriginalIndex = -1;
                            const auto& allSongs = logic->getAllSongsGlobalPlaylist().songsList;
                            for (int j = 0; j < allSongs.size; ++j) {
                                if (allSongs.get(j) && allSongs.get(j)->filePath == clickedSong.filePath) {
                                    allSongsOriginalIndex = j;
                                    break;
                                }
                            }
                            songToAddGlobalIndexForPrompt = allSongsOriginalIndex; // For add song prompt

                            if (event.mouseButton.button == sf::Mouse::Left) {
                                int indexInActivePlaylist = -1;
                                if (activePlaylist == &logic->getAllSongsGlobalPlaylist()) {
                                    indexInActivePlaylist = allSongsOriginalIndex;
                                }
                                else if (activePlaylist->name == "Queue") { // Special handling for queue view
                                    if (allSongsOriginalIndex != -1) {
                                        // Play directly from global index and remove from queue's deque
                                        auto& upcomingQueue = const_cast<std::deque<int>&>(logic->getUpcomingQueueIndices()); // Temporarily cast for modification
                                        auto it = std::find(upcomingQueue.begin(), upcomingQueue.end(), allSongsOriginalIndex);
                                        if (it != upcomingQueue.end()) {
                                            upcomingQueue.erase(it);
                                        }
                                        logic->playSongByIndexInActivePlaylist(allSongsOriginalIndex); // Play it as if from global all songs
                                    }
                                }
                                else { // Custom playlist
                                    Node<Song>* current = activePlaylist->songsList.head;
                                    int k = 0;
                                    while (current) {
                                        if (current->data.filePath == clickedSong.filePath) {
                                            indexInActivePlaylist = k;
                                            break;
                                        }
                                        current = current->next;
                                        k++;
                                    }
                                }

                                if (indexInActivePlaylist != -1) {
                                    logic->playSongByIndexInActivePlaylist(indexInActivePlaylist);
                                }
                                else {
                                    logic->showMessage("Error: Could not play song from active playlist.");
                                }
                            }
                            else if (event.mouseButton.button == sf::Mouse::Right && allSongsOriginalIndex != -1) {
                                showAddSongOptionsPrompt = true;
                            }
                            else if (event.mouseButton.button == sf::Mouse::Right && allSongsOriginalIndex == -1) {
                                logic->showMessage("Can only add songs from 'All Songs' library to custom playlists or queue.");
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                isScrubbing = false;
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            if (logic->getAppState() == AppState::MainApp && isScrubbing) {
                float progressBarStartX = progressBar.getPosition().x;
                float progressBarWidth = progressBar.getSize().x;
                float clickX = static_cast<float>(event.mouseMove.x);

                clickX = std::max(progressBarStartX, clickX);
                clickX = std::min(progressBarStartX + progressBarWidth, clickX);

                float normalizedPosition = (clickX - progressBarStartX) / progressBarWidth;
                logic->setMusicPlayingOffset(normalizedPosition);
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            if (logic->getAppState() == AppState::Login) {
                if (isLoginUsernameActive) handleInputString(loginUsernameString, loginUsernameInputText, event.text);
                else if (isLoginPasswordActive) handleInputString(loginPasswordString, loginPasswordInputText, event.text, true);
            }
            else if (logic->getAppState() == AppState::Signup) {
                if (isSignupUsernameActive) handleInputString(signupUsernameString, signupUsernameInputText, event.text);
                else if (isSignupPasswordActive) handleInputString(signupPasswordString, signupPasswordInputText, event.text, true);
                else if (isSignupConfirmPasswordActive) handleInputString(signupConfirmPasswordString, signupConfirmPasswordInputText, event.text, true);
            }
            else if (logic->getAppState() == AppState::MainApp) {
                if (isSearchingRight) {
                    handleInputString(searchInputStringRight, searchBarTextRight, event.text);
                    updateCurrentPlaylistDisplay();
                }
                else if (isCreatingPlaylist) {
                    handleInputString(newPlaylistInputString, newPlaylistInputText, event.text);
                }
            }
        }
        else if (event.type == sf::Event::MouseWheelScrolled) {
            if (logic->getAppState() == AppState::MainApp) {
                // Corrected: Removed redundant mapPixelToCoords call, it was already mapped by SFML event system
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));

                sf::FloatRect playlistsListArea(0, playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10, 250, leftPanel.getSize().y - (playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10) - (window.getSize().y - newPlaylistInputRect.getPosition().y) - 10);
                sf::FloatRect songsListArea(rightPanel.getPosition().x, 110, rightPanel.getSize().x, rightPanel.getSize().y - 110);

                int totalScrollablePlaylists = logic->getUserPlaylists().size() + 2; // +2 for All Songs and Queue
                int maxPlaylistsScrollOffset = std::max(0, totalScrollablePlaylists - maxDisplayItems);

                if (playlistsListArea.contains(mousePos)) {
                    if (event.mouseWheelScroll.delta < 0) playlistsScrollOffset = std::min(playlistsScrollOffset + 1, maxPlaylistsScrollOffset);
                    else playlistsScrollOffset = std::max(0, playlistsScrollOffset - 1);
                    updatePlaylistsDisplay();
                }
                else if (songsListArea.contains(mousePos)) {
                    int totalSongs = (logic->getCurrentActivePlaylist()->name == "Queue") ? logic->getUpcomingQueueIndices().size() : logic->getCurrentActivePlaylist()->songsList.size;
                    totalSongs = isSearchingRight && !searchInputStringRight.empty() ? logic->searchSongsInActivePlaylist(searchInputStringRight).size() : totalSongs;

                    int maxSongsScrollOffset = std::max(0, totalSongs - maxDisplayItems);
                    if (event.mouseWheelScroll.delta < 0) currentPlaylistSongsScrollOffset = std::min(currentPlaylistSongsScrollOffset + 1, maxSongsScrollOffset);
                    else currentPlaylistSongsScrollOffset = std::max(0, currentPlaylistSongsScrollOffset - 1);
                    updateCurrentPlaylistDisplay();
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                if (logic->getAppState() == AppState::Login) {
                    if (isLoginUsernameActive || isLoginPasswordActive) {
                        logic->attemptLogin(loginUsernameString, loginPasswordString);
                    }
                }
                else if (logic->getAppState() == AppState::Signup) {
                    if (isSignupUsernameActive || isSignupPasswordActive || isSignupConfirmPasswordActive) {
                        logic->attemptSignup(signupUsernameString, signupPasswordString, signupConfirmPasswordString);
                    }
                }
                else if (logic->getAppState() == AppState::MainApp) {
                    if (isCreatingPlaylist) {
                        if (!newPlaylistInputString.empty()) {
                            logic->createNewPlaylist(newPlaylistInputString);
                            newPlaylistInputString = "";
                            newPlaylistInputText.setString("New playlist name...");
                            isCreatingPlaylist = false;
                        }
                        else {
                            logic->showMessage("Playlist name cannot be empty.");
                        }
                    }
                }
            }
            else if (logic->getAppState() == AppState::MainApp) { // Other key controls for music playback
                if (event.key.code == sf::Keyboard::Right) logic->seekMusic(5.0f); // Changed from setMusicPlayingOffset
                else if (event.key.code == sf::Keyboard::Left) logic->seekMusic(-5.0f); // Changed from setMusicPlayingOffset
                else if (event.key.code == sf::Keyboard::Up) logic->adjustVolume(5);
                else if (event.key.code == sf::Keyboard::Down) logic->adjustVolume(-5);
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                if (showAddSongPrompt) {
                    showAddSongPrompt = false;
                    logic->showMessage("Add song to playlist cancelled.");
                }
                else if (showAddSongOptionsPrompt) {
                    showAddSongOptionsPrompt = false;
                    logic->showMessage("Add song options cancelled.");
                }
            }
        }
    }
}

void MusicPlayerGUI::render() {
    window.clear(sf::Color(10, 10, 10));

    if (logic->getAppState() == AppState::Login) {
        window.draw(loginBackground); window.draw(loginHeader);
        window.draw(loginUsernameInputRect); window.draw(loginUsernameInputText);
        window.draw(loginPasswordInputRect); window.draw(loginPasswordInputText);
        window.draw(loginButton); window.draw(loginButtonText);
        window.draw(goToSignupText);
        // Update input field text to show cursor or placeholder
        loginUsernameInputText.setString(loginUsernameString + (isLoginUsernameActive ? "_" : ""));
        loginPasswordInputText.setString(std::string(loginPasswordString.length(), '*') + (isLoginPasswordActive ? "_" : ""));
        if (!isLoginUsernameActive && loginUsernameString.empty()) loginUsernameInputText.setString("Username");
        if (!isLoginPasswordActive && loginPasswordString.empty()) loginPasswordInputText.setString("Password");
    }
    else if (logic->getAppState() == AppState::Signup) {
        window.draw(signupBackground); window.draw(signupHeader);
        window.draw(signupUsernameInputRect); window.draw(signupUsernameInputText);
        window.draw(signupPasswordInputRect); window.draw(signupPasswordInputText);
        window.draw(signupConfirmPasswordInputRect); window.draw(signupConfirmPasswordInputText);
        window.draw(signupButton); window.draw(signupButtonText);
        window.draw(goToLoginText);
        // Update input field text to show cursor or placeholder
        signupUsernameInputText.setString(signupUsernameString + (isSignupUsernameActive ? "_" : ""));
        signupPasswordInputText.setString(std::string(signupPasswordString.length(), '*') + (isSignupPasswordActive ? "_" : ""));
        signupConfirmPasswordInputText.setString(std::string(signupConfirmPasswordString.length(), '*') + (isSignupConfirmPasswordActive ? "_" : ""));
        if (!isSignupUsernameActive && signupUsernameString.empty()) signupUsernameInputText.setString("Username");
        if (!isSignupPasswordActive && signupPasswordString.empty()) signupPasswordInputText.setString("Password");
        if (!isSignupConfirmPasswordActive && signupConfirmPasswordString.empty()) signupConfirmPasswordInputText.setString("Confirm Password");
    }
    else if (logic->getAppState() == AppState::MainApp) {
        window.draw(leftPanel); window.draw(rightPanel); window.draw(bottomPanel);
        window.draw(homeText); window.draw(playlistsHeader);
        updatePlaylistsDisplay(); // Refresh playlist texts
        for (const auto& text : playlistsDisplayTexts) window.draw(text);
        window.draw(newPlaylistInputRect); window.draw(newPlaylistInputText);
        window.draw(createPlaylistButton); window.draw(createPlaylistButtonText);
        window.draw(logoutButton); window.draw(logoutButtonText);

        window.draw(libraryHeader); window.draw(titleColumnHeader); window.draw(artistColumnHeader);
        updateCurrentPlaylistDisplay(); // Refresh song texts
        for (const auto& pair : currentPlaylistSongDisplayTexts) {
            window.draw(pair.first); window.draw(pair.second);
        }
        window.draw(searchBarRectRight); window.draw(searchBarTextRight);

        currentSongDisplayBottom.setString(logic->getCurrentSongDisplay());
        window.draw(currentSongDisplayBottom);
        window.draw(progressBar);
        // Update scrubber position
        if (logic->getMusicDuration() != sf::Time::Zero) {
            float progressRatio = logic->getMusicPlayingOffset().asSeconds() / logic->getMusicDuration().asSeconds();
            float newScrubberX = progressBar.getPosition().x + (progressBar.getSize().x * progressRatio);
            scrubberCircle.setPosition(newScrubberX, progressBar.getPosition().y);
        }
        else {
            scrubberCircle.setPosition(progressBar.getPosition().x, progressBar.getPosition().y);
        }
        window.draw(scrubberCircle);
        currentTimeText.setString(formatTime(logic->getMusicPlayingOffset()));
        totalTimeText.setString(formatTime(logic->getMusicDuration()));
        window.draw(currentTimeText); window.draw(totalTimeText);

        playButtonText.setString(logic->getIsPlaying() ? "||" : ">");
        volumeLevel.setSize(sf::Vector2f(logic->getMusicVolume() / 100.0f * 80, 5));

        window.draw(prevButton); window.draw(prevButtonText);
        window.draw(playButton); window.draw(playButtonText);
        window.draw(nextButton); window.draw(nextButtonText);
        window.draw(volumeDownButton); window.draw(volumeDownButtonText);
        window.draw(volumeUpButton); window.draw(volumeUpButtonText);
        window.draw(volumeBar); window.draw(volumeLevel);

        if (showAddSongPrompt) {
            window.draw(promptBackground); window.draw(promptText);
            float currentY = promptBackground.getPosition().y + 50;
            const auto& userPlaylists = logic->getUserPlaylists();
            for (const auto& pl : userPlaylists) {
                sf::Text playlistOption(pl.name, font, 18);
                playlistOption.setFillColor(sf::Color::White);
                playlistOption.setString("- " + pl.name);
                playlistOption.setPosition(promptBackground.getPosition().x + 20, currentY);
                window.draw(playlistOption);
                currentY += 25;
            }
            window.draw(promptInstructionText);
        }
        else if (showAddSongOptionsPrompt) {
            window.draw(addOptionsBackground); window.draw(addOptionsHeader);
            window.draw(addOptionPlaylistButton); window.draw(addOptionPlaylistButtonText);
            window.draw(addOptionQueueButton); window.draw(addOptionQueueButtonText);
        }

        // Update input field text to show cursor or placeholder
        newPlaylistInputText.setString(newPlaylistInputString + (isCreatingPlaylist ? "_" : ""));
        searchBarTextRight.setString(searchInputStringRight + (isSearchingRight ? "_" : ""));
        if (!isCreatingPlaylist && newPlaylistInputString.empty()) newPlaylistInputText.setString("New playlist name...");
        if (!isSearchingRight && searchInputStringRight.empty()) searchBarTextRight.setString("Search songs...");
    }

    // Always draw message text
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(18);
    messageText.setFillColor(sf::Color::Cyan);
    messageText.setPosition(20, window.getSize().y - 30); // Adjusted position to be at the very bottom
    if (logic->getMessageElapsedTime().asSeconds() <= logic->getMessageDisplayDuration()) {
        messageText.setString(logic->getMessageText());
    }
    else {
        messageText.setString("");
    }
    window.draw(messageText);

    window.display();
}

void MusicPlayerGUI::updatePlaylistsDisplay() {
    playlistsDisplayTexts.clear();

    sf::Text allSongsText(logic->getAllSongsGlobalPlaylist().name, font, 18);
    allSongsText.setFillColor(sf::Color::White);
    allSongsText.setPosition(20, playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10);
    playlistsDisplayTexts.push_back(allSongsText);

    sf::Text queueText("Queue (" + std::to_string(logic->getQueueSize()) + ")", font, 18);
    queueText.setFillColor(sf::Color::White);
    queueText.setPosition(20, playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10 + (1 * 25));
    playlistsDisplayTexts.push_back(queueText);

    const auto& userPlaylists = logic->getUserPlaylists();
    int displayCount = 2;
    for (size_t i = 0; i < userPlaylists.size(); ++i) {
        if (i < playlistsScrollOffset) continue;
        sf::Text playlistText(userPlaylists[i].name, font, 18);
        playlistText.setFillColor(sf::Color::White);
        playlistText.setPosition(20, playlistsHeader.getPosition().y + playlistsHeader.getCharacterSize() + 10 + (displayCount * 25));
        playlistsDisplayTexts.push_back(playlistText);
        displayCount++;
        if (displayCount >= maxDisplayItems) break;
    }
}

void MusicPlayerGUI::updateCurrentPlaylistDisplay() {
    currentPlaylistSongDisplayTexts.clear();

    std::vector<Song> songsToDisplay;
    const Playlist* activePlaylist = logic->getCurrentActivePlaylist();

    if (activePlaylist && activePlaylist->name == "Queue") {
        const auto& upcomingQueueIndices = logic->getUpcomingQueueIndices();
        const auto& allSongsGlobal = logic->getAllSongsGlobalPlaylist().songsList;

        if (isSearchingRight && !searchInputStringRight.empty()) {
            std::string queryLower = searchInputStringRight;
            std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);
            for (int globalIndex : upcomingQueueIndices) {
                const Song* song = allSongsGlobal.get(globalIndex); // Use const get
                if (song) {
                    std::string songTitleLower = song->title;
                    std::transform(songTitleLower.begin(), songTitleLower.end(), songTitleLower.begin(), ::tolower);
                    if (songTitleLower.find(queryLower) != std::string::npos) {
                        songsToDisplay.push_back(*song);
                    }
                }
            }
        }
        else {
            for (int globalIndex : upcomingQueueIndices) {
                const Song* song = allSongsGlobal.get(globalIndex); // Use const get
                if (song) {
                    songsToDisplay.push_back(*song);
                }
            }
        }
    }
    else if (activePlaylist) {
        songsToDisplay = isSearchingRight && !searchInputStringRight.empty()
            ? activePlaylist->searchSongs(searchInputStringRight)
            : activePlaylist->getAllSongs();
    }

    int displayCount = 0;
    for (size_t i = 0; i < songsToDisplay.size(); ++i) {
        if (i < currentPlaylistSongsScrollOffset) continue;
        sf::Text titleText(songsToDisplay[i].title, font, 18);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(rightPanel.getPosition().x + 20, 110 + (displayCount * 25));

        sf::Text artistText(songsToDisplay[i].artist, font, 18);
        artistText.setFillColor(sf::Color::White);
        artistText.setPosition(rightPanel.getPosition().x + 20 + 300, 110 + (displayCount * 25));

        currentPlaylistSongDisplayTexts.push_back({ titleText, artistText });
        displayCount++;
        if (displayCount >= maxDisplayItems) break;
    }
}

void MusicPlayerGUI::setupInputField(sf::RectangleShape& rect, sf::Text& text, const std::string& placeholder, float yPos) {
    rect.setSize(sf::Vector2f(300, 40));
    rect.setFillColor(sf::Color(50, 50, 50));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color(80, 80, 80));
    // Position relative to the center of the login/signup background
    // Removed rect.getOrigin().x + as it's typically not needed and might cause offset.
    rect.setPosition(loginBackground.getPosition().x + loginBackground.getSize().x / 2 - rect.getSize().x / 2, yPos);

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color(180, 180, 180));
    text.setString(placeholder);
    text.setPosition(rect.getPosition().x + 10, rect.getPosition().y + 8);
}

void MusicPlayerGUI::setupButton(sf::RectangleShape& rect, sf::Text& text, const std::string& str, float yPos) {
    rect.setSize(sf::Vector2f(150, 45));
    rect.setFillColor(sf::Color(60, 60, 60));
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color(100, 100, 100));
    // For setupAddSongOptionsPromptUI, the position is set before calling this helper.
    // So, this helper should only set the text and its origin/position relative to the button.
    // The yPos parameter is thus used for the text's vertical centering within the button.

    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(rect.getPosition().x + rect.getSize().x / 2.0f, rect.getPosition().y + rect.getSize().y / 2.0f);
}


void MusicPlayerGUI::handleInputString(std::string& targetString, sf::Text& targetText, const sf::Event::TextEvent& textEvent, bool isPassword) {
    if (textEvent.unicode == '\b') {
        if (!targetString.empty()) targetString.pop_back();
    }
    else if (textEvent.unicode < 128 && textEvent.unicode != '\n' && textEvent.unicode != '\r' && textEvent.unicode != '\t') {
        targetString += static_cast<char>(textEvent.unicode);
    }
    if (isPassword) {
        targetText.setString(std::string(targetString.length(), '*') + "_");
    }
    else {
        targetText.setString(targetString + "_");
    }
}

std::string MusicPlayerGUI::formatTime(sf::Time time) {
    int minutes = static_cast<int>(time.asSeconds()) / 60;
    int seconds = static_cast<int>(time.asSeconds()) % 60;
    char buffer[6];
    sprintf_s(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    return buffer;
}