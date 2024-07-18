#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Effect.hpp"
const int INTERVAL = 10;
const int MAX = 1000;
int parseInt(const char* buff, size_t &m)
{
	bool flagNum = false;
	int num = 0;
	while (buff[m] != '\0')
	{
		//char c = buff[m];
		if (buff[m] >= '0' && buff[m] <= '9')
		{
			num = num * 10 + (buff[m] - '0');
			flagNum = true;

		}
		else if (flagNum == true)
		{
			m++;
			return num;
		}
		m++;
	}
	return num;
}
size_t parseSizeT(const char* buff, size_t &j)
{
	bool flagNum = false;
	size_t number = 0;
	while (buff[j] != '\0')
	{
		if (buff[j] >= '0' && buff[j] <= '9')
		{
			number = number * 10 + (buff[j] - '0');
			flagNum = true;
		}
		else if (flagNum == true)
		{
			flagNum = false;
			j++;
			return number;
		}
		j++;
	}
	return number;

	
}
size_t parseCountWord(const char* buff, size_t& i)
{
	size_t count = 0;
	bool flag = false;
	while (buff[i] != '\0')
	{
		if (((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' &&
			buff[i] <= 'Z') || (buff[i] >= '0' && buff[i] <= '9'))
			&& !flag)
		{
			flag = true;
			count++;
		}
		else if (flag && buff[i] == ' ')
		{
			flag = false;
		}
		i++;
	}
	return count;
}
const char* parseBuff(const char* buff, size_t& k)
{
	bool flagWord = false;
	char buff2[MAX];
	size_t count2 = 0;
	while (buff[k] != '\0')
	{
		if ((buff[k] >= 'a' && buff[k] <= 'z') || (buff[k] >= 'A' && buff[k] <= 'Z') || buff[k] == '.')
		{
			buff2[count2++] = buff[k];
			flagWord = true;
		}
		else  if (flagWord == true)
		{
			flagWord = false;
			k++;
			buff2[count2] = '\0';
			return buff2;
		}
		k++;
	}
	return buff2;
}
double parseDouble(const char* buff,size_t &k)
{
	double number = 0.0;
	int countT = 0;// broqch za tochkata
	int countToch = 1; // broqch za chislata sled desetichna zapetaq
	bool flag = false;
	while (buff[k] != '\0')
	{
		if (buff[k] == '.')
		{
			countT++;

		}
		else if (countT == 1)
		{
			countToch *= 10;
			number = number + ((buff[k] - '0') / (double)(countToch));
		}
		else if (buff[k] != ' ' && countT == 0)
		{
			number = number * 10 + buff[k] - '0';
			flag = true;
		}
		else if (buff[k] == ' ' && flag)
		{
			k++;
			return number;
		}
		k++;

	}
	return number;
}

template <typename T>
class Playlist
{
private:
	Sound<T>** sounds;
	Sound<T>* seq = nullptr;
	char* fileName;
	size_t sizeSounds = 0;
	size_t capacity = 8;
	void resize();
	void copy(const Playlist<T>& other);
	void free();

public:
	Playlist(const char* fileName);
	Playlist(const Playlist<T>& other);
	Playlist<T>& operator=(const Playlist<T>& other);
	~Playlist();
	size_t getSize() const;
	void readPlaylist(const char* fileName);
	void addFromFile(const char* buff, std::ifstream& file);
	void add(Sound<T>* sound);
	const Sound<T>& operator[](size_t index) const;
	operator Sound<T>& ()
	{
		if (seq != nullptr)
		{
			delete this->seq;
		}
			
			this->seq = new(std::nothrow) Sequence<T>(INTERVAL, this->sounds, this->sizeSounds);
			
			
			return *this->seq;
	}
	

	void savePlaylist(const char* name) const;
	void printPlaylist() const;

	Sound<T>* Song(const char* fileName, double number);
	Sound<T>* Pause(size_t lenght);
	Sound<T>* Fade(const char* fileName, size_t number1, double number2);
	Sound<T>* transition(Sound<T>* s1, Sound<T>* s2, int number1, int number2, double number3);
};

template<typename T>
 void Playlist<T>::resize()
 {
	 Sound<T>** tmp;
	 
	 try
	 {
		 tmp = new(std::nothrow)Sound<T> * [this->capacity *= 2];
		 if (tmp == nullptr)
		 {
			 throw std::bad_alloc();
			 
		 }
		 for (size_t i = 0; i <this->sizeSounds; i++)
		 {
			 tmp[i] = this->sounds[i];
		 }
		 delete[] this->sounds;
		 this->sounds = tmp;
	 }
	 catch (std::bad_alloc& exp)
	 {
		 std::cout << "Ne zadelena pamet!" << std::endl;
		// free();
		 for (size_t i = 0; i < this->sizeSounds;i++)
		 {
			 delete tmp[i];
		 }
		 delete[]tmp;
	 }
}

template<typename T>
void Playlist<T>::copy(const Playlist<T>& other)
{

	
	try
	{
		this->sounds= new(std::nothrow)Sound<T>* [other.capacity];
		if (this->sounds == nullptr)
		{
			throw std::bad_alloc();
		}
		
		for (size_t i = 0; i < other.sizeSounds; i++)
		{
			
			this->sounds[i] = other.sounds[i]->clone();
		}
		this->fileName = new(std::nothrow) char[strlen(other.fileName) + 1];
		strcpy(this->fileName, other.fileName);
		this->sizeSounds = other.sizeSounds;
		this->capacity = other.capacity;
		this->seq = other.seq->clone();
	}
	catch (std::bad_alloc& exp)
	{
		std::cout << "Ne zadelena pamet!" << std::endl;
		free();
	}
	
}

template<typename T>
 void Playlist<T>::free()
{
	 for (size_t i = 0; i < this->sizeSounds; i++)
	 {
		 delete this->sounds[i];
	 }
	 delete[] this->sounds;
	 delete this->fileName;
	 delete this->seq;
}

template<typename T>
 Playlist<T>::Playlist(const char* fileName)
{
	 this->sounds = new(std::nothrow)Sound<T>*[this->capacity];
	this->fileName = new(std::nothrow) char[strlen(fileName) + 1];
	strcpy(this->fileName, fileName);


}

template<typename T>
 Playlist<T>::Playlist(const Playlist<T>& other)
{
	 copy(other);
}

 template<typename T>
 Playlist<T>& Playlist<T>::operator=(const Playlist<T>& other)
 {
	 if (this != &other)
	 {
		 free();
		 copy(other);
	 }
	 return *this;
 }

template<typename T>
inline Playlist<T>::~Playlist()
{
	free();
}
template<typename T>
 void Playlist<T>::add(Sound<T>* sound)
{
	 Sound<T>* tmp = sound->clone(); 
	 //sound = 

	 if (tmp != nullptr)
	 {
		 if (this->sizeSounds == this->capacity)
		 {
			 resize();
		 }
		 this->sounds[this->sizeSounds++] = tmp;
		 
	 }
}
 template<typename T>
 const Sound<T>& Playlist<T>::operator[](size_t index) const
 {
	 if (index < this->sizeSounds)
	 {
		 return *this->sounds[index];
	 }
	 else if (index >= this->sizeSounds)
	 {
		 std::cout << "Nevaliden index!" << std::endl;
		 throw std::out_of_range("Index is out of bounds!");
	 }
 }//operator Sound<T>& ()
 
 template<typename T>
 void Playlist<T>::savePlaylist(const char* name) const
 {
	// Sound<T>* tmp = (Sound<T>&)(*this);
	 //Sound<T>* tmp = this->operator();
	// tmp->save(name);
	 //(Sound<T>&)(*this).save(name);
	 Playlist pl = (*this);
	 Sound<T>& ref = pl;
	 ref.save(name);
	 //delete tmp;
 }
 template<typename T>
 void Playlist<T>::printPlaylist()const
 {
	 for (size_t i = 0; i < this->sizeSounds; i++)
	 {
		 this->sounds[i]->print();
	 }
	 
 }

template<typename T>
void Playlist<T>::readPlaylist(const char* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw "Greshka s otvarqneto na faila!";
	}
	char buff[MAX];
	while (!file.eof())
	{
		//file << buff;
		file.getline(buff, MAX);
		if (strcmp(buff, "") == 0)
		{
			break;
		}
		addFromFile(buff, file);

		for (int l = 0; buff[l] != '\0'; l++)
		{
			buff[l] = '\0';
		}
	}
	file.close();
}

template<typename T>
void Playlist<T>::addFromFile(const char* buff, std::ifstream& file)
{
	//size_t countSize = 3;
	size_t count = 0;//broi dumi
	//size_t sizeBuff = strlen(buff);
	size_t i = 0;

	count = parseCountWord(buff, i);
	
	if (count == 1)
	{
		size_t j = 0;
		size_t number = parseSizeT(buff,j);
		add(Pause(number));
		
	}
	else if (count == 2)
	{

		
		double number = 0.0;
		size_t k = 0;
		char buff2[MAX];
		strcpy(buff2,parseBuff(buff, k));
		number = parseDouble(buff, k);
		add(Song(buff2, number));
		
	}
	else if (count == 3) // ili e prelivka ili e pesen sus zasilvane
	{

		bool isWord = false;
		size_t l = 0;
		while (buff[l] != '\0')
		{
			if ((buff[l] >= 'a' && buff[l] <= 'z') || (buff[l] >= 'A' && buff[l] <= 'Z'))
			{
				isWord = true;
				break;
			}
			
			l++;
		}
		if (isWord)
		{
            //e pesen sus zasilvane
			char buff3[MAX];
			size_t num1 = 0;
			double num2 = 0.0;
		    size_t n = 0;
			strcpy(buff3,parseBuff(buff, n));
			num1 = parseSizeT(buff, n);
			num2 = parseDouble(buff, n);
			add(Fade(buff3, num1, num2));
		}
		else {
			//e prelivka
			
			int num1 = 0;
			int num2 = 0;
			double num3 = 0.0;
           size_t m = 0;
			
			Sound<T>* tmp = this->sounds[this->sizeSounds - 1]->clone();
			//provera dali predishniqt zvuk e pesen

			if (tmp->getType() == TypeSound::SILENCE )
			{
				std::cout << "Nevaliden zvuk!" << std::endl;
				throw "Greshen zvuk";
			}
			num1 = parseInt(buff, m);
			num2 = parseInt(buff, m);
			num3 = parseDouble(buff, m);
			//pochavme sledvasht red
			char buff4[MAX];
				file.getline(buff4, MAX);
				if (strcmp(buff4, "") == 0)
				{
					std::cout << "Nevaliden zvuk!" << std::endl;
					throw "Greshen zvuk";
				}
				size_t t = 0;
				size_t count4 = parseCountWord(buff4, t);//broi dumi

				if (count4 != 2)//dali e pesen
				{
					std::cout << "Nevaliden zvuk!" << std::endl;
					throw "Greshen zvuk";
				}
				double numberS = 0.0;
				
				size_t f = 0;

				
				char buff5[MAX];
				strcpy(buff5,parseBuff(buff4, f));
				numberS = parseDouble(buff4, f);
				
				//Sound<T>* transition(Sound<T>* s1, Sound<T>* s2, int number1, int number2, double number3);
				//add(Song(buff5, number5));
				Sound<T>* tmp2 = Song(buff5, numberS);
				//Song tmp2(buff5, number5);
				
				Sound<T>* tmp3 = transition(tmp, tmp2, num1, num2, num3);
				//add(tmp3);
				//add(tmp2);
				add(tmp3);
				add(tmp2);
			delete tmp;
			delete tmp2;
			delete tmp3;
		}
	}
	
	
}

 template <typename T>
 size_t Playlist<T>::getSize() const
 {
	 return this->sizeSounds;
 }

 template<typename T>
 Sound<T>* Playlist<T>::Song(const char* fileName, double number)
 {

	 SoundFile<T> s(INTERVAL, fileName);
	 Volume<T> tran(number);

	 Effect<T> result(INTERVAL, &s, &tran);
	 //Effect<T> result(INTERVAL, s, tran);
	 Sound<T>* eff = result.clone();

	 return eff;
 }
 template<typename T>
 Sound<T>* Playlist<T>::Pause(size_t lenght)
 {
	 Silence<T> result(INTERVAL, lenght);


	 return result.clone();
 }
 template<typename T>
 Sound<T>* Playlist<T>::Fade(const char* fileName, size_t number1, double number2)
 {

	 SoundFile<T> s(INTERVAL, fileName);
	 FadeIn<T> tran(number2, number1);


	 Effect<T> result(INTERVAL, &s, &tran);
	 Sound<T>* eff = result.clone();
	 return eff;
 }

 template<typename T>
 Sound<T>* Playlist<T>::transition(Sound<T>* s1, Sound<T>* s2, int number1, int number2, double number3)
 {
	
	 FadeOut<T> fo1(number3, number1, number2);
	 Effect<T> eff1(INTERVAL, s1, &fo1);
	 T* arr1 = new(std::nothrow)T[number2];
	 T* arr2 = new(std::nothrow) T[s1->getLenght() - number2];
	 size_t index = 0;
	 for (index; index < number2; index++)
	 {
		 arr1[index] = eff1.operator[](index);
	 }
	 for (size_t i = 0; i < eff1.getLenght() - number2; i++)
	 {
		 arr2[i] = eff1.operator[](index++);
	 }
	 PeriodicalSound<T> pr1(INTERVAL, arr1, number2, 1);
	 PeriodicalSound<T> pr2(INTERVAL, arr2, eff1.getLenght() - number2, 1);

	 FadeOut<T> fo2(0, 0, pr2.getLenght());
	 Effect<T> eff2(INTERVAL, &pr2, &fo2);

	 FadeIn<T> fi1(number3, number2);
	 Effect<T> eff3(INTERVAL, s2, &fi1);
	 Sound<T>** array = new(std::nothrow) Sound<T>*[2];
	 array[0] = eff2.clone();
	 array[1] = eff3.clone();
	 
	 Mix<T> m1(INTERVAL, array, 2);
	 delete array[0];
	 delete array[1];

	 array[0] = pr1.clone();
	 array[1] = m1.clone();
	 Sequence<T> seq(INTERVAL, array, 2);
	 for (size_t i = 0; i < 2; i++)
	 {
		 delete array[i];
	 }

	 delete[] array;
	 delete[] arr1;
	 delete[] arr2;


	 return seq.clone();
 }
