#include <filesystem>
#include "IOsgbLoad.h"
#include "iostream"

namespace fs = std::filesystem;

class A
{
public:
    virtual ~A() = default;

    virtual void T() = 0;
};

class B : public A
{
public:
    int32_t b = 2;
    virtual void T() override
    {
        int32_t a = 2;
        b = 3;
    }
};

int main()
{
    //osg::setNotifyLevel(osg::DEBUG_INFO);
    //A* a = new B();
    //auto b = dynamic_cast<B*>(a);

    fs::path path = u8"/media/meng-pc/D/后洲街道osgb/data/Tile_0003_0015/Tile_0003_0015_0019.osgb";

    auto osgbLoad = IOsgbLoad::MakeOSGBLoad();
    osgbLoad->Load(path.string());
    IOsgbLoad::FOsgbData osgb_data;
    osgbLoad->GetOsgbData(osgb_data);
    std::vector<IOsgbLoad::FMeshData> mesh;
    osgbLoad->GetMesh(mesh);

    for (auto& m : mesh)
    {
        std::cout << "Triangle：" << std::endl;
        for (size_t i = 0; i < m.TriangleCount; i+=3)
        {
            std::cout << m.TrianglePtr[i] << "," << m.TrianglePtr[i+1] << "," << m.TrianglePtr[i+2] << "  ";
        }
        std::cout << std::endl << "Vertex：" << std::endl;
        struct FV
        {
            float p[3];
        };
        FV* v_base = (FV*)m.vertex;
        for (size_t i = 0; i < m.vertexCount; i++)
        {
            FV& v = v_base[i];
            std::cout << v.p[0] << "," << v.p[1] << "," << v.p[2] << "  ";
        }
    }

    return 0;
}