#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

//Calculates the number of different letters used within a text
int calculate_base(std::vector<int> int_text) {
	std::set<int> base;
	for (int value : int_text) {
		base.insert(value);
	}
	return base.size();
}

//Converts every string to an int vector where each int values corresponds to the ascii value of the string
std::vector<int> convertToAscii(std::string text) {
	std::vector<int> int_list;
	for (int i = 0; i < text.length(); ++i) {
		int_list.push_back(int(text.at(i)));
	}
	return int_list;
}

//returns the complete calculated hashvalue for an int vector
int first_hash(std::vector<int> int_text, int base ,int keyword_lenght, int prime) {
	int hash_value = 0;
	for (int i = 0; i < keyword_lenght; ++i) {
		hash_value += int_text.at(i)*pow(base, keyword_lenght - i - 1);
	}
	//if the hash_value is negative due to the modulooperation returns the positiv equivalent
	hash_value = hash_value % prime;
	if (hash_value < 0) {
		return hash_value + prime;
	} else {
		return hash_value;
	}
}

//Creates a new hashvalue based on the previous hashvalue
int continuous_hash(std::vector<int> int_text ,int prev_hash, int base, int keyword_length, int prime, int already_hashed) {
	int z = pow(base, keyword_length - 1);
	int hash_value = (int_text.at(already_hashed - 1 + keyword_length) + ((prev_hash - int_text.at(already_hashed - 1)*z)*base)) % prime;
	//if the hash_value is negative due to the modulooperation returns the positiv equivalent
	if (hash_value < 0) {
		return hash_value + prime;
	}
	else {
		return hash_value;
	}
}

int main() {
	std::string text;
	std::string keyword;
	
	//Receive user input
	std::cout << "Bitte geben Sie den Text ein der durchsucht werden soll.\n";
	std::getline(std::cin, text);
	std::cout << "Bitte geben Sie das Keyword ein, nachdem der Text durchsucht werden soll.\n";
	std::getline(std::cin, keyword);
	//Convert the strings into an int vector that stores the ascii values in the right order
	std::vector<int> int_text = convertToAscii(text);
	std::vector<int> int_keyword = convertToAscii(keyword);

	/* To test the functionality one can use these values. One needs to uncommend the lines that are initializing these variables.
	std::vector<int> int_text{ 1,0,2,3,2,1,3,1,2 };
	std::vector<int> int_keyword{2,3,2};
	int prime = 37;
	*/

	//Store the number of different letters in the text
	int base = calculate_base(int_text);
	//Defines the hash-value range. A large prime number is necessary when working with large alphabets
	int prime = 990313;
	//Stores the length of our keyword
	int keyword_length = int_keyword.size();

	//Creates a new in vector that stores the hash values for each substring
	std::vector<int> hash_values;
	for (int i = 0; i <= int_text.size() - keyword_length; ++i) {
		if (hash_values.size() == 0) {
			//for the first substring create the first hash value
			hash_values.push_back(first_hash(int_text, base, keyword_length, prime));
		} else {
			//for every other substing call continous_hash which creates the new hashvalue with help of the hash value from the previous substring
			hash_values.push_back(continuous_hash(int_text, hash_values.at(i - 1), base, keyword_length, prime, hash_values.size()));
		}
	}

	//hashes the keyword so that the keywordhash and the substring hashs can be compared
	int hashed_keyword = first_hash(int_keyword, base, keyword_length, prime);

	//Compares all the substring hashvalues with the keyword hashvalue and prints to the cli if they have the same value
	bool not_found = true;
	for (int i = 0; i < hash_values.size(); ++i) {
		if (hash_values.at(i) == hashed_keyword) {
			std::cout << "Das Keyword ist im Text vorhanden an Stelle: " << i + 1 << "\n";
			not_found = false;
		}
	}

	//prints to the cli if the keyword is not found within the text
	if (not_found) {
		std::cout << "Der Text enthält kein vorkommen des Wortes";
	}

	std::cin.get();
	return 0;
}
