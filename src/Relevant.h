/*
 * Relevant.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pfurtado
 */

#ifndef RELEVANT_H_
#define RELEVANT_H_

#include <string>

class Relevant {
private:
	std::string img;
public:
	Relevant();
	virtual ~Relevant();
	const std::string& getImg() const;
	void setImg(const std::string& img);
};

#endif /* RELEVANT_H_ */
