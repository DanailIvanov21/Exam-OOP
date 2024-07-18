#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include "Sound.hpp"
#include <fstream>
template<typename T>
class PeriodicalSound :public Sound<T>
{
private:
	//nie imame masiv ot chilsa T* ot bazoviuq klas
	T* sample = nullptr;
	unsigned int repeat;
	void copy(const PeriodicalSound<T>& other);
	void free();

public:
	PeriodicalSound(const T& interval, const T* sample, size_t lenght,int repeat);
	PeriodicalSound(const PeriodicalSound<T>& other);
	PeriodicalSound<T>& operator=(const PeriodicalSound<T>& other) = delete;
	~PeriodicalSound();
	TypeSound getType() const override;
	Sound<T>* clone() const override;
	size_t getLenght() const override;
	const T operator[](size_t index)const override;

	


};

template<typename T>
inline void PeriodicalSound<T>::copy(const PeriodicalSound<T>& other)
{
	T* tmp;
	try {

		tmp = new(std::nothrow) T[other.lenght];
		if (tmp == nullptr)
		{
			throw std::bad_alloc();
		}
		for (size_t i = 0; i < this->lenght; i++)
		{
			tmp[i] = other.sample[i];
		}
		this->lenght = other.lenght;
		this->sample = tmp;
		this->interval = other.interval;
		this->type = other.type;
		this->repeat = other.repeat;
		
	}
	catch (std::bad_alloc& exp)
	{
		std::cout << "Ne zadelena pamet!" << std::endl;
		delete[] tmp;
	}

}

template<typename T>
inline void PeriodicalSound<T>::free()
{
	delete[] sample;
}

template<typename T>
PeriodicalSound<T>::PeriodicalSound(const T& interval,const T* sample, size_t lenght, int repeat)
	:Sound<T>(interval)
{
	T* tmp;
	try {
		tmp = new(std::nothrow) T[lenght];
		if (tmp == nullptr)
		{
			throw std::bad_alloc();

		}
		for (size_t j = 0; j < lenght; j++)
		{
			tmp[j] = clip(sample[j], this->interval);

		}


		this->sample = tmp;
		this->repeat = repeat;
		this->lenght = lenght * repeat;

		this->type = TypeSound::PERIODICALSOUND;
	}
	catch (std::bad_alloc& exp)
	{
		std::cout << "Ne zadelena pamet!" << std::endl;
		delete[] tmp;
	}
	
}

template<typename T>
 PeriodicalSound<T>::PeriodicalSound(const PeriodicalSound<T>& other)
	 :Sound<T>(other)
 {
	copy(other);

}

template<typename T>
PeriodicalSound<T>::~PeriodicalSound()
{
	free();
}

template<typename T>
TypeSound PeriodicalSound<T>::getType() const
{
	return TypeSound::PERIODICALSOUND;
}

template<typename T>
Sound<T>* PeriodicalSound<T>::clone() const
{
	return new PeriodicalSound<T>(*this);
}

template<typename T>
size_t PeriodicalSound<T>::getLenght() const
{
	return this->lenght;
}

template<typename T>
const T PeriodicalSound<T>::operator[](size_t index) const
{

	if (index < this->lenght)
	{
		
		return clip(this->sample[index % (this->lenght / this->repeat)],this->interval);
	}
	else if (index >= this->lenght)
	{
		std::cout << "Nevaliden index!" << std::endl;
		throw std::out_of_range("Index is out of bounds!");
	}
	
	
}
