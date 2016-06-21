/* Copyright 2015 Myles Salholm */

#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include <math.h>

/* Engine information. */
#define TITLE "Madness Engine v0.03"
#define SHORT_TITLE "MAD"
#define VERSION 0.02f
#define WITH_EDITOR 1
#define SHIPPING 0

/* Target platform. */
#define WINDOWS 1
#if WINDOWS
#define WIN32_LEAN_AND_MEAN
#endif
#define UNIX 0;

/* Global engine constants. */
#define MS_PER_UPDATE (1000.0f / 120.0f)

/* Which graphics APIs we're compiling. */
#define DIRECTX 1
#define OPENGL 0

/* What networking structure we're using. */
#define SERVER 0
#define CLIENT 0
#define PEER_TO_PEER 0

/* Input settings. */
#define KEYBOARD 1
#define KEYS 256

/* Relative paths. */
#define CONTENTPATH "../../../Content/"
#define SOURCEPATH "../../../Source/"
#define INTERMEDIATEPATH "../../../Intermidiate/"
#define LOGPATH "../../../Content/Logs/"

/* Absolute paths, don't use in shipping. */
#define PATH_ABS "C:/MadnessEngine/MadnessEngine/"
#define CONTENTPATH_ABS PATH_ABS"Content/"
#define SOURCEPATH_ABS PATH_ABS"Source/"
#define INTERMEDIATEPATH_ABS PATH_ABS"Intermediate/"
#define LOGPATH_ABS INTERMEDIATEPATH_ABS"Logs/"

/* Math helpers */
#define RotToRad 0.0174532925f

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;

typedef int                 INT;
typedef unsigned int        UINT;

/* Enum that describes shutdown reasons. */
namespace EShutdownReason
{
	enum Type
	{
		Quit,
		FailedToInit,
		FailedToRender,
		Unknown,
	};
}

/* Enum that describes shaders. */
namespace EShader
{
	enum Type
	{
		Color,
		Texture,
		DiffuseLight,
		MAX
	};
}

/* Enum that represents key presses */
namespace EKey
{
	enum Type
	{
		None,
		LeftMouseButton,
		RightMouseButton,
		Cancel,
		MiddleMouseButton,
		Mouse4Button,
		Mouse5Button,
		Backspace = 0x08,
		Tab,
		Clear = 0x0C,
		Enter,
		Shift = 0x10,
		Ctrl,
		Alt,
		Pause,
		CapsLock,
		Escape = 0x1B,
		Spacebar = 0x20,
		PageUp,
		PageDown,
		End,
		Home,
		Left,
		Up,
		Right,
		Down,
		Select,
		Print,
		Execute,
		PrintScreen,
		Insert,
		Delete,
		Help,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		NaturalLeftWindowsKey,
		NaturalRightWindowsKey,
		NaturalApplicationsKey,
		Sleep = 0x5F,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		NumpadMultiply,
		NumpadAdd,
		NumpadEnter,
		NumpadSubtract,
		NumpadDecimal,
		NumpadDivide,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		Numlock = 0x90,
		ScrollLock,
		LeftShift = 0xA0,
		RightShift,
		LeftControl,
		RightControl,
		LeftWindows,
		RightWindows,
		BrowserBack,
		BrowserForward,
		BrwoserRefresh,
		BrowserStop,
		BrowserSearch,
		BrowserFavorites,
		BrowserHome,
		Mute,
		VolumeDown,
		VolumeUp,
		NextTrack,
		PrevTrack,
		StopMusic,
		Mail,
		Media,
		App1,
		App2,
		Packet = 0xE7,
		MAX = 0xFF
	};
}

struct Vector2D
{
	float X;
	float Y;

	Vector2D()
	{
		X = 0;
		Y = 0;
	}

	Vector2D(float x, float y)
	{
		X = x;
		Y = y;
	}

	Vector2D& operator+(Vector2D& other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	Vector2D& operator-(Vector2D& other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	Vector2D& operator*(Vector2D& other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	Vector2D& operator/(Vector2D& other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}

	Vector2D& operator+=(Vector2D& other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}
	Vector2D& operator-=(Vector2D& other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	Vector2D& operator*=(Vector2D& other)
	{
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	Vector2D& operator/=(Vector2D& other)
	{
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	Vector2D& operator=(float other)
	{
		X = other;
		Y = other;
		return *this;
	}
};

struct Vector3D
{
	float X;
	float Y;
	float Z;

	Vector3D()
	{
		X = 0;
		Y = 0;
		Z = 0;
	}
	Vector3D(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	Vector3D& operator+(Vector3D& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	Vector3D& operator-(Vector3D& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	Vector3D& operator*(Vector3D& other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		return *this;
	}
	Vector3D& operator/(Vector3D& other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		return *this;
	}
	Vector3D& operator+=(Vector3D& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	Vector3D& operator-=(Vector3D& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	Vector3D& operator*=(Vector3D& other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		return *this;
	}
	Vector3D& operator/=(Vector3D& other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		return *this;
	}
	Vector3D& operator=(float other)
	{
		X = other;
		Y = other;
		Z = other;
		return *this;
	}

	Vector3D operator*(float other)
	{
		Vector3D returnVal = *this;
		returnVal.X *= other;
		returnVal.Y *= other;
		returnVal.Z *= other;
		return returnVal;
	}
};

struct Vector4D
{
	float X;
	float Y;
	float Z;
	float W;

	Vector4D()
	{
		X = 0;
		Y = 0;
		Z = 0;
		W = 0;
	}
	Vector4D(float x, float y, float z, float w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}
	Vector4D(Vector3D& otherXYZ, float w)
	{
		X = otherXYZ.X;
		Y = otherXYZ.Y;
		Z = otherXYZ.Z;
		W = w;
	}

	Vector4D& operator+(Vector4D& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		W += other.W;
		return *this;
	}
	Vector4D& operator-(Vector4D& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		W -= other.W;
		return *this;
	}
	Vector4D& operator*(Vector4D& other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		W *= other.W;
		return *this;
	}
	Vector4D& operator/(Vector4D& other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		W /= other.W;
		return *this;
	}
	Vector4D& operator+=(Vector4D& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		W += other.W;
		return *this;
	}
	Vector4D& operator-=(Vector4D& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		W -= other.W;
		return *this;
	}
	Vector4D& operator*=(Vector4D& other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		W *= other.W;
		return *this;
	}
	Vector4D& operator/=(Vector4D& other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		W /= other.W;
		return *this;
	}
	Vector4D& operator=(float other)
	{
		X = other;
		Y = other;
		Z = other;
		W = other;
		return *this;
	}
};

struct Matrix4x4
{
	Vector4D row[4];

	Matrix4x4()
	{
		row[0] = 0;
		row[1] = 0;
		row[2] = 0;
		row[3] = 0;
	}

	Matrix4x4& operator+(Matrix4x4& other)
	{
		row[0] += other.row[0];
		row[1] += other.row[1];
		row[2] += other.row[2];
		row[3] += other.row[3];
		return *this;
	}
	Matrix4x4& operator-(Matrix4x4& other)
	{
		row[0] -= other.row[0];
		row[1] -= other.row[1];
		row[2] -= other.row[2];
		row[3] -= other.row[3];
		return *this;
	}
	Matrix4x4& operator*(Matrix4x4& other)
	{
		row[0] *= other.row[0];
		row[1] *= other.row[1];
		row[2] *= other.row[2];
		row[3] *= other.row[3];
		return *this;
	}
	Matrix4x4& operator/(Matrix4x4& other)
	{
		row[0] /= other.row[0];
		row[1] /= other.row[1];
		row[2] /= other.row[2];
		row[3] /= other.row[3];
		return *this;
	}
	Matrix4x4& operator+=(Matrix4x4& other)
	{
		row[0] += other.row[0];
		row[1] += other.row[1];
		row[2] += other.row[2];
		row[3] += other.row[3];
		return *this;
	}
	Matrix4x4& operator-=(Matrix4x4& other)
	{
		row[0] -= other.row[0];
		row[1] -= other.row[1];
		row[2] -= other.row[2];
		row[3] -= other.row[3];
		return *this;
	}
	Matrix4x4 dot(Matrix4x4& other)
	{
		Matrix4x4 returnVal;

		returnVal.row[0].X =
			row[0].X * other.row[0].X +
			row[0].Y * other.row[1].X +
			row[0].Z * other.row[2].X + 
			row[0].W * other.row[3].X;
		returnVal.row[0].Y = 
			row[0].X * other.row[0].Y +
			row[0].Y * other.row[1].Y + 
			row[0].Z * other.row[2].Y +
			row[0].W * other.row[3].Y;
		returnVal.row[0].Z = 
			row[0].X * other.row[0].Z +
			row[0].Y * other.row[1].Z +
			row[0].Z * other.row[2].Z +
			row[0].W * other.row[3].Z;
		returnVal.row[0].W =
			row[0].X * other.row[0].W +
			row[0].Y * other.row[1].W +
			row[0].Z * other.row[2].W + 
			row[0].W * other.row[3].W;
		
		returnVal.row[1].X = 
			row[1].X * other.row[0].X +
			row[1].Y * other.row[1].X +
			row[1].Z * other.row[2].X +
			row[1].W * other.row[3].X;
		returnVal.row[1].Y = 
			row[1].X * other.row[0].Y +
			row[1].Y * other.row[1].Y +
			row[1].Z * other.row[2].Y +
			row[1].W * other.row[3].Y;
		returnVal.row[1].Z =
			row[1].X * other.row[0].Z +
			row[1].Y * other.row[1].Z +
			row[1].Z * other.row[2].Z +
			row[1].W * other.row[3].Z;
		returnVal.row[1].W = 
			row[1].X * other.row[0].W +
			row[1].Y * other.row[1].W +
			row[1].Z * other.row[2].W +
			row[1].W * other.row[3].W;
		
		returnVal.row[2].X = 
			row[2].X * other.row[0].X +
			row[2].Y * other.row[1].X +
			row[2].Z * other.row[2].X +
			row[2].W * other.row[3].X;
		returnVal.row[2].Y = 
			row[2].X * other.row[0].Y +
			row[2].Y * other.row[1].Y +
			row[2].Z * other.row[2].Y +
			row[2].W * other.row[3].Y;
		returnVal.row[2].Z =
			row[2].X * other.row[0].Z +
			row[2].Y * other.row[1].Z +
			row[2].Z * other.row[2].Z +
			row[2].W * other.row[3].Z;
		returnVal.row[2].W = 
			row[2].X * other.row[0].W +
			row[2].Y * other.row[1].W +
			row[2].Z * other.row[2].W +
			row[2].W * other.row[3].W;
		
		returnVal.row[3].X = 
			row[3].X * other.row[0].X +
			row[3].Y * other.row[1].X +
			row[3].Z * other.row[2].X +
			row[3].W * other.row[3].X;
		returnVal.row[3].Y = 
			row[3].X * other.row[0].Y +
			row[3].Y * other.row[1].Y +
			row[3].Z * other.row[2].Y +
			row[3].W * other.row[3].Y;
		returnVal.row[3].Z = 
			row[3].X * other.row[0].Z +
			row[3].Y * other.row[1].Z +
			row[3].Z * other.row[2].Z +
			row[3].W * other.row[3].Z;
		returnVal.row[3].W =
			row[3].X * other.row[0].W +
			row[3].Y * other.row[1].W +
			row[3].Z * other.row[2].W + 
			row[3].W * other.row[3].W;
		return returnVal;
	}
	Matrix4x4& operator/=(Matrix4x4& other)
	{
		row[0] /= other.row[0];
		row[1] /= other.row[1];
		row[2] /= other.row[2];
		row[3] /= other.row[3];
		return *this;
	}
	Matrix4x4& operator=(float other)
	{
		row[0] = other;
		row[1] = other;
		row[2] = other;
		row[3] = other;
		return *this;
	}

	Vector4D operator*(Vector4D Translation)
	{
		Vector4D returnVal;
		returnVal.X = 
			row[0].X * Translation.X +
			row[0].Y * Translation.Y +
			row[0].Z * Translation.Z +
			row[0].W * Translation.W;
		returnVal.Y = 
			row[1].X * Translation.X +
			row[1].Y * Translation.Y +
			row[1].Z * Translation.Z +
			row[1].W * Translation.W;
		returnVal.Z = 
			row[2].X * Translation.X +
			row[2].Y * Translation.Y +
			row[2].Z * Translation.Z +
			row[2].W * Translation.W;
		returnVal.W = 
			row[3].X * Translation.X +
			row[3].Y * Translation.Y +
			row[3].Z * Translation.Z +
			row[3].W * Translation.W;
		return returnVal;
	}

	Vector3D operator*(Vector3D Translation)
	{
		Vector3D returnVal;
		returnVal.X =
			row[0].X * Translation.X +
			row[0].Y * Translation.Y +
			row[0].Z * Translation.Z;
		returnVal.Y =
			row[1].X * Translation.X +
			row[1].Y * Translation.Y +
			row[1].Z * Translation.Z;
		returnVal.Z =
			row[2].X * Translation.X +
			row[2].Y * Translation.Y +
			row[2].Z * Translation.Z;
		return returnVal;
	}

	static Matrix4x4 Identity()
	{
		static Matrix4x4 id;
		id.row[0].X = 1.0f;
		id.row[1].Y = 1.0f;
		id.row[2].Z = 1.0f;
		id.row[3].W = 1.0f;
		return id;
	}

	void Translate(Vector3D Translation)
	{
		row[0].W += Translation.X;
		row[1].W += Translation.Y;
		row[2].W += Translation.Z;
	}

	void RotX(float Angle)
	{
		Matrix4x4 rotation = Identity();
		rotation.row[1].Y = cos(Angle);
		rotation.row[1].Z = -sin(Angle);
		rotation.row[2].Y = sin(Angle);
		rotation.row[2].Z = cos(Angle);
		*this = this->dot(rotation);
	}
	void RotY(float Angle)
	{
		Matrix4x4 rotation = Identity();
		rotation.row[0].X = cos(Angle);
		rotation.row[0].Z = sin(Angle);
		rotation.row[2].X = -sin(Angle);
		rotation.row[2].Z = cos(Angle);
		*this = this->dot(rotation);
	}
	void RotZ(float Angle)
	{
		Matrix4x4 rotation = Identity();
		rotation.row[0].X = cos(Angle);
		rotation.row[0].Y = -sin(Angle);
		rotation.row[1].X = sin(Angle);
		rotation.row[1].Y = cos(Angle);
		*this = this->dot(rotation);
	}

	void Rotate(Vector3D Rotation)
	{
		// @TODO: Fix what's going wrong with these rotations
		if (Rotation.X != 0)
			RotX(Rotation.X * RotToRad);
		if (Rotation.Y != 0)
			RotY(Rotation.Y * RotToRad);
		if (Rotation.Z != 0)
			RotZ(Rotation.Z * RotToRad);
	}
};

#endif