#pragma once
#include <iostream>
#include "Sound.hpp"
#include "Silence.hpp"
#include "SoundFile.hpp"
#include "PeriodicalSound.hpp"
#include "Effect.hpp"
#include "Sequence.hpp"


template<typename T>
class Mix :public Sound<T>
{
private:
	Sound<T>** sounds;
	
	size_t size = 0;
	void copy(const Mix<T>& other);
	void free();
public:
	Mix(const T& interval,  Sound<T>** sounds, size_t size);
	Mix(const Mix<T>& other);
	Mix<T>& operator=(const Mix<T>& other) = delete;
	~Mix();
	const T operator[](size_t index)const override;

	Sound<T>* clone() const override;
	size_t getLenght() const override;
	TypeSound getType() const override;

};
template<typename T>
void Mix<T>::copy(const Mix<T>& other)
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
void Mix<T>::free()
{
	for (size_t i = 0; i < this->size; i++)
	{
		delete this->sounds[i];
	}
	delete[] this->sounds;
	
}


template<typename T>
 Mix<T>::Mix(const T& interval,  Sound<T>** sounds, size_t size)
 :Sound<T>(interval)
 {

	 size_t maxSize = 0;
	 
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
			  maxSize = this->sounds[0]->getLenght();
			 for (size_t i = 1; i < size; i++)
			 {
				 if (maxSize < this->sounds[i]->getLenght())
				 {
					 maxSize = this->sounds[i]->getLenght();
					 
				 }
			 }
			 this->lenght = maxSize;
			 this->size = size;
			 this->type = TypeSound::MIX;

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
 Mix<T>::Mix(const Mix<T>& other)
 {
	 copy(other);
 }

 

 template<typename T>
 Mix<T>::~Mix()
 {
	 free();
 }

template<typename T>
 const T Mix<T>::operator[](size_t index) const
{
	 T tmp = 0;
	 if (index < this->lenght)
	 {
		 for (size_t k = 0; k < this->size; k++)
		 {
			 if (index < sounds[k]->getLenght())
			 {
				 tmp += sounds[k]->operator[](index);
			 }

		 }
		 tmp /= this->size;
		 return clip(tmp, this->interval);
	 }
	 else if (index >= this->lenght)
	 {
		 std::cout << "Nevaliden index!" << std::endl;
		 throw std::out_of_range("Index is out of bounds!");
	 }
}

template<typename T>
 Sound<T>* Mix<T>::clone() const
{
	 return new Mix<T>(*this);
}

template<typename T>
 size_t Mix<T>::getLenght() const
{
	 return this->lenght;
}

template<typename T>
 TypeSound Mix<T>::getType() const
{
	return TypeSound::MIX;
}
