#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include<algorithm>
#include <errno.h>
#include <vector>
using namespace std;
const std::string red("\033[0;31m");
const std::string reset("\033[0m");
const std::string yellow("\033[1;33m");

void ls(char**,int);
void cd(char**,int);
void cat(char**,int);
void mkdirectory(char**,int);
void cp(char**,int);
void sortt(char**,int);
void grep(char**,int);
char *current_directory;//stores current directory
char *default_directory;
int flag_actual_current_directory=1;

static int myCompare(const void* a, const void* b) 
{ 
    return strcmp(*(const char**)a, *(const char**)b); 
} 

void checkcases(char** arg_vv,int arg_count){
if(strcmp(arg_vv[0],"ls")==0) ls(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"cd")==0) cd(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"cat")==0) cat(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"mkdir")==0) mkdirectory(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"cp")==0) cp(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"sort")==0) sortt(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"grep")==0) grep(arg_vv,arg_count);
else if(strcmp(arg_vv[0],"exit")==0||strcmp(arg_vv[0],"EXIT")==0)exit(1);
else if(strcmp(arg_vv[0],"\0")!=0){cout<<""<<arg_vv[0]<<": command not found\n";return ;}//if unknown command exist( not all space or new line)
else {return ;}//newline or newline after space(s)
}


int main(){
  
//Printing username, hostname and current directory
char *name=getenv("USER");
char hostname[1024],to_store[50000];
gethostname(hostname,1023);
char* arg_vv[1024];//stores argument
int arg_count=0;
char default_dir[10000];//stores default directory 
getcwd(default_dir, sizeof(default_dir)); 
default_directory=default_dir;
do
{
    char yo[8000];
    if(flag_actual_current_directory==1) //dont show current directory name if it is default
     {
         strcpy(yo,name);
         strcat(yo,"@");
         strcat(yo,hostname);
         strcat(yo,":~$ ");
     }
    else
    {
         strcpy(yo,name);
         strcat(yo,"@");
         strcat(yo,hostname);
         strcat(yo,":~/");
         strcat(yo,current_directory);
         strcat(yo,"$ ");
    //cout<<yellow<<name<<"@"<<hostname<<":"<<red<<"~/"<<current_directory<<reset<<"$";
    }
    //read line
    char *line=readline(yo);
    if(strlen(line)!=0)
    add_history(line); 
    arg_count=0;//this number denote (0,1...arg_count-1) index in arg_vv array
    arg_vv[arg_count]=strsep(&line," ");
    while(strlen(arg_vv[arg_count++])!=0)
    {
        arg_vv[arg_count]=strsep(&line," ");;
        if(arg_vv[arg_count]==NULL)
        {
            break;
        } 
        if(strlen(arg_vv[arg_count])==0)arg_count--;  
    }//lines parsed in array
    checkcases(arg_vv,arg_count);

} while (1);
return 0;
}

void ls(char** arg_vv,int arg_count)
{
    if(arg_count==1)//i.e. no -a
    {
        char current[10000]; 
        getcwd(current, sizeof(current)); 
        struct dirent *d;
        DIR* p=opendir(current);
        char* store[10000];
        char *test;
        int count=0;
        while(d=readdir(p)){
        test=d->d_name;
        if(test[0]!='.')store[count++]=test;
        }
        qsort(store, count, sizeof(char*), myCompare); 
        for(int i=0;i<count;i++)
        cout<<store[i]<<" ";
        cout<<"\n";
    }
    else if(arg_count==2)
    {
       if(strcmp(arg_vv[1],"-a")!=0)
       {
           cout<<"ls: cannot access '"<<arg_vv[1]<<"': No such file or directory\n";
           return;
       }
        char current[10000]; 
        getcwd(current, sizeof(current)); 
        current_directory=current;
        struct dirent *d;
        DIR* p=opendir(current);
        char* store[10000];
        int count=0;
        while(d=readdir(p)){
        store[count++]=d->d_name;
        }
        qsort(store, count, sizeof(char*), myCompare); 
        for(int i=0;i<count;i++)
        cout<<store[i]<<" ";
        cout<<"\n";
    }
    else
    {
        for(int i=1;i<arg_count;i++)
        {
            if(strcmp(arg_vv[i],"-a")==0)continue;
            cout<<"ls: cannot access '"<<arg_vv[i]<<"': No such file or directory\n";
        }
    }
}

void cd(char** arg_vv,int arg_count)
{
  if(arg_count==1)
  {
      if(flag_actual_current_directory==1)return;
      else {
       if(chdir(default_directory)!=0)//not 0 means error,,sys call
        {
            cout<<"Error\n";return;
        }
      current_directory=default_directory; flag_actual_current_directory=1;}
  }
  else if(arg_count==2)
  {
      if(chdir(arg_vv[1])!=0)//not 0 means error,,sys call
      {
          cout<<"bash: cd: "<<arg_vv[1]<<": No such file or directory\n";
      }
      else {flag_actual_current_directory=0;current_directory=arg_vv[1];}
  }
  else
  {
     cout<<"bash: cd: too many arguments\n"; 
  }
}

void cat(char** arg_vv,int arg_count)
{
    ifstream file;
    if(arg_count==1||(arg_count==2 && strcmp(arg_vv[1],"-n")==0))//if cat or only cat -n
    {
     int line_count=1;
        bool flag_to_number=false;
    }
    else 
    {
        int line_count=1;
        bool flag_to_number=false;
        for(int i=1;i<arg_count;i++){
            if(strcmp(arg_vv[i],"-n")==0)//check if to be labelled or not
            {
                flag_to_number=true;break;
            }
        }
        for(int i=1;i<arg_count;i++){
            if(strcmp(arg_vv[i],"-n")==0)//ignore
            {
                continue;
            }
        //check if file is directory to output it is a directory
            DIR* directory = opendir(arg_vv[i]);

            if(directory != NULL)
            {
            closedir(directory);
            cout<<"cat: "<<arg_vv[i]<<": Is a directory\n";
            }

          //open next argument 
            else{
                file.open(arg_vv[i]);
                string c,lost;
                if(file.is_open()!=true)
                {
                    cout<<"cat: "<<arg_vv[1]<<": No such file or directory\n";return;
                }
                if(flag_to_number==false){
                    // Execute a loop until EOF (End of File) 
                    while (file) { 
                        getline(file, c); 
                        cout << c<< endl; 
                    } 
                }
                else{
                    // Execute a loop until EOF (End of File) and add line number
                    while (file) 
                    { 
                        if(file.peek()==EOF)break;
                        getline(file, c); 
                        cout <<line_count++<<" "<< c<< endl; 
                    } 
                }
                file.close();
            }
        } 
    }
}
void mkdirectory(char** arg_vv,int arg_count)
{
    if(arg_count==1)
    {
        cout<<"mkdir: missing operand\n";return;
    }
    else if(strcmp(arg_vv[1],"-m777")==0)
        {
            int x=mkdir(arg_vv[2],S_IRWXU|S_IRWXG|S_IROTH|S_IWOTH|S_IXOTH);
            if(x==0)return;
            cout<<"mkdir: cannot create directory ‘"<<arg_vv[2]<<"’: "<<strerror(errno)<<endl;
        }
    else if((strcmp(arg_vv[1],"-m")==0))
    {
        if(arg_count==2){
            cout<<"mkdir: option requires an argument -- 'm'\n";
            return;
        }
        if(strcmp(arg_vv[2],"g=rw")==0){
            int x=mkdir(arg_vv[3],S_IRGRP|S_IWGRP);
            if(x==0)return;
        cout<<"mkdir: cannot create directory ‘"<<arg_vv[3]<<"’: "<<strerror(errno)<<endl;
      
        }
        else if(strcmp(arg_vv[2],"g=rx")==0){
            int x=mkdir(arg_vv[3],S_IRGRP|S_IXGRP);
            if(x==0)return;
        cout<<"mkdir: cannot create directory ‘"<<arg_vv[3]<<"’: "<<strerror(errno)<<endl;
      
        }
        else if(strcmp(arg_vv[2],"g=wx")==0){
            int x=mkdir(arg_vv[3],S_IWGRP|S_IXGRP);
            if(x==0)return;
        cout<<"mkdir: cannot create directory ‘"<<arg_vv[3]<<"’: "<<strerror(errno)<<endl;
      
        }
    
        else if(strcmp(arg_vv[2],"o=rx")==0){
            int x=mkdir(arg_vv[3],S_IROTH|S_IXOTH);
            if(x==0)return;
        cout<<"mkdir: cannot create directory ‘"<<arg_vv[3]<<"’: "<<strerror(errno)<<endl;
      
        }
        
        else if(strcmp(arg_vv[2],"a=rx")==0){
            int x=mkdir(arg_vv[3],S_IRUSR|S_IXUSR|S_IXGRP|S_IRGRP|S_IROTH|S_IXOTH);
            if(x==0)return;
        cout<<"mkdir: cannot create directory ‘"<<arg_vv[3]<<"’: "<<strerror(errno)<<endl;
      
        }
        else
        {
            cout<<"mkdir: missing operand\n";
            return;
        }
        
    }
    else{ 
            int x=mkdir(arg_vv[1],0777);
            if(x==0)return;
        cout<<"mkdir: cannot create directory ‘"<<arg_vv[2]<<"’: "<<strerror(errno)<<endl;
    }
    
}

static time_t getFileModifiedTime(const char *path)
{
    struct stat attr;
    if (stat(path, &attr) == 0)
    {
        //printf("%s: last modified time: %s", path, ctime(&attr.st_mtime));
        return attr.st_mtime;
    }
    return 0;
}
void cp(char** arg_vv,int arg_count)
{         
    char* source,*dest;
    ifstream in_file;
    ofstream out_file;
    if(arg_count==3)//no -u
    {
            source=arg_vv[1];
            dest=arg_vv[2];
            //check if file is directory to output it is a directory
            for(int i=1;i<3;i++){
            if(strcmp(arg_vv[i],"-u")==0)
                {
                   cout<<"cat: Not a valid format of cp"<<endl;
                   return;
                }
            }
            for(int i=1;i<3;i++){
                DIR* directory = opendir(arg_vv[i]);
                if(directory != NULL)
                {
                closedir(directory);
                cout<<"cp: "<<arg_vv[i]<<": Is a directory\n";
                return;
                }
            }
          in_file.open(source);
          out_file.open(dest);
                string c;
                if(in_file.is_open()!=true)
                {
                    cout<<"cp: '"<<source<<"': No such file \n";return;
                }
                 if(out_file.is_open()!=true)
                {
                    cout<<"cp: '"<<dest<<"': No such file \n";return;
                }
                
                    // Execute a loop until EOF (End of File) and add line number
                    while (in_file) 
                    { 
                        if(in_file.peek()==EOF)break;
                        getline(in_file,c);
                         c=c+"\n";
                        out_file<<c; 
                    } 
                out_file.close();
                in_file.close();  
    }
    else if(arg_count==4)
    {
              source=arg_vv[2];
                dest=arg_vv[3];
            

            if(strcmp(arg_vv[1],"-u")!=0)//if there is no -u
            {
                 cout<<"Not a valid format of cp\n";return;
            }


            //check if any of source or dest is directory or not
            
                DIR* directory = opendir(source);
                if(directory != NULL)
                {
                closedir(directory);
                cout<<"cp: "<<source<<": Is a directory\n";
                return;
                }
                directory = opendir(dest);
                if(directory != NULL)
                {
                closedir(directory);
                cout<<"cp: "<<dest<<": Is a directory\n";
                return;
                }

               
                string c;
               
                time_t t1 = getFileModifiedTime(source);
                time_t t2 = getFileModifiedTime(dest);
                in_file.open(dest);
                if((in_file.is_open()==true)&&(t1<t2))
                {
                    cout<<"Can't copy: source file is older than the destination file\n";
                    in_file.close();  
                    return;
                }
                else{
                    //cout<<source<<" "<<dest<<endl;
                    in_file.close();
                 in_file.open(source);
                 out_file.open(dest);
                if(in_file.is_open()!=true)
                {
                    cout<<"cp: '"<<source<<"': No such file \n";return;
                }
                 if(out_file.is_open()!=true)
                {
                    cout<<"cp: '"<<dest<<"': No such file \n";return;
                }
                    // Execute a loop until EOF (End of File) and add line number
                    while (in_file) 
                    { 
                        if(in_file.peek()==EOF)break;
                        getline(in_file,c);
                          c=c+"\n";
                          out_file<<c; 
                          
                    } 
                out_file.close();
                in_file.close();               
            }      
    }
    else
    {
        cout<<"Not a valid format of cp\n";
    }
}

bool comparisonFunc(const string c1, const string c2)
{
    if(c1<c2)return true;
    else return false;
}
bool comparisonFuncRev(const string c1, const string c2)
{
    if(c1>c2)return true;
    else return false;
}

void sortt(char** arg_vv, int arg_count)
{
    vector<string> lines;
    if(arg_count!=2&&arg_count!=3){cout<<"Not a valid format\n";return;}
    else if(arg_count==2)
    {
        ifstream file;
        file.open(arg_vv[1]);
        if(file.is_open()==false){cout<<"sort: '"<<arg_vv[1]<<"' file doesnt exist.\n";return;}
        string c;
        while (file) 
        { 
            if(file.peek()==EOF)break;
            getline(file,c);
            lines.push_back(c);
        } 
                        sort(lines.begin(),lines.end(),comparisonFunc);
                        for(vector<string>::iterator i = lines.begin(); i != lines.end(); ++i)
                        {
                            cout << *i << endl; // display each word on its own line
                        }
        file.close();
    }
    else
    {
        if((strcmp(arg_vv[1],"-r")!=0)&&(strcmp(arg_vv[2],"-r")!=0))
        {
            cout<<"Not a valid format\n";return;
        }
        ifstream file;
        file.open(arg_vv[2]);
        if(file.is_open()==false){cout<<"sort: '"<<arg_vv[2]<<"' file doesnt exist.\n";return;}
        string c;
        while (file) 
        { 
            if(file.peek()==EOF)break;
            getline(file,c);
            lines.push_back(c);
        } 
                        sort(lines.begin(),lines.end(),comparisonFuncRev);
                        for(vector<string>::iterator i = lines.begin(); i != lines.end(); ++i)
                        {
                            cout << *i << endl; // display each word on its own line
                        }
        file.close();
    }
    
}

void grep(char** arg_vv, int arg_count)
{
    int n_present=0;
    for(int i=1;i<arg_count;i++){
        if(strcmp(arg_vv[i],"-n")==0)
        {
            n_present=1;
            break;
        }
    }
    if(n_present==0)//-n not present
    {
        //if there exist "" to cover pattern, remove it
        if(arg_vv[1][0]=='\"'){
            int z=0;
            while(arg_vv[1][z+1]!='\"'){
                arg_vv[1][z]=arg_vv[1][z+1];
                z++;
            }
            arg_vv[1][z]='\0';
        }
        char* pattern=arg_vv[1];
        //for each file
        if(arg_count==3)//if only 1 file, dont show file name in output
        {
            ifstream file;
            file.open(arg_vv[2]);
            if(file.is_open()==false){cout<<"grep: '"<<arg_vv[2]<<"' file doesnt exist.\n";return;}
            string c;
            string spattern(pattern);
            while (file) 
            { 
                if(file.peek()==EOF)break;
                getline(file,c);
                size_t found = c.find(spattern); 
                if (found != string::npos) cout<<c<<endl;
            } 
            file.close();
        }
        else
        {//if more than 1 file, show file name in output
            for(int i=2;i<arg_count;i++)
            {
                ifstream file;
                file.open(arg_vv[i]);
                if(file.is_open()==false){cout<<"grep: '"<<arg_vv[i]<<"' file doesnt exist.\n";return;}
                string c;
                string spattern(pattern);
                while (file) 
                { 
                    if(file.peek()==EOF)break;
                    getline(file,c);
                    size_t found = c.find(spattern); 
                    if (found != string::npos) cout<<red<<arg_vv[i]<<yellow<<":"<<reset<<c<<endl;
                } 
                file.close();
            }
        }
    }
    else//-n present
    {
        //if there exist "" to cover pattern, remove it
        if(arg_vv[2][0]=='\"'){
            int z=0;
            while(arg_vv[2][z+1]!='\"'){
                arg_vv[2][z]=arg_vv[2][z+1];
                z++;
            }
            arg_vv[2][z]='\0';
        }
        char* pattern=arg_vv[2];
        //for each file
        
        if(arg_count==4)//if only 1 file, dont show file name in output
        {
                ifstream file;
                int lcount=1;
                file.open(arg_vv[3]);
                if(file.is_open()==false){cout<<"grep: '"<<arg_vv[3]<<"' file doesnt exist.\n";return;}
                string c;
                string spattern(pattern);
                while (file) 
                { 
                    if(file.peek()==EOF)break;
                    getline(file,c);
                    size_t found = c.find(spattern); 
                    if (found != string::npos) cout<<yellow<<lcount<<":"<<reset<<c<<endl;
                    lcount++;
                } 
                file.close();
        }
        else
        {//if more than 1 file, show file name in output
            for(int i=3;i<arg_count;i++)
            {
                ifstream file;
                file.open(arg_vv[i]);
                if(file.is_open()==false){cout<<"grep: '"<<arg_vv[i]<<"' file doesnt exist.\n";return;}
                string c;
                string spattern(pattern);
                int lcount=1;
                while (file) 
                { 
                    if(file.peek()==EOF)break;
                    getline(file,c);
                    size_t found = c.find(spattern); 
                    if (found != string::npos) cout<<red<<arg_vv[i]<<yellow<<":"<<lcount<<":"<<reset<<c<<endl;
                    lcount++;
                } 
                file.close();
            }
        }
    }
    
}