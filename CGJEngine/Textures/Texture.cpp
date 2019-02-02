/*
 *	LXSR Load textures
 * 	adapted from multiple sources by E 
 *	Texture.cpp
 *
 *  Created on: Jun 2, 2012
 */

#include <GL/glew.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <Textures/Texture.h>
#include <glm_wrapper.h>

extern "C" {
#include <stdlib.h>
#ifdef FREEIMG
#include <FreeImage.h>
#else
#include <jpeglib.h>
#include <png.h>
#endif
}

#include "Textures/Texture.h"

#ifndef FREEIMG
int loadPNG(const char* file_name, textureImage* texture){
    // This function was originally written by David Grayson for
    // https://github.com/DavidEGrayson/ahrs-visualizer
    //Adapted by Joao Neves
    //on 12-8-2015

    texture->compressed = false;
    texture->type = GL_UNSIGNED_BYTE;

    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0){
        perror(file_name);
        return 0;
    }
    // read the header
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr){
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info){
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type, NULL, NULL, NULL);

    
    texture->width = temp_width;
    texture->height = temp_height;
    
    if (bit_depth != 8){
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
        return 0;
    }

    switch(color_type){
	case PNG_COLOR_TYPE_RGB:
       		texture->format = GL_RGB;
       		texture->internalFormat = GL_RGB8;
		break;
    	case PNG_COLOR_TYPE_RGB_ALPHA:
        	//format = GL_RGBA;
       		texture->format = GL_RGBA;
       		texture->internalFormat = GL_RGBA8;
        	break;
    	default:
        	fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
        	return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    texture->data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (texture->data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(texture->data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++)
    {
        //row_pointers[temp_height - 1 - i] = texture->data + i * rowbytes;
        row_pointers[temp_height - 1 - i] = texture->data + (temp_height - 1 - i) * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);


    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(row_pointers);
    fclose(fp);
    return 1;
}

int loadJPEG(const char* filename, textureImage* texture){
	FILE* infile;
	struct jpeg_decompress_struct cinfo = {};
	struct jpeg_error_mgr jerr = {};
	JDIMENSION row_stride;     /* physical row width in output buffer */

	texture->format = GL_RGB;
    texture->internalFormat = GL_RGB8;
    texture->compressed = false;
    texture->type = GL_UNSIGNED_BYTE;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	if ((infile = fopen(filename, "rb")) == NULL) {
            fprintf(stderr, "can't open %s\n", filename);
            exit(1);
        }
        jpeg_stdio_src(&cinfo, infile);

	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	
	texture->width = cinfo.output_width;
	texture->height = cinfo.output_height;
	texture->data = (unsigned char*)malloc
	        (sizeof(unsigned char)*(cinfo.output_width*cinfo.output_height*cinfo.output_components));

	row_stride = cinfo.output_width * cinfo.output_components;
    	

	/* Make a one-row-high sample array that will go away when done with image */
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);

    long counter = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(texture->data+counter, buffer[0], row_stride);
        counter += row_stride;
	}

	free(buffer[0]);
    jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress (&cinfo);
	fclose(infile);
	return 1;
}


/* simple loader for 24bit bitmaps (data is in rgb-format) */
int loadBMP(const char *filename, textureImage *texture)
{
    FILE *file;
    unsigned short int bfType;
    long int bfOffBits;
    short int biPlanes;
    short int biBitCount;
    long int biSizeImage;
    int i;
    unsigned char temp;

	texture->format = GL_RGB;
    texture->internalFormat = GL_RGB8;
    texture->type = GL_UNSIGNED_BYTE;
    texture->compressed = false;

    /* make sure the file is there and open it read-only (binary) */
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("File not found : %s\n", filename);
        return 0;
    }
    if(!fread(&bfType, sizeof(short int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    /* check if file is a bitmap */
    if (bfType != 19778)
    {
        printf("Not a Bitmap-File!\n");
        return 0;
    }        
    /* get the file size */
    /* skip file size and reserved fields of bitmap file header */
    fseek(file, 8, SEEK_CUR);
    /* get the position of the actual bitmap data */
    if (!fread(&bfOffBits, sizeof(long int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Data at Offset: %ld\n", bfOffBits);
    /* skip size of bitmap info header */
    fseek(file, 4, SEEK_CUR);
    /* get the width of the bitmap */
    fread(&texture->width, sizeof(int), 1, file);
    printf("Width of Bitmap: %d\n", texture->width);
    /* get the height of the bitmap */
    fread(&texture->height, sizeof(int), 1, file);
    printf("Height of Bitmap: %d\n", texture->height);
    /* get the number of planes (must be set to 1) */
    fread(&biPlanes, sizeof(short int), 1, file);
    if (biPlanes != 1)
    {
        printf("Error: number of Planes not 1!\n");
        return 0;
    }
    /* get the number of bits per pixel */
    if (!fread(&biBitCount, sizeof(short int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Bits per Pixel: %d\n", biBitCount);
    if (biBitCount != 24)
    {
        printf("Bits per Pixel not 24\n");
        return 0;
    }
    /* calculate the size of the image in bytes */
    biSizeImage = texture->width * texture->height * 3;
    printf("Size of the image data: %ld\n", biSizeImage);
    texture->data = (unsigned char *)malloc(biSizeImage);
    /* seek to the actual data */
    fseek(file, bfOffBits, SEEK_SET);
    if (!fread(texture->data, biSizeImage, 1, file))
    {
        printf("Error loading file!\n");
        return 0;
    }
    /* swap red and blue (bgr -> rgb) */
    for (i = 0; i < biSizeImage; i += 3)
    {
        temp = texture->data[i];
        texture->data[i] = texture->data[i + 2];
        texture->data[i + 2] = temp;
    }
    return 1;
}
#endif

#ifdef FREEIMG
int loadFreeImage(const char* filename, textureImage *texture){
    texture->compressed = false;

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename,0);
    FIBITMAP* image = FreeImage_Load(format, filename);
    FIBITMAP* tmp;

    int bpp = FreeImage_GetBPP(image);
    switch (bpp){
        case 8:
            tmp = FreeImage_ConvertTo24Bits(image);
            FreeImage_Unload(image);
            image = tmp;
            //Fall through
        case 24:
            //IsTransparent does not work as expected, so we assume 24 bit as no alpha and 32bit as alpha
            texture->format = GL_BGR;
            texture->internalFormat = GL_RGB8;
            texture->type = GL_UNSIGNED_BYTE;
            break;
        case 32:
            texture->format = GL_BGRA;
            texture->internalFormat = GL_RGBA8;
            texture->type = GL_UNSIGNED_BYTE;
            break;
        case 48:
            if(FreeImage_GetColorType(image) == FIC_RGB){
                texture->format = GL_RGB;
                texture->internalFormat = GL_RGB8;
            }else {
                texture->format = GL_RGBA;
                texture->internalFormat = GL_RGBA8;
            }
            texture->type = GL_UNSIGNED_SHORT;
            break;
        case 64:
            if(FreeImage_GetColorType(image) == FIC_RGB){
                texture->format = GL_RGB;
                texture->internalFormat = GL_RGB8;
            }else {
                texture->format = GL_RGBA;
                texture->internalFormat = GL_RGBA8;
            }
            texture->type = GL_UNSIGNED_SHORT;
            break;

        default:
            std::cout << "Bad pixel depth " << bpp << " in " << filename << std::endl;
            FreeImage_Unload(image);
            return 0;
    }

    //std::cout << bpp << FreeImage_GetColorType(image) << FIC_RGB << FIC_CMYK << FIC_PALETTE << FIC_RGBALPHA << std::endl;

    texture->width = FreeImage_GetWidth(image);
    texture->height = FreeImage_GetHeight(image);

    texture->data = FreeImage_GetBits(image);
    texture->fibitmap = image;

    return 1;
}
#endif

GLuint generateGLTexture(textureImage* texti){
	GLuint texID = 0;
     	if(texti->data){
        	glGenTextures(1, &texID);   /* create the texture */
        	glBindTexture(GL_TEXTURE_2D, texID);
        	/* actually generate the texture */
            glTexImage2D(GL_TEXTURE_2D, 0, texti->internalFormat, texti->width, texti->height, 0,
                             texti->format, texti->type, texti->data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if(GLEW_EXT_texture_filter_anisotropic){
                GLfloat largestAnisotropic;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropic);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestAnisotropic);
            }else{
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
    	}
	return texID;
}

Texture::Texture() : _name("anonymous"){
    _texture = 0;
    _width = -1;
    _height = -1;
}

Texture::Texture(GLuint texture): _name("anonymous_copy") {
    _texture = texture;
    _width = -2;
    _height = -2;
}

Texture::Texture(std::string filename) : _name(filename) {
    textureImage* ti = LoadGLTexture(filename.c_str());
    _texture = ti->GLtexture;
    _width = ti->width;
    _height = ti->height;
    free(ti);
}

const std::string& Texture::getName() {
    return _name;
}

GLuint Texture::getTexture() {
    return _texture;
}

int Texture::getWidth() {
    return _width;
}

int Texture::getHeight() {
    return _height;
}

void Texture::destroyTexture() {
    glDeleteTextures(1, &_texture);
}

void Texture::bind() {
    glBindTexture( GL_TEXTURE_2D, _texture);
}

void Texture::changeTexture(GLuint texture) {
    _texture = texture;
}

textureImage* Texture::LoadFromFile(const char* name) {
    textureImage *texti;
    std::string fn(name);

    texti = (textureImage *) malloc(sizeof(textureImage));
#ifndef FREEIMG
    if (fn.substr(fn.find_last_of('.') + 1) == "bmp")
        loadBMP(name, texti);
    else if (fn.substr(fn.find_last_of('.') + 1) == "jpg" || fn.substr(fn.find_last_of('.') + 1) == "jpeg")
        loadJPEG(name, texti);
    else if (fn.substr(fn.find_last_of('.') + 1) == "png")
        loadPNG(name, texti);
    else
        free(texti);
        return nullptr;
#else
    if(!loadFreeImage(name, texti)) {
        free(texti);
        return nullptr;
    }
#endif
    return texti;
}

textureImage* Texture::LoadGLTexture(const char* name){
    textureImage* texti = LoadFromFile(name);
    GLuint textureID = 0;

	if(texti) {
        textureID = generateGLTexture(texti);
        /* free the ram we used in our texture generation process */
#ifdef FREEIMG
        FreeImage_Unload(texti->fibitmap);
#else
        if (texti->data)
           free(texti->data);
#endif
    }
    texti->GLtexture = textureID;
	return texti;
}

Texture::Texture(const std::string &right, const std::string &left, const std::string &top, const std::string &bottom,
                 const std::string &back, const std::string &front) : _name("cube") {
    textureImage* texti;
    GLuint textureID = 0;
    std::vector<std::string> faces = {right, left, top, bottom, back, front};

    _width = -8;
    _height = -8;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        texti = LoadFromFile(faces[i].c_str());
        if (texti){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, texti->internalFormat,
                         texti->width, texti->height, 0,
                         texti->format,
                         texti->type, texti->data);
#ifdef FREEIMG
            FreeImage_Unload(texti->fibitmap);
#else
            free(texti->data);
#endif
            free(texti);
        }else{
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    _texture = textureID;
}

void Texture::generateRandom(int width) {
    if(_texture){
        glDeleteTextures(1, &_texture);
    }
    int height = width;
    int numElements = width*height;
    auto texture = new float[numElements*3];
    for(int i= 0;i<numElements; i++){
        Vec3 random = Vec3( (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
                            (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
                            (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) );
        random = glm::normalize(random);
        texture[i*3+0] = random[0];
        texture[i*3+1] = random[1];
        texture[i*3+2] = random[2];
    }

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] texture;

    _width = width;
    _height = height;
}

void Texture::bindCubeMap() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
}