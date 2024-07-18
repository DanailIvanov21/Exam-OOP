#pragma once
#include <iostream>
#include "Sound.hpp"
#include "Silence.hpp"
#include "SoundFile.hpp"
#include "PeriodicalSound.hpp"
#include "Mix.hpp"
#include "Effect.hpp"



template<typename T>
class Sequence :public Sound<T>
{
private:
	Sound<T>** sounds;
	size_t size = 0;
	void copy(const Sequence<T>& other);
  void free();
public:
	Sequence(const T& interval, Sound<T>** sounds, size_t size);
	Sequence(const Sequence<T>& other);
	Sequence<T>& operator=(const Sequence<T>& other) = delete;
	~Sequence();


	//T operator[](size_t index) override;
	const T operator[](size_t index)const override;

	Sound<T>* clone() const override;
	size_t getLenght() const override;
	TypeSound getType() const override;

};

template<typename T>
void Sequence<T>::copy(const Sequence<T>& other)
{


	try
	{
		this->sounds = new(std::nothrow)Sound<T>*[other.size];
		if (this->sounds == nullptr)
		{
			throw std::bad_alloc();
		}

		for (size_t i = 0; i < other.size; i++)
		{

			this->sounds[i] = other.sounds[i]->clone();
		}
		//this->sounds = tmp;
		this->size = other.size;
		this->lenght = other.lenght;
		this->interval = other.interval;
		this->type = other.type;

	}
	catch (std::bad_alloc& exp)
	{
		std::cout << "Ne zadelena pamet!" << std::endl;
		free();
		
	}

}

template<typename T>
void Sequence<T>::free()
{
	for (size_t i = 0; i < this->size; i++)
	{
		delete this->sounds[i];
	}
	delete[] this->sounds;

}

template<typename T>
Sequence<T>::Sequence(const T& interval, Sound<T>** sounds, size_t size)
	:Sound<T>(interval)
{
	
	size_t sumSize = 0;
	
	Sound<T>** tmp;
	try
	{
		//this->sounds = new(std::nothrow)Sound<T>*[size];
		tmp = new(std::nothrow)Sound<T>*[size];
		if (tmp == nullptr)
		{
			throw std::bad_alloc();
		}

		for (size_t i = 0; i < size; i++)
		{
			tmp[i] = sounds[i]->clone();
		}
		this->sounds = tmp;
		for (size_t i = 0; i < size; i++)
		{

			sumSize += this->sounds[i]->getLenght();

		}
		this->lenght = sumSize;
		this->size = size;
		this->type = TypeSound::SEQUENCE;

	}
	catch (std::bad_alloc& exp)
	{
		std::cout << "Ne zadelena pamet!" << std::endl;
		//free();
		for (size_t i = 0; i < size;i++)
		{
			delete tmp[i];
		}
		delete[]tmp;
	}


}
template<typename T>
Sequence<T>::Sequence(const Sequence<T>& other)
{
	copy(other);
}



template<typename T>
Sequence<T>::~Sequence()
{
	free();
}


template<typename T>
const T Sequence<T>::operator[](size_t index) const
{
	T tmp;
	size_t i = 0;
	size_t count = 0;
	if (index < this->lenght)
	{
		count = sounds[0]->getLenght();
		while (index >= count)
		{
			index -= count;
			i++;
			count = sounds[i]->getLenght();
		}
		tmp = sounds[i]->operator[](index);
		
		return clip(tmp, this->interval);
		
		
	}
	else if (index >= this->lenght)
	{
		std::cout << "Nevaliden index!" << std::endl;
		throw std::out_of_range("Index is out of bounds!");
	}
}

template<typename T>
Sound<T>* Sequence<T>::clone() const
{
	return new Sequence<T>(*this);
}

template<typename T>
size_t Sequence<T>::getLenght() const
{
	return this->lenght;
}

template<typename T>
TypeSound Sequence<T>::getType() const
{
	return TypeSound::SEQUENCE;
}
