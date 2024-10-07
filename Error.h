
/* GENERAL
*/
#define OKAY								  0
#define BAD									 -1


/* DirEntry
*/
#define BADDATATYPE						-170	// TAG specified an unexpected data type
#define NODATATOWRITE					-171	
#define BADWRITE							-172



/* IFD
*/
#define BADTAGREAD						-200
#define BADTAGWRITE						-201
#define NODIRENTRY						-202
#define UNKNOWNTAG						-203
#define BAD_NUM_DE						-204


/* Image
*/
#define BADTYPE							-180
#define MISSINIMAGE						-181
#define BADIMAGESIZE						-182


/* TIFF
*/
#define BADFILEOPEN						-150
#define BADBYTEORDER						-151
#define NOIFD								-152
#define NOSUCHIFD							-153
#define NOROOMFORTAG						-154
#define NOSUCHTAG							-155
#define NOSUCHIMAGE						-156
#define ALREADYALLOCATED				-157
#define MISSINDATABUF					-158
#define BADTIFFVERSION					-159
#define BADTIFFHEADER					-160


/* SimpleTIFF
*/
#define BADCOMPRESSION					-190
#define UNEQUAL_BPP						-191
#define BAD_SPP							-192
#define BADENTRYTYPE						-193
#define BADPLANARCONFIG					-194
#define BAD_COLOR_SPACE					-195


/* Target
*/
#define COL_ROW_NOT_ODD					-300
#define COLOR_DELTA_TOO_HIGH			-301
#define COLOR_DELTA_TOO_LOW			-302
#define BAD_CREATE_BMP					-303
#define BAD_TEXT_OUT						-304
#define BAD_CREATE_STRUCT				-305
#define BAD_CREATE_DC					-306



/* Spline
*/
#define PROBLEM_X_DATA					-500
#define PROBLEM_Y_DATA					-501


/* CreateMedia
*/
#define FILE_NOT_OPEN					-600


/* InkDlg
*/
#define BAD_SWATCH_WID					-701
#define BAD_SWATCH_LEN					-702
#define BAD_NUM_ITERA					-703
#define BAD_COLOR_DELTA					-704
#define BAD_DPI							-705
#define BAD_DENSITY_ENTRY				-706
#define BAD_PERCENT_ENTRY				-707