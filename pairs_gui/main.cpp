/* Memory game
 *
 * Description:
 * Program executes a pairs game where there is a variable amount of cards and
 * players. At first player is asked for the amount of cards and players and
 * then player names. The program checks if the inputs are compatible with the
 * game and reacts accordingly if they are not.
 * After that the game begins. Every round the player in turn
 * clicks two cards open. If they are pair, the player gets two points,
 * continues their turn and cards are deleted from the board, if not, player in
 * turn changes and the cards stay on the board but they are once again hidden.
 * Game ends when all pairs are found.
 * The player has also an option to start a new game or quit game while playing
 * and at the end of the game.
 *
 * Programmer
 * Name: Joni Koskinen
 * Student number: H291631
 * User id: cqjoko
 * E-mail: joni.e.koskinen@tuni.fi
 */

#include "mainwindow.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


}
