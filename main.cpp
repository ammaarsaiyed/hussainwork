#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

string toLowerCase(const string& input) {
    string result = input;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

class OpenAndReadFile 
{
    public:
        vector<string> lines;

        OpenAndReadFile(string filename) 
        {
            ifstream infile(filename);

            if (!infile) 
            {
                cerr << "Failed to open file: " << filename << endl;
                return;
            }

            string line;
            while (getline(infile, line)) 
            {
                if (!line.empty() && line.back() == '\r') // check if last character is carriage return
                {
                    line.erase(line.size() - 1);
                }
                lines.push_back(line);
            }

            infile.close();
        }
        
        vector<string> getLines() 
        {
            return lines;
        }
};

class TweetFilter 
{
    public:
        vector<string> tweets;
        vector<string> words;
        vector<string> filteredTweets;
        vector<string> filteredWords;
        static vector<int> counterArray;

        TweetFilter(vector<string> tweets, vector<string> words) 
        {
            this->tweets = tweets;
            this->words = words;
        }

        vector<string> filter() 
        {
            for (int i = 0; i < tweets.size(); i++) 
            {
                string& tweet = tweets[i];
                string filteredTweet = tweet; // create a copy of the tweet
                for (int j = 0; j < words.size(); j++) 
                {
                    const string& word = words[j];
                    int pos = 0;
                    while ((pos = toLowerCase(tweet).find(toLowerCase(word), pos)) != string::npos) 
                    {
                        adder(j);
                        filteredTweet.replace(pos, word.size(), filteredWords[j]);
                        pos += word.size();
                    }
                }
                filteredTweets.push_back(filteredTweet); // add filtered tweet to the vector
            }
            return filteredTweets;
        }

        vector<string> replaceMiddleChars() 
        {
            for (const string& word : words) 
            {
                string filteredWord = word;
                cout << filteredWord << endl;
                int middleIndex = word.length() / 2;
                filteredWord[middleIndex] = '*';
                filteredWords.push_back(filteredWord);
            }
            return filteredWords;
        }

        void adder(int indexOfWord)
        {
            counterArray[indexOfWord]++;
        }

};

vector<int> TweetFilter::counterArray; // Define the static vector in the implementation file

int main() 
{
    vector<string> arr[4];

    OpenAndReadFile tweetsReader("tweets1.txt");
    vector<string> tweets1 = tweetsReader.getLines();
    arr[0] = tweets1;

    OpenAndReadFile tweetsReader2("tweets2.txt");
    vector<string> tweets2 = tweetsReader2.getLines();
    arr[1] = tweets2;

    OpenAndReadFile tweetsReader3("tweets3.txt");
    vector<string> tweets3 = tweetsReader3.getLines();
    arr[2] = tweets3;

    OpenAndReadFile tweetsReader4("tweets4.txt");
    vector<string> tweets4 = tweetsReader4.getLines();
    arr[3] = tweets4;

    OpenAndReadFile postiveReader("positive.txt");
    vector<string> positive = postiveReader.getLines();

    OpenAndReadFile negativeReader("negative.txt");
    vector<string> negative = negativeReader.getLines();

    OpenAndReadFile wordsReader("banned.txt");
    vector<string> words = wordsReader.getLines();

    for(vector<string> tweetVector : arr)
    {
        TweetFilter filter(tweetVector, words);
        if (TweetFilter::counterArray.empty())
        {
            TweetFilter::counterArray.resize(words.size());
        }
        vector<string> filteredWords = filter.replaceMiddleChars();
        vector<string> filteredTweets = filter.filter();

        for (int i = 0; i<filteredTweets.size(); i++)
    {
        //cout << filteredTweets[i] << endl;
    }
        
    }

     vector<pair<int, int> > vp;
    
    for (int i = 0; i < words.size(); ++i) 
    {
           vp.push_back(make_pair(TweetFilter::counterArray[i], i));
            }
        
            sort(vp.begin(), vp.end());

            for(int i = words.size() - 1;i >= words.size()-10 &&  i>=0; --i)
            {
                cout << vp[i].first << "\t"
                << words[vp[i].second] << endl;
            }

    

    return 0;
}
