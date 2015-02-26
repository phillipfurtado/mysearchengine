/*
 * Document.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: pfurtado
 */

#include "Document.h"

#include <cmath>

Document::Document() {
	TF = 0;
	magnitude = 0;
	dotProduct = 0;
}

const std::string& Document::getId() const {
	return id;
}

void Document::setId(const std::string& id) {
	this->id = id;
}

const std::string& Document::getName() const {
	return name;
}

void Document::setName(const std::string& name) {
	this->name = name;
}

int Document::getTF() const {
	return TF;
}

void Document::setTF(int tf) {
	TF = tf;
}

double Document::getMagnitude() const {
	return magnitude;
}

void Document::setMagnitude(double magnitude) {
	this->magnitude = magnitude;
}

double Document::getDotProduct() const {
	return dotProduct;
}

void Document::setDotProduct(double dotProduct) {
	this->dotProduct = dotProduct;
}

double Document::calculateTF(void) const {
	if (this->TF == 0)
        return 0;

    return 1 + log(this->TF);
}

void Document::increaseTF() const {
	this->TF++;
}

Document::~Document() {

}


