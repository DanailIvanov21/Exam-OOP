#pragma once
#include <iostream>
#include "Sound.hpp"
#include "Silence.hpp"
#include "SoundFile.hpp"
#include "PeriodicalSound.hpp"
#include "Mix.hpp"
#include "Sequence.hpp"
#include "Transform.hpp"



template<typename T>
class Effect :public Sound<T>
{
private:
	
	 Sound<T>* sound;
	Transform<T>* trs;
	void copy(const Effect& other);
	void free();
	
public:
	//Effect(const T& interval,Sound<T>& given_sound, Transform<T>& trs);
	Effect(const T& interval, Sound<T>* given_sound, Transform<T>* trs);
	Effect(const Effect& other);
	Effect& operator=(const Effect& other) = delete;
	~Effect();

	const T operator[](size_t index)const override;

	Sound<T>* clone() const override;
	size_t getLenght() const override;
	TypeSound getType() const override;
	


};

template<typename T>
 void Effect<T>::copy(const Effect& other)
{
	 this->sound = other.sound->clone();
	 this->trs = other.trs->clone();
	 this->lenght = other.lenght;
	 this->type = other.type;
	 this->interval = other.interval;
}

template<typename T>
void Effect<T>::free()
{
	delete this->sound;
	delete this->trs;
}

template<typename T>
Effect<T>::Effect(const T& interval, Sound<T>*given_sound, Transform<T>* trs)
:Sound<T>(interval)
 {
	 
	this->sound = given_sound->clone();
	this->trs = trs->clone();

	 this->lenght = sound->getLenght();
	 this->type = TypeSound::EFFECT;
	 


}
template<typename T>
 Effect<T>::Effect(const Effect& other)
{
	 copy(other);
}
template<typename T>
inline Effect<T>::~Effect()
{
	free();
	
}


template<typename T>
 const T Effect<T>::operator[](size_t index) const
{
	 if (index < this->lenght)
	 {
		 
         T tmp = this->trs->operator()(this->sound->operator[](index), index);
		 return clip(tmp, this->interval);
	 }
	 else if (index >= this->lenght)
	 {
		 std::cout << "Nevaliden index!" << std::endl;
		 throw std::out_of_range("Index is out of bounds!");
	 }
	
}

template<typename T>
inline Sound<T>* Effect<T>::clone() const
{
	return new Effect<T>(*this);
}

template<typename T>
inline size_t Effect<T>::getLenght() const
{
	return this->lenght;
}

template<typename T>
inline TypeSound Effect<T>::getType() const
{
	return TypeSound::EFFECT;
}

template<typename T>
double Norm(const Sound<T>* given_sound, T coef)
{
	T max = 0;
	for (size_t i = 0; i < given_sound->getLenght(); i++)
	{
		//T sample = given_sound[i];
		T temp = given_sound->operator[](i);
		if (temp < 0)
		{
			temp *= -1;
		}
		if (temp > max)
		{
			max = temp;
		}
	}
	return (double)coef/(double)max;
}
