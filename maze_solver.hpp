#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#define MAX_VERT 1000
#define MAX_HOR  1000

class maze_solver {
    private:
        void pass_through();
        void open_file(std::string file_name);
        void get_content();
        void fill_array();
        void copy_array();
        void start_solving();
        void check_case(int i,int j);
        void get_hor_ver_size();
        std::string file_content;
        char maze_array[MAX_HOR][MAX_VERT], maze_array_copy[MAX_HOR][MAX_VERT];
        char start,end,wall,path;
        int size_ver,size_hor;
        std::ifstream infile;
        bool find_something_to_change;
    public:
        maze_solver(char _start,char _end,char _wall,char _path);
        ~maze_solver();
        void solve(std::string file_name);
        void print();
}; 

maze_solver::maze_solver(char _start,char _end, char _wall,char _path)
{
    file_content = "";
    start        = _start;
    end          = _end;
    wall         = _wall;
    path         = _path;
    size_hor     = size_ver = 0;
    find_something_to_change = true;
}

maze_solver::~maze_solver()
{
}

void maze_solver::fill_array() 
{
    int ite = 0, ite2 = 0;
    for (int i=0;i<file_content.size();i++) {
        if(file_content[i] !='\n') {
            maze_array[ite][ite2] = maze_array_copy[ite][ite2] =file_content[i]; 
            std::cout<<"\e[0;0m"<<maze_array[ite][ite2];
            ite++;
        }
        else {
            std::cout<<"\n";
            ite = 0;
            ite2 ++;
        }
    }
}

void maze_solver::check_case(int i,int j)
{
    if ( 
        (maze_array_copy[i][j]!=wall &&
        maze_array_copy[i][j]!=start&&
        maze_array_copy[i][j]!=end  &&
        i!=size_hor && i!= 0 && /*not taking the limit walls*/
        j!=size_ver && j!= 0 )&&
            (
            /* XXX  XXXX X X  XXXX
             * X X  X    X X     X
             * X X  XXXX XXX  XXXX
             */
            ( maze_array_copy[i][j-1]==wall && 
                maze_array_copy[i-1][j]==wall && 
                maze_array_copy[i+1][j]==wall ) 
            ||
            ( maze_array_copy[i-1][j]==wall &&
                maze_array_copy[i][j-1]==wall&&
                maze_array_copy[i][j+1]==wall)
            ||
            ( maze_array_copy[i][j+1]==wall&&
                maze_array_copy[i-1][j]==wall&&
                maze_array_copy[i+1][j]==wall)
            ||
            ( maze_array_copy[i+1][j]==wall&&
                maze_array_copy[i][j-1]==wall&&
                maze_array_copy[i][j+1]==wall)
            )
       ){
        maze_array_copy[i][j] = wall;
        find_something_to_change = true;
    }
}

void maze_solver::start_solving()
{
    while(find_something_to_change) {
        find_something_to_change=false;
        for (int i=0;i<size_ver;i++) {
            for (int j=0;j<size_hor;j++)  check_case(j,i);
            for (int j=size_hor;j>=0;j--) check_case(j,i);
        }
        for (int i=size_ver;i>=0;i--) {
            for (int j=0;j<size_hor;j++)  check_case(j,i);
            for (int j=size_hor;j>=0;j--) check_case(j,i);
        }    
    }

    for (int i=0;i<size_hor;i++)
        for (int j=0;j<size_ver;j++)
            if ( maze_array_copy[i][j] ==' ' ) maze_array[i][j] = path;
}

void maze_solver::get_content()
{        
    char temp[1000];
    while(!infile.eof()) {
        infile.getline(temp ,1000);
        file_content+=temp;
        file_content+="\n";
    }
}

void maze_solver::get_hor_ver_size()
{
    int temp = size_ver = size_hor = 0;
    size_hor = file_content.find("\n",0);
    if (size_hor == std::string::npos) {
        std::cerr << "[-] No horizontal size!\n";
        exit(1);
    } 
    while (1) {
        temp = file_content.find("\n",temp);
        if (temp == std::string::npos) break;
        temp++;
        size_ver ++;
    }
    if (size_ver<5) {
        std::cerr << "[-] The maze is too small!\n";
        exit(1);
    }
}

void maze_solver::open_file(std::string file_name)
{    
    infile.open(file_name.c_str(),std::ios::in);
    if (infile.fail()) {
        std::cerr << "[-] Could not open file!\n";
        exit(1);
    }    
    if (file_content!="") file_content = "";
}

void maze_solver::solve(std::string file_name)
{
    open_file(file_name);
    get_content();
    infile.close();
    get_hor_ver_size();
    fill_array();
    start_solving();    
}

void maze_solver::print()
{
    if (maze_array) {
        for (int i =0;i<size_ver;i++) {
            for(int j=0;j<size_hor;j++){
                if(maze_array[j][i]==wall) std::cout<<"\e[0;0m";
                else if(maze_array[j][i]==start
                        ||maze_array[j][i]==end)std::cout<<"\e[0;32m";
                else if(maze_array[j][i]==path) std::cout<<"\e[1;36m";
                std::cout<<maze_array[j][i];
            }
        std::cout<<"\n";
        }
    }
}
