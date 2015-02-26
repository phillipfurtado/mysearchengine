#include "SearchEngine.h"

#include <vector>
#include <string>
#include <cstring>
#include <set>
#include <cmath>
#include <unordered_map>
#include <fstream>
#include <ctime>
#include <math.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "Document.h"
#include "Term.h"
#include "QueryDescriptor.h"
#include "Relevant.h"
#include "Produto.h"

using namespace std;

unordered_map<string, Term> vocabulary;

unordered_map<string, Document> documents;

typedef set<Document> DocumentVector;

unordered_map<string, DocumentVector> inverted;

set<string> stopWords;

/**
 * calculate cosine distance between result elements
 */
struct cosineVectorCompare {
	bool operator()(pair<string, Document> const &left, pair<string, Document> const &right) const {
		double cosLeft = 0;
		double cosRight = 0;
		if (left.second.getMagnitude() > 0) {
			cosLeft = left.second.getDotProduct() / sqrt(left.second.getMagnitude());
		}

		if (right.second.getMagnitude() > 0) {
			cosRight = right.second.getDotProduct() / sqrt(right.second.getMagnitude());
		}

		return cosLeft > cosRight;
	}
};


/**
 * Index each token normalizing and them in the inverted list
 */
void indexToken(Produto produto, string token) {

	token = normalize(token);

	//ignore stop words
	if (stopWords.find(token) == stopWords.end()) {

		Term term = Term();
		if (vocabulary.count(token) == 0) {
			term.setName(token);
			term.setGlobalFrequency(1);
		} else {
			term = vocabulary[token];
		}

		string docId = produto.getId();
		string docName = produto.getImg();

		Document document = Document();
		if (documents.count(docId) == 0) {
			document.setId(docId);
			document.setName(docName);
			documents[docId] = document;
		} else {
			document = documents[docId];
		}

		//add token and related document to inverted list
		if (inverted.count(token) == 0) {
			document.increaseTF();
			DocumentVector dv;
			dv.insert(document);
			inverted.insert(std::pair<string, DocumentVector>(token, dv));
		} else {

			//check if is already added, if yes update it
			set<Document> docs = inverted[token];
			set<Document>::iterator docTempIt = docs.find(document);

			if (docTempIt != docs.end()) {

				Document tmpdoc = *docTempIt;
				tmpdoc.increaseTF();

				std::pair<std::set<Document>::iterator, bool> result = inverted[token].insert(tmpdoc);
				if (!result.second) {
					inverted[token].erase(result.first);
					inverted[token].insert(tmpdoc);
				}

			} else {
				inverted[token].insert(document);
				term.increaseGlobalFrequency();
			}
		}
		vocabulary[token] = term;
	}

}
/**
 * Call indexToken for a product from DafitiPosthaus XML
 */
void indexProduto(Produto produto, string descriptor) {

	//search index the product title, description and image descriptor
	string word = (produto.getTitulo() + " " + produto.getDescricao() + " " + produto.getCategoria() + " " + descriptor);

	vector<string> tokens = string_split(word, ' ');

	for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		indexToken(produto, *it);
	}
}

/**
 * First calculate IDF for each Term than calculate each document Magnitude and TF
 */
int calculateMagnitude() {

	cout << "Calculating Magnitude and TF...";

	clock_t start;
	double duration;
	start = clock();

	for (unordered_map<string, Term>::iterator itTerm = vocabulary.begin(); itTerm != vocabulary.end(); ++itTerm) {

		Term term = itTerm->second;

		term.calculateIDF(MAX_PRODUCTS_TO_FETCH);
		double IDF = term.getIDF();
		vocabulary[itTerm->first] = term;

		set<Document> docs = inverted[term.getName()];

		for (set<Document>::iterator docTempIt = docs.begin(); docTempIt != docs.end(); ++docTempIt) {

			double TF = docTempIt->calculateTF();
			Document doc = documents[docTempIt->getId()];

			//sum magnitude
			double magnitude = doc.getMagnitude() + pow(IDF * TF, 2);
			doc.setMagnitude(magnitude);
			documents[docTempIt->getId()] = doc;
		}

	}

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;

	cout << "took " << duration << " seconds." << endl;

	return EXIT_SUCCESS;

}
/**
 * Load DafitiPosthaus XML nodes and save each one in a object
 * Class index function only for product that have a valid image file
 */
int parseAndIndexDB() {

	cout << "Parsing and Indexing the database...";

	clock_t start;
	double duration;
	start = clock();

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(DATABASE_PATH);

	//missing file not allowed
	if (doc == NULL) {
		cout << stderr << "Failed to parse, missing file and/or invalid path. \n";
		return EXIT_FAILURE;
	}

	cur = xmlDocGetRootElement(doc);

	//empty file not allowed
	if (cur == NULL) {
		cout << stderr << "Failed to parse, empty document. \n";
		xmlFreeDoc(doc);
		return EXIT_FAILURE;
	}

	Produto produto;

	//the xml must be valid
	if (xmlStrcmp(cur->name, (const xmlChar *) XML_ROOT)) {
		cout << stderr << "Failed to parse, document of the wrong type, root node != produtos";
		xmlFreeDoc(doc);
		return EXIT_FAILURE;
	}

	cur = cur->xmlChildrenNode;

	int nodes = 0;

	while (cur != NULL && nodes < MAX_PRODUCTS_TO_FETCH) {

		if (!xmlStrcmp(cur->name, (const xmlChar *) NODE_ROOT)) {

			xmlNodePtr currentNode = cur->xmlChildrenNode;

			while (currentNode != NULL) {
				if (!xmlStrcmp(currentNode->name, (const xmlChar *) "id"))
					produto.setId((char *) xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1));
				if (!xmlStrcmp(currentNode->name, (const xmlChar *) "categoria"))
					produto.setCategoria((char *) xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1));
				if (!xmlStrcmp(currentNode->name, (const xmlChar *) "titulo"))
					produto.setTitulo((char *) xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1));
				if (!xmlStrcmp(currentNode->name, (const xmlChar *) "descricao"))
					produto.setDescricao((char *) xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1));
				if (!xmlStrcmp(currentNode->name, (const xmlChar *) "img"))
					produto.setImg((char *) xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1));

				currentNode = currentNode->next;
			}

			//get image name replace extension to sdlc and open
			string img(produto.getImg());

			std::string token = img.substr(0, img.find(EXTENSION_DELIMITER));

			string sdlcFilename = DATABASE_COLLECTION_PATH + token + SDLC_EXTENSION;

			ifstream imageFile(sdlcFilename);

			//index only good images that were transformed
			if(imageFile.good()){
				string descriptor;
				getline(imageFile, descriptor);
				indexProduto(produto,descriptor);
			}

			nodes++;
		}

		cur = cur->next;
	}

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;

	cout << "took " << duration << " seconds." << endl;

	cout << "Finished vocabulary with " << vocabulary.size() << " terms." << endl;

	return EXIT_SUCCESS;
}

/**
 * Calculate Term Frequency for a query
 */
double calculateQueryTF(string query, string term) {
	int occurrences = 0;
	string::size_type start = 0;

	while ((start = query.find(term, start)) != string::npos) {
		++occurrences;
		start += term.length();
	}

	if (occurrences == 0) {
		return occurrences;
	} else {
		return 1 + log(occurrences);
	}
}

/**
 * Search for terms in the inverted list
 */
vector<pair<string, Document>> find(string query) {

	clock_t start;
	double duration;
	start = clock();

	unordered_map<string, Document> results;

	query = normalize(query);

	char separator[] = " ";

	char * token = strtok(&query[0], separator);

	while (token != NULL) {

		//do we have this term in our vocabulary?
		Term term = vocabulary[token];

		//any document point to this term?
		set<Document> docs = inverted[token];

		int queryTF = calculateQueryTF(query, token);

		for (set<Document>::iterator itDoc = docs.begin(); itDoc != docs.end(); ++itDoc) {
			Document doc = *itDoc;

			double dotProduct = (term.getIDF() * doc.getTF()) * (term.getIDF() * queryTF);
			doc.setDotProduct(dotProduct);

			std::pair<std::unordered_map<string, Document>::iterator, bool> it = results.insert(
					std::pair<string, Document>(doc.getName(), doc));

			if (!it.second) { //if already exists
				double dp = it.first->second.getDotProduct() + doc.getDotProduct();
				doc.setDotProduct(dp);
			}

			results[doc.getName()] = doc;

		}

		token = strtok(NULL, separator);

	}

	//add magnitude to results
	for (unordered_map<string, Document>::iterator it = results.begin(); it != results.end(); ++it) {
		Document doc = it->second;
		Document originalDoc = documents[doc.getId()];
		doc.setMagnitude(originalDoc.getMagnitude());
		results[it->first] = doc;
	}

	vector<pair<string, Document>> sorted_results(results.begin(), results.end());

	//sort by cos
	sort(sorted_results.begin(), sorted_results.end(), cosineVectorCompare());

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;

	cout << "Took " << duration << " seconds." << endl;

	cout << "Found " << results.size() << " documents." << endl;

	return sorted_results;

}

vector<Relevant> parseRelevant(char * file_prefix) {

	vector<Relevant> result;

	xmlDocPtr doc;
	xmlNodePtr cur;

	char * fileName = (char *) malloc(RELEVANT_STRING_SIZE);
	strcpy(fileName, RELEVANTS_PATH);
	strcat(fileName, file_prefix);
	strcat(fileName, RELEVANT_SUFIX);

	doc = xmlParseFile(fileName);

	//missing file not allowed
	if (doc == NULL) {
		cout << stderr << "Failed to parse, missing file and/or invalid path. \n";
		return result;
	}

	cur = xmlDocGetRootElement(doc);

	//empty file not allowed
	if (cur == NULL) {
		cout << stderr << "Failed to parse, empty document. \n";
		xmlFreeDoc(doc);
		return result;
	}

	//the xml must be valid
	if (xmlStrcmp(cur->name, (const xmlChar *) "relevantes")) {
		cout << stderr << "Failed to parse, document of the wrong type, root node != produtos";
		xmlFreeDoc(doc);
		return result;
	}

	cur = cur->xmlChildrenNode;

	Relevant relevante;

	while (cur != NULL) {

		if (!xmlStrcmp(cur->name, (const xmlChar *) "relevante")) {

			xmlNodePtr currentNode = cur->xmlChildrenNode;

			while (currentNode != NULL) {
				if (!xmlStrcmp(currentNode->name, (const xmlChar *) "img"))
					relevante.setImg((char *) xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1));

				currentNode = currentNode->next;
			}

			result.push_back(relevante);

		}

		cur = cur->next;
	}

	return result;
}

vector<QueryDescriptor> getRelevants() {

	string queriesPath = QUERIES_PATH;

	vector<QueryDescriptor> relevants;

	for (int i = 0; i < MAX_QUERIES_TO_EVALUATE; i++) {

		char numstr[21]; // enough to hold all numbers up to 64-bits
		int query_prefix = i + 1;
		sprintf(numstr, "%d", query_prefix);

		QueryDescriptor query;

		string file = queriesPath + numstr + TEXT_EXTENSION;
		ifstream queryFile(file);
		string line;
		if (queryFile.is_open()) {
			getline(queryFile, line);
			query.setStr(line);
		}

		string sdlcFilename = queriesPath + numstr + SDLC_EXTENSION;
		ifstream sdlcFile(sdlcFilename);

		//index only good images that were transformed
		if (sdlcFile.good()) {
			string descriptor;
			getline(sdlcFile, descriptor);
			query.setSdlcText(descriptor);
		}

		query.setRelevants(parseRelevant(numstr));
		relevants.push_back(query);

		queryFile.close();

	}

	return relevants;

}

double calculatePrecision(vector<pair<string, Document>> queryResults, vector<Relevant> relevants, int point) {

	int relevantsCount = 0;

	int lenght = point;

	if (queryResults.size() < lenght)
		lenght = queryResults.size();

	for (int i = 0; i < lenght; i++) {

		for (vector<Relevant>::iterator it = relevants.begin(); it != relevants.end(); ++it) {

			if (queryResults[i].first == it->getImg()) {
				relevantsCount++;
			}
		}
	}

	return (double) relevantsCount / (double) point;

}

double calculateAP(vector<pair<string, Document>> queryResults, vector<Relevant> relevants) {

	double precisionAcummulator = 0;

	int lenght = 10;

	if (queryResults.size() < lenght)
		lenght = queryResults.size();

	for (int i = 0; i < lenght; i++) {

		for (vector<Relevant>::iterator it = relevants.begin(); it != relevants.end(); ++it) {

			if (queryResults[i].first == it->getImg()) {
				precisionAcummulator += calculatePrecision(queryResults, relevants, i + 1);
			}
		}
	}

	return (double) precisionAcummulator / relevants.size();

}

void evaluateModel() {

	double map = 0;
	double p10 = 0;

	//load queries and respective relevants
	vector<QueryDescriptor> relevants = getRelevants();

	//evaluate MAP and Precision At Point 10
	for (vector<QueryDescriptor>::iterator itDoc = relevants.begin(); itDoc != relevants.end(); ++itDoc) {

		vector<pair<string, Document>> queryResults = find(itDoc->getStr());

		map += calculateAP(queryResults, itDoc->getRelevants());
		p10 += calculatePrecision(queryResults, itDoc->getRelevants(), 10);
	}

	cout << "Evaluations text queries (" << MAX_QUERIES_TO_EVALUATE << " queries)" << endl;
	cout << "MAP: " << map / MAX_QUERIES_TO_EVALUATE << endl;
	cout << "P@10: " << p10 / MAX_QUERIES_TO_EVALUATE << endl;

	//evaluate MAP and Precision At Point 10
	for (vector<QueryDescriptor>::iterator itDoc = relevants.begin(); itDoc != relevants.end();
			++itDoc) {

		vector<pair<string, Document>> queryResults = find(itDoc->getSdlcText());

		map += calculateAP(queryResults, itDoc->getRelevants());
		p10 += calculatePrecision(queryResults, itDoc->getRelevants(), 10);
	}

	cout << "Evaluations image descriptors queries (" << MAX_QUERIES_TO_EVALUATE << " queries)" << endl;
	cout << "MAP: " << map / MAX_QUERIES_TO_EVALUATE << endl;
	cout << "P@10: " << p10 / MAX_QUERIES_TO_EVALUATE << endl;

}

int main(int argc, char * argv[]) {

	ifstream stopwordsFile(STOPWORDS_PATH);
	string line;
	while (std::getline(stopwordsFile, line)) {
		stopWords.insert(line);
	}

	stopwordsFile.close();

	cout << "Welcome to my search engine!" << endl
			<< "Please be patient while I check files integrity, parse, index and do some math stuff with them." << endl
			<< "This will take a lot of your time, depending the size of your database...this parameter can be changed (search_engine.h#MAX_PRODUCTS_TO_FETCH)"
			<< endl;

	int status = parseAndIndexDB();

	status = calculateMagnitude();

	if (status == EXIT_SUCCESS) {
		cout << "Finished the hard work." << endl;
	}

	if (status == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if(argv[1]){
		string args(argv[1]);
		if(args == "-q") {
			if(argv[2]){
				string q(argv[2]);

				vector<pair<string, Document>> results = find(q);

				cout << "Results for \"" << q << "\" are: " << endl;

				if (results.size() == 0)
					cout << "None" << endl;

				for (vector<pair<string, Document>>::iterator itDoc = results.begin(); itDoc != results.end(); ++itDoc) {
					cout << itDoc->first << endl;
				}
			}else {
				cout << "-q requires parameter." << endl;
				return EXIT_FAILURE;
			}

		}
	} else {
		evaluateModel();
	}

	return EXIT_SUCCESS;

}
