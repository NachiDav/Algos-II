#ifndef _QUERY_H_INCLUDED_
#define _QUERY_H_INCLUDED_

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include "Array.h"
#include "sensornet.h"


using namespace std;

class query {
	double mean_ = 0;
	double max_ = 0;
	double min_ = 0;
	size_t amount_ = 0;

	
public:
	query();
	query(data & D);
	query( const query & Q); 
	~query( );

	double		max() const;			//Devuelve el valor maximo del arreglo
	double		min() const;			//Devuelve el valor minimo del arreglo
	double		mean() const;			//Devuelve el promedio del arreglo
	size_t		amount() const;		//Devuelve la cantidad de lugares del arreglo

	query&		operator=( const query & Q); 
	bool 		operator==( const query & Q) const; 
	bool 		operator!=( const query & Q) const; 


	friend
	bool		check_id(string str, sensornet & S, size_t id,bool & first);
	friend
	bool		check_pos(sensornet & S, size_t id, size_t & pos1, size_t & pos2);
	friend
	bool		read_query(istream & is,ostream & os, sensornet & S, size_t id, size_t & pos1, size_t & pos2, bool & end);

	void		process_data(sensornet & S, size_t id, size_t & pos1, size_t & pos2);
	
	void 		process_data_std(sensornet & S, size_t id, size_t & pos1, size_t & pos2);

	void 		process_data_stree(sensornet & S, size_t id, size_t & pos1, size_t & pos2);

	data& 		find_data(size_t pos1, size_t pos2, size_t index, segment_tree & s_tree);

	friend
	ostream& 	operator<<(ostream & os, query Q); 	

};

query::query(){
}

query::query(data & D){
	this->mean_ = D.sum()/D.amount();
	this->max_ = D.max();
	this->min_ = D.min();
	this->amount_ = D.amount();
}


query::query( const query & Q){
	this->mean_ = Q.mean_;
	this->max_ = Q.max_;
	this->min_ = Q.min_;
	this->amount_ = Q.amount_;
} 

query::~query( ){

}

double
query::max() const{
	return this->max_;
}

double
query::min() const{
	return this->min_;
}

double
query::mean() const{
	return this->mean_;
}
	
size_t
query::amount() const{
	return this->amount_;
}


query&
query::operator=( const query & Q ){
	if (this->mean_ == Q.mean_ && this->max_ == Q.max_ && this->min_ == Q.min_ && this->amount_ == Q.amount_){
		return *this;
	}
	if(this->mean_ != Q.mean_){
		this->mean_ = Q.mean_;
	}
	if(this->max_ != Q.max_){
		this->max_ = Q.max_;
	}
	if(this->min_ != Q.min_){
		this->min_ = Q.min_;
	}
	if(this->amount_ != Q.amount_){
		this->amount_ = Q.amount_;
	}
	return *this;
} 
	

bool
query::operator==( const query & Q) const{
	if(this->mean_ != Q.mean_){
		return false;
	}
	else{
		if(this->max_ != Q.max_){
			return false;
		}
		else{
			if(this->min_ != Q.min_){
				return false;
			}
			else{
				if(this->amount_ != Q.amount_){
					return false;
		 		}
			}
		}
	}
	return true;
} 



bool
query::operator!=( const query & Q) const{
	if(this->mean_ == Q.mean_){
		return false;
	}
	else{
		if(this->max_ == Q.max_){
			return false;
		}
		else{
			if(this->min_ == Q.min_){
				return false;
			}
			else{
				if(this->amount_ == Q.amount_){
					return false;
				}
			}
		}
	}
	return true;
} 



bool
check_id(string str, sensornet & S, size_t id, bool &first){

	size_t S_size = S.size();
	size_t i;

	for (i = 0; i < S_size; i++){
		if (str == S[i].id()){		//Si encuentra el sensor que pide el query, agrega su posicion
			id = i;
			return true;
		}
	}

	return false;
}



bool
check_pos(sensornet & S, size_t id, size_t & pos1, size_t & pos2){

	if(pos1 > S[id].size() || pos1 >= pos2 || pos1 < 0){	//Chequea que la posicion mas baja no sobrepase
		return false;						//la cantidad de lugares del arreglo	
	}
	else{
		if (pos2 > S[id].size()){	//Chequea si la posicion mas alta sobrepaso la
			pos2= S[id].size();	//cantidad de lugares del arreglo y de ser asi,
		}										//coloca como posicion mas alta la cantidad de 
	}											//lugares del arreglo
	return true;
}



bool
read_query(istream & is,ostream & os, sensornet & S, size_t id, size_t & pos1, size_t & pos2,bool & q_state){
	string str,str2,str3, str4;
	Array <size_t> id_number;		//Aca se guarda la posicion (dentro de sensornet) de cada sensor
	bool first = true;
	size_t j;
	q_state = true;
	
	getline(is, str);
	if(str.empty()){
		q_state = false;
		return true;
	}
	stringstream str_st(str);
	if (!getline(str_st, str2, ',')){			//Se leen solo los q_ids
		os << "BAD QUERY"<<endl;
		q_state	= false;
		return true;
	}
	else{
		if(str2.empty()){
			id = S.size()-1;
		}
		else{
			if(!check_id(str2, S, id,first)){	//Se chequea que los q_id sean correctos
				os << "UNKNOWN ID" << endl;
				q_state = false;
				return true;
		
			}
		}	
	}
	if (!getline(str_st, str2, ',')){			//Se leen solo los q_ids
		os << "BAD QUERY"<< "dos"<< str2<< endl;
		q_state	= false;
		return true;
	}

	j=0;
	while(str2[j] == ' '){
		j++;
	}

 	for (size_t i = j; i < str2.size(); i++){
   		if (str2[i-1] == ' ' && str2[i] != ' '){
    		os << "BAD QUERY"<< "tres"<<  endl;
			q_state	= false;
			return true;
    	}
   		else{
   			if(isdigit(str2[i]) == 0){
     			os << "BAD QUERY"<< "cuatro"<< endl;
				q_state	= false;
				return true;
    		}
  		}
  	}


		stringstream str_st1(str2);
		str_st1 >> pos1;

		if (!getline(str_st, str2, ',')){			//Se leen solo los q_ids
			os << "BAD QUERY"<< endl;
			q_state	= false;
			return true;
		}
		
		j=0;
		while(str2[j] == ' '){
  			j++;
 		}

  		for (size_t i = j; i < str2.size(); i++){
    		if (str2[i-1] == ' ' && str2[i] != ' '){
    			os << "BAD QUERY"<< endl;
				q_state	= false;
				return true;
    		}
   			else{
   				if(!isdigit(str2[i])){
     				os << "BAD QUERY"<< endl;
					q_state	= false;
					return true;
    			}
  			}
  		}

		stringstream str_st2(str2);
		str_st2 >> pos2;

		if(!check_pos(S, id, pos1, pos2)){	//Se chequea que las posiciones sean correctas
			os << "NO DATA" << endl;
			q_state = false;
			return true;
		}
		str_st2 >> str4;
		if(str4!="\0"){
			os<< "BAD QUERY"<< "BARRA CERO" << endl;
			q_state = false;
			return true;
		}


	return true;

}


void
query::process_data(sensornet & S, size_t id, size_t & pos1, size_t & pos2){
	if (!S.stree_mode()){
		process_data_std(S, id, pos1, pos2);
	}
	else{
		process_data_stree(S, id, pos1, pos2);
	}
}

void
query::process_data_std(sensornet & S, size_t id, size_t & pos1, size_t & pos2){

	Array <data> aux_arr = 1;
	size_t i, j = 0;
	bool first = true;
	data aux, d;
	
	
	for (i = pos1; i < pos2; i++){
		if (aux_arr.size() == 1 && first == true){
			aux_arr[j] = S[id][i];
			first = false;
		}
		else{
			aux_arr.push(S[id][i]);
		}
		j++;
	}


	for (i = 0; i < aux_arr.size(); i++){
		data aux(aux, aux_arr[i]);
		d = aux;
	}
	

	this-> min_ = d.min();
	this-> max_ = d.max();
	this-> mean_ = d.sum()/d.amount();
	this-> amount_ = d.amount();

}

void
query::process_data_stree(sensornet & S, size_t id, size_t & pos1, size_t & pos2){
	data aux;
	size_t index = floor(log2(pos2-pos1));

	aux = find_data(pos1, pos2, index, S[id].s_tree());

	this-> min_ = aux.min();
	this-> max_ = aux.max();
	this-> mean_ = aux.sum()/aux.amount();
	this-> amount_ = aux.amount();
	
}


data &
query::find_data(size_t pos1, size_t pos2, size_t index, segment_tree & s_tree){
	size_t new_index;
	data d_false;

	while (s_tree[index].pos1() != pos1){
		index++;
	}
	if (pos2 == s_tree[index].pos2()){
		return s_tree[index];
	}
	else if (pos2 > s_tree[index].pos2()){
		new_index = floor(log2(pos2-s_tree[index].pos2()));
		data aux(s_tree[index], find_data(s_tree[index].pos2(),pos2,new_index,s_tree));
		return aux;
	}
	else{
		find_data(pos1,pos2,index+1,s_tree);
	}
}

//Esta funcion devuelve los resultados obtenidos
ostream& operator<<(ostream & os, query Q){

	char ch = ',';
	
	os << Q.mean();
	os << ch;
	os << Q.min();
	os << ch;
	os << Q.max();
	os << ch;
	os << Q.amount();
	os << endl;
	
	return os;
}	



#endif