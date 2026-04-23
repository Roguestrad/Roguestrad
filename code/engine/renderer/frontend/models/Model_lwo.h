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

#ifndef __LWO2_H__
#define __LWO2_H__

/*
======================================================================

	LWO2 loader. (LightWave Object)

	Ernie Wright  17 Sep 00

======================================================================
*/

/* chunk and subchunk IDs */

#define LWID_( a, b, c, d ) ( ( ( a ) << 24 ) | ( ( b ) << 16 ) | ( ( c ) << 8 ) | ( d ) )

#define ID_FORM				LWID_( 'F', 'O', 'R', 'M' )
#define ID_LWO2				LWID_( 'L', 'W', 'O', '2' )
#define ID_LWOB				LWID_( 'L', 'W', 'O', 'B' )

/* top-level chunks */
#define ID_LAYR				LWID_( 'L', 'A', 'Y', 'R' )
#define ID_TAGS				LWID_( 'T', 'A', 'G', 'S' )
#define ID_PNTS				LWID_( 'P', 'N', 'T', 'S' )
#define ID_BBOX				LWID_( 'B', 'B', 'O', 'X' )
#define ID_VMAP				LWID_( 'V', 'M', 'A', 'P' )
#define ID_VMAD				LWID_( 'V', 'M', 'A', 'D' )
#define ID_POLS				LWID_( 'P', 'O', 'L', 'S' )
#define ID_PTAG				LWID_( 'P', 'T', 'A', 'G' )
#define ID_ENVL				LWID_( 'E', 'N', 'V', 'L' )
#define ID_CLIP				LWID_( 'C', 'L', 'I', 'P' )
#define ID_SURF				LWID_( 'S', 'U', 'R', 'F' )
#define ID_DESC				LWID_( 'D', 'E', 'S', 'C' )
#define ID_TEXT				LWID_( 'T', 'E', 'X', 'T' )
#define ID_ICON				LWID_( 'I', 'C', 'O', 'N' )

/* polygon types */
#define ID_FACE				LWID_( 'F', 'A', 'C', 'E' )
#define ID_CURV				LWID_( 'C', 'U', 'R', 'V' )
#define ID_PTCH				LWID_( 'P', 'T', 'C', 'H' )
#define ID_MBAL				LWID_( 'M', 'B', 'A', 'L' )
#define ID_BONE				LWID_( 'B', 'O', 'N', 'E' )

/* polygon tags */
#define ID_SURF				LWID_( 'S', 'U', 'R', 'F' )
#define ID_PART				LWID_( 'P', 'A', 'R', 'T' )
#define ID_SMGP				LWID_( 'S', 'M', 'G', 'P' )

/* envelopes */
#define ID_PRE				LWID_( 'P', 'R', 'E', ' ' )
#define ID_POST				LWID_( 'P', 'O', 'S', 'T' )
#define ID_KEY				LWID_( 'K', 'E', 'Y', ' ' )
#define ID_SPAN				LWID_( 'S', 'P', 'A', 'N' )
#define ID_TCB				LWID_( 'T', 'C', 'B', ' ' )
#define ID_HERM				LWID_( 'H', 'E', 'R', 'M' )
#define ID_BEZI				LWID_( 'B', 'E', 'Z', 'I' )
#define ID_BEZ2				LWID_( 'B', 'E', 'Z', '2' )
#define ID_LINE				LWID_( 'L', 'I', 'N', 'E' )
#define ID_STEP				LWID_( 'S', 'T', 'E', 'P' )

/* clips */
#define ID_STIL				LWID_( 'S', 'T', 'I', 'L' )
#define ID_ISEQ				LWID_( 'I', 'S', 'E', 'Q' )
#define ID_ANIM				LWID_( 'A', 'N', 'I', 'M' )
#define ID_XREF				LWID_( 'X', 'R', 'E', 'F' )
#define ID_STCC				LWID_( 'S', 'T', 'C', 'C' )
#define ID_TIME				LWID_( 'T', 'I', 'M', 'E' )
#define ID_CONT				LWID_( 'C', 'O', 'N', 'T' )
#define ID_BRIT				LWID_( 'B', 'R', 'I', 'T' )
#define ID_SATR				LWID_( 'S', 'A', 'T', 'R' )
#define ID_HUE				LWID_( 'H', 'U', 'E', ' ' )
#define ID_GAMM				LWID_( 'G', 'A', 'M', 'M' )
#define ID_NEGA				LWID_( 'N', 'E', 'G', 'A' )
#define ID_IFLT				LWID_( 'I', 'F', 'L', 'T' )
#define ID_PFLT				LWID_( 'P', 'F', 'L', 'T' )

/* surfaces */
#define ID_COLR				LWID_( 'C', 'O', 'L', 'R' )
#define ID_LUMI				LWID_( 'L', 'U', 'M', 'I' )
#define ID_DIFF				LWID_( 'D', 'I', 'F', 'F' )
#define ID_SPEC				LWID_( 'S', 'P', 'E', 'C' )
#define ID_GLOS				LWID_( 'G', 'L', 'O', 'S' )
#define ID_REFL				LWID_( 'R', 'E', 'F', 'L' )
#define ID_RFOP				LWID_( 'R', 'F', 'O', 'P' )
#define ID_RIMG				LWID_( 'R', 'I', 'M', 'G' )
#define ID_RSAN				LWID_( 'R', 'S', 'A', 'N' )
#define ID_TRAN				LWID_( 'T', 'R', 'A', 'N' )
#define ID_TROP				LWID_( 'T', 'R', 'O', 'P' )
#define ID_TIMG				LWID_( 'T', 'I', 'M', 'G' )
#define ID_RIND				LWID_( 'R', 'I', 'N', 'D' )
#define ID_TRNL				LWID_( 'T', 'R', 'N', 'L' )
#define ID_BUMP				LWID_( 'B', 'U', 'M', 'P' )
#define ID_SMAN				LWID_( 'S', 'M', 'A', 'N' )
#define ID_SIDE				LWID_( 'S', 'I', 'D', 'E' )
#define ID_CLRH				LWID_( 'C', 'L', 'R', 'H' )
#define ID_CLRF				LWID_( 'C', 'L', 'R', 'F' )
#define ID_ADTR				LWID_( 'A', 'D', 'T', 'R' )
#define ID_SHRP				LWID_( 'S', 'H', 'R', 'P' )
#define ID_LINE				LWID_( 'L', 'I', 'N', 'E' )
#define ID_LSIZ				LWID_( 'L', 'S', 'I', 'Z' )
#define ID_ALPH				LWID_( 'A', 'L', 'P', 'H' )
#define ID_AVAL				LWID_( 'A', 'V', 'A', 'L' )
#define ID_GVAL				LWID_( 'G', 'V', 'A', 'L' )
#define ID_BLOK				LWID_( 'B', 'L', 'O', 'K' )

/* texture layer */
#define ID_TYPE				LWID_( 'T', 'Y', 'P', 'E' )
#define ID_CHAN				LWID_( 'C', 'H', 'A', 'N' )
#define ID_NAME				LWID_( 'N', 'A', 'M', 'E' )
#define ID_ENAB				LWID_( 'E', 'N', 'A', 'B' )
#define ID_OPAC				LWID_( 'O', 'P', 'A', 'C' )
#define ID_FLAG				LWID_( 'F', 'L', 'A', 'G' )
#define ID_PROJ				LWID_( 'P', 'R', 'O', 'J' )
#define ID_STCK				LWID_( 'S', 'T', 'C', 'K' )
#define ID_TAMP				LWID_( 'T', 'A', 'M', 'P' )

/* texture coordinates */
#define ID_TMAP				LWID_( 'T', 'M', 'A', 'P' )
#define ID_AXIS				LWID_( 'A', 'X', 'I', 'S' )
#define ID_CNTR				LWID_( 'C', 'N', 'T', 'R' )
#define ID_SIZE				LWID_( 'S', 'I', 'Z', 'E' )
#define ID_ROTA				LWID_( 'R', 'O', 'T', 'A' )
#define ID_OREF				LWID_( 'O', 'R', 'E', 'F' )
#define ID_FALL				LWID_( 'F', 'A', 'L', 'L' )
#define ID_CSYS				LWID_( 'C', 'S', 'Y', 'S' )

/* image map */
#define ID_IMAP				LWID_( 'I', 'M', 'A', 'P' )
#define ID_IMAG				LWID_( 'I', 'M', 'A', 'G' )
#define ID_WRAP				LWID_( 'W', 'R', 'A', 'P' )
#define ID_WRPW				LWID_( 'W', 'R', 'P', 'W' )
#define ID_WRPH				LWID_( 'W', 'R', 'P', 'H' )
#define ID_VMAP				LWID_( 'V', 'M', 'A', 'P' )
#define ID_AAST				LWID_( 'A', 'A', 'S', 'T' )
#define ID_PIXB				LWID_( 'P', 'I', 'X', 'B' )

/* procedural */
#define ID_PROC				LWID_( 'P', 'R', 'O', 'C' )
#define ID_COLR				LWID_( 'C', 'O', 'L', 'R' )
#define ID_VALU				LWID_( 'V', 'A', 'L', 'U' )
#define ID_FUNC				LWID_( 'F', 'U', 'N', 'C' )
#define ID_FTPS				LWID_( 'F', 'T', 'P', 'S' )
#define ID_ITPS				LWID_( 'I', 'T', 'P', 'S' )
#define ID_ETPS				LWID_( 'E', 'T', 'P', 'S' )

/* gradient */
#define ID_GRAD				LWID_( 'G', 'R', 'A', 'D' )
#define ID_GRST				LWID_( 'G', 'R', 'S', 'T' )
#define ID_GREN				LWID_( 'G', 'R', 'E', 'N' )
#define ID_PNAM				LWID_( 'P', 'N', 'A', 'M' )
#define ID_INAM				LWID_( 'I', 'N', 'A', 'M' )
#define ID_GRPT				LWID_( 'G', 'R', 'P', 'T' )
#define ID_FKEY				LWID_( 'F', 'K', 'E', 'Y' )
#define ID_IKEY				LWID_( 'I', 'K', 'E', 'Y' )

/* shader */
#define ID_SHDR				LWID_( 'S', 'H', 'D', 'R' )
#define ID_DATA				LWID_( 'D', 'A', 'T', 'A' )

/* generic linked list */

typedef struct st_lwNode {
	struct st_lwNode *next, *prev;
	void*			  data;
} lwNode;

/* plug-in reference */

typedef struct st_lwPlugin {
	struct st_lwPlugin *next, *prev;
	char*				ord;
	char*				name;
	int					flags;
	void*				data;
} lwPlugin;

/* envelopes */

typedef struct st_lwKey {
	struct st_lwKey *next, *prev;
	float			 value;
	float			 time;
	unsigned int	 shape; /* ID_TCB, ID_BEZ2, etc. */
	float			 tension;
	float			 continuity;
	float			 bias;
	float			 param[4];
} lwKey;

typedef struct st_lwEnvelope {
	struct st_lwEnvelope *next, *prev;
	int					  index;
	int					  type;
	char*				  name;
	lwKey*				  key; /* linked list of keys */
	int					  nkeys;
	int					  behavior[2]; /* pre and post (extrapolation) */
	lwPlugin*			  cfilter;	   /* linked list of channel filters */
	int					  ncfilters;
} lwEnvelope;

#define BEH_RESET	  0
#define BEH_CONSTANT  1
#define BEH_REPEAT	  2
#define BEH_OSCILLATE 3
#define BEH_OFFSET	  4
#define BEH_LINEAR	  5

/* values that can be enveloped */

typedef struct st_lwEParam {
	float val;
	int	  eindex;
} lwEParam;

typedef struct st_lwVParam {
	float val[3];
	int	  eindex;
} lwVParam;

/* clips */

typedef struct st_lwClipStill {
	char* name;
} lwClipStill;

typedef struct st_lwClipSeq {
	char* prefix; /* filename before sequence digits */
	char* suffix; /* after digits, e.g. extensions */
	int	  digits;
	int	  flags;
	int	  offset;
	int	  start;
	int	  end;
} lwClipSeq;

typedef struct st_lwClipAnim {
	char* name;
	char* server; /* anim loader plug-in */
	void* data;
} lwClipAnim;

typedef struct st_lwClipXRef {
	char*			  string;
	int				  index;
	struct st_lwClip* clip;
} lwClipXRef;

typedef struct st_lwClipCycle {
	char* name;
	int	  lo;
	int	  hi;
} lwClipCycle;

typedef struct st_lwClip {
	struct st_lwClip *next, *prev;
	int				  index;
	unsigned int	  type; /* ID_STIL, ID_ISEQ, etc. */
	union {
		lwClipStill still;
		lwClipSeq	seq;
		lwClipAnim	anim;
		lwClipXRef	xref;
		lwClipCycle cycle;
	} source;
	float	  start_time;
	float	  duration;
	float	  frame_rate;
	lwEParam  contrast;
	lwEParam  brightness;
	lwEParam  saturation;
	lwEParam  hue;
	lwEParam  gamma;
	int		  negative;
	lwPlugin* ifilter; /* linked list of image filters */
	int		  nifilters;
	lwPlugin* pfilter; /* linked list of pixel filters */
	int		  npfilters;
} lwClip;

/* textures */

typedef struct st_lwTMap {
	lwVParam size;
	lwVParam center;
	lwVParam rotate;
	lwVParam falloff;
	int		 fall_type;
	char*	 ref_object;
	int		 coord_sys;
} lwTMap;

typedef struct st_lwImageMap {
	int		 cindex;
	int		 projection;
	char*	 vmap_name;
	int		 axis;
	int		 wrapw_type;
	int		 wraph_type;
	lwEParam wrapw;
	lwEParam wraph;
	float	 aa_strength;
	int		 aas_flags;
	int		 pblend;
	lwEParam stck;
	lwEParam amplitude;
} lwImageMap;

#define PROJ_PLANAR		 0
#define PROJ_CYLINDRICAL 1
#define PROJ_SPHERICAL	 2
#define PROJ_CUBIC		 3
#define PROJ_FRONT		 4

#define WRAP_NONE		 0
#define WRAP_EDGE		 1
#define WRAP_REPEAT		 2
#define WRAP_MIRROR		 3

typedef struct st_lwProcedural {
	int	  axis;
	float value[3];
	char* name;
	void* data;
} lwProcedural;

typedef struct st_lwGradKey {
	struct st_lwGradKey *next, *prev;
	float				 value;
	float				 rgba[4];
} lwGradKey;

typedef struct st_lwGradient {
	char*	   paramname;
	char*	   itemname;
	float	   start;
	float	   end;
	int		   repeat;
	lwGradKey* key;	 /* array of gradient keys */
	short*	   ikey; /* array of interpolation codes */
} lwGradient;

typedef struct st_lwTexture {
	struct st_lwTexture *next, *prev;
	char*				 ord;
	unsigned int		 type;
	unsigned int		 chan;
	lwEParam			 opacity;
	short				 opac_type;
	short				 enabled;
	short				 negative;
	short				 axis;
	union {
		lwImageMap	 imap;
		lwProcedural proc;
		lwGradient	 grad;
	} param;
	lwTMap tmap;
} lwTexture;

/* values that can be textured */

typedef struct st_lwTParam {
	float	   val;
	int		   eindex;
	lwTexture* tex; /* linked list of texture layers */
} lwTParam;

typedef struct st_lwCParam {
	float	   rgb[3];
	int		   eindex;
	lwTexture* tex; /* linked list of texture layers */
} lwCParam;

/* surfaces */

typedef struct st_lwGlow {
	short	 enabled;
	short	 type;
	lwEParam intensity;
	lwEParam size;
} Glow;

typedef struct st_lwRMap {
	lwTParam val;
	int		 options;
	int		 cindex;
	float	 seam_angle;
} lwRMap;

typedef struct st_lwLine {
	short		   enabled;
	unsigned short flags;
	lwEParam	   size;
} lwLine;

typedef struct st_lwSurface {
	struct st_lwSurface *next, *prev;
	char*				 name;
	char*				 srcname;
	lwCParam			 color;
	lwTParam			 luminosity;
	lwTParam			 diffuse;
	lwTParam			 specularity;
	lwTParam			 glossiness;
	lwRMap				 reflection;
	lwRMap				 transparency;
	lwTParam			 eta;
	lwTParam			 translucency;
	lwTParam			 bump;
	float				 smooth;
	int					 sideflags;
	float				 alpha;
	int					 alpha_mode;
	lwEParam			 color_hilite;
	lwEParam			 color_filter;
	lwEParam			 add_trans;
	lwEParam			 dif_sharp;
	lwEParam			 glow;
	lwLine				 line;
	lwPlugin*			 shader; /* linked list of shaders */
	int					 nshaders;
} lwSurface;

/* vertex maps */

typedef struct st_lwVMap {
	struct st_lwVMap *next, *prev;
	char*			  name;
	unsigned int	  type;
	int				  dim;
	int				  nverts;
	int				  perpoly;
	int*			  vindex; /* array of point indexes */
	int*			  pindex; /* array of polygon indexes */
	float**			  val;

	// added by duffy
	int				  offset;
} lwVMap;

typedef struct st_lwVMapPt {
	lwVMap* vmap;
	int		index; /* vindex or pindex element */
} lwVMapPt;

/* points and polygons */

typedef struct st_lwPoint {
	float	  pos[3];
	int		  npols; /* number of polygons sharing the point */
	int*	  pol;	 /* array of polygon indexes */
	int		  nvmaps;
	lwVMapPt* vm; /* array of vmap references */
} lwPoint;

typedef struct st_lwPolVert {
	int		  index; /* index into the point array */
	float	  norm[3];
	int		  nvmaps;
	lwVMapPt* vm; /* array of vmap references */
} lwPolVert;

typedef struct st_lwPolygon {
	union {
		intptr_t   index;
		lwSurface* ptr;
	} surf;
	int			 part;		/* part index */
	int			 smoothgrp; /* smoothing group */
	int			 flags;
	unsigned int type;
	float		 norm[3];
	int			 nverts;
	lwPolVert*	 v; /* array of vertex records */
} lwPolygon;

typedef struct st_lwPointList {
	int		 count;
	int		 offset; /* only used during reading */
	lwPoint* pt;	 /* array of points */
} lwPointList;

typedef struct st_lwPolygonList {
	int		   count;
	int		   offset;	/* only used during reading */
	int		   vcount;	/* total number of vertices */
	int		   voffset; /* only used during reading */
	lwPolygon* pol;		/* array of polygons */
} lwPolygonList;

/* geometry layers */

typedef struct st_lwLayer {
	struct st_lwLayer *next, *prev;
	char*			   name;
	int				   index;
	int				   parent;
	int				   flags;
	float			   pivot[3];
	float			   bbox[6];
	lwPointList		   point;
	lwPolygonList	   polygon;
	int				   nvmaps;
	lwVMap*			   vmap; /* linked list of vmaps */
} lwLayer;

/* tag strings */

typedef struct st_lwTagList {
	int	   count;
	int	   offset; /* only used during reading */
	char** tag;	   /* array of strings */
} lwTagList;

/* an object */

typedef struct st_lwObject {
	ID_TIME_T	timeStamp;
	lwLayer*	layer; /* linked list of layers */
	lwEnvelope* env;   /* linked list of envelopes */
	lwClip*		clip;  /* linked list of clips */
	lwSurface*	surf;  /* linked list of surfaces */
	lwTagList	taglist;
	int			nlayers;
	int			nenvs;
	int			nclips;
	int			nsurfs;
} lwObject;

//! Loads a LightWave object file and returns a parsed object structure
lwObject*	lwGetObject( const char* filename, unsigned int* failID, int* failpos );

//! Frees the memory used by an lwObject structure and its nested components.
void		lwFreeObject( lwObject* object );

//! Frees the memory used by an lwLayer structure and its associated data.
void		lwFreeLayer( lwLayer* layer );

//! Frees the memory allocated for a lwPointList structure and its contained data.
void		lwFreePoints( lwPointList* point );

//! Frees the memory allocated for a lwPolygonList structure and its nested data.
void		lwFreePolygons( lwPolygonList* plist );

//! Reads point records from a PNTS chunk in an LWO2 file and adds them to the provided point list.
int			lwGetPoints( idFile* fp, int cksize, lwPointList* point );

//! Calculates the bounding box for a point list only if it hasn't been initialized yet
void		lwGetBoundingBox( lwPointList* point, float bbox[] );

//! Allocates or extends polygon arrays to hold new records
int			lwAllocPolygons( lwPolygonList* plist, int npols, int nverts );

/*!
	\brief Reads polygon records from a POLS chunk in an LWO2 file and adds them to the provided polygon list

	This function processes a POLS chunk from an LWO2 file format, parsing polygon data including vertex indices and flags. It allocates memory for the polygons and vertices based on the chunk size,
   then populates the lwPolygonList with the parsed polygon information. The function handles error cases by freeing allocated memory and returning failure codes. The ptoffset parameter is used to
   adjust vertex indices when adding polygons to the list. The function returns 1 on success and 0 on failure.

	\param fp File pointer to the LWO2 file being read
	\param cksize Size of the POLS chunk in bytes
	\param plist Pointer to the polygon list structure to be populated
	\param ptoffset Offset value to adjust vertex indices by
	\return Returns 1 on successful parsing and population of the polygon list, or 0 on failure
*/
int			lwGetPolygons( idFile* fp, int cksize, lwPolygonList* plist, int ptoffset );

//! Calculates polygon normals using the cross product of the first and last edges
void		lwGetPolyNormals( lwPointList* point, lwPolygonList* polygon );

//! Populates each point with the indices of polygons that share the point
int			lwGetPointPolygons( lwPointList* point, lwPolygonList* polygon );

/*!
	\brief Resolves polygon surface tags into actual surface pointers, creating default surfaces for missing tags.

	This function takes a list of polygons and a tag list containing surface names, and resolves the surface tags into actual lwSurface pointers. It iterates through the polygon list and for each
   polygon, it looks up the corresponding surface in the provided surface list. If a surface is not found for a given tag, a default surface is created and added to the surface list. The function also
   updates the polygon's surface pointer to point to the resolved surface. The nsurfs parameter is updated to reflect the number of surfaces in the list.

	\param polygon Pointer to the polygon list structure containing the polygons to resolve
	\param tlist Pointer to the tag list structure containing the surface names
	\param surf Double pointer to the surface list structure where resolved surfaces are stored
	\param nsurfs Pointer to an integer that tracks the number of surfaces in the list
	\return Returns 1 on successful resolution, 0 on failure due to allocation issues or invalid indices
*/
int			lwResolvePolySurfaces( lwPolygonList* polygon, lwTagList* tlist, lwSurface** surf, int* nsurfs );

//! Computes vertex normals by averaging polygon normals for shared points
void		lwGetVertNormals( lwPointList* point, lwPolygonList* polygon );

//! Frees the memory used by a lwTagList structure.
void		lwFreeTags( lwTagList* tlist );

//! Reads tag strings from a TAGS chunk in an LWO2 file and adds them to the provided tag list array.
int			lwGetTags( idFile* fp, int cksize, lwTagList* tlist );

/*!
	\brief Reads polygon tags from a PTAG chunk in an LWO2 file format

	This function processes polygon tag data from a PTAG chunk within an LWO2 file, handling different tag types such as surface, particle, and smooth group tags. It reads the tag type from the file,
   validates it against known identifiers, and then reads polygon-tag pairs to populate the appropriate fields in the polygon list. The function supports seeking to skip over chunks that don't match
   the expected tag types. It is primarily used during LWO2 model loading to associate polygon data with tags, such as surface materials, particle systems, or smoothing groups.

	\param fp File pointer to the LWO2 file being read
	\param cksize Size of the chunk to read
	\param tlist Pointer to the tag list structure to populate
	\param plist Pointer to the polygon list structure to associate tags with
	\return Returns 1 on successful processing, 0 if there is an error in reading or processing
*/
int			lwGetPolygonTags( idFile* fp, int cksize, lwTagList* tlist, lwPolygonList* plist );

//! Frees the memory allocated for a lwVMap structure and its embedded data members.
void		lwFreeVMap( lwVMap* vmap );

/*!
	\brief Reads and parses a vertex map (VMAP or VMAD) chunk from an LWO2 file format.

	This function reads a vertex map chunk from a file pointer, parses its structure, and allocates memory for the vertex map data. It supports both VMAP and VMAD chunks with optional per-polygon
   mapping. The chunk contains vertex indices, optional polygon indices, and vertex data values. The function handles memory allocation and cleanup, returning NULL on failure.

	\param fp File pointer to read the chunk data from
	\param cksize Size of the chunk to read in bytes
	\param ptoffset Point offset for mapping vertex indices
	\param poloffset Polygon offset for mapping polygon indices
	\param perpoly Flag indicating if mapping is per-polygon (1) or per-vertex (0)
	\return Pointer to a new lwVMap structure containing parsed vertex map data, or NULL on failure
*/
lwVMap*		lwGetVMap( idFile* fp, int cksize, int ptoffset, int poloffset, int perpoly );

//! Populates the lwVMapPt structure for each point based on the provided vmap data.
int			lwGetPointVMaps( lwPointList* point, lwVMap* vmap );

//! Populates the lwVMapPt structure for each vertex in the polygon list based on the provided vmap data
int			lwGetPolyVMaps( lwPolygonList* polygon, lwVMap* vmap );

//! Frees the memory allocated for a lightwave clip structure and its associated resources
void		lwFreeClip( lwClip* clip );

//! Reads image references from a CLIP chunk in an LWO2 file and returns a parsed clip structure.
lwClip*		lwGetClip( idFile* fp, int cksize );

//! Returns an lwClip pointer given a clip index from a list of clips
lwClip*		lwFindClip( lwClip* list, int index );

//! Frees the memory used by an lwEnvelope structure and its associated data.
void		lwFreeEnvelope( lwEnvelope* env );

//! Reads an ENVL chunk from an LWO2 file and returns a populated envelope structure.
lwEnvelope* lwGetEnvelope( idFile* fp, int cksize );

//! Returns an lwEnvelope pointer from a list given an envelope index
lwEnvelope* lwFindEnvelope( lwEnvelope* list, int index );
float		lwEvalEnvelope( lwEnvelope* env, float time );

//! Frees the memory used by an lwPlugin structure and its associated fields
void		lwFreePlugin( lwPlugin* p );

//! Frees the memory used by an lwTexture structure and its associated data.
void		lwFreeTexture( lwTexture* t );

//! Frees the memory allocated for an lwSurface structure and its associated resources.
void		lwFreeSurface( lwSurface* surf );

//! Reads a texture map header from a SURF.BLOK in an LWO2 file.
int			lwGetTHeader( idFile* fp, int hsz, lwTexture* tex );

//! Reads texture map data from an LWO2 file surface block
int			lwGetTMap( idFile* fp, int tmapsz, lwTMap* tmap );

//! Reads an image map from a SURF.BLOK in an LWO2 file
int			lwGetImageMap( idFile* fp, int rsz, lwTexture* tex );

//! Reads a procedural texture definition from a LWO2 file block
int			lwGetProcedural( idFile* fp, int rsz, lwTexture* tex );

//! Reads gradient data from a LWO2 file's SURF.BLOK section into a texture structure
int			lwGetGradient( idFile* fp, int rsz, lwTexture* tex );

//! Reads an lwTexture from a SURF.BLOK in an LWO2 file
lwTexture*	lwGetTexture( idFile* fp, int bloksz, unsigned int type );

//! Reads a shader record from a SURF.BLOK in an LWO2 file
lwPlugin*	lwGetShader( idFile* fp, int bloksz );

//! Reads and parses an lwSurface structure from an LWO2 file format.
lwSurface*	lwGetSurface( idFile* fp, int cksize );

//! Allocates and initializes a default surface object.
lwSurface*	lwDefaultSurface();

//! Parses and creates a surface definition from an LWOB file
lwSurface*	lwGetSurface5( idFile* fp, int cksize, lwObject* obj );

/*!
	\brief Reads polygon records from a POLS chunk in an LWOB file and adds them to the provided polygon list.

	This function processes a POLS chunk from an LWOB file format, parsing polygon data including vertex indices and surface information. It allocates memory for the polygons and vertices based on the
   chunk size and data structure. The function handles offset adjustments for vertex indices using the provided ptoffset parameter. It returns 1 on success or 0 on failure, with error handling that
   frees allocated memory and returns NULL for surface-related functions.

	\param fp File pointer to the LWOB file being read
	\param cksize Size of the POLS chunk in bytes
	\param plist Pointer to the polygon list structure where polygons will be added
	\param ptoffset Offset value to adjust vertex indices by
	\return 1 if the polygon data was successfully read and processed, 0 if there was an error during processing
	\throws NULL pointer dereference if fp or plist are NULL, memory allocation failure if lwAllocPolygons fails
*/
int			lwGetPolygons5( idFile* fp, int cksize, lwPolygonList* plist, int ptoffset );

//! Loads a LightWave object file and returns a parsed object structure
lwObject*	lwGetObject5( const char* filename, unsigned int* failID, int* failpos );

//! Frees all nodes in a linked list by calling the provided free function on each node.
void		lwListFree( void* list, void ( *freeNode )( void* ) );

//! Appends a node to the end of a linked list.
void		lwListAdd( void** list, void* node );

//! Inserts a node into a list in sorted order using the provided comparison function.
void		lwListInsert( void** vlist, void* vitem, int ( *compare )( void*, void* ) );

//! Computes the dot product of two 3D vectors represented as float arrays
float		dot( float a[], float b[] );

//! Computes the cross product of two 3D vectors and stores the result in a third vector
void		cross( float a[], float b[], float c[] );

//! Normalizes the input vector in place by dividing each component by its magnitude.
void		normalize( float v[] );
#define vecangle( a, b ) ( float )idMath::ACos( dot( a, b ) )

//! Sets the file length to the specified integer value.
void		   set_flen( int i );

//! Returns the current file length value.
int			   get_flen();

//! Reads a specified number of bytes from a file into a newly allocated memory block
void*		   getbytes( idFile* fp, int size );

//! Skips n bytes in the given file pointer.
void		   skipbytes( idFile* fp, int n );

//! Reads a signed 8-bit integer from the given file pointer.
int			   getI1( idFile* fp );

//! Reads a signed 16-bit integer from a file stream and returns it
short		   getI2( idFile* fp );

//! Reads a 32-bit signed integer from the given file handle.
int			   getI4( idFile* fp );

//! Reads a single unsigned byte from the given file pointer.
unsigned char  getU1( idFile* fp );

//! Reads a 16-bit unsigned integer from a file and returns it in host byte order.
unsigned short getU2( idFile* fp );

//! Reads a 32-bit unsigned integer from the given file handle in big-endian byte order.
unsigned int   getU4( idFile* fp );

//! Reads and decodes a variable-length integer value from a file
int			   getVX( idFile* fp );

//! Reads a 32-bit floating point value from a file handle
float		   getF4( idFile* fp );

//! Reads a null-terminated string from a file pointer
char*		   getS0( idFile* fp );

//! Reads a signed 8-bit integer from the byte pointer and advances the pointer
int			   sgetI1( unsigned char** bp );

//! Reads a signed 16-bit integer from the given byte pointer and advances the pointer.
short		   sgetI2( unsigned char** bp );

//! Reads a 32-bit signed integer from the given byte pointer and advances the pointer.
int			   sgetI4( unsigned char** bp );

//! Reads and returns the next unsigned byte from the given byte pointer, advancing the pointer.
unsigned char  sgetU1( unsigned char** bp );

//! Reads an unsigned short value from a byte buffer and advances the buffer pointer by two bytes.
unsigned short sgetU2( unsigned char** bp );

//! Reads a 4-byte unsigned integer from a byte pointer in big-endian format and advances the pointer.
unsigned int   sgetU4( unsigned char** bp );

//! Reads a variable-length signed integer from a buffer and advances the buffer pointer.
int			   sgetVX( unsigned char** bp );

//! Reads a 32-bit float value from the given byte pointer, advances the pointer, and returns the value.
float		   sgetF4( unsigned char** bp );

//! Reads a null-terminated string from a buffer and advances the buffer pointer.
char*		   sgetS0( unsigned char** bp );

#endif /* !__LWO2_H__ */
