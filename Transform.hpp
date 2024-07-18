#pragma once
#include <iostream>
#include "Sound.hpp"

template<typename T>
class Transform
{
protected:
	double coef;
public:
	Transform(double coef)
		:coef(coef)
	{}
	Transform<T>& operator=(const Transform<T>& other) = delete;
	virtual T operator()(T sample, size_t index) const = 0;
	
	virtual Transform<T>* clone() const = 0;
	
	virtual ~Transform() = default;
};

template<typename T>
class Volume :public Transform<T>
{
public:
	Volume(double coef) 
		:Transform<T>(coef)
	{}
	Volume<T>& operator=(const Volume<T>& other) = delete;
T operator()(T sample, size_t index) const override;

Transform<T>* clone() const override;
};

template<typename T>
 T Volume<T>::operator()(T sample, size_t index) const
{
	return this->coef * sample;
}

template<typename T>
 Transform<T>* Volume<T>::clone() const
{
	return new Volume<T>(*this);
}

 template<typename T>
 class FadeIn :public Transform<T>
 {
 private:
	 size_t count;

 public:

	 FadeIn(double coef, size_t count)
		 :Transform<T>(coef), count(count)
	 {}
	 FadeIn<T>& operator=(const FadeIn<T>& other) = delete;
	 T operator()(T sample, size_t index) const override;

	 Transform<T>* clone() const override;
 };
 
 template<typename T>
  T FadeIn<T>::operator()(T sample, size_t index) const
 {
	  if (index < count)
	  {


		  double curr = this->coef / this->count;
		 

		  
		  return curr * sample * index;
	  }
	  else {
		  return sample;

	  }
	  
 }

 template<typename T>
 Transform<T>* FadeIn<T>::clone() const
 {
	 return new FadeIn<T>(*this);
 }


 template <typename T>
 class FadeOut : public Transform<T>
 {
 private:
	 size_t from;
	 size_t to;
 public:
	 FadeOut(double coef,size_t from, size_t to)
		 :Transform<T>(coef), from(from), to(to)
	 {}
	 FadeOut<T>& operator=(const FadeOut& other) = delete;
	 T operator()(T sample, size_t index) const override;

	 Transform<T>* clone() const override;
 };
 template<typename T>
 T FadeOut<T>::operator()(T sample, size_t index) const
 {
	 if (index >= this->from && index <= this->to)
	 {

		 size_t count = this->to - this->from;
		 double step = (1 - this->coef) / count;
		
		// ((step * ((count + number1) - index)) + coef)* sample
		 return ((step * (this->to - index)) + this->coef) * sample;
		 //return ((step * index) + coef) * sample;
	 }
	 else if (index > to)
	 {
		 return sample * this->coef;
	 }
	 else{
		 return sample;

	 }

 }

 template<typename T>
 Transform<T>* FadeOut<T>::clone() const
 {
	 return new FadeOut<T>(*this);
 }
