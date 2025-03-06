#pragma once
#include "IOsgbLoad.h"
#include <osg/Node>

class FOsgbLoad : public IOsgbLoad
{
protected:

    osg::ref_ptr<osg::Node> node = nullptr;

    void FindGeode(osg::ref_ptr<osg::Node> n, std::vector<osg::Geode*>& geodes);

public:

    virtual bool Load(const std::string &filePath) override;
#if _WIN32
    virtual bool LoadW(const std::wstring &filePath) override;
#endif
    virtual bool GetMesh(std::vector<FMeshData> &mesh) override;
    virtual bool GetOsgbData(FOsgbData &data) override;
};