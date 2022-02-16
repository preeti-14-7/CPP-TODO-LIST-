#include <iostream>
#include <string>
#include <string.h>
#include <ctime>
#include <fstream>
using namespace std;

//string lsMessage = "Usage :- \n$ ./todo add \"todo item\"  # Add a new todo \n$ ./todo ls               # Show remaining todos\n$ ./todo del NUMBER       # Delete a todo\n$ ./todo done NUMBER      # Complete a todo\n$ ./todo help             # Show usage\n$ ./todo report           # Statistics";
void printHelpMessage()
{
    cout<<"Usage :-\n$ ./todo add \"todo item\"  # Add a new todo"<<endl<<"$ ./todo ls               # Show remaining todos"<<endl<<"$ ./todo del NUMBER       # Delete a todo"<<endl<<"$ ./todo done NUMBER      # Complete a todo"<<endl<<"$ ./todo help             # Show usage"<<endl<<"$ ./todo report           # Statistics";
}
struct TodoMessage
{
    string message;
    bool completed;
};

void getTodaysDate()
{
    time_t ttime = time(0);
    tm *local_time = localtime(&ttime);
    cout  << 1900 + local_time->tm_year << "-" << 1 + local_time->tm_mon << "-" << local_time->tm_mday <<" ";
}

int readFromeFile(string filename, struct TodoMessage myArray[])
{
    // Object to read from file
    ifstream file_obj;
    // Opening file in input mode
    file_obj.open(filename.c_str(), ios::in);

    // Reading from file into object "obj"
    if (!file_obj)
    {
        return 0;
    }

    int i = 0;
    while (file_obj.good())
    {
        // file_obj.read((char *)&myArray[i], sizeof(myArray[i]));
        // file_obj.ignore();
        // i++;

        string::size_type sz;
        if (!file_obj.read(reinterpret_cast<char *>(&sz), sizeof(string::size_type)))
            break;

        myArray[i].message.resize(sz);
        file_obj.read(&myArray[i].message[0], sz);
        file_obj.read(reinterpret_cast<char *>(&myArray[i].completed), sizeof(bool));
        //std::cout << myArray[i].message << " " << myArray[i].completed << std::endl;
        i++;
    }
    file_obj.close();

    return i;
}

void putIntoFile(string filename, struct TodoMessage myArray[], int curindex)
{

    // Object to write in file
    ofstream file_obj;
    // Opening file in append mode
    file_obj.open(filename.c_str(), ios::out);

    // Writing the object's data in file
    struct TodoMessage var1;
    for (int i = 0; i < curindex; i++)
    {
        //file_obj.write((char *)&myArray[i], sizeof(var1));
        // determine the size of the string
        string::size_type sz = myArray[i].message.size();
        file_obj.write(reinterpret_cast<char *>(&sz), sizeof(string::size_type));
        // and actual string
        file_obj.write(myArray[i].message.c_str(), sz);
        file_obj.write(reinterpret_cast<char *>(&myArray[i].completed), sizeof(bool));
    }
    file_obj.close();
}

int main(int argc, char *argv[])
{
    struct TodoMessage messages[100];
    int curindex = 0;
    string filename = "todo.txt";

    string command = "help";

    if (argc >= 2)
        command = argv[1];

        //cout<<command<<" "<<argc<<" ";

    if (command == "ls")
    {

        // read from file and populate messages myArray from file and populate curindex with myArray count
        curindex = readFromeFile(filename, messages);
        if(curindex == 0)
        {
            cout<<"There are no pending todos!";
            return 0;
        }
        for (int i = curindex - 1; i >= 0; i--)
        {
            //cout << "Message " << messages[i].message << " messages[i].completed " << messages[i].completed << " - ";
            if (messages[i].completed == false)
            {
                cout << "[" << i + 1 << "]"<<" "<<messages[i].message<<endl;
               
            }   
        }
    }
    else if (command == "add")
    {
        // read from file and populate messages myArray from file and populate curindex with myArray count
        curindex = readFromeFile(filename, messages);
        //cout << "curindex " << curindex << " \n ";
        if(argc <= 2){
            cout<<"Error: Missing todo string. Nothing added!";
            return 0;
        }
        messages[curindex].message = argv[2];
        messages[curindex].completed = false;
        curindex++;

        //cout << "Message array " << messages[curindex - 1].message << " " << messages[curindex - 1].completed << " ";
        //put messages myArray content into same file
        putIntoFile(filename, messages, curindex);
        cout << "Added todo: \"" <<argv[2]<<"\"";
    }
    else if (command == "done")
    {
        // read from file and populate messages myArray from file and populate curindex with myArray count
        curindex = readFromeFile(filename, messages);
        if(argc <= 2){
            cout<<"Error: Missing NUMBER for marking todo as done.";
            return 0;
        }
        int number = stoi(argv[2]);
        if(number <= 0 || number > curindex)
        {
            cout<<"Error: todo #"<<number<<" does not exist.";
            return 0;
        }
        
        messages[number - 1].completed = true;
        //put messages myArray content into same file
        putIntoFile(filename, messages, curindex);
        cout << "Marked todo #" << number << " as done.";
    }
    else if (command == "del")
    {
        // read from file and populate messages myArray from file and populate curindex with myArray count
        curindex = readFromeFile(filename, messages);
        if(argc <= 2){
            cout<<"Error: Missing NUMBER for deleting todo.";
            return 0;
        }
        int number = stoi(argv[2]);
        if(number <= 0 || number > curindex)
        {
            cout<<"Error: todo #"<<number<<" does not exist. Nothing deleted.";
            return 0;
        }
       
        for (int i = number - 1; i <= curindex - 2; i++)
        {
            messages[i].message = messages[i + 1].message;
            messages[i].completed = messages[i + 1].completed;
        }
        //put messages myArray content into same file
        putIntoFile(filename, messages, curindex - 1);
        cout << "Deleted todo #" << number;
    }
    else if (command == "help")
    {
        printHelpMessage();
    }
    else if (command == "report")
    {
        // read from file and populate messages myArray from file and populate curindex with myArray count
        curindex = readFromeFile(filename, messages);
        int completedCount = 0;
        int notCompletedCount = 0;
        for (int i = 0; i < curindex; i++)
        {
            if (messages[i].completed == true)
                completedCount++;
            else
                notCompletedCount++;
        }
        getTodaysDate();
        cout << "Pending : " << notCompletedCount << " Completed : " << completedCount;
    }
    else
    {
        cout << "Invalid command \n ";
    }

    return 0;
}
