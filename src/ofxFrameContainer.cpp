#include "ofxFrameContainer.h"


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
ofxFrameContainer::ofxFrameContainer()
{
    _fp     = NULL;
    _pixels = NULL;
    
    _width       = 0;
    _height      = 0;
    _color       = 0;
    _pixels_size = 0;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
ofxFrameContainer::~ofxFrameContainer()
{
    this->close();
}

//------------------------------------------------------------------------------
// open
//------------------------------------------------------------------------------
bool ofxFrameContainer::open(const char *path)
{
    return this->open(path, 0, 0, 0);
}

bool ofxFrameContainer::open(const char *path, int32_t width, int32_t height, int8_t color)
{
    _fp = fopen(path, "a+b");
    if (_fp != NULL) {
        // Header
        fpos_t pos = 0;
        fgetpos(_fp, &pos);
        if (pos == 0) {
            if (width == 0 || height == 0 || color == 0) {
                fclose(_fp);
                return false;
            }
            this->writeHeader(width, height, color);
        } else {
            if (!this->readHeader()) {
                fclose(_fp);
                return false;
            }
        }
        
        // Pixels
        _pixels_size = _width * _height * _color;
        _pixels = new unsigned char[_pixels_size];
        
        return true;
    }
    
    return false;
}

//------------------------------------------------------------------------------
// close
//------------------------------------------------------------------------------
void ofxFrameContainer::close()
{
    delete[] _pixels;
    _pixels = NULL;
    
    fclose(_fp);
}

//------------------------------------------------------------------------------
// getFrame
//------------------------------------------------------------------------------
unsigned char* ofxFrameContainer::getFrame()
{
    fread(_pixels, sizeof(unsigned char), _pixels_size, _fp);
    return _pixels;
}

unsigned char* ofxFrameContainer::getFrame(int32_t frame_num)
{
    fseek(_fp, HEADER_SIZE+(_pixels_size*(frame_num+1)), SEEK_SET);
    return this->getFrame();
}

//------------------------------------------------------------------------------
// addFrame
//------------------------------------------------------------------------------
void ofxFrameContainer::addFrame(unsigned char *pixels)
{
    fwrite(pixels, sizeof(unsigned char), _pixels_size, _fp);
}

//------------------------------------------------------------------------------
// readHeader
//------------------------------------------------------------------------------
bool ofxFrameContainer::readHeader()
{
    fseek(_fp, 0, SEEK_SET);
    
    // Signature
    char signature[8];
    fread(signature, sizeof(signature), 1, _fp);
    if (!(signature[0] == 'o' && signature[1] == 'f' && signature[2] == 'x' &&
          signature[3] == 'F' && signature[4] == 'C'))
    {
        return false;
    }
    
    // Width
    fread(&_width,  sizeof(_width),  1, _fp);
    
    // Height
    fread(&_height, sizeof(_height), 1, _fp);
    
    // Color
    fread(&_color,  sizeof(_color),  1, _fp);
    
    return true;
}

//------------------------------------------------------------------------------
// writeHeader
//------------------------------------------------------------------------------
void ofxFrameContainer::writeHeader(int32_t width, int32_t height, int8_t color)
{
    fseek(_fp, 0, SEEK_SET);
    
    // Signature
    char signature[8] = "ofxFC";
    fwrite(signature, sizeof(signature), 1, _fp);   // 8 byte
    
    // Width
    _width = width;
    fwrite(&_width, sizeof(_width), 1, _fp);        // 4 byte
    
    // Height
    _height = height;
    fwrite(&_height, sizeof(_height), 1, _fp);      // 4 byte
    
    // Color
    _color = color;
    fwrite(&_color, sizeof(_color), 1, _fp);        // 1 byte
}
