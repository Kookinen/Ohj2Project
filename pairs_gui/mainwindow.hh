/* Mainwindow class
 * Programmer: Joni Koskinen
 * Student number: H291631
 * User_id: cqjoko
 * Email: joni.e.koskinen@tuni.fi
 */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include "player.hh"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMainWindow>
#include <vector>
#include <QHBoxLayout>
#include <utility>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <random>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Slot is called when card amount is set and the button is clicked.
    // After getting the card amount, slot calls card creation methods and
    // the method for getting player amount.
    void handle_card_amount_button_click();

    // Slot is called when card is being clicked. Slot gets the position of the
    // click and if it matches with a card position that isn't already open, it
    // opens the card. Slot also calls wait slot if there are two cards
    // visible.
    void handle_card_button_click();

    // Slot is called when there are two cards visible. Slot has a timer for 1
    // second before cards are turned or deleted. Slot checks if cards are same
    // ore different and acts accordingly
    void wait();

    // Slot is called when player amount is set and the button is clicked.
    // Slot sets up as many lineEdits as player amount says and let's player
    // interact with them.
    void handle_player_amount_button_click();

    // Slot is called when player names have been entered and the button is
    // clicked. Cards are being set enabled and players will be added to a
    // vector to handle turns.
    void handle_name_confirmation_button_click();

    // Slot is called when quit button is being clicked. Slot deletes all
    // necessary dynamic memory and ends the program.
    void handle_quit_button_click();

    // Slot is called when new game button is being clicked. Slot deletes
    // old games dynamic memory, resets game variables and starts a new game by
    // calling init_car_amount_input.
    void handle_new_game_button_click();


private:
    Ui::MainWindow *ui;

    // Vector of animals
    const std::vector<std::string>
            png_animals = {"monkey", "horse", "mouse", "snake", "platypus",
                           "giraffe", "fox", "cat", "dog", "bear", "panda",
                           "panther", "raccoon", "pidgeon", "bunny"};
    // Card back const
    const std::string BACK = "back";
    // Defining where the images can be found and what kind of images they are
    const std::string PREFIX=":/";
    const std::string SUFFIX=".png";

    std::default_random_engine random_number;
    int WARMUP_PERIOD = 20;

    const int MAX_PLAYER_AMOUNT = 4;

    int ANIMAL_SIZE = 50;

    // Space between cards
    const int MARGIN = 20;

    //space between LineEdit and button
    const int SPACE = 100;

    //space between scoreboard and playernames
    const int SMALLER_SPACE = 80;

    const int DOUBLE_MARGIN = 40;

    // Constants for cards
    const int CARD_BUTTON_WIDTH = 60;
    const int CARD_BUTTON_HEIGTH = 80;

    // Button constants
    const int DEFAULT_BUTTON_WIDTH = 80;
    const int DEFAULT_BUTTON_HEIGTH = 20;

    // Location of the graphics view and scene
    const int VIEW_X = 20;
    const int VIEW_Y = 20;

    // Card visibility time
    const int DELAY = 1000;

    // Text constants
    const int TEXT_LENGTH = 340;
    const int TEXT_Y_LOCATION = 70;
    const int SMALL_TEXT_HEIGHT = 20;

    const int PLAYER_LABEL_LOCATION = -25;

    // name length to know if it needs to be shortened and name length
    // to set geometry to names.
    const int NAME_LENGTH = 10;
    const int NAME_PIXEL_LENGTH = 100;

    // Constants for scoreboard "cards"
    const int RECT_HEIGTH = 10;
    const int RECT_WIDTH = 60;


    // setting up card and player amount
    int card_amount = 0;
    int player_amount = 0;

    // Amount of rows and columns
    int smaller_factor_ = 0;
    int bigger_factor_ = 0;

    // indexes for two different cards to be turned
    int card_index = 0;
    int card_index2 = 0;

    // setting up visibility and turn counter
    int visible = 0;
    int in_turn = 0;

    bool game_over = false;

    // Struct to see if the two cards turned have the same image
    struct animal
    {
        QPixmap image;
        unsigned int id;
    };

    std::vector<QPushButton*> cards;
    std::vector<QLineEdit*> player_name_line_edits;
    std::vector<QLabel*> player_labels;
    std::vector<QLabel*> scoreboard_labels;
    std::vector<Player*> players;
    std::vector<animal> animals;
    std::vector<animal> playable_animals;
    std::vector<QLabel*> score_labels;
    QPixmap card_back;
    QPoint previous_position = QPoint(-1, -1);
    QPushButton* confirm_button;
    QPushButton* name_confirm_button;
    QPushButton* quit_button;
    QPushButton* new_game_button;
    QLineEdit* user_input;
    QLabel* error_message;
    QLabel* in_turn_message;
    QGraphicsScene* scoreboard;
    QGraphicsView* game_board;
    QGraphicsView* score_board_view;

    // Method creates a LineEdit widget to ask for card amount and
    // QPushButton to confirm it
    void init_card_amount_input();

    // Method deletes widgets that have no more use
    void delete_widgets();

    // Method calculates the nearest two numbers that give the amount
    // of cards as a result to multiplication.
    void calculate_product();

    // Method initializes the gameboard with empty cards
    void init_empty_cards();

    // Method sets the cards back image
    void set_card_back();

    // Method gets the same amount of cards as requested by the user and
    // shuffles them
    void fill_cards();

    // Method sets up the pictures in the cards
    void get_animals();

    // Method creates a LineEdit widget to ask for player amount and
    // QPushButton to confirm it
    void init_player_amount_input();

    // Method sets a clicked card visible
    void set_visible(int);

    // Method sets up the scoreboard and the player name labels on top of it.
    void set_up_scoreboard();

    // Method adds two cards in the scoreboard when a player finds a pair
    void add_card_block(int, int);

    // Method creates a button to quit and a button to restart the game.
    void init_game_reset_buttons();

    // Method deletes the gameboard and card widgets.
    void delete_gameboard();

    // Method deletes the scoreboard and rest of the widgets to clear the table
    void delete_scoreboard();

    // Method finds out the winner and ends the game
    void victory();

    // Method creates scoreboard labels at the end of the game.
    void show_scores();

};
#endif // MAINWINDOW_HH
