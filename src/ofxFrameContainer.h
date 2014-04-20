#ifndef __ofxFrameContainer__
#define __ofxFrameContainer__

#include <stdio.h>
#include <stdint.h>


class ofxFrameContainer
{
public:
    ofxFrameContainer();
    virtual ~ofxFrameContainer();
    
    bool           open(const char *path);
    bool           open(const char *path, int32_t width, int32_t height, int8_t color);
    void           close();
    unsigned char* getFrame();
    unsigned char* getFrame(int32_t frame_num);
    void           addFrame(unsigned char *pixels);
    
    int32_t getWidth()  { return _width; }
    int32_t getHeight() { return _height; }
    int8_t  getColor()  { return _color; }
    
protected:
    FILE          *_fp;
    unsigned char *_pixels;
    
    int32_t _width;
    int32_t _height;
    int8_t  _color;
    int32_t _pixels_size;
    
private:
    static const int32_t HEADER_SIZE = 17;
    
    bool readHeader();
    void writeHeader(int32_t width, int32_t height, int8_t color);
};

#endif // __ofxFrameContainer__
