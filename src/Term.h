/*
 * Term.h
 *
 *  Created on: Feb 14, 2015
 *      Author: pfurtado
 */

#ifndef TERM_H_
#define TERM_H_

#include<string>
#include <iostream>

class Term {
private:
	std::string name;
	int globalFrequency;
	mutable double IDF;
public:
	Term();
	virtual ~Term();
	int getGlobalFrequency() const;
	void setGlobalFrequency(int globalFrequency);
	void increaseGlobalFrequency();
	double getIDF() const;
	void setIDF(double idf);
	const std::string& getName() const;
	void setName(const std::string& name);
	void calculateIDF(const int N);
	bool operator<(const Term &other) const {
		return name < other.name;
	}
	bool operator==(const Term &other) const {
		if (name == other.getName())
			return true;
		else
			return false;
	}
	friend std::ostream& operator <<(std::ostream& os, const Term& m) {
		os << m.name << " Gfreq: " << m.globalFrequency << std::endl;
		return os;
	}
};

#endif /* TERM_H_ */
