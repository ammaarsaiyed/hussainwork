#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include<vector>
#include <string>
using namespace std;
const int navMenuUpperBound=4;
const int navMenuLowerBound = 1;
struct Player {
    string name;
    string team;
    int goals = 0;
    int redCards = 0;
    int yellowCards = 0;
};

void addPlayer(vector<Player>& players) {
    Player newPlayer;
    cout << "Enter the players name:\t" << endl;
    cin >> newPlayer.name;
    while (1)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(80, '\n');
            // Ask the user once more 
            cout << "Please only enter a name " << endl;
            cout << "--------------------------------" << endl;
            cin >> newPlayer.name;
        }
        else
            break;
    }
    cout << "Enter the players team:\t" << endl;
    cin >> newPlayer.team;

    cout << "Enter the players goals: \t" << endl;
    cin >> newPlayer.goals;
    
    cout << "Enter the players red cards\t" << endl;
    cin >> newPlayer.redCards;

    cout << "Enter the players yellow cards\t" << endl;
    cin >> newPlayer.yellowCards;

    players.push_back(newPlayer);
}
string toLower(string myString)
{
    string lowerString;
    for (auto c : myString) lowerString += tolower(c);
    return lowerString;
}

int printFile()
{
    ifstream infile("teamStats.csv");
    if (!infile)
    {
        cout << "Error opening file 'teamStats.csv'" << endl;
        return 1;
    }

    vector<vector<string>> data;
    string line;
    char delim = ',';
    while (getline(infile, line))
    {
        istringstream ss(line);
        vector<string> row;
        string tmp;
        while (getline(ss, tmp, delim))
        {
            row.push_back(tmp);
        }
        data.push_back(row);
    }

    infile.close();

    // Print the data stored in the 2D vector
    for (const auto& row : data)
    {
        for (const auto& val : row)
        {
            cout << val << ",";
        }
        cout << endl;
    }

    return 0;

}
void updatePlayer(vector<vector<string>>& data)
{
    string playerUpdateTerm;
    cout << "--------------------------------" << endl;
    cout << "Enter the name of the player you want to update: \t" << endl;
    cin >> playerUpdateTerm;
    playerUpdateTerm = toLower(playerUpdateTerm);

    bool foundPlayer = false;
    for (auto& row : data)
    {
        if (toLower(row[0]) == playerUpdateTerm)
        {
            foundPlayer = true;
            cout << "--------------------------------" << endl;
            cout << "Updating stats for " << row[0] << ":" << endl;
            cout << "Team: " << row[1] << endl;
            cout << "Goals Scored: " << row[2] << endl;
            cout << "Yellow Cards: " << row[3] << endl;
            cout << "Red Cards: " << row[4] << endl;

            cout << "Enter the new team name: ";
            cin >> row[1];

            cout << "Enter the new number of goals scored: ";
            cin >> row[2];

            cout << "Enter the new number of yellow cards: ";
            cin >> row[3];

            cout << "Enter the new number of red cards: ";
            cin >> row[4];

            cout << "Stats for " << row[0] << " have been updated." << endl;
            break;
        }
    }
    if (!foundPlayer)
    {
        cout << "No stats found for player " << playerUpdateTerm << endl;
    }
}

void teamSearch()
{
    string teamSearchTerm;
    string teamOne = "uclan";
    string teamTwo = "jaguar";
    string teamThree = "apple";
    cout << "--------------------------------" << endl;
    cout << "Enter the name of the team you want the stats of:\t" << endl;
    cin >> teamSearchTerm;
    teamSearchTerm=toLower(teamSearchTerm);
    while (1)
    {
        if (cin.fail() || (teamSearchTerm != teamOne && teamSearchTerm != teamTwo && teamSearchTerm != teamThree))
        {
            cin.clear();
            cin.ignore(80, '\n');
            // Ask the user once more 
            cout << "Please only enter a name from the following: " << endl;
            cout << teamOne << endl;
            cout << teamTwo << endl;
            cout << teamThree << endl;
            cout << "--------------------------------" << endl;
            cin >> teamSearchTerm;
            teamSearchTerm = toLower(teamSearchTerm);
        }
        else
            break;
    }
    int totalGoals = 0;
    int totalYellowCards = 0;
    int totalRedCards = 0;
    bool found = false;

    ifstream infile("teamStats.csv");
    if (!infile)
    {
        cout << "Error opening file 'teamStats.csv'" << endl;
        return;
    }

    string line;
    getline(infile, line); // skip the header line
    while (getline(infile, line))
    {
        istringstream ss(line);
        vector<string> row;
        string tmp;
        while (getline(ss, tmp, ','))
        {
            row.push_back(tmp);
        }

        if (toLower(row[1]) == teamSearchTerm)
        {
            found = true;
            totalGoals += stoi(row[2]);
            totalYellowCards += stoi(row[3]);
            totalRedCards += stoi(row[4]);
        }
    }

    infile.close();

    if (!found)
    {
        cout << "Team not found" << endl;
    }
    else
    {
        cout << "Total Goals: " << totalGoals << endl;
        cout << "Total Yellow Cards: " << totalYellowCards << endl;
        cout << "Total Red Cards: " << totalRedCards << endl;
    }


    
}

void playerSearch(const vector<vector<string>>& data)
{
    string nameOne = "mitchell";
    string nameTwo = "jobs";
    string nameThree = "edwards";
    string nameFour = "dan";
    string playerSearchTerm;
    cout << "--------------------------------" << endl;
    cout << "Enter the name of the player you want the stats of: \t" << endl;
    cin >> playerSearchTerm;
    playerSearchTerm = toLower(playerSearchTerm);
    while (1)
    {
        if (cin.fail() || (playerSearchTerm != nameOne && playerSearchTerm != nameTwo && playerSearchTerm != nameThree && playerSearchTerm != nameFour))
        {
            cin.clear();
            cin.ignore(80, '\n');
            // Ask the user once more 
            cout << "Please only enter a name from the following: " << endl;
            cout << nameOne << endl;
            cout << nameTwo << endl;
            cout << nameThree << endl;
            cout << nameFour << endl;
            cout << "--------------------------------" << endl;
            cin >> playerSearchTerm;
            playerSearchTerm = toLower(playerSearchTerm);

        }
        else
            break;


    }
    bool foundPlayer = false;
    for (const auto& row : data) {
        if (toLower(row[0]) == playerSearchTerm) {
            cout << "--------------------------------" << endl;
            cout << "Stats for " << row[0] << ":" << endl;
            cout << "Team: " << row[1] << endl;
            cout << "Goals Scored: " << row[2] << endl;
            cout << "Yellow Cards: " << row[3] << endl;
            cout << "Red Cards: " << row[4] << endl;
            foundPlayer = true;
            break;
        }
    }
    if (!foundPlayer) {
        cout << "No stats found for player " << playerSearchTerm << endl;
    }

}


void mainMenu(vector<Player>& players)
{
    int navigationChoice;
  
    cout << "NAVIGATION MENU" << endl;
    cout << "\t 1. Show all stats for all players" << endl;
    cout << "\t 2. Search for a specific player's stats" << endl;
    cout << "\t 3. Search for a teams stats" << endl;
    cout << "\t 4. Update records for a player" << endl;   
    cin >> navigationChoice;
    while (1)
    {
        if (cin.fail() || navigationChoice < navMenuLowerBound || navigationChoice>navMenuUpperBound)
        {
            cin.clear();
            cin.ignore(80, '\n');
            // Ask the user once more 
            cout << "Please only enter a positive integer from 1-4" << endl;
            cin >> navigationChoice;
        }
        else
            break;

    }
    if (navigationChoice == 1)
    {
        printFile();
        mainMenu(players);
    }
    else if (navigationChoice == 2)
    {
        vector<vector<string>> data;
        ifstream infile("teamStats.csv");
        if (!infile)
        {
            cout << "Error opening file 'teamStats.csv'" << endl;
        }
        else
        {
            string line;
            char delim = ',';
            while (getline(infile, line))
            {
                istringstream ss(line);
                vector<string> row;
                string tmp;
                while (getline(ss, tmp, delim))
                {
                    row.push_back(tmp);
                }
                data.push_back(row);
            }
            infile.close();
            playerSearch(data);
        }
        mainMenu(players);
    }
    else if (navigationChoice == 3)
    {
        teamSearch();
        mainMenu(players);
    }
    else
    {
        addPlayer(players);
        printFile();
    }
    

}


int main()
{
    vector<Player> players;
    mainMenu(players);
}
 