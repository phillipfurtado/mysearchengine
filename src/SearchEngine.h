#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_PRODUCTS_TO_FETCH 23155 //23155
#define MAX_QUERIES_TO_EVALUATE 50
#define RELEVANT_STRING_SIZE 31

const char * DATABASE_PATH = "data/textDescDafitiPosthaus.xml";
const char * DATABASE_COLLECTION_PATH = "data/colecaoDafitiPosthaus/";
const char * RELEVANTS_PATH = "data/relevants/";
const char * QUERIES_PATH = "data/queries/";
const char * RELEVANT_SUFIX = "_relevante.xml";
const char * STOPWORDS_PATH = "stopwords.list";
const char * SDLC_EXTENSION = ".sdlc";
const char * EXTENSION_DELIMITER = ".";
const char * TEXT_EXTENSION = ".txt";
const char * XML_ROOT = "produtos";
const char * NODE_ROOT = "produto";

char removeSpecialChars(char c) {
	if (c == '.' || c == ',') {
		return ' ';
	}
	return c;
}

string& trim_right_inplace(string& s, const string& delimiters = " \f\n\r\t\v") {
	return s.erase(s.find_last_not_of(delimiters) + 1);
}

string& trim_left_inplace(string& s, const string& delimiters = " \f\n\r\t\v") {
	return s.erase(0, s.find_first_not_of(delimiters));
}

string& trim_inplace(string& s, const string& delimiters = " \f\n\r\t\v") {
	return trim_left_inplace(trim_right_inplace(s, delimiters), delimiters);
}

string normalize(string str) {

	//remove special chars
	transform(str.begin(), str.end(), str.begin(), removeSpecialChars);

	//to lower case
	transform(str.begin(), str.end(), str.begin(), ::tolower);

	trim_inplace(str);

	return str;
}

vector<string> string_split(string s, const char delimiter) {
	size_t start = 0;
	size_t end = s.find_first_of(delimiter);

	vector<string> output;

	while (end <= string::npos) {
		output.emplace_back(s.substr(start, end - start));

		if (end == string::npos)
			break;

		start = end + 1;
		end = s.find_first_of(delimiter, start);
	}

	return output;
}

#endif
