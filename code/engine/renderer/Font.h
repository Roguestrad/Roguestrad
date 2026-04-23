/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#ifndef __FONT_H__
#define __FONT_H__

/*
============
Limit glyphs to a curated BFG charset so the atlas fits at FONT_SIZE.
Only codepoints that map to a non-zero glyph index are collected.

BFG_CHARSET overview:
- 32..126: Basic Latin (printable ASCII)
- 160..255: Latin-1 Supplement (NBSP and Western European accents)
- 338/339: Œ/œ (Latin Extended-A)
- 352/353: Š/š (Latin Extended-A)
- 376: Ÿ (Latin Extended-A)
- 381/382: Ž/ž (Latin Extended-A)
- 402: ƒ (Latin Extended-A)
- 710: ˆ (modifier letter)
- 732: ˜ (small tilde)
- 8211/8212: –/— (en/em dash)
- 8216/8217: ‘/’ (single quotes)
- 8218: ‚ (single low-9 quote)
- 8220/8221: “/” (double quotes)
- 8222: „ (double low-9 quote)
- 8224/8225: †/‡ (dagger/double dagger)
- 8226: • (bullet)
- 8230: … (ellipsis)
- 8240: ‰ (per mille)
- 8249/8250: ‹/› (single angle quotes)
- 8364: € (Euro sign)
- 8482: ™ (trademark)
- 1024..1279: Cyrillic
============
*/

// clang-format off
static const uint32 BFG_CHARSET[] = {
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 160,
	161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
	177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
	193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
	209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
	225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
	241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 338,
	339, 352, 353, 376, 381, 382, 402, 710, 732, 8211, 8212, 8216, 8217, 8218, 8220, 8221,
	8222, 8224, 8225, 8226, 8230, 8240, 8249, 8250, 8364, 8482,
	1024, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039,
	1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1055,
	1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063, 1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071,
	1072, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1086, 1087,
	1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095, 1096, 1097, 1098, 1099, 1100, 1101, 1102, 1103,
	1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119,
	1120, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135,
	1136, 1137, 1138, 1139, 1140, 1141, 1142, 1143, 1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151,
	1152, 1153, 1154, 1155, 1156, 1157, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167,
	1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183,
	1184, 1185, 1186, 1187, 1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199,
	1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215,
	1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231,
	1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247,
	1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 1261, 1262, 1263,
	1264, 1265, 1266, 1267, 1268, 1269, 1270, 1271, 1272, 1273, 1274, 1275, 1276, 1277, 1278, 1279
};
// clang-format on

struct scaledGlyphInfo_t {
	float					top, left;
	float					width, height;
	float					xSkip;
	float					s1, t1, s2, t2;
	const class idMaterial* material;
};

/*!
	\class idFont
	\brief A font management class for handling font resources and rendering information.

	This class provides comprehensive management of font resources including loading, initialization, and rendering data retrieval. It supports both pre-loaded font files and dynamic generation from
   TrueType fonts. The class maintains font properties such as line height, ascender, and character widths at various scales. It also provides functionality for remapping font names, retrieving glyph
   information, and serializing font data for debugging. Resource management is handled through constructor, destructor, and touch methods to ensure proper loading and retention of font assets.

*/
class idFont
{
public:
	//! Constructs a font object with the specified name, initializing its properties and handling font aliasing.
	idFont( const char* n );

	//! Destroys the font object and frees all associated memory.
	~idFont();

	//! Ensures the font resources are loaded and marked for retention.
	void		Touch();

	//! Returns the name of the font.
	const char* GetName() const { return name; }

	//! Returns true if the font has been properly initialized and is ready for use.
	const bool	IsValid() const { return ( alias != nullptr || fontInfo != nullptr ); }

	//! Returns the line height for the font at the specified scale.
	float		GetLineHeight( float scale ) const;

	//! Returns the ascender height of the font scaled by the given factor.
	float		GetAscender( float scale ) const;

	//! Returns the maximum width of any character in the font scaled by the given factor.
	float		GetMaxCharWidth( float scale ) const;

	//! Returns the width of a specified glyph character scaled by a given factor.
	float		GetGlyphWidth( float scale, uint32 idx ) const;

	//! Retrieves scaled glyph information for a specified font index and scale factor
	void		GetScaledGlyph( float scale, uint32 idx, scaledGlyphInfo_t& glyphInfo ) const;

private:
	//! Remaps a font name to a registered font, returning NULL if the font cannot be remapped.
	static idFont* RemapFont( const char* baseName );

	//! Returns the glyph index for a given character index from the font data
	int			   GetGlyphIndex( uint32 idx ) const;

	//! Loads font data from a file or generates it from a TrueType font if the file is not found.
	bool		   LoadFont();

	//! Loads a TrueType font file and initializes font information for rendering.
	bool		   LoadFromTrueTypeFont();

	//! Writes font information to a binary file for future use
	bool		   WriteFont();

	//! Serializes font information to a JSON file for debugging purposes
	void		   DumpFontToJSON();

public:
	struct glyphInfo_t {
		byte		width;	// width of glyph in pixels
		byte		height; // height of glyph in pixels
		signed char top;	// distance in pixels from the base line to the top of the glyph
		signed char left;	// distance in pixels from the pen to the left edge of the glyph
		byte		xSkip;	// x adjustment after rendering this glyph
		uint16		s;		// x offset in image where glyph starts (in pixels)
		uint16		t;		// y offset in image where glyph starts (in pixels)
	};
	struct fontInfo_t {
		struct oldInfo_t {
			float maxWidth;
			float maxHeight;
		} oldInfo[3];

		short			  ascender;
		short			  descender;

		short			  numGlyphs;
		glyphInfo_t*	  glyphData;

		// This is a sorted array of all characters in the font
		// This maps directly to glyphData, so if charIndex[0] is 42 then glyphData[0] is character 42
		uint32*			  charIndex;

		// As an optimization, provide a direct mapping for the ascii character set
		char			  ascii[128];

		const idMaterial* material;
	};

	// base name of the font (minus "fonts/" and ".dat")
	idStr		name;

	// Fonts can be aliases to other fonts
	idFont*		alias;

	// If the font is NOT an alias, this is where the font data is located
	fontInfo_t* fontInfo;
};

#endif
