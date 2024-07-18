#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996);
#include <cstring>
#include <iostream>
#include "Sound.hpp"
#include <fstream>

template<typename T>
class SoundFile :public Sound<T>
{
private:
    T* sample = nullptr;
	char* filename;
	void copy(const SoundFile<T>& other);
    void free();

public:
	SoundFile( const T& interval,const char* filename);
	SoundFile(const SoundFile<T>& other);
	SoundFile<T>& operator=(const SoundFile<T>& other) = delete;
	~SoundFile();

	
	const T operator[](size_t index)const override;

	Sound<T>* clone() const override;
	size_t getLenght() const override;
	TypeSound getType() const override;

};

template<typename T>
void SoundFile<T>::copy(const SoundFile<T>& other)
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
        this->filename = new(std::nothrow) char[strlen(other.filename) + 1];
        strcpy(this->filename, other.filename);
    }
    catch (std::bad_alloc& exp)
    {
        std::cout << "Ne zadelena pamet!" << std::endl;
        delete[] tmp;
    }


    
}

template<typename T>
 void SoundFile<T>::free()
{
     delete[] sample;
     delete[] filename;
}

template<typename T>
SoundFile<T>::SoundFile(const T& interval, const char* filename)
    :Sound<T>(interval)
{
     this->filename = new(std::nothrow) char[strlen(filename) + 1];
     strcpy(this->filename, filename);
     //this->filename[strlen(filename)] = '\0';
     std::ifstream file(this->filename, std::ios::binary);
     if (!file.is_open())
     {
         std::cout << "Error" << std::endl;
     }
     else
     {

         T buff = 0;
         size_t currentPos = file.tellg();
         file.seekg(0, std::ios::end);
         size_t size = file.tellg();

         file.seekg(currentPos);
         
         this->lenght = size / sizeof(T);
 
        
         
         T* tmp;
         try {
             tmp = new(std::nothrow) T[this->lenght];
             if (tmp == nullptr)
             {
                 throw std::bad_alloc();
             }

             for (int j = 0; j < this->getLenght(); j++)
             {
                 file.read((char*)&buff, sizeof(buff));
                 tmp[j] = clip(buff, this->interval);

             }

             this->sample = tmp;

             this->type = TypeSound::SOUNDFILE;
         }
         catch (std::bad_alloc& exp)
         {
             std::cout << "Ne zadelena pamet!" << std::endl;
             delete[] tmp;
         }
     }
 }

template<typename T>
 SoundFile<T>::SoundFile(const SoundFile<T>& other)
     :Sound<T>(other)
{
     copy(other);
}
 
template<typename T>
 SoundFile<T>::~SoundFile()
{
    free();
  
}
 
template<typename T>
 const T SoundFile<T>::operator[](size_t index) const
{
    if (index < this->lenght)
    {
        return this->sample[index];
    }
    else if (index >= this->lenght)
    {
        std::cout << "Nevaliden index!" << std::endl;
        throw std::out_of_range("Index is out of bounds!");
    }
}

template<typename T>
 Sound<T>* SoundFile<T>::clone() const
{
    return new SoundFile<T>(*this);
}

template<typename T>
 size_t SoundFile<T>::getLenght() const
{
    return this->lenght;
}

template<typename T>
 TypeSound SoundFile<T>::getType() const
{
    return TypeSound::SOUNDFILE;
}
