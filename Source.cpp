#include <iostream>
#define construct typedef
struct null {};

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
	RED   = 0x00ff0000,
	GREEN = 0x0000ff00,
	BLUE  = 0x000000ff,
	WHITE = Bitwise::Or <Bitwise::Or <RED, GREEN>::result, BLUE>::result,
	BLACK = Bitwise::And<Bitwise::And<RED, GREEN>::result, BLUE>::result
};
template<int r, int g, int b> struct RGB {
	static constexpr int Red   = r;
	static constexpr int Green = g;
	static constexpr int Blue  = b;
};
template<int _color> struct RGB_From_Color {
	static constexpr int Red   = Bitwise::RightBitShift<Bitwise::And <_color, 0x00ff0000>::result, 0x10>::result;
	static constexpr int Green = Bitwise::RightBitShift<Bitwise::And <_color, 0x0000ff00>::result, 0x08>::result;
	static constexpr int Blue  = Bitwise::RightBitShift<Bitwise::And <_color, 0x000000ff>::result, 0x00>::result;
};
template<int x, int y, typename _RGB> struct pixel {
	static constexpr int X = x;
	static constexpr int Y = y;
	static constexpr int Red   = _RGB::Red;
	static constexpr int Green = _RGB::Green;
	static constexpr int Blue  = _RGB::Blue;
};
struct _int_0x14_byte {
	const int X_coor;
	const int Y_coor;
	const int RED;
	const int GREEN;
	const int BLUE;
	constexpr _int_0x14_byte(int F, int S, int r, int g, int b) :
		X_coor(F), Y_coor(S), RED(r), GREEN(g), BLUE(b)
	{}
	constexpr _int_0x14_byte() :
		X_coor(0), Y_coor(0), RED(0), GREEN(0), BLUE(0)
	{}
};
typedef _int_0x14_byte __int20;
constexpr int ModifiedNumberOfArgumentsInOf__int20 = (sizeof(__int20) - 0x01)/sizeof(int);

template<typename pix> struct PixelEncoder {
	static constexpr __int20 encode = __int20(
		pix::X, pix::Y, pix::Red, pix::Green, pix::Blue
	);
};
template<size_t sz, typename...px> struct __Bitmap {
	static constexpr int BitmapPixelSize = sz;
	static constexpr int BitmapLightPixelsListSize = sizeof...(px);
	static constexpr __int20 LightBitmap[sizeof...(px)] = { PixelEncoder<px>::encode... };
};
template<size_t Size> struct Combine {
	static int FinalArray[Size*Size * sizeof(__int20)];
	static int* perform(const __int20* lightPixels, size_t sizeOfPixelList) {
		size_t count = 0;
		while (count != sizeOfPixelList) {
			for (size_t i = 0; i != Size*Size; ++i) {
				__int20 tmp = lightPixels[count];
				if (i != (tmp.X_coor)*Size + (tmp.Y_coor)) {
					FinalArray[i * 4] = i;
					FinalArray[(i * 4) + 1] = 0;
					FinalArray[(i * 4) + 2] = 0;
					FinalArray[(i * 4) + 3] = 0;
				}
				else {
					FinalArray[i * 4] = (tmp.X_coor)*Size + (tmp.Y_coor);
					FinalArray[(i * 4) + 1] = tmp.RED;
					FinalArray[(i * 4) + 2] = tmp.GREEN;
					FinalArray[(i * 4) + 3] = tmp.BLUE;
					++count;
				}
			}
		}
		return FinalArray;
	}
};
template<size_t Size> int Combine<Size>::FinalArray[Size*Size * sizeof(__int20)] = { 0 };

/*  Here is the code for SmallBitmap  */

namespace SmallBitmap {
	template<int Coord, typename _RGB> struct EncodedPixel {
		static constexpr int coordinate = Coord;
		static constexpr int Red_Value = _RGB::Red;
		static constexpr int Green_Value = _RGB::Green;
		static constexpr int Blue_Value = _RGB::Blue;
	};
	template<typename FirstEncodedPixel, typename... EncPx> struct ListOfPixels {
		typedef FirstEncodedPixel head;
		typedef ListOfPixels<EncPx...> rest;
	};
	template<typename FirstEncodedPixel> struct ListOfPixels<FirstEncodedPixel> {
		typedef FirstEncodedPixel head;
		typedef null rest;
	};
	template<typename... _ListOfPixel> struct ConcatanatePixels;
	template<typename EncPx_first, typename EncPx_others, typename... EncPx, typename... EncPx_2, typename... rest>
	struct ConcatanatePixels<ListOfPixels<EncPx_first, EncPx...>, ListOfPixels<EncPx_others, EncPx_2...>, rest...> {
		typedef ListOfPixels<EncPx_first, EncPx_others, EncPx..., EncPx_2...> first_two;
		typedef typename ConcatanatePixels<first_two, rest...>::result result;
	};
	template<typename EncPx_first, typename... EncPx> struct ConcatanatePixels<ListOfPixels<EncPx_first, EncPx...>> {
		typedef ListOfPixels<EncPx_first, EncPx...> result;
	};
	template<typename EncPx_first> struct ConcatanatePixels<EncPx_first> {
		typedef ListOfPixels<EncPx_first> result;
	};
	template<typename EncPx_first> struct ConcatanatePixels<EncPx_first, ListOfPixels<null>> {
		typedef ListOfPixels<EncPx_first> result;
	};
	template<int at, typename list, typename EncPx> struct SubstitudeAt {
		typedef ListOfPixels<typename list::head> HeadList;
		typedef typename SubstitudeAt<at - 1, typename list::rest, EncPx>::result TailList;
		typedef typename ConcatanatePixels<HeadList, TailList>::result result;
	};
	template<typename list, typename EncPx> struct SubstitudeAt<0, list, EncPx> {
		typedef ListOfPixels<EncPx> temp_result;
		typedef typename ConcatanatePixels<temp_result, typename list::rest>::result result;
	};
	template<int begin, int end> struct ZeroList {
		typedef RGB<0, 0, 0> Black;
		typedef EncodedPixel<begin, Black> InitialDense_Pixel;
		typedef ListOfPixels<InitialDense_Pixel> HeadList;
		typedef typename ZeroList<begin + 1, end>::result TailList;
		typedef typename ConcatanatePixels<HeadList, TailList>::result result;
	};
	template<int end> struct ZeroList<end, end> {
		typedef RGB<0, 0, 0> Black;
		typedef EncodedPixel<end, Black> InitialDense_Pixel;
		typedef ListOfPixels<InitialDense_Pixel> result;
	};
	template struct ZeroList<90,  500>;
	template struct ZeroList<180, 500>;
	template struct ZeroList<270, 500>;
	template struct ZeroList<360, 500>;
	template struct ZeroList<450, 500>;
	template<int _Size, typename list, typename _List> struct ListMaker;
	template<int _Size, typename list, typename First_EncPx, typename ...EncPxs> struct ListMaker<_Size, list, ListOfPixels<First_EncPx, EncPxs...>> {
		static constexpr int temp_coord = First_EncPx::coordinate;
		typedef typename SubstitudeAt<temp_coord, list, First_EncPx>::result substituted_list;
		typedef typename ListMaker<_Size, substituted_list, ListOfPixels<EncPxs...>>::result result;
	};
	template<int _Size, typename list, typename First_EncPx> struct ListMaker<_Size, list, ListOfPixels<First_EncPx>> {
		static constexpr int temp_coord = First_EncPx::coordinate;
		typedef typename SubstitudeAt<temp_coord, list, First_EncPx>::result result;
	};
	template<int Square_Size, typename _ListOfPixels> struct SmallBitmap {
		static constexpr int SquareSize = Square_Size;
		typedef typename ZeroList<0, Square_Size>::result blackList;
		typedef typename ListMaker<Square_Size, blackList, _ListOfPixels>::result result;
	};
	template<typename list> struct Print {
		static void pr() {
			std::cout << list::head::coordinate << "-> R: " <<
				list::head::Red_Value << "-> G: " <<
				list::head::Green_Value << "-> B: " <<
				list::head::Blue_Value << std::endl;
			Print<typename list::rest>::pr();
		}
	};
	template<> struct Print<null> {
		static void pr() {
		}
	};
}
	template<size_t __sz, typename...__px> struct Bitmap {
		static constexpr int BitmapPixelSize = __Bitmap<__sz, __px...>::BitmapPixelSize;
		static constexpr int BitmapLightPixelsListSize = __Bitmap<__sz, __px...>::BitmapLightPixelsListSize;
		static constexpr __int20 LightBitmap[sizeof...(__px)] = __Bitmap<__sz, __px...>::LightBitmap;
		static int* DecodedPixelList;
	};
template<size_t __sz, typename...__px> int* Bitmap<__sz, __px...>::DecodedPixelList = Combine<__Bitmap<__sz, __px...>::BitmapPixelSize>::perform(
	__Bitmap<__sz, __px...>::LightBitmap, __Bitmap<__sz, __px...>::BitmapLightPixelsListSize
);
struct Print {
	static void print(int* _bitmapList, size_t sz) {
		for (size_t i = 0; i != sz*sz; ++i)
		{
			std::cout << "Pixel (" << i / sz << "," << i - (i / sz)*sz << "):" << std::endl;
			std::cout << *(_bitmapList + (4 * i)) << std::endl;
			std::cout << *(_bitmapList + (4 * i) + 1) << std::endl;
			std::cout << *(_bitmapList + (4 * i) + 2) << std::endl;
			std::cout << *(_bitmapList + (4 * i) + 3) << std::endl;
			std::cout << "....." << std::endl;
		}
	}
};
int main()
{
	construct RGB<102, 11, 76>             BLENDEDCOLOR; 
	construct RGB_From_Color<Color::WHITE> WHITE;
	construct pixel<1, 2, BLENDEDCOLOR>    px1;
	construct pixel<9, 6, WHITE>           px2;
	construct pixel<9, 9, WHITE>           px3;

	constexpr int sz1 = 10;
	typedef Bitmap<sz1, px1, px2, px3> bitmap;
	Print::print (
		bitmap::DecodedPixelList,
		bitmap::BitmapPixelSize
	);
	construct SmallBitmap::SmallBitmap<500, SmallBitmap::ListOfPixels<
																		SmallBitmap::EncodedPixel<1,   WHITE>,
																		SmallBitmap::EncodedPixel<12,  WHITE>,
																		SmallBitmap::EncodedPixel<15,  WHITE>,
																		SmallBitmap::EncodedPixel<45,  WHITE>,
																		SmallBitmap::EncodedPixel<96,  WHITE>,
																		SmallBitmap::EncodedPixel<127, WHITE>
																												>
																													> bitmap_2;
	SmallBitmap::Print<bitmap_2::result>::pr();
}
