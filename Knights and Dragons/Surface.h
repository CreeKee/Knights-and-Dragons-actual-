/*#pragma once
#include "Window.h"
#include "Exceptions.h"
#include <string>
#include <assert.h>
#include <memory>


class Surface
{
public:
	class Color
	{
	public:
		unsigned int dword;
	public:
		constexpr Color() noexcept : dword(){}
		constexpr Color( const Color& col ) noexcept
			:
			dword( col.dword )
		{}
		constexpr Color( unsigned int dw ) noexcept
			:
			dword( dw )
		{}
		constexpr Color( unsigned char x,unsigned char r,unsigned char g,unsigned char b ) noexcept
			:
			dword( (x << 24u) | (r << 16u) | (g << 8u) | b )
		{}
		constexpr Color( unsigned char r,unsigned char g,unsigned char b ) noexcept
			:
			dword( (255u << 24u) | (r << 16u) | (g << 8u) | b )
		{}
		constexpr Color( Color col,unsigned char x ) noexcept
			:
			Color( (x << 24u) | col.dword )
		{}
		Color& operator =( Color color ) noexcept
		{
			dword = color.dword;
			return *this;
		}
		constexpr unsigned char getX() const noexcept { return dword >> 24u; }
		constexpr unsigned char getA() const noexcept { return getX(); }
		constexpr unsigned char getR() const noexcept { return (dword >> 16u) & 0xFFu; }
		constexpr unsigned char getG() const noexcept { return (dword >> 8u) & 0xFFu; }
		constexpr unsigned char getB() const noexcept { return dword & 0xFFu; }
		void setX( unsigned char x ) noexcept { dword = (dword & 0xFFFFFFu) | (x << 24u); }
		void setA( unsigned char a ) noexcept {	setX( a ); }
		void setR( unsigned char r ) noexcept {	dword = (dword & 0xFF00FFFFu) | (r << 16u); }
		void setG( unsigned char g ) noexcept {	dword = (dword & 0xFFFF00FFu) | (g << 8u); }
		void setB( unsigned char b ) noexcept {	dword = (dword & 0xFFFFFF00u) | b; }
	};

	class Exception : public Exceptions
	{
	public:
		Exception( int line,const char* file,std::string note,std::optional<HRESULT> hr = {} ) noexcept;
		Exception( int line,const char* file,std::string filename,std::string note,std::optional<HRESULT> hr = {} ) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		const std::string& getNote() const noexcept;
	private:
		std::string note;
	};

	Surface( unsigned int width,unsigned int height );
	Surface( Surface&& source ) noexcept = default;
	Surface( Surface& ) = delete;
	Surface& operator=( Surface&& donor ) noexcept = default;
	Surface& operator=( const Surface& ) = delete;
	~Surface() = default;
	void Clear( Color fillValue ) noexcept;
	void PutPixel( unsigned int x,unsigned int y,Color c );
	Color GetPixel( unsigned int x,unsigned int y ) const;
	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	unsigned int GetBytePitch() const noexcept;
	Color* GetBufferPtr() noexcept;
	const Color* GetBufferPtr() const noexcept;
	const Color* GetBufferPtrConst() const noexcept;
	static Surface FromFile( const std::string& name );
	void Save( const std::string& filename ) const;
	bool AlphaLoaded() const noexcept;
private:
	Surface( DirectX::ScratchImage scratch ) noexcept;
private:
	static constexpr DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	DirectX::ScratchImage scratch;
};*/