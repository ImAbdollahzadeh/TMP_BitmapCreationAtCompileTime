#include <iostream>
#define construct typedef
struct _Null {};
template<typename... _ListOfPixel> struct ConcatPixels;
namespace Bitwise {
	template<int First, int Second> struct And {
		static constexpr int result = First & Second;
	};
	template<int First, int Second> struct Or {
		static constexpr int result = First | Second;
	};
	template<int First, int Second> struct LeftBitShift {
		static constexpr int result = First << Second;
	};
	template<int First, int Second> struct RightBitShift {
		static constexpr int result = First >> Second;
	};
}
enum Color {
	RED   = 0x00FF0000,
	GREEN = 0x0000FF00,
	BLUE  = 0x000000FF,
	WHITE = Bitwise::Or <Bitwise::Or <RED, GREEN>::result, BLUE>::result,
	BLACK = Bitwise::And<Bitwise::And<RED, GREEN>::result, BLUE>::result
};
template<typename TrueType, typename FalseType> struct SameAs {
	static constexpr bool result = false;
};
template<typename TrueType> struct SameAs<TrueType, TrueType> {
	static constexpr bool result = true;
};
template<bool cond, typename TrueType, typename FalseType> struct _if {
	typedef TrueType type;
};
template<typename TrueType, typename FalseType> struct _if<false, TrueType, FalseType> {
	typedef FalseType type;
};
template<Color first, Color... colors> struct BlendColors {
	static constexpr Color BlendedResult =
		Bitwise::Or<first, BlendColors<colors...>::BlendedResult>::result;
};
template<Color last> struct BlendColors<last> {
	static constexpr Color BlendedResult = last;
};
template<Color first> struct Subtract_Red {
	static constexpr Color SubtractedResult = Bitwise::Or<first, Color::RED>::result;
};
template<Color first> struct Subtract_Green {
	static constexpr Color SubtractedResult = Bitwise::Or<first, Color::GREEN>::result;
};
template<Color first> struct Subtract_Blue {
	static constexpr Color SubtractedResult = Bitwise::Or<first, Color::BLUE>::result;
};
template<Color first> struct Subtract_White {
	static constexpr Color SubtractedResult = Bitwise::Or<first, Color::WHITE>::result;
};
template<Color first> struct Subtract_Black {
	static constexpr Color SubtractedResult = Bitwise::Or<first, Color::BLACK>::result;
};
template<Color color, template<Color>typename Mode> struct Subtract {
	static constexpr Color SubtractedResult = 
		_if<SameAs<Mode<color>, Subtract_Red<color>>::result, Subtract_Red<color>, 
			_if<SameAs<Mode<color>, Subtract_Green<color>>::result, Subtract_Green<color>, 
				_if<SameAs<Mode<color>, Subtract_Blue<color>>::result, Subtract_Blue<color>, 
					_if<SameAs<Mode<color>, Subtract_White<color>>::result, Subtract_White<color>, 
						_if<SameAs<Mode<color>, Subtract_Black<color>>::result, Subtract_Black<color>, _Null
						>::type
					>::type
				>::type
			>::type
		>::type::SubtractedResult;
};
template<int x, int y, int z> struct RGB {
	static constexpr int Red   = Bitwise::Or<Bitwise::And<Color::RED,   0x00>::result, x>::result;
	static constexpr int Green = Bitwise::Or<Bitwise::And<Color::GREEN, 0x00>::result, y>::result;
	static constexpr int Blue  = Bitwise::Or<Bitwise::And<Color::BLUE,  0x00>::result, z>::result;
};
template<Color color> struct RGBFromColor {
	static constexpr int Red   = Bitwise::Or<Color::RED,   color>::result;
	static constexpr int Green = Bitwise::Or<Color::GREEN, color>::result;
	static constexpr int Blue  = Bitwise::Or<Color::BLUE,  color>::result;
};
template<int x, int y, typename _RGB> struct Pixel {
	static constexpr int X           = x;
	static constexpr int Y           = y;
	static constexpr int Red_Value   = _RGB::Red;
	static constexpr int Green_Value = _RGB::Green;
	static constexpr int Blue_Value  = _RGB::Blue;
};
template<typename FirstPixel, typename... px> struct ListOfPixels {
	typedef FirstPixel head;
	typedef ListOfPixels<px...> rest;
};

template<typename List> struct SizeOfListOfPixels {
	typedef typename List::FirstPixel head;
	typedef typename List::rest rest;
	static constexpr int __size = 1 + SizeOfListOfPixels<rest>::__size;
};
template<> struct SizeOfListOfPixels<_Null> {
	static constexpr int __size = 0;
};

template<int index, typename _ListOfPixel> struct PixelAt {
	typedef typename PixelAt<index - 1, _ListOfPixel>::result result;
};
template<typename _ListOfPixel> struct PixelAt<0, _ListOfPixel> {
	typedef typename _ListOfPixel::head result;
};

template<typename px_first, typename px_others, typename... px, typename... px_2, typename... rest>
struct ConcatPixels<ListOfPixels<px_first, px...>, ListOfPixels<px_others, px_2...>, rest...> {
	typedef ListOfPixels<px_first, px_others, px..., px_2...> first_two;
	typedef typename ConcatPixels<first_two, rest...>::result result;
};
template<typename px_first, typename... px>
struct ConcatPixels<ListOfPixels<px_first, px...>> {
	typedef ListOfPixels<px_first, px...> result;
};

template<typename _ListOfPixels> struct PixelnumbersInTheList {
	static constexpr int number = SizeOfListOfPixels<_ListOfPixels>::__size;
};
template<int xbegin, int xend, int ybegin, int yend, typename _list> struct _YFor {
	typedef RGB<0, 0, 0> Dark_RGB;
	typedef ListOfPixels<_Null> FinalListOfPixels;
	typedef ConcatPixels<FinalListOfPixels, ListOfPixels<typename Pixel<xbegin, ybegin, Dark_RGB>::result>> _FinalListOfPixels_;
	typedef typename _YFor<xbegin, xend, ybegin + 1, yend, _FinalListOfPixels_>::result result;
};
template<int xbegin, int xend, int yend, typename _list> struct _YFor<xbegin, xend, yend, yend, _list> {
	typedef RGB<0, 0, 0> Dark_RGB;
	typedef ConcatPixels<_list, ListOfPixels<typename PixelAt<xend, _list>::result>> result;
};
template<int xbegin, int xend, int ybegin, int yend, typename _list> struct _XFor {
	typedef RGB<0, 0, 0> Dark_RGB;
	typedef ListOfPixels<_Null> FinalListOfPixels;
	typedef ConcatPixels<FinalListOfPixels, ListOfPixels<typename Pixel<xbegin, ybegin, Dark_RGB>::result>> _FinalListOfPixels_;
	typedef typename _YFor<xbegin, xend, ybegin, yend, _FinalListOfPixels_>::result result_star;
	typedef typename _XFor<xbegin + 1, xend, ybegin, yend, result_star>::result result;
};
template<int xend, int ybegin, int yend, typename _list> struct _XFor<xend, xend, ybegin, yend, _list> {
	typedef RGB<0, 0, 0> Dark_RGB;
	typedef ConcatPixels<ListOfPixels<typename Pixel<xend, ybegin, Dark_RGB>::result>> _FinalListOfPixels_;
	typedef typename _YFor<xend, xend, ybegin, yend, _FinalListOfPixels_>::result result;
};

template<int xLimit, int yLimit, typename _ListOfPixels> struct Fill {
	typedef typename _XFor<0, xLimit, 0, yLimit, _ListOfPixels>::result result;
};

template<int Square_Size, typename _ListOfPixels> struct Bitmap {
	static constexpr int squareSize = Square_Size;
	typedef _ListOfPixels __ListOfPixels;
	typedef typename Fill<128,128,_ListOfPixels>::result Array;
};
template<typename _Bitmap1, typename _Bitmap2, typename... _Bitmap> struct AddBitmaps {
	static constexpr int squareSize = _Bitmap1::squareSize;
	typedef typename _Bitmap1::__ListOfPixels firstBitmapListOfPixels;
	typedef typename _Bitmap2::__ListOfPixels secondBitmapListOfPixels;
	typedef ListOfPixels<
							typename ConcatPixels<
								firstBitmapListOfPixels,
								secondBitmapListOfPixels
							>::result
															> resultedFirstTwoConcatanated;
	typedef typename AddBitmaps<Bitmap<squareSize, resultedFirstTwoConcatanated>, _Bitmap...>::result result;
};


template<typename _Bitmap> struct AddBitmaps<_Bitmap, _Null> {
	static constexpr int squareSize = _Bitmap::squareSize;
	typedef typename Bitmap::__ListOfPixels firstBitmapListOfPixels;
	typedef ListOfPixels<typename ConcatPixels<firstBitmapListOfPixels>::result> resultedLastConcatanated;
	typedef typename Bitmap<squareSize, resultedLastConcatanated> result;
};
int main()
{
	construct RGB<1, 1, 1> White;
	
	construct Bitmap<512, ListOfPixels<
											Pixel<0, 1, White>,
											Pixel<1, 1, White>,
											Pixel<8, 6, White>
									  >
																	> bitmap;
	construct Bitmap<512, ListOfPixels<
											Pixel<0, 1, White>
									  >
	                                                                > bitmap2;
	construct Bitmap<512, ListOfPixels<
											Pixel<18, 1, White>,
											Pixel<0, 21, White>
									  >
																	> bitmap3;
	construct AddBitmaps<bitmap, bitmap2, bitmap3> sum;

	/*test*/ construct Subtract<Color::WHITE, Subtract_Green> newColor;
}