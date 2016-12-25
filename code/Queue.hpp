/*
 * Queue.hpp
 *
 *  Created on: 14 Nov 2016
 *      Author: Nico
 */

#ifndef LOGGER_SRC_QUEUE_HPP_
#define LOGGER_SRC_QUEUE_HPP_

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

#include <iostream>
#include <thread>

template <class T>
class Queue
{
public:
	/**
	 * Function that passes a string into the queue
	 * @param t t containing the information that has to be parsed into the queue
	 */
	void enqueue(T t);

	/**
	 * Function that returns the first item of the queue. Will also remove the item from the queue.
	 * @return Returns the value placed on the first location of the queue.
	 */
	T dequeue();

	/**
	 * Function that returns the size of the queue. This function is for unittesting.
	 * @return Returns the value placed on the first location of the queue.
	 */
	unsigned long getQueueSize() const;

	/**
	 * Function that returns the queue. This function is for unittesting.
	 * @return Returns the queue.
	 */
	const std::queue<T>& getQueue() const;

	bool empty() const ;

	const T& front() const;

private:
	std::queue<T> queue;
	std::mutex queueBusy;
	std::condition_variable queueFull;
};

template<class T>
inline void Queue<T>::enqueue(T t)
{
	std::unique_lock<std::mutex> lock(queueBusy);
	queue.push(t);
	queueFull.notify_one();
}

template<class T>
inline T Queue<T>::dequeue()
{
	std::unique_lock<std::mutex> lock(queueBusy);

	while (queue.size()==0) queueFull.wait(lock);  //lock op de queue. Wanneer notify_one() wordt aangeroepen wordt de queueFull getriggered

	T front = queue.front();
	queue.pop();
	std::cout<<"Dequeue: "<<front<<std::endl;
	return front;
}

template<class T>
inline unsigned long Queue<T>::getQueueSize() const
{
	return queue.size();
}

template<class T>
inline const std::queue<T>& Queue<T>::getQueue() const
{
	return queue;
}

template<class T>
inline bool Queue<T>::empty() const
{
	return queue.size() == 0;
}

template<class T>
inline const T& Queue<T>::front() const
{
	return queue.front();
}

#endif /* LOGGER_SRC_QUEUE_HPP_ */
