#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// function to convert an entire string to lowercase characters
string toLower(string myString) 
{
    string lowerString;
    for (auto c : myString) lowerString += tolower(c);
    return lowerString;
}

// Class that holds data from a file, can read a file, and return the file data
class OpenAndReadFile 
{
    public:
        vector<string> lines;

        // class function that takes in a filename, and reads each line 
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
                if (!line.empty() && line.back() == '\r')
                {
                    line.erase(line.size() - 1);
                }
                lines.push_back(line);
            }

            infile.close();
        }
        
        // function that returns the files data / the vector 
        vector<string> getLines() 
        {
            return lines;
        }
};

/*
    Class that filters out the banned words from the tweets. Contains a method
    that does the actual filtering, a method that determines how to filter the
    word, and a method to increment the banned word counter. 
*/
class BannedWordFilter 
{
    public:
        // initialise class variables to hold tweets, words, and counter
        vector<string> tweets;
        vector<string> words;
        vector<string> filteredTweets;
        vector<int> filteredWords;
        //define as static so that the adder function can be used in other class
        static vector<int> counterArray;

        BannedWordFilter(vector<string> tweets, vector<string> words) 
        {
            this->tweets = tweets;
            this->words = words;
        }

        vector<string> filter() 
        {
            // loop through every tweet in the file
            for (int i = 0; i < tweets.size(); i++) 
            {
                string& tweet = tweets[i];
                // make a copy of the tweet that will filter out the words
                string filteredTweet = tweet; 

                // loop through all the banned words 
                for (int j = 0; j < words.size(); j++) 
                {
                    const string& word = words[j];
                    int pos = 0;

                    // search the current tweet for the current banned word
                    while ((pos = toLower(tweet).find(toLower(word), pos)) != string::npos) 
                    {
                        // if found, increment the count, and filter out word
                        adder(j);
                        filteredTweet.replace(pos+filteredWords[j], 1, "*");
                        pos += word.size();
                    }
                }

                // add the new filtered tweet to the filtered tweet vector
                filteredTweets.push_back(filteredTweet); 
            }
            return filteredTweets;
        }

        /* 
            method that finds the middle character position for each banned word
            and stores the position for each word in an int vector that can be 
            used when filtering out the word rather than calculating each time
        */
        vector<int> replaceMiddleChars() 
        {
            // loop through the banned words, 
            for (string& word : words) 
            {
                // store the middle index number
                int middleIndex = word.length() / 2;
                filteredWords.push_back(middleIndex);
            }
            return filteredWords;
        }

        // method that increments the counter array for the relevant banned word
        void adder(int indexOfWord)
        {
            counterArray[indexOfWord]++;
        }

};

// Class that creates a file and writes to it.
class CreateAndWriteToFile
{
    public:

        // Method that takes filename and generates a new name with filtered
        string changeFilenameFiltered(string filename)
        {
            int indexOfDot = filename.find_last_of('.'); 
            string fileNameBeforeTxt = filename.substr(0, indexOfDot); 
            string extension = filename.substr(indexOfDot);
            return fileNameBeforeTxt + "Filtered" + extension; 
        }

        CreateAndWriteToFile(vector<string> contentsToWrite, string fileName)
        {
            ofstream file(changeFilenameFiltered(fileName)); 
            if (file.is_open()) 
            { 
                for(auto content: contentsToWrite)
                {
                    file<<content<<endl;
                }
                file.close(); 
            } 
            else 
            {
                cerr << "Failed to write to file" << endl;
            }
        }
};

class TweetSentimentAnalyzer
{
    public:
        vector<string> positiveWords;
        vector<string> negativeWords;
        vector<string> tweets;

        // constructor takes in positive words, negative words, and tweets vec
        TweetSentimentAnalyzer(vector<string> positiveWords, 
                               vector<string> negativeWords, 
                               vector<string> tweets) 
        {
            this->positiveWords = positiveWords; 
            this->negativeWords = negativeWords;
            this->tweets = tweets;
        }

        // method that analyses tweets and stores their sentiments
        vector<string> analyseTweetSentiment() 
        {
            // vector to store sentiment for all tweets in a file
            vector<string> sentimentScores;

            // loop through tweets from a file
            for (string tweet : tweets) 
            {
                int positiveCount = countMatches(tweet, positiveWords); 
                int negativeCount = countMatches(tweet, negativeWords); 

                // after the count, check if there are more pos/neg
                string sentimentScore;
                if (positiveCount > negativeCount)
                {
                    sentimentScore = "Positive";
                }
                else if (negativeCount > positiveCount)
                {
                    sentimentScore = "Negative";
                }
                else
                {
                    sentimentScore = "Neutral";
                }

                // store the scores
                sentimentScores.push_back(sentimentScore); 
            }
            return sentimentScores; 
        }

        // method to count the number of instances of words from list
        int countMatches(string tweet, vector<string>& words) 
        {
            int count = 0;
            // loop through words
            for (string word : words)
            {
                if (toLower(tweet).find(toLower(word)) != string::npos)
                {
                    count++;
                }
            }
            return count;
        }
};

/* 
    define the variable for counter array outside the class as it is static
    and needs to be accessed by multiple functions and classes
*/
vector<int> BannedWordFilter::counterArray;  

int main() 
{
    vector<vector<string>> vectorOfTweets;
    vector<string> tweetsToRead = {"tweets1.txt", "tweets2.txt", "tweets3.txt",
                                   "tweets4.txt"};

    OpenAndReadFile postiveReader("positive.txt");
    vector<string> positive = postiveReader.getLines();

    OpenAndReadFile negativeReader("negative.txt");
    vector<string> negative = negativeReader.getLines();

    OpenAndReadFile wordsReader("banned.txt");
    vector<string> words = wordsReader.getLines();

    // loop through each file
    for(size_t i=0; i<tweetsToRead.size(); i++)
    {
        // read in file and add it to vectorOfTweets
        OpenAndReadFile tweetsReader(tweetsToRead[i]);
        vectorOfTweets.push_back(tweetsReader.getLines());

        // initialise BannedWordFilter object
        BannedWordFilter filter(vectorOfTweets[i], words);
        if (BannedWordFilter::counterArray.empty())
        {
            BannedWordFilter::counterArray.resize(words.size());
        }

        // initalise filteredWords object and then filter the tweets
        vector<int> filteredWords = filter.replaceMiddleChars();
        vector<string> filteredTweets = filter.filter();
        
        TweetSentimentAnalyzer tsa(positive, negative, vectorOfTweets[i]);
        vector<string> scores = tsa.analyseTweetSentiment();

        cout << "Sentiment Analysis for " << tweetsToRead[i] << "\n" << endl;
        for(size_t j=0; j<filteredTweets.size();j++)
        {
            cout << scores[j] << "\t: " << filteredTweets[j] << endl;
        }
        cout << "----------------------------------------" << endl;

        CreateAndWriteToFile write(filteredTweets, tweetsToRead[i]);        
    }

    /* 
        create a vector of pairs, the first element holds the index of the word
        the second element holds the count. this allows the vector to be sorted
        by value, while maintaining the original index. this index is then used
        again to display the relevant word
    */
    vector<pair<int, int> > vp;
    cout << "Top 10 Words found in all files: \n" << endl;
    for (int i = 0; i < words.size(); ++i) 
    {
        vp.push_back(make_pair(BannedWordFilter::counterArray[i], i));
    }
      
    // sort the pair vector
    sort(vp.begin(), vp.end());

    // display the top 10 words, and their count
    for(int i = words.size() - 1;i >= words.size()-10 &&  i>=0; --i)
    {
        cout << "Found " <<  words[vp[i].second] << "\t "
             << vp[i].first << " times."<< endl;
    }

    return 0;
}
