#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------



// TODO 1: Create a struct needed to hold the information to Map node



enum Orientation {
	orthogonal = 1,
	isometric,
	staggered,
	hexagonal
};
enum Renderorder {
	right_down = 1,
	right_up,
	left_down,
	left_up

};

struct Tileset {
	uint firstgid;
	p2SString img_source;
	p2SString name;
	uint tilewidth;
	uint tileheight;
	uint spacing;
	uint margin;
};

struct Map {
	enum Orientation orientation;
	enum Renderorder renderorder;
	uint width;
	uint height;
	uint tilewidth;
	uint tileheight;
	int nextobjectid;
	uint tileset_num=0;

	
	
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	

private:

	void Fill_map(pugi::xml_document&);

	void Fill_tileset(pugi::xml_document& doc);

	void LOG_all(pugi::xml_document& doc);

	Orientation String_to_orientation(p2SString str);
	Renderorder String_to_renderorder(p2SString str);

public:
	

	// TODO 1: Add your struct for map info as public for now
	Map map_struct;
	Tileset map_struct_tileset;

private:

	pugi::xml_document	map_file;

	p2SString			folder;
	bool				map_loaded;
	p2List<SDL_Texture*>	Map_texture;
};

#endif // __j1MAP_H__