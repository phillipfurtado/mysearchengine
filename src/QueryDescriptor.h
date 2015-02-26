/*
 * QueryDescriptor.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pfurtado
 */

#ifndef QUERYDESCRIPTOR_H_
#define QUERYDESCRIPTOR_H_

#include <string>
#include <vector>
#include "Relevant.h"

class QueryDescriptor {
private:
	std::string str;
	std::string sdlcText;
	std::vector<Relevant> relevants;
public:
	QueryDescriptor();
	virtual ~QueryDescriptor();
	const std::string& getSdlcText() const;
	void setSdlcText(const std::string& sdlcText);
	const std::string& getStr() const;
	void setStr(const std::string& str);

	bool operator==(const QueryDescriptor &other) const {
		return str == other.str;
	}

	const std::vector<Relevant>& getRelevants() const;
	void setRelevants(const std::vector<Relevant>& relevants);
};

#endif /* QUERYDESCRIPTOR_H_ */
