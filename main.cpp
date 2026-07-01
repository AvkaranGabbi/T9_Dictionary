#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<termios.h>
#include<unistd.h>
using namespace std;


void setRawMode(bool enable) {
    static struct termios oldt;
    if (enable) {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

class Node
{
    public:
        char name[20];
        Node *next;
};

int checkname(Node *start, int len, string srch_word)
{
    system("clear");
    cout<<"Your Word: "<<srch_word<<endl<<endl;

    int flag=1;
    int count=0;
    cout<<endl;
    while(start!=NULL)
    {
        flag=1;
        for(int i=0; i<len ; i++)
        {
            if(start->name[i] == srch_word[i])
            {
                continue;
            }
            else
            {
                flag=0;
                break;
            }
        }
        if(flag==1)
        {
            count++;
            cout<<start->name;
            cout<<endl;
        }
        start = start->next;
    }
    if(count==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int main()
{
    cout<<endl<<"Press Enter to store the word."<<endl;

    int count=0;

    ifstream file("words.txt");
    string word, srch;

    Node *start, *first;
    start = new Node();
    first = start;

    while(getline(file, word))
    {
        strncpy(start->name, word.c_str(), 19);
        start->name[19] = '\0';

        if(file.peek() != EOF)
        {
            start->next = new Node();
            start = start->next;
        }
        else
        {
            start->next = NULL;
        }
    }

    start = first;

    cout << endl << endl << "Enter your word: ";
    cout.flush();

    string srch_word = "";
    setRawMode(true);

    while (true) 
    {
        char ch = getchar();

        if (ch == '\n' || ch == '\r')
        {
            break;
        } 
        else if (ch == 127 || ch == '\b') 
        {
            if (!srch_word.empty()) 
            {
                srch_word.pop_back();
                cout << "\b \b";               
                cout.flush();
            }
        } 
        else 
        {
            srch_word += ch;
            cout << ch;                       
            cout.flush();
        }
        count = checkname(start, srch_word.length(), srch_word);
    }

    setRawMode(false);
    cout << endl << "Final word: " << srch_word << endl;

    char choice;

    if(count == 0)
    {
        cout<<endl<<"Do you want to save this word? (Y/N)"<<endl;
        cin>>choice;

        if(choice == 'Y' || choice == 'y')
        {
            Node *newNode = new Node();

            strncpy(newNode->name, srch_word.c_str(), 19);
            newNode->name[19] = '\0';
            newNode->next = NULL;

            if(strcmp(srch_word.c_str(), first->name) < 0)
            {
                newNode->next = first;
                first = newNode;
            }
            else
            {
                Node *curr = first;

                while(curr->next != NULL &&
                    strcmp(curr->next->name, srch_word.c_str()) < 0)
                {
                    curr = curr->next;
                }

                newNode->next = curr->next;
                curr->next = newNode;
            }   
        }
    }

    ofstream outFile("words.txt", ios::out);

    Node *temp = first;

    while(temp != NULL)
    {
        outFile << temp->name << endl;
        temp = temp->next;
    }

    outFile.close();

    return 0;
}
