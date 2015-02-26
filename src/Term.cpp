/*
 * Term.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: pfurtado
 */

#include "Term.h"

#include <cmath>

Term::Term() {
	globalFrequency = 0;
	IDF = 0;
}

int Term::getGlobalFrequency() const {
	return globalFrequency;
}

void Term::setGlobalFrequency(int globalFrequency) {
	this->globalFrequency = globalFrequency;
}

double Term::getIDF() const {
	return IDF;
}

void Term::setIDF(double idf) {
	IDF = idf;
}

void Term::increaseGlobalFrequency() {
	this->globalFrequency++;
}

const std::string& Term::getName() const {
	return name;
}

void Term::setName(const std::string& name) {
	this->name = name;
}

void Term::calculateIDF(const int N) {
	if (this->globalFrequency == 0)
	        IDF = 0;
	IDF = log( (double)N / this->globalFrequency);
}

Term::~Term() {

}

