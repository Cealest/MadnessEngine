/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Patterns/Observer.h"


class FController
{
private:
	class FMoveForwardObserver : public FObserver
	{
	public:
		virtual void OnNotify() override;
		FController* Controller;
	};

	class FMoveLeftObserver : public FObserver
	{
	public:
		virtual void OnNotify() override;
		FController* Controller;
	};

	class FMoveRightObserver : public FObserver
	{
	public:
		virtual void OnNotify() override;
		FController* Controller;
	};

	class FMoveBackObserver : public FObserver
	{
	public:
		virtual void OnNotify() override;
		FController* Controller;
	};

	class FMouseMoveObserver : public FObserver
	{
	public:
		virtual void OnNotify() override;
		FController* Controller;
	};
#if !defined(DELETE_ME)
	class FImportObserver : public FObserver
	{
	public:
		virtual void OnNotify() override;
	};
#endif

public:
	FController();
	~FController();

	void SetCamera(class FCamera* InCamera);

	virtual bool Initialize();
	virtual void Update();

	FMoveForwardObserver MoveForwardObserver;
	FMoveLeftObserver MoveLeftObserver;
	FMoveRightObserver MoveRightObserver;
	FMoveBackObserver MoveBackObserver;
	FMouseMoveObserver MouseMoveObserver;
#if !defined(DELETE_ME)
	FImportObserver ImportObserver;
#endif

private:
	class FCamera* Camera;
};