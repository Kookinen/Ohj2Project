/* Player class
 * Programmer: Joni Koskinen
 * Student number: H291631
 * User_id: cqjoko
 * Email: joni.e.koskinen@tuni.fi
 */
#ifndef PLAYER_HH
#define PLAYER_HH
#include <iostream>
#include <QString>

class Player
{
public:
    Player();

    // constructor gets the name of the player as an attribute.
    Player(const QString& name);

    ~Player();

    // Method returns the amount of points the player has
    int get_points();

    // Method returns the name of the player as QString
    QString get_name();

    // Method adds one point to the player
    void add_points();

private:
    int points_ = 0;
    QString name_;
};

#endif // PLAYER_HH
