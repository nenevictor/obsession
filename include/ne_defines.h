#ifndef NE_DEFINES_H
#define NE_DEFINES_H

#include <stdio.h>
#include <malloc.h>
#include <cstdint>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Math block
typedef signed char		     i8_t;
typedef signed short	     i16_t;
typedef signed int		     i32_t;
typedef signed long long     i64_t;
typedef intptr_t		     isize_t;
						     
typedef unsigned char	     u8_t;
typedef unsigned short	     u16_t;
typedef unsigned int	     u32_t;
typedef unsigned long int    u64_t;
typedef size_t				 usize_t;
						     
typedef float	             f32_t;
typedef double               f64_t;
						     
typedef struct		         v2i8				   { i8_t x, y; } v2i8;
typedef struct		         v2i16				   { i16_t x, y; } v2i16;
typedef struct		         v2i32				   { i32_t x, y; } v2i32;
typedef struct		         v2i64				   { i64_t x, y; } v2i64;
typedef struct		         v2u8				   { u8_t x, y; } v2u8;
typedef struct		         v2u16				   { u16_t x, y; } v2u16;
typedef struct		         v2u32				   { u32_t x, y; } v2u32;
typedef struct		         v2u64				   { u64_t x, y; } v2u64;
typedef struct		         v2f32				   { f32_t x, y; } v2f32;
typedef struct		         v2f64				   { f64_t x, y; } v2f64;
					         					   
typedef struct		         v3i8				   { i8_t x, y, z; } v3i8;
typedef struct		         v3i16				   { i16_t x, y, z; } v3i16;
typedef struct		         v3i32				   { i32_t x, y, z; } v3i32;
typedef struct		         v3i64				   { i64_t x, y, z; } v3i64;
typedef struct		         v3u8				   { u8_t x, y, z; } v3u8;
typedef struct		         v3u16				   { u16_t x, y, z; } v3u16;
typedef struct		         v3u32				   { u32_t x, y, z; } v3u32;
typedef struct		         v3u64				   { u64_t x, y, z; } v3u64;
typedef struct		         v3f32				   { f32_t x, y, z; } v3f32;
typedef struct		         v3f64				   { f64_t x, y, z; } v3f64;
					         					   
typedef struct		         v4i8				   { i8_t x, y, z, w; } v4i8;
typedef struct		         v4i16				   { i16_t x, y, z, w; } v4i16;
typedef struct		         v4i32				   { i32_t x, y, z, w; } v4i32;
typedef struct		         v4i64				   { i64_t x, y, z, w; } v4i64;
typedef struct		         v4u8				   { u8_t x, y, z, w; } v4u8;
typedef struct		         v4u16				   { u16_t x, y, z, w; } v4u16;
typedef struct		         v4u32				   { u32_t x, y, z, w; } v4u32;
typedef struct		         v4u64				   { u64_t x, y, z, w; } v4u64;
typedef struct		         v4f32				   { f32_t x, y, z, w; } v4f32;
typedef struct		         v4f64				   { f64_t x, y, z, w; } v4f64;
					         					   
typedef struct		         RGBA				   { u8_t r, g, b, a; } RGBA;
typedef struct		         Rect				   { u32_t x, y, w, h; } Rect;
//typedef struct		         Bitmap				   { u8* data; u32 x, y, comp; } Bitmap; // Bitmap
						     					   
#ifdef __cplusplus		     					   		     					   
#define						 TOI8(_value)          static_cast<i8_t>   (_value)
#define						 TOI16(_value)         static_cast<i16_t>  (_value)
#define						 TOI32(_value)         static_cast<i32_t>  (_value)
#define						 TOI64(_value)         static_cast<i64_t>  (_value)
#define						 TOIS(_value)          static_cast<isize_t>(_value)
							 				       
#define						 TOU8(_value)          static_cast<u8_t>   (_value)
#define						 TOU16(_value)         static_cast<u16_t>  (_value)
#define						 TOU32(_value)         static_cast<u32_t>  (_value)
#define						 TOU64(_value)         static_cast<u64_t>  (_value)
#define						 TOUS(_value)          static_cast<usize_t>(_value)
							 				       
#define						 TOF32(_value)         static_cast<f32_t>  (_value)
#define						 TOF64(_value)         static_cast<f64_t>  (_value)
							 				       
//							 			           
#define						 TOI8P(_value)         reinterpret_cast<i8_t *>    (_value)
#define						 TOI16P(_value)        reinterpret_cast<i16_t *>   (_value)
#define						 TOI32P(_value)        reinterpret_cast<i32_t *>   (_value)
#define						 TOI64P(_value)        reinterpret_cast<i64_t *>   (_value)
#define						 TOISP(_value)         reinterpret_cast<isize_t *> (_value)
							 				     	 					    
#define						 TOU8P(_value)         reinterpret_cast<u8_t *>    (_value)
#define						 TOU16P(_value)        reinterpret_cast<u16_t *>   (_value)
#define						 TOU32P(_value)        reinterpret_cast<u32_t *>   (_value)
#define						 TOU64P(_value)        reinterpret_cast<u64_t *>   (_value)
#define						 TOUSP(_value)         reinterpret_cast<usize_t *> (_value)
							 				       
#define						 TOF32P(_value)        reinterpret_cast<f32_t *>	(_value)
#define						 TOF64P(_value)        reinterpret_cast<f64_t *>	(_value)
							 				       
#else						 				       						 				       
#define						 TOI8(_value)          (i8_t)     (_value)
#define						 TOI16(_value)         (i16_t)    (_value)
#define						 TOI32(_value)         (i32_t)    (_value)
#define						 TOI64(_value)         (i64_t)    (_value)
#define						 TOIS(_value)          (isize_t)  (_value)
							 					   	    
#define						 TOU8(_value)          (u8_t)     (_value)
#define						 TOU16(_value)         (u16_t)    (_value)
#define						 TOU32(_value)         (u32_t)    (_value)
#define						 TOU64(_value)         (u64_t)    (_value)
#define						 TOUS(_value)          (usize_t)  (_value)
							 					   	    
#define						 TOF32(_value)         (f32_t)    (_value)
#define						 TOF64(_value)         (f64_t)    (_value)
							 					   	    
//							 			         	 	    
#define						 TOI8P(_value)         (i8_t *)   (_value)
#define						 TOI16P(_value)        (i16_t *)  (_value)
#define						 TOI32P(_value)        (i32_t *)  (_value)
#define						 TOI64P(_value)        (i64_t *)  (_value)
#define						 TOISP(_value)         (isize_t*) (_value)
							 					   	    
#define						 TOU8P(_value)         (u8_t *)   (_value)
#define						 TOU16P(_value)        (u16_t *)  (_value)
#define						 TOU32P(_value)        (u32_t *)  (_value)
#define						 TOU64P(_value)        (u64_t *)  (_value)
#define						 TOUSP(_value)         (usize_t*) (_value)
							 					   	    
#define						 TOF32P(_value)        (f32_t *)  (_value)
#define						 TOF64P(_value)        (f64_t *)  (_value)
						 						   
#define						 nullptr 0			   
#endif						 // __cplusplus		   
							 				   	   
#define						 POW2(_value)	       ((_value) * (_value))
#define						 POW3(_value)	       ((_value) * (_value) * (_value))
#define						 MAX(_a, _b)           (((_a) > (_b)) ? (_a) : (_b))
#define						 MIN(_a, _b)           (((_a) < (_b)) ? (_a) : (_b))
#define						 CLAMP(_x, _a, _b)     (((_x) < (_a)) ? (_a) : (((_x) > (_b)) ? (_b) : (_x)))
#define						 ABS(_value)           (((_value) < 0) ? -(_value) : (_value))
#define                      LERP(_a, _b, _f)      ((_a) + ((_b) - (_a)) * (_f))
#define                      MOD(_a, _b)           ((_b + (_a % _b)) % _b)

// Flags
#define                      HASFLAG(_value, _flag) ((_value & (_flag)) == (_flag))
#define                      TOGGLEFLAG(_value, _flag) ((_value) &= ~(_flag))
#define                      REMOVEEFLAG(_value, _flag) ((_value) ^= (_flag))

// Useful makros								   
#define						 TOLOWERCASE(_char)    ((_char) - 'A' + 'a')
#define						 TOUPPERCASE(_char)    ((_char) - 'a' + 'A')
							 				       
#define						 UNUSED(_value)        (void)(_value)
												   
#define						 clog(_string)         printf(_string"\n")
#define						 clogf(_fmt, ...)      printf(_fmt"\n", __VA_ARGS__)
							 					   
#define						 CLOG_WARN             "[ WARNING ]: "
#define						 CLOG_INFO             "[ INFO ]:    "
#define						 CLOG_ERR              "[ ERROR ]:   "
							 					   
#ifdef _DEBUG									   
#define						 d_clog(_string)       printf(_string"\n")
#define						 d_clogf(_fmt, ...)    printf(_fmt"\n", __VA_ARGS__)
#define						 d_clogf_fl(_fmt, ...) printf(_fmt" (File: %s; Line: %u)\n", __VA_ARGS__, __FILE__, __LINE__)
#else
#define						 d_clog(_string)  
#define						 d_clogf(_fmt, ...)	   
#define						 d_clogf_fl(_fmt, ...)
#endif

#endif
