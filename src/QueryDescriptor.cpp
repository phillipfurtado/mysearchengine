/*
 * QueryDescriptor.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: pfurtado
 */

#include "QueryDescriptor.h"

QueryDescriptor::QueryDescriptor() {
}

const std::string& QueryDescriptor::getSdlcText() const {
	return sdlcText;
}

void QueryDescriptor::setSdlcText(const std::string& sdlcText) {
	this->sdlcText = sdlcText;
}

const std::string& QueryDescriptor::getStr() const {
	return str;
}

void QueryDescriptor::setStr(const std::string& str) {
	this->str = str;
}

QueryDescriptor::~QueryDescriptor() {

}

const std::vector<Relevant>& QueryDescriptor::getRelevants() const {
	return relevants;
}

void QueryDescriptor::setRelevants(const std::vector<Relevant>& relevants) {
	this->relevants = relevants;
}
