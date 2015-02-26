/*
 * Produto.h
 *
 *  Created on: Feb 25, 2015
 *      Author: pfurtado
 */

#ifndef PRODUTO_H_
#define PRODUTO_H_

#include <string>

using namespace std;

class Produto {
private:
	string id;
	string categoria;
	string titulo;
	string descricao;
	string preco;
	string img;
public:
	Produto();
	virtual ~Produto();
	const string& getCategoria() const;
	void setCategoria(const string& categoria);
	const string& getDescricao() const;
	void setDescricao(const string& descricao);
	const string& getId() const;
	void setId(const string& id);
	const string& getImg() const;
	void setImg(const string& img);
	const string& getPreco() const;
	void setPreco(const string& preco);
	const string& getTitulo() const;
	void setTitulo(const string& titulo);
};

#endif /* PRODUTO_H_ */
