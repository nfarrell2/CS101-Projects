#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <list>

using namespace std;

// Store the return of this function as an unsigned long!
unsigned long djb2(string str) {
	const char *ptr = str.c_str();
	unsigned long hash = 5381;
	int c;
	while ((c = *ptr++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

string get(list<string> l, int i)
{
	list<string>::iterator j = l.begin();
	for(int k = 0; k < i; k++)
	{
		j++;
		if(j == l.end())
		{
			return "LIST IS SHORTER THAN " + to_string(i) + " INDEXES";
		}
	}
	return *j;
}

class HashTable {
	public:
		vector<list<string>> table;
		int tcollisions;
		HashTable(int size, string strategy, int param = 0)
		{
			strat = strategy;
			tableSize = size;
			table.resize(tableSize);
			par = param;
			tcollisions = 0;
		}
		int size()
		{
			return tableSize;
		}
		int h(string key)
		{
			return djb2(key) % tableSize;
		}
		int h2(string key)
		{
			return par - (djb2(key) % par);
		}
		int insert(string value, int line)
		{
			bucket = h(value);

			if(table[bucket].empty())
			{
				table[bucket].push_front(value);
				table[bucket].push_back(to_string(line));
				return 0;
			}
			else
			{
				if(table[bucket].front() == value)
				{
					table[bucket].push_back(to_string(line));
					return 0;
				}
				else{
					for(int j = 1; j < tableSize-1; j++)
					{
						if(strat == "lp")
						{
							bucket = (h(value) + j) % tableSize;
						}
						else if(strat == "qp")
						{
							bucket = (h(value) + j*j) % tableSize;
						}
						else if(strat == "dh")
						{
							bucket = (h(value) + j*h2(value)) % tableSize;
						}
						
						tcollisions++;
						if(table[bucket].empty())
						{
							table[bucket].push_front(value);
							table[bucket].push_back(to_string(line));
							break;
						}
						else if(table[bucket].front() == value)
						{
							table[bucket].push_back(to_string(line));
							break;
						}
					}
					return 0;
				}
			}
			return 1;
		}	
		void printBucket(int bucket)
		{
			cout << table[bucket].front() << " appears on lines [";
			list<string>::iterator it = table[bucket].begin();
			it++;
			while(it != table[bucket].end())
			{
				if(*it == table[bucket].back())
				{
					cout << *it << "]" << endl;
				}
				else
				{
					cout << *it << ",";
				}
				it++;
			}
		}	
		int find(string value) 
		{
			bucket = h(value);
			int col = 0;

			if(table[bucket].front() == value)
			{
				printBucket(bucket);
				return col;
			}
			else
			{
				for(int j = 1; j < tableSize-1; j++)
				{
					if(strat == "lp")
					{
						bucket = (h(value) + j) % tableSize;
					}
					else if(strat == "qp")
					{
						bucket = (h(value) + j*j) % tableSize;
					}
					else if(strat == "dh")
					{
						bucket = (h(value) + j*h2(value)) % tableSize;
					}
					col++;
					if(table[bucket].front() == value)
					{
						printBucket(bucket);
						return col;
					}
					if(table[bucket].empty())
					{
						break;
					}
				}
				cout << value << " appears on lines []" << endl;
			}
			return col;
		}	

	private:
		int tableSize, bucket, par;
		string key, strat;

};

bool getWord(string& line, string& word)
{
	word = "";
	size_t i = 0;
   
	while(i < line.size() && !isalpha(line[i]))
	{
		i++;
	}

	while(i < line.size() && isalpha(line[i]))
	{
		word += tolower(line[i]);
		i++;
	}
   
	line.erase(0, i);

	return !word.empty();
}

int main(int argc, char **argv) {
	string input = argv[1];
	string query = argv[2];
	int size = stoi(argv[3]);
	string strategy = argv[4];
    int param, numWords = 0;
    if(strategy == "dh")
	{
		param = stoi(argv[5]);
    }

	HashTable table(size, strategy, param);

    string line, word;
    int col, lineNum = 1, coll = 0, unique = 0;
    ifstream inFS(input);
    while(getline(inFS, line))
    {
		while(getWord(line, word))
		{
		table.insert(word, lineNum);
		numWords++;
		}
		lineNum++;
	}
	inFS.close();

	cout << "The number of words found in the file was " << numWords << endl;
	for(int i = 0; i < table.size(); i++)
	{
		if(!table.table[i].empty())
		{
			unique++;
		}
	}
	coll = table.tcollisions;
	cout << "The number of unique words found in the file was " << unique << endl;
	cout << "The number of collisions was " << coll << endl << endl;

	inFS.open(query);
	vector<string> queries;
	while(getline(inFS, word))
	{
		queries.push_back(word);
	}
	inFS.close();

	for(int i = 0; i < int(queries.size()); i++)
	{
		col = table.find(queries[i]);
		cout << "The search had " << col << " collisions" << endl;
		if(i != (int(queries.size()) - 1))
		{
			cout << endl;
		}
	}




    //table.printTable();
    return 0;
}
