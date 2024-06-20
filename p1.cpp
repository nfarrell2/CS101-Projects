#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
using namespace std;

void PrintCmd(){
    cout << "command list:" << endl << "\tprint" << endl << "\tcols" << endl << "\tsearch *|col_name val" << endl
    << "\tmin col_name" << endl << "\tmax col_name" << endl << "\tavg col_name" << endl << endl;
}

int getCol(string& colName, string cols[51]){
    if(colName == "*"){
        return -2;
    }
    int i = 0;
    while(cols[i] != colName){
        i++;
        if(cols[i] == ""){
            return -1;
        }
    }
    return i;
}

bool searchLine(string keyword, string line){
    string word;
    istringstream iss(line);
    while(getline(iss, word, ',')){
        if(word == keyword){
            return true;
        }
    }
    return false;
}

bool searchLine(string keyword, string line, int column){
    string word;
    istringstream iss(line);
    for(int i = 0; i < column+1; i++){
        getline(iss, word, ',');
    }
    if(word == keyword){
        return true;
    }
    return false;
}

int main(int argc, char *argv[]){
    //Usage error case
    if(argc != 3){
        cout << "Usage: ./a database.csv #col_width" << endl;
        return 0;
    }

    string filename = argv[1];
    int width = stoi(argv[2]);
    string columnName[51];
    string header, line, word, command;
    int i = 0;

    //Opened file safely error case
    ifstream ifs(filename);
    if(!ifs){
        cout << "Failed to open \"" << filename << "\"" << endl;
        return 0;
    }

    //consume header line
    getline(ifs, header);
    istringstream iss(header);
    while(getline(iss, word, ',')){
        columnName[i] = word;
        i++;
    } 

    cout << "Enter a command or \"help\" for a command list:" << endl;
    cin >> command;
    while(command != "quit"){
        ifs.close();
        ifs.open(filename);
        getline(ifs, header); //consume header for repeat usage

        if(command == "help"){
            PrintCmd();
        }
        else if(command == "print"){
            for(int k = 0; k < i; k++){
                cout << setw(width) << right << columnName[k];
            }
            cout << '\n';
            while(getline(ifs, line)){
                istringstream iss(line);
                for(int k = 0; k < i; k++){
                    getline(iss, word, ',');
                    cout << setw(width) << right << word;
                }
                cout << '\n';
            }
        }
        else if(command == "cols"){
            for(int k = 0; k < i; k++){
                cout << columnName[k] << endl;
            }
        }
        else if(command == "search"){
            int count = 0;
            string column, searchVal, temp;
            cin >> quoted(column) >> quoted(searchVal);
            getline(cin, temp);
            int col = getCol(column, columnName);
            if(col == -1){
                cout << "Invalid column \"" << column << "\"" << endl;
                count = 5;
            }
            else if(col == -2){
                while(getline(ifs, line)){
                    if(searchLine(searchVal, line)){
                        if(count == 0){
                            for(int k = 0; k < i; k++){
                                cout << setw(width) << right << columnName[k];
                            }
                            cout << '\n';
                        }
                        istringstream iss(line);
                        for(int k = 0; k < i; k++){
                            getline(iss, word, ',');
                            cout << setw(width) << right << word;
                        }
                        cout << '\n';
                        count++;
                    }
                }
            }
            else{
                while(getline(ifs, line)){
                    if(searchLine(searchVal, line, col)){
                        if(count == 0){
                            for(int k = 0; k < i; k++){
                                cout << setw(width) << right << columnName[k];
                            }
                            cout << '\n';
                        }
                        istringstream iss(line);
                        for(int k = 0; k < i; k++){
                            getline(iss, word, ',');
                            cout << setw(width) << right << word;
                        }
                        cout << '\n';
                        count++;
                    }
                }
            }
            if(count == 0){
                cout << "No results" << endl;
            }
        }
        else if(command == "min"){
            string column, minWord;
            cin >> quoted(column);
            int c = getCol(column, columnName);
            int count = 0;
            double min = 0, num = 0;
            while(getline(ifs, line) && c != -1){
                istringstream iss(line);
                for(int k = 0; k < c+1; k++){
                    getline(iss, word, ',');
                }
                if(word != ""){
                    num = stod(word);
                    if(count == 0){
                        min = num;
                        count++;
                    }
                    else if(num < min){
                        min = num;
                        count++;
                    }
                }
            }
            if(c == -1){
                cout << "Invalid column \"" << column << "\"" << endl;
            }
            else if(count == 0){
                cout << "The min for col \"" << column << "\" = N/A" << endl;
            }
            else{
                cout << "The min for col \"" << column << "\" = " << min << endl;
            }
        }
        else if(command == "max"){
            string column, maxWord;
            cin >> quoted(column);
            int c = getCol(column, columnName);
            int count = 0;
            double max = 0, num = 0;
            while(getline(ifs, line) && c != -1){
                istringstream iss(line);
                for(int k = 0; k < c+1; k++){
                    getline(iss, word, ',');
                }
                if(word != ""){
                    num = stod(word);
                    if(count == 0){
                        max = num;
                        count++;
                    }
                    else if(num > max){
                        max = num;
                        count++;
                    }
                }
            }
            if(c == -1){
                cout << "Invalid column \"" << column << "\"" << endl;
            }
            else if(count == 0){
                cout << "The max for col \"" << column << "\" = N/A" << endl;
            }
            else{
                cout << "The max for col \"" << column << "\" = " << max << endl;
            }
        }
        else if(command == "avg"){
            string column, avgWord;
            cin >> ws >> quoted(column);
            int c = getCol(column, columnName);
            double total = 0, count = 0, num = 0;
            while(getline(ifs, line) && c != -1){
                istringstream iss(line);
                for(int k = 0; k < c+1; k++){
                    getline(iss, word, ',');
                }
                if(word != ""){
                    num = stod(word);
                    total += num;
                    count++;
                }
            }
            if(c == -1){
                cout << "Invalid column \"" << column << "\"" << endl;
            }
            else if(count == 0){
                cout << "The avg for col \"" << column << "\" = N/A" << endl;
                
            }
            else{
                double avg = total/count;
                cout << "The avg for col \"" << column << "\" = " << avg << endl;
            }
        }
        else{
            cout << "Invalid command" << endl;
        }

        cout << "Enter a command or \"help\" for a command list:" << endl;
        cin >> command;
    }
    return 0;
}