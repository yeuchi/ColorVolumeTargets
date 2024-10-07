
/* NewSubfileType
*/
#define NEWSUBFILETYPE					254	// kind of data in this subfile
#define DEFAULT 0									// see TIFF specs.  	


/* SubfileType
*/
#define SUBFILETYPE						255	// kinda data in subfile
#define FULL_RES		1							// full res. image 
#define LOW_RES		2							// reduced res. image
#define MULTI_PAGE	3							// single page of multi-page image


/* ImageWidth
*/
#define IMAGEWIDTH						256	// columns of pixels in image


/* ImageLength
*/
#define IMAGELENGTH						257	// rows of pixels in image


/* BitsPerSample		 
*/
#define BITSPERSAMPLE					258	// number of bits per component


/* Compression			
*/
#define COMPRESSION						259	// scheme for image data
#define NO_COMPRESSION	1						// default
#define CCITT3_CMPRSSN	2						// CCITT group 3
#define PCKBIT_CMPRSSN	32773					// pack bits 


/* PhotometricInterpretation
*/
#define PHOTOMETRICINTERPRETATION	262	// color space of image
#define WHITE_ZERO	0							// white is zero for bilevel & gray scale images
#define BLACK_ZERO	1							// black is zero for bilevel & gray scale images
#define RGB_CLR		2							// RGB 
#define PAL_CLR		3							// RGB palette Color
#define MASK			4							// transparency mask
#define CMYK_CLR		5							// CMYK color space
#define YCbCr			6							// Y I Q space
#define CIE_Lab		7							// CIE space

/* Thresholding
*/
#define THRESHOLDING						263	// black and white only
#define NO_HALFTONE	1							// no dithering applied
#define AM_SCREEN		2							// ordered dithering, amplitude modulation applied
#define FM_SCREEN		3							// randomized process, frequency modulation applied


/* CellWidth			
*/
#define CELLWIDTH							264	// width of halftone matrix


/* CellLength			
*/
#define CELLLENGTH						265	// length of halftone matrix


/* FillOrder
*/
#define FILLORDER							266	// logical order of bits in a byte
#define LOWHIGH	1								//	i.e. Pixel 0 store in high-order bit (default)
#define HIGHLOW	2								// i.e. Pixel 0 store in low-order bit


/* Document name
*/
#define DOCUMENTNAMET					269


/* ImageDescription
*/
#define IMAGEDESCRIPTION				270	// string describing image


/* Make
*/
#define MAKE								271	// scanner manufacturer


/* Model
*/
#define MODEL								272	// scanner model name or number


/* StripOffsets
*/
#define STRIPOFFSETS						273	// byte offset to each strip


/* Orientation
*/
#define ORIENTATION						274	// orientation of image 
#define TOP_LEFT	1							// default
#define TOP_RIGHT	2
#define BOT_RIGHT	3
#define BOT_LEFT	4
#define LEFT_TOP	5
#define RIGHT_TOP	6
#define RIGHT_BOT	7
#define LEFT_BOT	8


/* SamplePerPixel
*/
#define SAMPLESPERPIXEL					277	// number of components per pixel


/* RowsPerStrip
*/
#define ROWSPERSTRIP						278	// rows per image strip


/* StripByteCounts
*/
#define STRIPBYTECOUNTS					279	// bytes in strip after compression


/* MinSampleValue
*/
#define MINSAMPLEVALUE					280	// minimum component value used


/* MaxSampleValue
*/
#define MAXSAMPLEVALUE					281	// maximum component value used


/* XResolution
*/
#define XRESOLUTION						282	// num of pixels per resolution unit ( X )


/* YResolution
*/#define YRESOLUTION					283	// num of pixels per resolution unit ( Y )


/* PlanarConfiguration
*/
#define PLANARCONFIGURATION			284	// order the pixels are stored
#define CHUNCKY	1								// Interlaced data i.e. RGBRGBRGB....
#define PLANAR		2								// separate planes i.e. RRRR..... GGGG....


/* Page name
*/
#define PAGENAME							285


/* FreeOffsets
*/
#define FREEOFFSETS						288	// offset of the unused string


/* FreeByteCount
*/
#define FREEBYTECOUNTS					289	// num of bytes of a unused string


/* GrayResponseUnit
*/
#define GRAYRESPONSEUNIT				290	// precision of data in GrayResponseCurve
#define TENTHS			1
#define HUNDREDTHS	2							// default
#define THOUSANDTHS	3
#define TEN_THOUSAN	4
#define HUNDRED_THOU 5


/* GrayResponseCurve							
*/
#define GRAYRESPONSECURVE				291	// optical density of every possible pixel
														// gray scale data ONLY!

/* ResolutionUnit
*/
#define RESOLUTIONUNIT					296	// unit of resolution measure
#define NO_ABSOLUTE	1							// for non-square aspect ratio
#define INCH			2							// default
#define CENT			3							// centimeter


/* Software
*/
#define SOFTWARE							305	// name & version of software


/* DateTime		
*/	
#define DATETIME							306	// date & time of image creation


/* Artist								
*/
#define ARTIST								315	// person who create the image


/* HostComputer
*/
#define HOSTCOMPUTER						316	// computer at time of image creation


/* ColorMap				
*/
#define COLORMAP							320	// color map for palette color image


/* ExtraSamples 
*/
#define EXTRASAMPLES						338	// description of extra component
#define UNSPECIFIED	0							// unspecified
#define ASSOC_ALPHA	1							// associated alpha data
#define UNASS_ALPHA	2							// unassociated alpha data


/* Copyright						
*/
#define COPYRIGHT							33432	// copyright notice