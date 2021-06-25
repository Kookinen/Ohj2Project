#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QPoint>
#include <QCursor>
#include <QGraphicsView>
#include <QDebug>
#include <algorithm>
#include <QTimer>
#include <QFont>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::random_device dev;
    random_number = std::default_random_engine(dev());
    random_number.discard(WARMUP_PERIOD);
    init_card_amount_input();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_card_amount_button_click()
{

    QString qstr_card_amount = user_input->text();
    if(qstr_card_amount.toInt() > 0 && qstr_card_amount.toInt() % 2 == 0
        && qstr_card_amount.toInt() <= 30){
        card_amount = qstr_card_amount.toInt();
        delete_widgets();
        calculate_product();
        init_empty_cards();
        fill_cards();
        init_player_amount_input();


    }
    else{
        error_message->setText("Invalid input.");
    }

}

void MainWindow::handle_card_button_click()
{
    // Storing global cursor position
    QPoint global_click_position = QCursor::pos();


    // Counting local cursor position, i.e. decreasing
    // Main Window's location from the global one
    int local_x = global_click_position.x() - geometry().x();
    int local_y = global_click_position.y() - geometry().y();
    QPoint local_click_position = QPoint(local_x, local_y);

    for(unsigned int index = 0; index < cards.size(); ++index)
    {
        if(cards.at(index)->geometry().contains(local_click_position)){
            // Here if the chosen card is in the previous click position
            // we know that it has been turned already.
            if(!(cards.at(index)->geometry().contains(previous_position))
                   && visible != 2){
                   set_visible(index);
                   if(visible == 1){
                       card_index2 = index;
                   }
                   else{
                       card_index = index;
                   }
                   visible += 1;
                   if(visible == 2){
                       QTimer::singleShot(DELAY, this, SLOT(wait()));
                       //resetting the previous position
                       previous_position = QPoint(-1, -1);
                   }
                   else{
                       previous_position = local_click_position;
                   }
                }

            return; // For efficiency reasons
                    // (only one button can be clicked at a time)
        }

    }
}

void MainWindow::wait()
{
    // if cards are same delete cards and add points
    if(playable_animals.at(card_index).id == playable_animals.at(card_index2).id){
        cards.at(card_index)->deleteLater();
        cards.at(card_index2)->deleteLater();
        cards.erase(cards.begin()+card_index);
        playable_animals.erase(playable_animals.begin()+card_index);

        if(card_index > card_index2){
            cards.erase(cards.begin()+card_index2);
            playable_animals.erase(playable_animals.begin()+card_index2);;
        }
        else{
            cards.erase(cards.begin()+card_index2-1);
            playable_animals.erase(playable_animals.begin()+card_index2-1);
        }

        add_card_block(in_turn, players.at(in_turn)->get_points());
        players.at(in_turn)->add_points();

        if(cards.size() == 0){
            victory();
        }

    }
    // if cards are not same just reset the cards
    else{
        cards.at(card_index)->setIcon(card_back);
        cards.at(card_index)->setIconSize(QSize(CARD_BUTTON_WIDTH,
                                                CARD_BUTTON_HEIGTH));
        cards.at(card_index)->setStyleSheet("background-color:white");
        cards.at(card_index2)->setIcon(card_back);
        cards.at(card_index2)->setIconSize(QSize(CARD_BUTTON_WIDTH,
                                                 CARD_BUTTON_HEIGTH));
        cards.at(card_index2)->setStyleSheet("background-color:white");
        if(in_turn == player_amount - 1){
            in_turn = 0;
        }
        else{
            in_turn += 1;
        }
        in_turn_message->setText(players.at(in_turn)->get_name() + "'s turn");
    }
    visible = 0;
}

void MainWindow::handle_player_amount_button_click()
{
    QString qstr_player_amount = user_input->text();

    if(qstr_player_amount.toInt() > 0 &&  qstr_player_amount.toInt() <=
            MAX_PLAYER_AMOUNT){
        confirm_button->hide();
        player_amount = qstr_player_amount.toInt();

        for(int index=1;index<=player_amount; index++){
            QString str = "";
            str.setNum(index);

            QLabel* player_label = new QLabel(this);
            player_label->setGeometry(bigger_factor_*
                                     (CARD_BUTTON_WIDTH+DOUBLE_MARGIN),
                                      PLAYER_LABEL_LOCATION +
                                      index*TEXT_Y_LOCATION,
                                      DEFAULT_BUTTON_WIDTH,
                                      DEFAULT_BUTTON_HEIGTH);
            player_label->setText("Player " + str);
            player_label->show();
            player_labels.push_back(player_label);

            QLineEdit* ask_player_name = new QLineEdit("",this);
            ask_player_name->setGeometry(bigger_factor_*
                                        (CARD_BUTTON_WIDTH+DOUBLE_MARGIN),
                                         index*TEXT_Y_LOCATION,
                                         DEFAULT_BUTTON_WIDTH,
                                         DEFAULT_BUTTON_HEIGTH);
            ask_player_name->show();
            player_name_line_edits.push_back(ask_player_name);

        }
        QPushButton* confirm_names = new QPushButton("Start", this);
        confirm_names->setGeometry(SPACE + bigger_factor_*
                                  (CARD_BUTTON_WIDTH+DOUBLE_MARGIN),
                                   TEXT_Y_LOCATION, DEFAULT_BUTTON_WIDTH,
                                   DEFAULT_BUTTON_HEIGTH);
        name_confirm_button = confirm_names;
        confirm_names->show();
        connect(confirm_names, &QPushButton::clicked,
                this, &MainWindow::handle_name_confirmation_button_click);
        error_message->setText("");


    }
    else{
        error_message->setText("Invalid input!");
    }

}



void MainWindow::handle_name_confirmation_button_click()
{
    bool i = true;
    for(unsigned player_index = 0; player_index<player_name_line_edits.size();
        player_index++){

        if(player_name_line_edits.at(player_index)->text().size() == 0){
            i = false;
            error_message->setText("Invalid input!");

        }
    }
    if(i){
        delete_widgets();
        // making cards enabled now that we have the players
        for(unsigned card_i = 0; card_i<cards.size();card_i++){
            cards.at(card_i)->setEnabled(true);
        }
        // Creating new player classes
        for(unsigned player_index = 0;
            player_index<player_name_line_edits.size();player_index++){

            // Modifying players name if the name is too long
            QString name = player_name_line_edits.at(player_index)->text();
            if(name.length() > NAME_LENGTH){
                name = name.remove(NAME_LENGTH, name.length()-NAME_LENGTH);
                name += "...";
            }
            Player* player = new Player(name);
            players.push_back(player);
            player_name_line_edits.at(player_index)->deleteLater();
            player_labels.at(player_index)->deleteLater();
        }
        name_confirm_button->deleteLater();
        init_game_reset_buttons();
        set_up_scoreboard();
    }


}

void MainWindow::handle_quit_button_click()
{
    delete_scoreboard();
    delete_gameboard();
    QApplication::quit();
}

void MainWindow::handle_new_game_button_click()
{
    delete_scoreboard();
    delete_gameboard();
    game_over = false;
    // resetting the game variables in case that the new game was clicked while
    // only one card was turned
    visible = 0;
    in_turn = 0;
    previous_position = QPoint(-1, -1);
    init_card_amount_input();

}


void MainWindow::init_card_amount_input()
{
    QLineEdit* ask_card_amount = new QLineEdit("",this);
    ask_card_amount->setGeometry(VIEW_X, VIEW_Y, DEFAULT_BUTTON_WIDTH,
                                 DEFAULT_BUTTON_HEIGTH);
    ask_card_amount->show();
    user_input = ask_card_amount;

    QPushButton* confirm_card_amount = new QPushButton("Start", this);
    confirm_card_amount->setGeometry(VIEW_X+SPACE, VIEW_Y,
                                     DEFAULT_BUTTON_WIDTH,
                                     DEFAULT_BUTTON_HEIGTH);
    confirm_card_amount->show();
    connect(confirm_card_amount, &QPushButton::clicked,
            this, &MainWindow::handle_card_amount_button_click);
    confirm_button = confirm_card_amount;

    QLabel* message = new QLabel(this);
    message->setGeometry(VIEW_X, TEXT_Y_LOCATION, TEXT_LENGTH, SMALL_TEXT_HEIGHT);
    message->setText("Enter card amount (an even number (max 30)):");
    message->show();
    error_message = message;


}

void MainWindow::delete_widgets()
{
    user_input->deleteLater();
    confirm_button->deleteLater();
    error_message->deleteLater();
}

void MainWindow::calculate_product()
{
    for(int index = 1; index * index <= card_amount; ++index)
    {
        if(card_amount % index == 0)
        {
            smaller_factor_ = index;
        }
    }
    bigger_factor_ = card_amount / smaller_factor_;
}


void MainWindow::init_empty_cards()
{

    QGraphicsView* view_ = new QGraphicsView(this);
    game_board = view_;
    view_->setGeometry(VIEW_X, VIEW_Y,
                       MARGIN + bigger_factor_*(CARD_BUTTON_WIDTH+MARGIN),
                       MARGIN + smaller_factor_*(CARD_BUTTON_HEIGTH+
                                        MARGIN));
    set_card_back();
    for(int y=0;y<smaller_factor_;y++){
        for(int x=0;x<bigger_factor_; x++){
            QPushButton* cardPushButton = new QPushButton("", this);
            cardPushButton->setStyleSheet("background-color:white");
            cardPushButton->setIcon(card_back);
            cardPushButton->setIconSize(QSize(CARD_BUTTON_WIDTH,
                                              CARD_BUTTON_HEIGTH));
            cardPushButton->setGeometry(DOUBLE_MARGIN + x *
                                       (CARD_BUTTON_WIDTH + MARGIN),
                                       DOUBLE_MARGIN +
                                       y*(CARD_BUTTON_HEIGTH+ MARGIN),
                                       CARD_BUTTON_WIDTH,
                                       CARD_BUTTON_HEIGTH);
            cards.push_back(cardPushButton);
            cardPushButton->show();
            view_->show();
            connect(cardPushButton, &QPushButton::clicked,
                    this, &MainWindow::handle_card_button_click);
            cardPushButton->setDisabled(true);

        }

    }

}

void MainWindow::set_card_back()
{
    std::string filename = PREFIX + BACK + SUFFIX;
    QPixmap back_image = QString::fromStdString(filename);
    back_image = back_image.scaled(CARD_BUTTON_WIDTH, CARD_BUTTON_HEIGTH);
    card_back = back_image;
}


void MainWindow::fill_cards()
{
    get_animals();
    playable_animals = {animals.begin(), animals.begin()+card_amount};
    shuffle(playable_animals.begin(), playable_animals.end(), random_number);
}

void MainWindow::get_animals()
{
    // Converting image (png) to a pixmap
    for(unsigned int index = 0; index<png_animals.size(); index++){
        std::string filename = PREFIX + png_animals.at(index) + SUFFIX;
        QPixmap image = QString::fromStdString(filename);
        image = image.scaled(ANIMAL_SIZE, ANIMAL_SIZE);
        // to find out if two images are the same, the program saves the index
        // of the image here
        animal a = { image, index };
        animals.push_back(a);
        animals.push_back(a);
    }
}



void MainWindow::init_player_amount_input()
{
    QLineEdit* ask_player_amount = new QLineEdit("",this);
    ask_player_amount->setGeometry(bigger_factor_*
                                  (DEFAULT_BUTTON_WIDTH+MARGIN),
                                   TEXT_Y_LOCATION,
                                   DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGTH);
    ask_player_amount->show();
    user_input = ask_player_amount;

    QPushButton* confirm_player_amount = new QPushButton("Confirm", this);
    confirm_player_amount->setGeometry(bigger_factor_*
                                      (DEFAULT_BUTTON_WIDTH+MARGIN),
                                       2*TEXT_Y_LOCATION,
                                       DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGTH);
    confirm_player_amount->show();
    connect(confirm_player_amount, &QPushButton::clicked, this,
            &MainWindow::handle_player_amount_button_click);
    confirm_button = confirm_player_amount;

    QLabel* message = new QLabel(this);
    message->setGeometry(bigger_factor_*(DEFAULT_BUTTON_WIDTH+MARGIN),
                         VIEW_Y, TEXT_LENGTH, SMALL_TEXT_HEIGHT);
    message->setText("Enter player amount:");
    message->show();
    error_message = message;
}



void MainWindow::set_visible(int index)
{
    cards.at(index)->setIcon(playable_animals.at(index).image);
    cards.at(index)->setIconSize(QSize(100,100));
    cards.at(index)->setStyleSheet("background-color:blue");
}



void MainWindow::set_up_scoreboard()
{

    QGraphicsView* view = new QGraphicsView(this);
    score_board_view = view;
    view->setGeometry(VIEW_X, smaller_factor_*(CARD_BUTTON_HEIGTH+
                             MARGIN) + SPACE,
                             player_amount*(RECT_WIDTH+DOUBLE_MARGIN)+2,
                             RECT_HEIGTH*card_amount+2);
    scoreboard = new QGraphicsScene(this);
    view->setScene(scoreboard);
    view->show();
    scoreboard->setSceneRect(0, 0, player_amount*(RECT_WIDTH+DOUBLE_MARGIN)-1,
                             RECT_HEIGTH*card_amount-1);

    for(int player_index = 0; player_index < player_amount; player_index++){
        QLabel* name = new QLabel(this);
        scoreboard_labels.push_back(name);
        name->setText(players.at(player_index)->get_name());
        name->setGeometry(VIEW_X+DOUBLE_MARGIN+player_index*SPACE,
                          smaller_factor_*(CARD_BUTTON_HEIGTH+MARGIN)
                          + SMALLER_SPACE, NAME_PIXEL_LENGTH,
                          SMALL_TEXT_HEIGHT);
        name->show();
    }


}

void MainWindow::add_card_block(int index, int points)
{
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    scoreboard->addRect(MARGIN+index*SPACE,
                        RECT_HEIGTH*(card_amount-1)-RECT_HEIGTH*points*2,
                        RECT_WIDTH, RECT_HEIGTH, blackPen, redBrush);
    scoreboard->addRect(MARGIN+index*SPACE,
                        RECT_HEIGTH*(card_amount-2)-RECT_HEIGTH*points*2,
                        RECT_WIDTH, RECT_HEIGTH, blackPen, redBrush);
}

void MainWindow::init_game_reset_buttons()
{
    QLabel* message = new QLabel(this);
    QPushButton* quit = new QPushButton(this);
    QPushButton* new_game = new QPushButton(this);

    new_game->setGeometry(MARGIN + bigger_factor_*
                         (CARD_BUTTON_WIDTH+DOUBLE_MARGIN),
                          VIEW_Y+ SPACE, DEFAULT_BUTTON_WIDTH,
                          DEFAULT_BUTTON_HEIGTH);
    quit->setGeometry(MARGIN + bigger_factor_*
                     (CARD_BUTTON_WIDTH+DOUBLE_MARGIN),
                      VIEW_Y + SPACE + MARGIN, DEFAULT_BUTTON_WIDTH,
                      DEFAULT_BUTTON_HEIGTH);
    message->setGeometry(MARGIN + bigger_factor_*
                        (CARD_BUTTON_WIDTH+DOUBLE_MARGIN),
                         VIEW_Y, TEXT_LENGTH, TEXT_Y_LOCATION);

    quit->setText("Quit");
    new_game->setText("New Game");
    message->setText(players.at(in_turn)->get_name() + "'s turn");

    quit->show();
    new_game->show();
    message->show();

    QFont font("Sans Serif", 20);
    message->setFont(font);

    in_turn_message = message;
    quit_button = quit;
    new_game_button = new_game;

    connect(quit, &QPushButton::clicked,
            this, &MainWindow::handle_quit_button_click);
    connect(new_game, &QPushButton::clicked,
            this, &MainWindow::handle_new_game_button_click);
}

void MainWindow::delete_gameboard()
{
    if(!game_over){
        game_board->deleteLater();
        for(unsigned int index = 0; index<cards.size(); index++){
            cards.at(index)->deleteLater();
        }
        for(int index = 0; index<player_amount; index++){
            delete players.at(index);
        }
        cards.clear();
        players.clear();
        player_name_line_edits.clear();
        player_labels.clear();

    }


}

void MainWindow::delete_scoreboard()
{
    delete in_turn_message;
    quit_button->deleteLater();
    new_game_button->deleteLater();
    scoreboard->deleteLater();
    score_board_view->deleteLater();

    //Deleting name labels from scoreboard
    for(int index = 0; index<player_amount; index++){
        scoreboard_labels.at(index)->deleteLater();
    }
    scoreboard_labels.clear();

    //Deleting score labels
    for(unsigned int index = 0; index<score_labels.size(); index++){
        delete score_labels.at(index);
    }
    score_labels.clear();
}

void MainWindow::victory()
{
    bool tie = false;
    int winner = 0;
    if(player_amount == 1){
        winner = 0;
    }
    else{
        for(int index = 1; index<player_amount; index++){
            int points = players.at(index)->get_points();
            if(points > players.at(winner)->get_points()){
                winner = index;
                tie = false;
            }
            else if(points == players.at(winner)->get_points()){
                tie = true;
            }
        }
    }
    if(tie){
        in_turn_message->setText("It's a tie!");
    }
    else{
        in_turn_message->setText(players.at(winner)->get_name() + " WINS!");
    }
    show_scores();
    delete_gameboard();
    game_over = true;   
}

void MainWindow::show_scores()
{
    QLabel* scores = new QLabel(this);
    scores->setGeometry(VIEW_X, smaller_factor_*(CARD_BUTTON_HEIGTH+MARGIN)-CARD_BUTTON_HEIGTH,
                        DEFAULT_BUTTON_WIDTH, SMALL_TEXT_HEIGHT);
    scores->setText("Scores:");
    scores->show();
    score_labels.push_back(scores);
    for(int index = 0;index<player_amount; index++){
        QLabel* name_and_score = new QLabel(this);
        name_and_score->setGeometry(VIEW_X,
                                    MARGIN+smaller_factor_*(CARD_BUTTON_HEIGTH+MARGIN)
                                    -CARD_BUTTON_HEIGTH +
                                    index*MARGIN, players.at(index)->
                                    get_name().length()+SPACE,
                                    SMALL_TEXT_HEIGHT);
        QString points = QString::number(players.at(index)->get_points());
        name_and_score->setText(players.at(index)->get_name() + " " + points);
        name_and_score->show();
        score_labels.push_back(name_and_score);
    }
}


