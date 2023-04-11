#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class WordCounter
{
public:
    vector<string> words;
    int* counterArray;

    WordCounter(vector<string> words)
    {
        this->words = words;
        this->counterArray = new int[words.size()];
    }

    void adder(int indexOfWord)
    {
        counterArray[indexOfWord]++;
    }

    void top10()
    {
        vector<pair<int, int> > vp;
 
        for (int i = 0; i < words.size(); ++i) {
            vp.push_back(make_pair(counterArray[i], i));
        }
    
        sort(vp.begin(), vp.end());

        for(int i = words.size() - 1; i >= words.size()-10 && i>=0; --i)
        {
            cout << vp[i].first << "\t"
             << words[vp[i].second] << endl;
        }
    }

};

int main() 
{
    vector<string> myStrings = {"apple", "banana", "orange", "grape", "watermelon","a","s","d","f","g","h","j","k","l","z","x","c","v","b","n","m","q"};

    WordCounter mywc(myStrings);

    for (int i = 0; i < myStrings.size(); i++) // Use index-based loop to access counterArray
    {
        cout << myStrings[i] << ": " << mywc.counterArray[i] << endl;
    }
    mywc.top10();
    return 0;
}
