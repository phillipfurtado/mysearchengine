/*
 * Document.h
 *
 *  Created on: Feb 14, 2015
 *      Author: pfurtado
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include<string>
#include<iostream>

class Document {
private:
	std::string id;
	std::string name;
	mutable int TF;
	mutable double magnitude;
	mutable double dotProduct;
public:
	Document();
	virtual ~Document();
	const std::string& getId() const;
	void setId(const std::string& id);
	const std::string& getName() const;
	void setName(const std::string& name);
	int getTF() const;
	void setTF(int tf);
	double getMagnitude() const;
	void setMagnitude(double magnitude);
	double getDotProduct() const;
	void setDotProduct(double dotProduct);
	double calculateTF(void) const;
	void increaseTF() const;
	bool operator<(const Document &other) const {
		return id < other.getId();
	}
	bool operator==(const Document &other) const {
			return id == other.id;
	}
	friend std::ostream& operator <<(std::ostream& os, const Document& m) {
		os << m.id << std::endl;
		return os;
	}


};

#endif /* DOCUMENT_H_ */
