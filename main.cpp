#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

string toLowerCase(const string& input) 
{
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
                if (!line.empty() && line.back() == '\r')
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
                string filteredTweet = tweet; 
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
                filteredTweets.push_back(filteredTweet); 
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
            size_t dotIndex = filename.find_last_of('.'); 
            string nameWithoutExtension = filename.substr(0, dotIndex); 
            string extension = filename.substr(dotIndex);
            return nameWithoutExtension + "Filtered" + extension; 
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
        vector<string> tweets;

        TweetSentimentAnalyzer(vector<string> positiveWords, 
                               vector<string> negativeWords, 
                               vector<string> tweets) 
        {
            this->positiveWords = positiveWords; 
            this->negativeWords = negativeWords;
            this->tweets = tweets;
        }

        vector<string> analyzeTweetSentiment() 
        {
            vector<string> sentimentScores;
            for (auto& tweet : tweets) 
            {
                int positiveCount = countMatches(tweet, positiveWords); 
                int negativeCount = countMatches(tweet, negativeWords); 

                string sentimentScore = positiveCount > negativeCount ? \
                "Positive" : (negativeCount > positiveCount ? "Negative" : "Neutral"); 
                sentimentScores.push_back(sentimentScore); 
            }
            return sentimentScores; 
        }

        int countMatches(string tweet, vector<string>& words) 
        {
            int count = 0;
            for (const auto& word : words)
            {
                if (toLowerCase(tweet).find(toLowerCase(word)) != string::npos)
                {
                    count++;
                }
            }
            return count;
        }
};

vector<int> TweetFilter::counterArray;  

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
        for(size_t j=0; j<filteredTweets.size();j++)
        {
            cout << scores[j] << "\t: " << filteredTweets[j] << endl;
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
