#pragma once
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
enum TypeSound
{
	SILENCE = 1,
	SOUNDFILE = 2,
	PERIODICALSOUND = 3,
	MIX = 4,
	SEQUENCE = 5,
	EFFECT= 6,
	
};


template<typename T>
class Sound
{
protected:

	size_t lenght = 0;
	T interval;
	TypeSound type; 
	
private:
	
public:

	Sound() = default;
	Sound(const T& interval);
	
	virtual ~Sound() = default;

	
	virtual const T operator[](size_t index) const = 0;

	virtual Sound<T>* clone() const = 0;
	virtual size_t getLenght() const = 0;
	virtual TypeSound getType() const = 0;
	//T clip(T sample, T limit);
	void print() const;
	void save(const char* name) const;
};

template<typename T>
 Sound<T>::Sound(const T& interval)
 {
	
	 if (interval < 0)
	 {
		 this->interval = -1 * interval;
	 }
	 else {
		 this->interval = interval;
	 }
	
}
 
 template<typename T>
 T clip(T sample, T limit)
 {
	 if (sample > limit)
	 {
		 return limit;
	 }
	 else if (sample < limit * -1)
	 {
		 return limit * -1;
	 }
	 return sample;
 }

 template<typename T>
 void Sound<T>::print() const {
	 for (size_t i = 0; i < this->lenght; i++)
	 {
		 std::cout << this->operator[](i) << ' '; 
	 }
	 std::cout << std::endl;
 }

 template<typename T>
 void Sound<T>::save(const char* fileName) const
 {

	 std::ofstream file(fileName, std::ios::binary);
	 if (!file.is_open())
	 {
		 throw "file";
	 }
	 T tmp;
	
	 for (size_t i = 0; i < this->lenght; i++)
	 {
		  tmp = this->operator[](i);
		 file.write((const char*)&tmp, sizeof(T));
	 }
	 file.close();
 }
