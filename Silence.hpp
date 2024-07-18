#pragma once
#include <iostream>
#include "Sound.hpp"

template<typename T>
class Silence :public Sound<T>
{
public:
	Silence( const T& interval, size_t lenght);
	const T operator[](size_t index)const override;

		Sound<T>*clone() const override;
	size_t getLenght() const override;
	 TypeSound getType() const override;
};

template<typename T>
 Silence<T>::Silence(const T& interval, size_t lenght)
	 :Sound<T>(interval)
 {
	
	 this->lenght = lenght;
	 this->type = TypeSound::SILENCE;

 }

template<typename T>
 const T Silence<T>::operator[](size_t index) const
{
	if (index < this->lenght)
	{
	
		return 0;
	}
   else if(index >= this->lenght)
	{
		std::cout << "Nevaliden index!" << std::endl;
      throw std::out_of_range("Index is out of bounds!");
	}


	
}

template<typename T>
Sound<T>* Silence<T>::clone() const
{
	return new Silence<T>(*this);
}

template<typename T>
size_t Silence<T>::getLenght() const
{
	return this->lenght;
}

template<typename T>
TypeSound Silence<T>::getType() const
{
	return TypeSound::SILENCE;
}
