/*************************************************************************/
/*  texture_loader_dds.cpp                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "texture_loader_dxt1.h"
#include "core/os/file_access.h"

#define PF_FOURCC(s) ((uint32_t)(((s)[3] << 24U) | ((s)[2] << 16U) | ((s)[1] << 8U) | ((s)[0])))

RES ResourceFormatDXT1::load(const String &p_path, const String &p_original_path, Error *r_error) {

	if (r_error)
		*r_error = ERR_CANT_OPEN;

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::READ, &err);
	if (!f)
		return RES();

	FileAccessRef fref(f);
	if (r_error)
		*r_error = ERR_FILE_CORRUPT;

	ERR_FAIL_COND_V_MSG(err != OK, RES(), "Unable to open DXT1 texture file '" + p_path + "'.");

	uint32_t format_fourcc = f->get_32();
	if (format_fourcc != PF_FOURCC("DXT1")) {
		ERR_FAIL_V_MSG(RES(), "Unrecognized or unsupported color layout in DXT1 '" + p_path + "'.");
	}
	
	uint16_t width = f->get_16();
	uint16_t height = f->get_16();

	uint16_t w = width + (4 - width % 4) % 4;
    uint16_t h = height + (4 - height % 4) % 4;
    uint16_t size = w * h / 2;
	
	PoolVector<uint8_t> src_data;
	src_data.resize(size);
	PoolVector<uint8_t>::Write wb = src_data.write();
	f->get_buffer(wb.ptr(), size);

	Ref<Image> img = memnew(Image(width, height, 0, Image::FORMAT_DXT1, src_data));

	Ref<ImageTexture> texture = memnew(ImageTexture);
	texture->create_from_image(img);
	texture->set_flags(0);

	if (r_error)
		*r_error = OK;

	return texture;
}

void ResourceFormatDXT1::get_recognized_extensions(List<String> *p_extensions) const {

	p_extensions->push_back("dxt1");
}

bool ResourceFormatDXT1::handles_type(const String &p_type) const {

	return ClassDB::is_parent_class(p_type, "Texture");
}

String ResourceFormatDXT1::get_resource_type(const String &p_path) const {

	if (p_path.get_extension().to_lower() == "dxt1")
		return "ImageTexture";
	return "";
}
