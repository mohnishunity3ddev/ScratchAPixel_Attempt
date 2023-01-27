#include <iostream>
#include <file_operations.h>
#include <stdint.h>
#include <jmemory.h>
#include <jmath.h>
#include <jassert.h>
#include <jinterpolate.h>

typedef uint32_t uint32;

uint32_t Width = 512;
uint32_t Height = 512;

uint8_t
GetRandomByte()
{
    return (rand() % 0xff);
}

inline Vec3ui
GetRandomColor255()
{
    Vec3ui Result = {};
    Result.x = GetRandomByte();
    Result.y = GetRandomByte();
    Result.z = GetRandomByte();
    return Result;
}

void
DrawCell(uint8_t *Image, uint32_t ImageWidth,  
         uint32_t CellWidth, uint32_t CellHeight,
         const cell_colors &CellColors)
{
    for(uint32_t i = 0; i < CellHeight; ++i)
    {
        uint8_t *Row = Image + 3*i*ImageWidth;
        float V = 1.0f - ((float)i / (float)CellHeight);
        
        for(uint32_t j = 0; j < CellWidth; ++j)
        {
            uint8_t *Pixel = Row + 3*j;

            float U = (float)j / (float)CellWidth;
            Vec3f PixelColor = bilinear(U, V, CellColors);
            // Vec3ui PixelColor = CellColors.C00;
            *Pixel++ = (uint8_t)PixelColor.x;
            *Pixel++ = (uint8_t)PixelColor.y;
            *Pixel++ = (uint8_t)PixelColor.z;
        }
    }
}

void
GenerateGridColors(Vec3ui *GridColors, uint32_t GridSizeX, uint32_t GridSizeY)
{
    for(uint32_t i = 0; i < GridSizeY; ++i)
    {
        for(uint32_t j = 0; j < GridSizeX; ++j)
        {
            Vec3ui *GridColor = (GridColors + (i*GridSizeY + j));
            *GridColor = GetRandomColor255();
        }
    }
}

void
MakeGrid(void *ImageBuffer, Vec3ui &ImageDimensions, 
         const uint32_t GridSizeX, const uint32_t GridSizeY)
{
    uint32_t ImageWidth = ImageDimensions.x;
    uint32_t ImageHeight = ImageDimensions.y;
    
    uint32_t CellWidth = (uint32_t)((float)ImageWidth / (float)GridSizeX);
    uint32_t CellHeight = (uint32_t)((float)ImageHeight / (float)GridSizeY);
    
    uint8_t *Image = (uint8_t *) ImageBuffer;
    uint8_t *Row = Image;
    
    Vec3ui GridColors[GridSizeY][GridSizeX];
    GenerateGridColors(&GridColors[0][0], GridSizeX, GridSizeY);

    for(uint32_t i = 0; i < GridSizeY; ++i)
    {
        for (uint32_t j = 0; j < GridSizeX; ++j)
        {
            uint32_t MinX = j*CellWidth;
            uint8_t *CellTop = Row + 3*MinX;

            uint32_t MaxX = MinX + CellWidth;
            if(MaxX >= ImageWidth)
            {
                MaxX = ImageWidth - 1;
            }
            uint32_t MinY = i*CellHeight;
            uint32_t MaxY = MinY + CellHeight;
            if(MaxY >= ImageHeight)
            {
                MaxY = ImageHeight - 1;
            }
            uint32_t CWidth = MaxX - MinX;
            uint32_t CHeight = MaxY - MinY;

            uint32 PreviousColorIndex = i == 0 ? GridSizeY - 1 : i - 1;
            
            cell_colors CellColors = {
                .C00 = GridColors[i][j],
                .C10 = GridColors[i][(j+1)%GridSizeX],
                .C01 = GridColors[PreviousColorIndex][j],
                .C11 = GridColors[PreviousColorIndex][(j+1)%GridSizeX]
            };
            
            // Vec3f RandomColor { GetRandomByte(), GetRandomByte(), GetRandomByte() };
            DrawCell(CellTop, ImageWidth, CWidth, CHeight, CellColors);
        }
        Row += ImageWidth*CellHeight*3;
    }
}

int
main()
{
    srand(1873);
    // 24 bits to represent a color.
    uint64_t RequiredSize = 3*(Width * Height); 
    void *ImageBuffer = 0;
    ImageBuffer = Alloc(RequiredSize);

    
    if(!ImageBuffer)
    {
        std::cout << "Could not allocate " << RequiredSize << " Bytes.\n";
        return -1;
    }
    
    Vec3ui ImageDimensions{Width, Height, 0};
    
    MakeGrid(ImageBuffer, ImageDimensions, 5, 5);
    
    WritePPMImage("test_bilinear.ppm", ImageBuffer, ImageDimensions);
    
    Free(ImageBuffer);
    return 0;
}
