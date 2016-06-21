/* Copyright 2015 Myles Salholm */
#pragma once

class FObserver
{
public:
	FObserver()
	{
		Next = nullptr;
		Prev = nullptr;
	}
	virtual ~FObserver() 
	{
		if (Prev)
			Prev->Next = Next;
		if (Next)
			Next->Prev = Prev;
	}
	virtual void OnNotify() = 0;

	void* Data;

private:
	FObserver* Next;
	FObserver* Prev;

	friend class FSubject;
};

class FSubject
{
public:
	FSubject()
	{
		Head = nullptr;
	}
	void AddObserver(FObserver* InObserver);
	void RemoveObserver(FObserver* InObserver);
	bool ContainsObserver(FObserver* InObserver);

	void Notify();
	void Notify(void* Data);

private:
	// The head of the linked list of observers
	FObserver* Head;
};