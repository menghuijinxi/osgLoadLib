#pragma once
#include "macro.h"
#include "memory"
#include "string"
#include "vector"

//加载接口
class OSGBLoadLib_API IOsgbLoad
{
public:
    virtual ~IOsgbLoad() = default;

    //加载osgb文件，只加载文件到内存中，不进行解析和转换
    virtual bool Load(const std::string& filePath) = 0;

#if _WIN32
    //加载osgb文件，只加载文件到内存中，不进行解析和转换。
    virtual bool LoadW(const std::wstring& filePath) = 0;
#endif

    struct FVector
    {
        FVector():x(0.), y(0.), z(0.){}
        FVector(const double _x, const double _y, const double _z):x(_x), y(_y), z(_z)
        {}

        double x,y,z;
    };

    struct FOsgbData
    {
        //模型中心位置
        std::vector<FVector> Center = std::vector<FVector>();

        //模型半径
        std::vector<double> Radius = std::vector<double>();

        //显示范围，屏占比
        std::vector<double> ShowSize = std::vector<double>();

        //下一个模型的路径
        std::vector<const char*> NextModePath = std::vector<const char*>();
    };

    //获取osgb数据
    virtual bool GetOsgbData(FOsgbData& data) = 0;

    struct FMeshData
    {
        //顶点
        void* vertex = nullptr;
        int32_t vertexCount = 0;
        //uv
        void* uv = nullptr;
        //索引
        uint32_t* TrianglePtr = nullptr;
        int32_t TriangleCount = 0;

        //宽
        int32_t textureWidth = 0;
        //高
        int32_t textureHeight = 0;
        //贴图
        uint8_t* texturePtr = nullptr;
    };

    //获取网格数据
    virtual bool GetMesh(std::vector<FMeshData>& mesh) = 0;

    static std::shared_ptr<IOsgbLoad> MakeOSGBLoad();
};
