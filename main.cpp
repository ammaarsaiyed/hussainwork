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
        vector<int> filteredWords;
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
                        filteredTweet.replace(pos+filteredWords[j], 1, "*");
                        pos += word.size();
                    }
                }
                filteredTweets.push_back(filteredTweet); // add filtered tweet to the vector
            }
            return filteredTweets;
        }

        vector<int> replaceMiddleChars() 
        {
            for (const string& word : words) 
            {
                string filteredWord = word;
                int middleIndex = word.length() / 2;
                filteredWord[middleIndex] = '*';
                filteredWords.push_back(middleIndex);
            }
            return filteredWords;
        }

        void adder(int indexOfWord)
        {
            counterArray[indexOfWord]++;
        }

};

class CreateAndWriteToFile
{
    public:
        string appendFiltered(string filename)
        {
            size_t dotIndex = filename.find_last_of('.'); // Find the last dot (.) in the filename
            string nameWithoutExtension = filename.substr(0, dotIndex); // Extract the filename without extension
            string extension = filename.substr(dotIndex); // Extract the file extension
            return nameWithoutExtension + "Filtered" + extension; // Append "Filtered" and the extension to the filename
        }

        CreateAndWriteToFile(vector<string> contentsToWrite, string fileName)
        {
            ofstream file(appendFiltered(fileName)); 
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
    vector<string> tweets; // Change to vector of vector of strings for multiple tweets

    TweetSentimentAnalyzer(vector<string> positiveWords, vector<string> negativeWords, vector<string> tweets) // Modify the constructor to take vector of vector of strings
    {
        this->positiveWords = positiveWords; // Assign positiveWords vector
        this->negativeWords = negativeWords; // Assign negativeWords vector
        this->tweets = tweets; // Assign tweets vector
    }

    vector<string> analyzeTweetSentiment() // Modify return type to vector of ints
    {
        vector<string> sentimentScores; // Create a vector to store sentiment scores for each tweet
        for (auto& tweet : tweets) // Iterate through each tweet in the vector of tweets
        {
            int positiveCount = countMatches(tweet, positiveWords); // Call countMatches for positiveWords
            int negativeCount = countMatches(tweet, negativeWords); // Call countMatches for negativeWords

            string sentimentScore = positiveCount > negativeCount ? "Positive" : (negativeCount > positiveCount ? "Negative" : "Neutral"); // Calculate sentiment score for the tweet
            sentimentScores.push_back(sentimentScore); // Store sentiment score in the vector
        }
        return sentimentScores; // Return the vector of sentiment scores
    }

    int countMatches(string tweet, vector<string>& words) // Modify parameter type to vector of strings
{
    int count = 0;
    for (const auto& word : words)
    {
        if (toLowerCase(tweet).find(toLowerCase(word)) != string::npos) // Call find on tweet instead of tweetWord
        {
            count++;
        }
    }
    return count;
}

};

vector<int> TweetFilter::counterArray; // Define the static vector in the implementation file

int main() 
{
    vector<vector<string>> vectorOfTweets;
    vector<string> tweetsToRead = {"tweets1.txt", "tweets2.txt", "tweets3.txt", "tweets4.txt"};

    OpenAndReadFile postiveReader("positive.txt");
    vector<string> positive = postiveReader.getLines();

    OpenAndReadFile negativeReader("negative.txt");
    vector<string> negative = negativeReader.getLines();

    OpenAndReadFile wordsReader("banned.txt");
    vector<string> words = wordsReader.getLines();

    for(size_t i=0; i<tweetsToRead.size(); i++)
    {
        
        OpenAndReadFile tweetsReader(tweetsToRead[i]);
        vectorOfTweets.push_back(tweetsReader.getLines());

        TweetFilter filter(vectorOfTweets[i], words);
        if (TweetFilter::counterArray.empty())
        {
            TweetFilter::counterArray.resize(words.size());
        }
        vector<int> filteredWords = filter.replaceMiddleChars();
        vector<string> filteredTweets = filter.filter();
        
        TweetSentimentAnalyzer tsa(positive, negative, vectorOfTweets[i]);
        vector<string> scores = tsa.analyzeTweetSentiment();

        cout << "Sentiment Analysis for " << tweetsToRead[i] << "\n" << endl;
        for(size_t j=0; j<vectorOfTweets[i].size();j++)
        {
            cout << scores[j] << "\t: " << vectorOfTweets[i][j] << endl;
        }

        cout << "----------------------------------------" << endl;
        CreateAndWriteToFile write(filteredTweets, tweetsToRead[i]);        
    }

     vector<pair<int, int> > vp;
    cout << "Top 10 Words found in all files: \n" << endl;
    for (int i = 0; i < words.size(); ++i) 
    {
           vp.push_back(make_pair(TweetFilter::counterArray[i], i));
            }
        
            sort(vp.begin(), vp.end());

            for(int i = words.size() - 1;i >= words.size()-10 &&  i>=0; --i)
            {
                cout << "Found " <<  words[vp[i].second] << "\t "
                << vp[i].first << " times."<< endl;
            }

    

    return 0;
}
