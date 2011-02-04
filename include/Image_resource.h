#ifndef Image_resource_h
#define Image_resource_h


#include "Resource.h"
#include "main.h"

class Image_resource
	: public Resource
{
	public:
		Image_resource();
		virtual ~Image_resource();
		virtual bool Load(const std::string &filename);
		ALLEGRO_BITMAP *Get_allegro_bitmap();

	private:
		ALLEGRO_BITMAP *bitmap;
};


#endif  //  Image_resource_h
