#include "unistd.h"
#include "stdio.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

int modifiString(std::string &str) {
    int fd[2];
    //std::cout << s;
    pipe(fd);
    int id = fork();
    if (id == -1)
    {
        perror("fork error");
        return -1;
    }
    if (id == 0)
        std::cout << id << std::endl;
    return 0;
}

std::string deliteVowel(std::string &str) {
    std::string s = "";
    for (auto c : str)
    {
        auto isLowercaseVowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');

        // evaluates to 1 (true) if c is an uppercase vowel
        auto isUppercaseVowel = (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
        if ((isalpha(c) && (isLowercaseVowel || isUppercaseVowel)) || c == '\n')
            s += c;
    }
    return s;
}

int main()
{
    std::ifstream file("Read.txt");
    int fd1[2];
    int fd2[2];
    //fd[0] -read
    //fd[1] -write
    int p1 = pipe(fd1);
    int p2 = pipe(fd2);
    if(p1 == -1 || p2 == -1)
        return -1;
    int id = fork();
    if (id != 0) //parent
    {
        int id2 = fork();

        if (id2 != 0) //parent
        {
            std::string str;
            close(fd1[0]);
            close(fd2[0]);
            std::string smal;
            std::string big;
            std::ifstream in("Read.txt");
            while (getline(in, str, '\n'))
            {
                if (str.size() > 10)
                {
                    write(fd1[1], str.c_str(), str.length() + 1);
                }
                else
                {
                    write(fd2[1], str.c_str(), str.length() + 1);
                }
            }
            close(fd1[1]);
            close(fd2[1]);
        }

        if (id2 == 0 ) // child
        {
            close(fd1[1]);
            close(fd2[1]);
            std::string str;
            int n;
            //read(fd1[0], &n, sizeof(int));
            char ch;
            while (read(fd1[0], &ch, 1) > 0)
            {
                if (ch != 0)
                    str.push_back(ch);
                else
                    str += '\n';
            }
            //read(fd1[0], &str, n);
            //std::cout << str << std::endl;
            std::cout << deliteVowel(str) << std::endl;
            fflush(stdout);
            close(fd1[0]);
            close(fd2[0]);
        }
    }
    else if (id == 0) //child
    {
        close(fd1[1]);
        close(fd2[1]);
        std::string str;
        int n;
        //read(fd2[0], &n, sizeof(int));
        //read(fd2[0], &str, n);
        char ch;
        while (read(fd2[0], &ch, 1) > 0)
        {
            if (ch != 0)
                str.push_back(ch);
            else
                str += '\n';
        }
        //std::cout << str << "string" << std::endl;
        std::cout << deliteVowel(str) << std::endl;
        fflush(stdout);
        close(fd1[0]);
        close(fd2[0]);

    }
    return 0;
}
