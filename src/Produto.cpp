/*
 * Produto.cpp
 *
 *  Created on: Feb 25, 2015
 *      Author: pfurtado
 */

#include "Produto.h"

Produto::Produto() {

}

const string& Produto::getCategoria() const {
	return categoria;
}

void Produto::setCategoria(const string& categoria) {
	this->categoria = categoria;
}

const string& Produto::getDescricao() const {
	return descricao;
}

void Produto::setDescricao(const string& descricao) {
	this->descricao = descricao;
}

const string& Produto::getId() const {
	return id;
}

void Produto::setId(const string& id) {
	this->id = id;
}

const string& Produto::getImg() const {
	return img;
}

void Produto::setImg(const string& img) {
	this->img = img;
}

const string& Produto::getPreco() const {
	return preco;
}

void Produto::setPreco(const string& preco) {
	this->preco = preco;
}

const string& Produto::getTitulo() const {
	return titulo;
}

void Produto::setTitulo(const string& titulo) {
	this->titulo = titulo;
}

Produto::~Produto() {

}

