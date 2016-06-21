/* Copyright 2015 Myles Salholm */

#include "Runtime/Public/Patterns/Observer.h"

void FSubject::AddObserver(FObserver* InObserver)
{
	if (InObserver)
	{
		if (Head == nullptr)
		{
			Head = InObserver;
			Head->Next = nullptr;
			Head->Prev = nullptr;
		}
		else if (Head == InObserver)
			return;
		else if (Head->Next)
		{
			FObserver* CurrentObserver = Head->Next;
			while (CurrentObserver != nullptr)
			{
				if (CurrentObserver == InObserver)
					return;
				else if (CurrentObserver->Next)
					CurrentObserver = CurrentObserver->Next;
				else
				{
					CurrentObserver->Next = InObserver;
					InObserver->Prev = CurrentObserver;
				}
			}
		}
		else
		{
			Head->Next = InObserver;
			InObserver->Prev = Head;
		}
	}
}

void FSubject::RemoveObserver(FObserver* InObserver)
{
	if (InObserver != nullptr && Head != nullptr)
	{
		if (Head == InObserver)
		{
			Head = nullptr;
			return;
		}
		if (InObserver->Prev)
			InObserver->Prev->Next = InObserver->Next;
		if (InObserver->Next)
			InObserver->Next->Prev = InObserver->Prev;
	}
}

bool FSubject::ContainsObserver(FObserver* InObserver)
{
	if (InObserver != nullptr && Head != nullptr)
	{
		if (InObserver == Head)
		{
			return true;
		}
		if (Head->Next)
		{
			FObserver* CurrentObserver = Head->Next;
			while (CurrentObserver != nullptr)
			{
				if (CurrentObserver == InObserver)
					return true;
				else
					CurrentObserver = CurrentObserver->Next;
			}
		}		
	}
	return false;
}

void FSubject::Notify()
{
	Notify(nullptr);
}

void FSubject::Notify(void* Data)
{
	if (Head)
	{
		Head->Data = Data;
		Head->OnNotify();
		if (Head->Next)
		{
			FObserver* CurrentObserver = Head->Next;
			while (CurrentObserver != nullptr)
			{
				CurrentObserver->Data = Data;
				CurrentObserver->OnNotify();
				if (CurrentObserver->Next)
					CurrentObserver = CurrentObserver->Next;
			}
		}
	}
}