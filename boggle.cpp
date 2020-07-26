#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<algorithm>
#include<thread>
#include<chrono>
#include<mutex>
#include<curses.h>
#include<condition_variable>
#define SIZE 26
#define Bsize 4
using namespace std;

struct tree
{
    tree *child[SIZE];
    bool leaf;
};

tree * get_node()
{
    tree *new_node=new tree;
    new_node->leaf=false;
    for(int i=0;i<SIZE;i++)
        new_node->child[i]=NULL;
    return new_node;
}

tree *root=get_node();
vector<string> ans;

void insert_into(string s)
{
    tree *Nchild=root;
    for(int i=0;i<s.size();i++)
    {
        int index=s[i]-'a';
        if(Nchild->child[index]==NULL)
            Nchild->child[index]=get_node();
        Nchild=Nchild->child[index];
    }
    Nchild->leaf=true;
}

bool is_valid(int i, int j, bool reach[Bsize][Bsize])
{
    if(i>=0 && i<Bsize && j>=0 && j<Bsize && reach[i][j]==false)
        return true;
    else
        return false;
}

void search_word(tree *node, char boggle[Bsize][Bsize], int i, int j, bool reach[][Bsize],string str)
{
    if(node->leaf==true)
        ans.push_back(str);
    if(is_valid(i,j,reach))
    {
        reach[i][j]=true;
        for(int k=0;k<SIZE;k++)
        {
            if(node->child[k]!=NULL)
            {
                char ch=(char)k+(char)'a';
                if (is_valid(i+1,j+1,reach) && boggle[i+1][j+1] == ch)
                    search_word(node->child[k],boggle,i+1,j+1,reach,str+ch);
                if (is_valid(i, j+1,reach)  && boggle[i][j+1] == ch)
                    search_word(node->child[k],boggle,i, j+1,reach,str+ch);
                if (is_valid(i-1,j+1,reach) && boggle[i-1][j+1] == ch)
                    search_word(node->child[k],boggle,i-1, j+1,reach,str+ch);
                if (is_valid(i+1,j, reach)  && boggle[i+1][j] == ch)
                    search_word(node->child[k],boggle,i+1, j,reach,str+ch);
                if (is_valid(i+1,j-1,reach) && boggle[i+1][j-1] == ch)
                    search_word(node->child[k],boggle,i+1, j-1,reach,str+ch);
                if (is_valid(i, j-1,reach)&& boggle[i][j-1] == ch)
                    search_word(node->child[k],boggle,i,j-1,reach,str+ch);
                if (is_valid(i-1,j-1,reach) && boggle[i-1][j-1] == ch)
                    search_word(node->child[k],boggle,i-1, j-1,reach,str+ch);
                if (is_valid(i-1, j,reach) && boggle[i-1][j] == ch)
                    search_word(node->child[k],boggle,i-1, j, reach,str+ch);
            }
        }
        reach[i][j]=false;
    }
}

void find_word(char boggle[Bsize][Bsize])
{
    bool reach[Bsize][Bsize]={false};
    string str="";
    for(int i=0;i<Bsize;i++)
        for(int j=0;j<Bsize;j++)
        {
            if(root->child[boggle[i][j]-'a'])
            {
                str=boggle[i][j];
                search_word(root->child[boggle[i][j]-'a'],boggle,i,j,reach,str);
                str="";
            }
        }
}

vector< vector<string> > order(5);
char boggle[Bsize][Bsize] = {};
condition_variable cv;
string value;

void read_value()
{
    cout<<"Enter the word you found: ";
    getline(cin,value);
    cv.notify_one();
}

void print_grid()
{
    cout<<endl;
    for(int i=0;i<Bsize;i++)
    {
        for(int j=0;j<Bsize;j++)
            cout<<" | "<<boggle[i][j];
        cout<<" | "<<endl;
    }
}

void print_ans()
{
    for(int i=0;i<5;i++)
    {
        cout<<"\n"<<i+3<<" letter words are: "<<endl;
        for(int j=0;j<order[i].size();j++)
            cout<<order[i][j]<<"  ";
    }
    cout<<endl;
}

bool enter()
{
    thread th(read_value);
    mutex mtx;
    unique_lock<mutex> lck(mtx);
    if(cv.wait_for(lck, chrono::seconds(10)) == cv_status::timeout)
    {
        system("reset");
        cout<<"\nYou have been timed out(10 seconds). Please enter to continue\n";
        th.join();
        return false;
    }
    else
    {
        th.join();
        system("reset");
        cout<<"You entered: "<<value;
        return true;
    }
}

int main()
{
    srand((unsigned)time(0));
    fstream f;
    string s,vowel="aaaeeeiiiooouu";
    int list_size, find_counter=0;
    f.open("wordlist_new.txt",ios::in);
    while(f)
    {
        f>>s;
        insert_into(s);
    }
    for(int i=0;i<Bsize;i++)
        for(int j=0;j<Bsize;j++)
        {
            int x=rand()%40;
            if (x<26)
                boggle[i][j]=x+'a';
            else
                boggle[i][j]=vowel[x-26];
        }
    find_word(boggle);
    vector<string> found;
    sort(ans.begin(),ans.end());
    auto it=unique(ans.begin(),ans.end());
    ans.resize(distance(ans.begin(), it));
    list_size=ans.size();
    for(int i=0;i<ans.size();i++)
        order[ans[i].size()-3].push_back(ans[i]);
    int s1=0, s2=0;
    bool f1=false, f2=false, f3=false;
    while(true)
    {
        if(find_counter==list_size)
            f1=f2=f3=true;
        if(f1==true and f2==true)
        {
            system("reset");
            if(f3==true)
                cout<<"All the words have been found by players! "<<endl;
            cout<<"\nGAME OVER"<<endl;
            if(s1>s2)
                cout<<"\nPlayer 1 is the winner with "<<s1<<" points beating Player 2 by "<<s2<<" point(s)";
            else if(s2>s1)
                cout<<"\nPlayer 2 is the winner with "<<s2<<" points beating Player 1 by "<<s1<<" point(s)";
            else
                cout<<"\nDraw between both players and score is "<<s1;
            cout<<"\nThe answers for the game were: ";
            print_ans();
            exit(0);
        }
        if(f1==false)
        {
            system("reset");
            print_grid();
            //print_ans();
            cout<<"\nThe scores are Player 1: "<<s1<<" and Player 2: "<<s2;
            cout<<"\nPlayer 1, it's your chance: (Enter # to forfeit)\n";
            if(enter()==true)
            {
                if(value=="#")
                {
                    cout<<"\nPlayer 1, you have chosen to forfeit at this point; \nYou will no longer be given chances after this point. Enter 'y' to confirm: ";
                    string z;
                    getline(cin,z);
                    if(z=="y")
                    {
                        cout<<"Confirmed";
                        f1=true;
                    }
                    else
                        cout<<"Not confirmed please continue ";
                }
                else
                {
                    if(find(found.begin(),found.end(),value)!=found.end())
                        cout<<"\nThis word has already been found by you/ your opponent";
                    else
                    {
                        int x=value.size();
                        if(x<3 || x>7)
                            cout<<"\nThis game is being played with words of word-count in range 3-7";
                        else
                        {
                            auto it=find(order[x-3].begin(),order[x-3].end(),value);
                            if(it==order[x-3].end())
                                cout<<"\nThis word is not a valid word/ is not in this game's dictionary";
                            else
                            {
                                found.push_back(value);
                                cout<<"\nGood Job! You have gained "<<x-2<<" point(s)";
                                s1+=(x-2);
                                find_counter++;
                            }
                        }
                    }
                }
                getch();
            }
        }
        if(find_counter==list_size)
            continue;
        if(f2==false)
        {
            system("reset");
            print_grid();
            //print_ans();
            cout<<"\nThe scores are Player 1: "<<s1<<" and Player 2: "<<s2;
            cout<<"\nPlayer 2, it's your chance: (Enter # to forfeit)\n";
            if(enter()==true)
            {
                if(value=="#")
                {
                    cout<<"\nPlayer 2, you have chosen to forfeit at this point; \nYou will no longer be given chances after this point. Enter 'y' to confirm: ";
                    string z;
                    getline(cin,z);
                    if(z=="y")
                    {
                        cout<<"Confirmed";
                        f2=true;
                    }
                    else
                        cout<<"Not confirmed please continue ";
                }
                else
                {
                    if(find(found.begin(),found.end(),value)!=found.end())
                        cout<<"\nThis word has already been found by you/ your opponent";
                    else
                    {
                        int x=value.size();
                        if(x<3 || x>7)
                            cout<<"\nThis game is being played with words of word-count in range 3-7";
                        else
                        {
                            auto it=find(order[x-3].begin(),order[x-3].end(),value);
                            if(it==order[x-3].end())
                                cout<<"\nThis word is not a valid word/ is not in this game's dictionary";
                            else
                            {
                                found.push_back(value);
                                cout<<"\nGood Job! You have gained "<<x-2<<" point(s)";
                                s2+=(x-2);
                                find_counter++;
                            }
                        }
                    }
                }
                getch();
            }
        }
    }
    return 0;
}
