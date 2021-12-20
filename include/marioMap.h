
//{{BLOCK(marioMap)

//======================================================================
//
//	marioMap, 512x256@4, 
//	+ palette 256 entries, not compressed
//	+ 29 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x32 
//	Total size: 512 + 928 + 4096 = 5536
//
//	Time-stamp: 2019-06-08, 18:27:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MARIOMAP_H
#define GRIT_MARIOMAP_H

#define marioMapTilesLen 928
extern const unsigned short marioMapTiles[464];

#define marioMapMapLen 4096
extern const unsigned short marioMapMap[2048];

#define marioMapPalLen 512
extern const unsigned short marioMapPal[256];

#endif // GRIT_MARIOMAP_H

//}}BLOCK(marioMap)
