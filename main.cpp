#include <iostream>
#include "BigInt.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class FileLogger
{
    public:
        FileLogger(const std::string& filename) 
        {
            myfile.open(filename);
        }

        void write(const std::string& message) 
        {
            myfile << message << std::endl;
        }

        ~FileLogger() // Make sure that the file is closed
        {
            if (myfile.is_open()) 
            {
                myfile.close();
            }
        }

    private:
        std::ofstream myfile;
};

std::string generate_number(int x) // Generate a random number with x digits.
{
    // std::cout << "generate number function receiving " << x << std::endl;
    std::string result = "";
    int random;
    for(int i = 0; i < x; i++)
    {
        if(i == 0)
        {
            random = rand() % (9) + 1;
        }
        else
        {
            random = rand() % (10);
        }
        result += std::to_string(random);
    }
    // std::cout << "generate number function returning " << result << std::endl;
    return result;
}

int get_digital_root(std::string num) // Gets the digital root of the number.
{
    if (num.compare("0") == 0)
        return 0;

    int ans = 0;
    for (int i=0; i<num.length(); i++)
        ans = (ans + num[i]-'0') % 9;

    return (ans == 0)? 9 : ans % 9;
}

bool alternating(std::string num) // Check if the number's digits alternate between ascending and descending order.
{
    int previous_digit;
    int new_digit;
    bool ascending_previous = false;
    bool ascending_new = false;
    for(int i=0; i<num.length(); i++)
    {
        new_digit = num[i]-'0';
        if(i == 0)
        {
            previous_digit = num[i]-'0';
            continue;
        }
        else
        {
            if(new_digit > previous_digit)
            {
                if(i % 2 == 0)
                {
                    ascending_new = true;
                }
                else
                {
                    ascending_previous = true;
                }
            }
            else
            {
                if(i % 2 == 0)
                {
                    ascending_new = false;
                }
                else
                {
                    ascending_previous = false;
                }
            }

            previous_digit = num[i]-'0';
            if(i > 1 && ascending_new == ascending_previous)
            {
                return false;
            }
        }
    }

    return true;
}

int display_options()
{
    std::cout << "============================================" << std::endl;
    std::cout << "1 to generate numbers whose digits alternate between ascending and descending order, with digital root 7." << std::endl;
    std::cout << "2 to check for s_numbers from 1 to x." << std::endl;
    std::cout << "3 to clear the logs." << std::endl;
    std::cout << "4 to remove duplicates from the logs." << std::endl;
    std::cout << "5 to methodically generate numbers whose digits alternate between ascending and descending order, without checking for digital roots." << std::endl;
    std::cout << "============================================" << std::endl << std::endl;
    std::cout << "Your command: ";
    return 0;
}

int clearlogs()
{
    std::ofstream ofs("numbers.txt", std::ios::trunc);
    ofs.close();
    return 0;
}

int remove_duplicates()
{
    std::ifstream ifs("numbers.txt");
    if (!ifs) 
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(ifs, line)) 
    {
        auto it = std::find(lines.begin(), lines.end(), line);
        if (it == lines.end()) 
        {
            lines.push_back(line);
        }
    }

    ifs.close();

    std::ofstream ofs("myfile.txt", std::ios::trunc);
    if (!ofs) 
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    for (const auto& l : lines)
    {
        ofs << l << '\n';
    }

    ofs.close();

    return 0;
}

int s_algorithm()
{
    int digits;
    std::cout << std::endl << std::endl << "Digits per number: ";
    std::cin >> digits;
    FileLogger logger = FileLogger("numbers.txt");
    std::string number;
    std::string new_number;
    int previous_number;
    bool ascending;
    double duration;
    std::clock_t start;
    std::clock_t end;
    bool finished = false;
    
    while(finished == false)
    {
        for(int i = 0; i < digits+1; i++)
        {
            duration = (end - start) / (double)CLOCKS_PER_SEC;
            std::system("clear");
            std::cout << "Generating " << i << "/" << digits << std::endl;
            std::cout << duration*(digits-i) << " seconds remaining" << std::endl << std::endl;
            start = std::clock();
            if(i == 0)
            {
                new_number = generate_number(1);
                while(new_number == "0")
                {
                    new_number = generate_number(1);
                }
                number = new_number;
            }
            else
            {
                if(i == 1)
                {
                    new_number = generate_number(1);
                    previous_number = number[number.length()-1]-'0';
                    while(std::stoi(new_number) == previous_number)
                    {
                        new_number = generate_number(1);
                    }
                    if(std::stoi(new_number) > previous_number)
                    {
                        ascending = true;
                    }
                    else                                                                                                                            
                    {
                        ascending = false;
                    }
                    number += new_number;
                }
                else
                {
                    new_number = generate_number(1);
                    previous_number = number[number.length()-1]-'0';
                    if(ascending == true)
                    {
                        while(std::stoi(new_number) >= previous_number)
                        {
                            new_number = generate_number(1);
                        }
                        ascending = false;
                    }
                    else
                    {
                        while(std::stoi(new_number) <= previous_number)
                        {
                            new_number = generate_number(1);
                        }
                        ascending = true;
                    }
                    number += new_number;
                }
            }
            end = std::clock();
        }

        if(get_digital_root(number) == 7)
        {
            std::cout << number << std::endl << std::endl;
            logger.write(number + "\n\n");
            finished = true;
        }
    }
}

int generate_numbers()
{
    std::string number;
    int digital_root = 0;
    bool running = true;
    FileLogger logger = FileLogger("numbers.txt");
    std::string output;
    int digits = 0;
    std::cout << std::endl << std::endl << "Number of digits per number: ";
    std::cin >> digits;
    while(running == true)
    {
        number = generate_number(digits);
        digital_root = get_digital_root(number);
        if(alternating(number) && digital_root == 7)
        {
            output = number + "\n";
            std::cout << output << std::endl;
            logger.write(output);
        }
    }
    return 0;
}

int check_numbers()
{
    int x;
    std::cout << std::endl << std::endl << "Number to count up to (x): ";
    std::cin >> x;
    FileLogger logger = FileLogger("numbers.txt");
    std::stringstream stream;
    std::string output;
    for(int i = 1; i <= x; i++)
    {
        stream << i;
        if(alternating(stream.str()) && get_digital_root(stream.str()) == 7)
        {
            output = stream.str() + "\n";
            std::cout << output << std::endl;
            logger.write(output);
        }
    }
}

int main()
{
    std::string input = "";
    bool running = true;
    while(running == true)
    {
        display_options();
        std::cin >> input;
        if(input == "1")
        {
            generate_numbers();
        }
        if(input == "2")
        {
            check_numbers();
        }
        if(input == "3")
        {
            clearlogs();
        }
        if(input == "4")
        {
            remove_duplicates();
        }
        if(input == "5")
        {
            s_algorithm();
        }
    }
}

