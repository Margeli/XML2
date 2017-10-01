#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

	for (uint i = 0; i<=map_struct.tileset_num ; i++) {

		SDL_Texture* aux_texture;

		aux_texture = App->tex->Load(map_struct_tileset.img_source.GetString());
		Map_texture.add(aux_texture);
		App->render->Blit(Map_texture[i], 0, 0);
	}
	
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());//map file-> file_name

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data		
		Fill_map(map_file);
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	Fill_tileset(map_file);

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		LOG_all(map_file);
	}

	map_loaded = ret;

	return ret;
}

void j1Map::Fill_map(pugi::xml_document& doc){

	pugi::xml_node map_node = doc.child("map");

	map_struct.orientation = String_to_orientation(map_node.attribute("orientation").as_string());
	map_struct.renderorder = String_to_renderorder(map_node.attribute("renderorder").as_string());
	map_struct.width = map_node.attribute("width").as_uint();
	map_struct.height = map_node.attribute("height").as_uint();
	map_struct.tilewidth = map_node.attribute("tilewidth").as_uint();
	map_struct.tileheight = map_node.attribute("tileheight").as_uint();
	map_struct.nextobjectid = map_node.attribute("nextobjectid").as_uint();


}

void j1Map::Fill_tileset(pugi::xml_document& doc) {
	
	for (pugi::xml_node tileset = doc.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset")) {

		p2SString source= "maps/";
		source += tileset.child("image").attribute("source").as_string();
		map_struct_tileset.img_source = source;

		map_struct_tileset.firstgid = tileset.attribute("firstgid").as_uint();		
		map_struct_tileset.name = tileset.attribute("name").as_string();
		map_struct_tileset.tilewidth = tileset.attribute("tilewidth").as_uint();
		map_struct_tileset.tileheight = tileset.attribute("tileheight").as_uint();
		map_struct_tileset.spacing = tileset.attribute("spacing").as_uint();
		map_struct_tileset.margin = tileset.attribute("margin").as_uint();
		map_struct.tileset_num++;
		
	}
}
	


Orientation j1Map::String_to_orientation(p2SString str){

	if (str == "orthogonal") {
	
		return orthogonal;
	}
	if (str == "isometric") {
	
		return isometric;
	}
	if (str == "staggered") {

		return staggered;
	}
	if (str == "hexagonal") {

		return hexagonal;
	}
}

Renderorder j1Map::String_to_renderorder(p2SString str) {

	if (str == "right-down") {

		return right_down;
	}
	if (str == "right-up") {

		return right_up;
	}
	if (str == "left-down") {

		return left_down;
	}
	if (str == "left-up") {

		return left_up;
	}
}

void j1Map::LOG_all(pugi::xml_document& doc) {

	LOG("Successfully parsed map XML file!");
	LOG("------------Map Info--------------");
	LOG("width: %i		  height: %i   ", map_struct.width, map_struct.height);
	LOG("tile_width: %i   tile_height: %i", map_struct.tilewidth, map_struct.tileheight);

	for (int i = 0; i<=map_struct.tileset_num; i++) {

		LOG("------ Tileset ------");
		LOG("name: %s		firstgid: %i	", map_struct_tileset.name.GetString(), map_struct_tileset.firstgid);
		LOG("tile_width: %i   tile_height: %i", map_struct_tileset.tilewidth, map_struct_tileset.tileheight);
		LOG("spacing: %i   margin: %i", map_struct_tileset.spacing, map_struct_tileset.margin);
		LOG("----------------------------------");
	}
}
