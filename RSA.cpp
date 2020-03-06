/* Authors: Ruth Navarrete (SID 862106344), Sabrina Carlos (SID 862111206)
 */

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <cstdio>
#include <vector>
#include <map>

void prompt(int&, int&, int&, int&, int&, std::string&, std::string&, int&);
bool check_validity(int, int, int&, int&, int&, int&);
int gcd(int, int);
int phi_of_n(int, int);
std::vector<int> prime_factors_n(int);
bool find_p_q(int, int&, int&);
void find_d(int&, int, int);
void encryption(int, int, std::string);
void decryption(int, int, std::string);
void populate_coding();
void find_mod(int&, int, int);
int exponent_by_square(int, int, int);

std::map<char, int> coding;

int main(int argc, char* argv[]) {
    std::string option, input_file;
    std::vector<int> factors;
    int e = 0, n = 0, p = 0, q = 0, d = 0, phi = 0;

std::cout << "t1" << std::endl;
    e = atoi(argv[1]);
std::cout << "t2" << std::endl;
    n = atoi(argv[2]);
std::cout << "t3" << std::endl;
    option = argv[3];
std::cout << "t4" << std::endl;
    input_file = argv[4];
std::cout << "t5" << std::endl;

    prompt(e, n, p, q, d, option, input_file, phi);
    find_d(d, e, phi);
    populate_coding();

    printf("e=%d, n=%d, p=%d, q=%d, phi=%d, d=%d", e, n, p, q, phi, d);

    if (option == "e") {
        encryption(e, n, input_file);
        std::cout << "\"incrypted.txt\"";
    }
    else {
        decryption(d, n, input_file);
        std::cout << "p = " << p << ", q = " << q << ", \"decrypted.txt\"";
    }
    /*
    printf("e=%d, n=%d, p=%d, q=%d, phi=%d, d=%d", e, n, p, q, phi, d);
    std::cout << ", option = " << option << std::endl;
    */
    return 0;
}

void prompt(int& e, int& n, int& p, int& q, int& d, std::string& option, std::string& input_file, int& phi) {
    //std::cout << "Enter public key in the order [e, n], where e is the encryption exponent and n is a number resulting from two distinct prime numbers\nExample: 5, 72 [ENTER]\n";

    /*
    do {
        //std::cout << "encryption key: ";
        /std::cin >> e;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(100, '\n');
            std::cin >> e;
        }
        std::cout << "number resulting from two distinct prime numbers: ";
        std::cin >> n;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(100, '\n');
            std::cin >> n;
        }
    } while (!check_validity(e, n, p, q, d, phi));

    do {
        std::cout << "Encryption or decryption? ";
        std::cin >> option;
        if (option.size() > 0) {
            option.at(0) = tolower(option.at(0));
        }
    } while ((option.size() < 1) || !((option.at(0) == 'e') || (option.at(0) == 'd')));

    std::cout << "Enter input file name: ";
    do {
        std::cin >> input_file;
    } while (input_file == "" || input_file == " ");
    std::cout << std::endl;
    */
    /*
    do {
        std::cout << "testing" << std::endl;
        std::cin >> e >> n >> option >> input_file;
    } while (!check_validity(e, n, p, q, d, phi));
    */
    //std::cin >> e >> n >> option >> input_file;
    /*
    option.at(0) = tolower(option.at(0));

    if (option.at(0) == 'e') {
        option = "e";
    }
    else if (option.at(0) == 'd') {
        option = "d";
    }
    */
    if (!check_validity(e, n, p, q, d, phi)) {
        exit(1);
    }
}

bool check_validity(int e, int n, int& p, int& q, int& d, int& phi) {
    /* verify that p != q; ie, n is not a perfect square */
    if ( (sqrtl(n) - floor(sqrtl(n))) == 0) {
        //std::cout << "sqrt false" << std::endl;
        return false;
    }

    /* verify that n=pq; p, q are distinct primes */
    if (!find_p_q(n, p, q)) {
        return false;
    }

    /* verify gcd(e, phi(n)) == 1 */
    phi = phi_of_n(p, q);
    if (gcd(e, phi) != 1) {
        return false;
    }

    return true;
}

int gcd(int e, int n) {
    if (e == 0) {
        return n;
    }
    if (n == 0) {
        return e;
    }
    int mod = n;
    if (e == n) {
        return e;
    }
    if (e < n) {
        std::swap(e, n);
    }
    return gcd(e - n, n);
}

int phi_of_n(int p, int q) {
    return ((p-1) * (q-1));
}

std::vector<int> prime_factors_n(int n) {
    std::vector<int> prime_factors;

    prime_factors.push_back(1);

    /* n is even so we can divide out all factors of 2 */
    if ((n % 2) == 0) {
        prime_factors.push_back(2);
        while ((n % 2) == 0) {
            n /= 2;
        }
    }

    /* n is odd so we can check odd numbers from 3 to sqrt(n) */
    for (unsigned i = 3; i < n; i += 2) {
        if ((n % i) == 0) {
            prime_factors.push_back(i);
        }
    }

    if (n > 2) {
        prime_factors.push_back(n);
    }
    return prime_factors;
}

bool find_p_q(int n, int& p, int& q) {
    std::vector<int> factors = prime_factors_n(n);

    int temp_p = 0;
    int temp_q = 0;

    for (unsigned i = 0; i < factors.size(); i++) {
        for (unsigned j = i + 1; j < factors.size(); j++) {
            if ((factors.at(i) * factors.at(j)) == n) {
                p = factors.at(i);
                q = factors.at(j);
            }
        }
    }

    if (p == q) {
        return false;
    }
    return true;
}

void find_d(int& d, int e, int phi) {
    d = -1;
    int k = 0;
    while (d == -1) {
        int num = (k * phi) + 1;
        if ((num % e) == 0) {
            d = num / e;
            return;
        }
        k++;
    }
}

void encryption(int e, int n, std::string input_file) {
    std::ifstream e_input(input_file);
    std::ofstream e_output("incrypted.txt");

    if (e_input.fail()) {
        std::cout << "Could not open input file" << std::endl;
        return;
    }
    if (e_output.fail()) {
        std::cout << "Could not open output file" << std::endl;
        return;
    }

    char temp_c;
    double C = -1;
    int M = -1;

    while (e_input.get(temp_c)) {
        std::cout << "in encoding loop" << std::endl;

        if (isalpha(temp_c)) {
            temp_c = toupper(temp_c);
        }
        
        if (coding.find(temp_c) == coding.end()) {
            std::cout << "Error encoding" << std::endl;
            exit(1);
        }

        M = coding.find(temp_c)->second;
        
        C = (exponent_by_square(M, e, n)) % n;
        e_output << C;
        if (e_input.peek() != EOF) {
            e_output << ' ';
        }
    }

    e_input.close();
    e_output.close();
}

void decryption (int d, int n, std::string input_file) {
    std::cout << "in decryption" << std::endl;
    std::ifstream d_input(input_file);
    std::ofstream d_output("decrypted.txt");

    if (d_input.fail()) {
        std::cout << "Could not open input file" << std::endl;
        return;
    }
    if (d_output.fail()) {
        std::cout << "Could not open output file" << std::endl;
        return;
    }
    
    char temp_c;
    long long temp_i;
    int C = -1;
    int M = -1;
    while (d_input >> temp_i) {
        std::cout << "in decoding loop" << std::endl;
        C = (int)temp_i;

        M = exponent_by_square(C, d, n) % n;

        std::cout << "C=" << temp_i << ", M=" << M << std::flush;

        for (std::map<char, int>::iterator it = coding.begin(); it != coding.end(); it++) {
            if (it->second == M)  {
                d_output << it->first;
                std::cout << ", character=" << it->first << std::endl;
            }
        }
    }

    std::cout << "out of decoding loop" << std::endl;

    d_input.close();
    d_output.close();
}

void populate_coding() {
    int C = 2;
    char M = 'A';

    /* encode the alphabet */
    for ( ; C < 28; M++, C++) {
        coding.insert(std::pair<char, int>(M, C));
    }

    coding.insert(std::pair<char, int>(' ', C));
    C++;
    
    /* encode the numbers */
    
    M = '0';
    for ( ; C < 39; M++, C++) {
        coding.insert(std::pair<char, int>(M, C));
    }

    coding.insert(std::pair<char, int>('\n', C));
    C++;
    
    coding.insert(std::pair<char, int>('!', C));
    C++;

    coding.insert(std::pair<char, int>('.', C));
    C++;

    coding.insert(std::pair<char, int>('?', C));
    C++;

    coding.insert(std::pair<char, int>(',', C));
    C++;

    coding.insert(std::pair<char, int>('\'', C));
    C++;

    coding.insert(std::pair<char, int>('\"', C));
    C++;

    coding.insert(std::pair<char, int>(';', C));
    C++;

    coding.insert(std::pair<char, int>(':', C));
    C++;
    
}

int exponent_by_square(int C, int d, int n) {
    if (d == 0) {
        return 1;
    }
    if (d == 1) {
        return C % n;
    }
    if ((d % 2) == 0) {
        long int test = exponent_by_square((C * C) % n, d / 2, n);
        return test % n;
    }
    else {
        long int test = exponent_by_square((C * C) % n, d / 2, n) * (C % n);
        return test % n;
    }
}