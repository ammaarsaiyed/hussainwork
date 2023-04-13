#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include<vector>
#include <string>
#include <set>  
#include <algorithm>  


using namespace std;

const int navMenuUpperBound = 5;
const int navMenuLowerBound = 1;

struct Player {
    string name;
    string team;
    int goals = 0;
    int redCards = 0;
    int yellowCards = 0;
};

bool compareByName(Player& player1, Player& player2) {
    return player1.name < player2.name;
}

void printPlayer(Player& player) {
    cout << left << setw(15) << player.name
         << setw(15) << player.team
         << setw(10) << player.goals
         << setw(15) << player.yellowCards
         << setw(10) << player.redCards << endl;
}

void writeToFile(vector<Player>& playersData) {
    // Open file for writing
    ofstream outputFile("teamStats.csv");

    if (!outputFile.is_open()) {
        cout << "Failed to open file teamStats.csv" << endl;
        return;
    }

    outputFile << "Name,Team,Goals Scored,Number of Yellow Cards,Number of Red Cards" << endl;

    // Write player data to CSV file
    for (Player& player : playersData) {
        outputFile << player.name << ","
                    << player.team << ","
                    << player.goals << ","
                    << player.yellowCards << ","
                    << player.redCards << endl;
    }

    outputFile.close();
    cout << "Players written to CSV file" << endl;
}

void addPlayer(vector<Player>& playersData) 
{
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

    playersData.push_back(newPlayer);
    cout << "Player added." << endl;
    cout << "--------------------------------" << endl;
    writeToFile(playersData);
}

string toLower(string myString)
{
    string lowerString;
    for (auto c : myString) lowerString += tolower(c);
    return lowerString;
}

void readFile(vector<Player>& playersData)
{
    ifstream infile("teamStats.csv");
    if (!infile)
    {
        cout << "Error opening file 'teamStats.csv'" << endl;
        return;
    }

    string line;
    char delim = ',';
    bool headerRow = true;

    while (getline(infile, line))
    {
        if (headerRow) {
            headerRow = false;
            continue;
        }

        Player player;
        istringstream ss(line);
        string item;
        
        // Get the name
        if (getline(ss, item, ',')) {
            player.name = item;
        }

        // Get the team
        if (getline(ss, item, ',')) {
            player.team = item;
        }

         // Get the goals scored
        if (getline(ss, item, ',')) {
            stringstream ss2(item);
            ss2 >> player.goals;
        }

        // Get the number of yellow cards
        if (getline(ss, item, ',')) {
            stringstream ss2(item);
            ss2 >> player.yellowCards;
        }

        // Get the number of red cards
        if (getline(ss, item, ',')) {
            stringstream ss2(item);
            ss2 >> player.redCards;
        }

        // Add the new player playersData to the vector
        playersData.push_back(player);
    }

    infile.close();
}

int printFile(vector<Player>& playersData)
{
    // Print the playersData stored in the 2D vector
    cout << left << setw(15) << "Name"
         << setw(15) << "Team"
         << setw(10) << "Goals"
         << setw(15) << "Yellow Cards"
         << setw(10) << "Red Cards" << endl;

    sort(playersData.begin(), playersData.end(), compareByName);

    for (Player player : playersData)
    {
        printPlayer(player);
    }

    return 0;
}

void updatePlayer(vector<Player>& playersData)
{
    string playerUpdateTerm;

    set<string> playersNames;
    for (Player player : playersData)
    {
        playersNames.insert(player.name);
    }    
    cout << "--------------------------------" << endl;

    cout << "Enter the name of the player you want to update: \t" << endl;
    cin >> playerUpdateTerm;
    playerUpdateTerm = toLower(playerUpdateTerm);

    bool foundPlayer = false;
    for (Player& player : playersData)
    {
        if (toLower(player.name) == playerUpdateTerm)
        {
            foundPlayer = true;
            cout << "--------------------------------" << endl;
            cout << "Updating stats for " << player.name << ":" << endl;
            cout << "Team: " << player.team << endl;
            cout << "Goals Scored: " << player.goals << endl;
            cout << "Yellow Cards: " << player.yellowCards << endl;
            cout << "Red Cards: " << player.redCards << endl;

            cout << "Enter the new team name: ";
            cin >> player.team;

            cout << "Enter the new number of goals scored: ";
            cin >> player.goals;

            cout << "Enter the new number of yellow cards: ";
            cin >> player.yellowCards;

            cout << "Enter the new number of red cards: ";
            cin >> player.redCards;

            cout << "Stats for " << player.name << " have been updated." << endl;
            printPlayer(player);
            writeToFile(playersData);
            break;
        }
    }
    if (!foundPlayer)
    {
        cout << "Player not found : " << playerUpdateTerm << endl;
    }
}

void teamSearch(vector<Player>& playersData)
{
    string teamSearchTerm;

    set<string> teams;
    for (Player player : playersData)
    {
        teams.insert(player.team);
    }

    cout << "--------------------------------" << endl;
    cout << "Enter the name of the team you want the stats of:\t" << endl;
    cin >> teamSearchTerm;
    teamSearchTerm = toLower(teamSearchTerm);

    while (1)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(80, '\n');
            // Ask the user once more 
            cout << "Please enter a team from the following: " << endl;
            for (string team : teams)
            {
                cout << team << endl;
            }
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

    for (Player player : playersData)
    {
        if (toLower(player.team) == teamSearchTerm)
        {
            found = true;
            totalGoals += player.goals;
            totalYellowCards += player.yellowCards;
            totalRedCards += player.redCards;
        }
    }

    if (!found)
    {
        cout << "Team not found" << endl;
        cout << "Please enter a team from the following: " << endl;
        for (string team : teams)
        {
            cout << team << endl;
        }
        teamSearch(playersData);
    }
    else
    {
        cout << "Total Goals: " << totalGoals << endl;
        cout << "Total Yellow Cards: " << totalYellowCards << endl;
        cout << "Total Red Cards: " << totalRedCards << endl;
    }
}

void playerSearch(vector<Player> playersData)
{
    string playerSearchTerm;

    set<string> players;
    for (Player player : playersData)
    {
        players.insert(player.name);
    }

    cout << "--------------------------------" << endl;
    cout << "Enter the name of the player you want the stats of: \t" << endl;
    cin >> playerSearchTerm;
    playerSearchTerm = toLower(playerSearchTerm);

    while (1)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(80, '\n');
            // Ask the user once more 
            cout << "Please only enter a name from the following: " << endl;
            for (string player : players)
            {
                cout << player << endl;
            }
            cout << "--------------------------------" << endl;
            cin >> playerSearchTerm;
            playerSearchTerm = toLower(playerSearchTerm);

        }
        else
            break;
    }

    bool foundPlayer = false;
    for (Player player : playersData) {
        if (toLower(player.name) == playerSearchTerm) {
            cout << "--------------------------------" << endl;
            cout << "Stats for " << player.name << ":" << endl;
            printPlayer(player);
            foundPlayer = true;
            break;
        }
    }
    if (!foundPlayer) {
        cout << "Player not found : " << playerSearchTerm << endl;
        cout << "Please enter a player from the following: " << endl;
        for (string player : players)
        {
            cout << player << endl;
        }
        playerSearch(playersData);
    }

}

void mainMenu(vector<Player> playersData)
{
    int navigationChoice;

    cout << "NAVIGATION MENU" << endl;
    cout << "\t 1. Show all stats for all players" << endl;
    cout << "\t 2. Search for a specific player's stats" << endl;
    cout << "\t 3. Search for a teams stats" << endl;
    cout << "\t 4. Add a new player" << endl;   
    cout << "\t 5. Update records for a player" << endl;   
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
        printFile(playersData);
        mainMenu(playersData);
    }
    else if (navigationChoice == 2)
    {
        playerSearch(playersData);
        mainMenu(playersData);
    }
    else if (navigationChoice == 3)
    {
        teamSearch(playersData);
        mainMenu(playersData);
    }
    else if (navigationChoice == 4)
    {
        addPlayer(playersData);
        mainMenu(playersData);
    }
    else if (navigationChoice == 5)
    {
        updatePlayer(playersData);
        mainMenu(playersData);
    }
}

int main()
{
    vector<Player> playersData;
    readFile(playersData);
    mainMenu(playersData);
}
 