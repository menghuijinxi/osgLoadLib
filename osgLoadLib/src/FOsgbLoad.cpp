//
// Created by wap80 on 2025/3/4.
//

#include "FOsgbLoad.h"
#include "iostream"
#include <osgDB/WriteFile>
#include <osg/PagedLOD>
#include <osg/Texture2D>

void FOsgbLoad::FindGeode(osg::ref_ptr<osg::Node> n, std::vector<osg::Geode *> &geodes)
{
  if (!n.valid()) return;

  if (auto geo = n->asGeode())
  {
    geodes.push_back(geo);
  }
  else if (auto pagLod = dynamic_cast<osg::PagedLOD*>(n.get()))
  {
    if (auto child = pagLod->getChild(0))
    {
      if (auto pagLodGeo = child->asGeode())
      {
        geodes.push_back(pagLodGeo);
      }
    }
  }
  else if (auto group = dynamic_cast<osg::Group*>(n.get()))
  {
    for (unsigned int index = 0; index < group->getNumChildren(); index++)
    {
      auto groupChild = group->getChild(index);
      if (auto pagLodGroup = dynamic_cast<osg::PagedLOD*>(groupChild))
      {
        if (auto child = pagLodGroup->getChild(0))
        {
          if (auto pagLodGeo = child->asGeode())
          {
            geodes.push_back(pagLodGeo);
          }
        }
      }
      else if (auto pagLodGeo = groupChild->asGeode())
      {
        geodes.push_back(pagLodGeo);
      }
      else if(auto g = groupChild->asGroup())
      {
        FindGeode(g, geodes);
      }
    }
  }
}

bool FOsgbLoad::Load(const std::string &filePath)
{
  osg::ref_ptr<osgDB::ReaderWriter> rw = osgDB::Registry::instance()->getReaderWriterForExtension("osgb");
  auto f = std::ifstream(filePath, std::ios::binary);
  if (!f.is_open())
  {
    std::cout << "打开osgb文件失败，可能文件不存在：" << filePath << std::endl;
    return false;
  }
  osgDB::ReaderWriter::ReadResult rr = rw->readNode(f);
  node = rr.getNode();
  if (!node)
  {
    std::cout << "读取osgb文件失败，可能是损坏的文件，错误码：" << rr.status() << std::endl;
    return false;
  }
  f.close();
  return true;
}

#if _WIN32
bool FOsgbLoad::LoadW(const std::wstring &filePath)
{
  osg::ref_ptr<osgDB::ReaderWriter> rw = osgDB::Registry::instance()->getReaderWriterForExtension("osgb");
  auto f = std::ifstream(filePath, std::ios::binary);
  if (!f.is_open())
  {
    return false;
  }
  osgDB::ReaderWriter::ReadResult rr = rw->readNode(f);
  node = rr.getNode();
  if (!node)
  {
    std::cout << "读取osgb文件失败，可能是损坏的文件，错误码：" << rr.status() << std::endl;
    return false;
  }
  f.close();
  return true;
}
#endif

bool FOsgbLoad::GetMesh(std::vector<FMeshData> &mesh)
{
  std::vector<osg::Geode*> geodes;
  FindGeode(node, geodes);
  if (geodes.size() <= 0) return false;

  for (size_t index = 0; index < geodes.size(); index++)
  {
    auto geode = geodes[index];

    if(!geode) continue;
    osg::Geometry* geometry = nullptr;

    for(unsigned int i = 0; i < geode->getNumChildren(); i++)
    {
      auto darw = geode->getDrawable(i);
      if(darw)
      {
        geometry = dynamic_cast<osg::Geometry*>(darw);
        if(geometry)
        {
          break;
        }
      }
    }

    if (!geometry) continue;

    FMeshData data;

    //贴图
    osg::Texture2D* tex2D = dynamic_cast<osg::Texture2D*>(geometry->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXTURE));
    osg::Image* image = tex2D->getImage();
    if (!image) break;
    data.textureWidth = image->s();
    data.textureHeight = image->t();
    data.texturePtr = image->data();

    //网格
    auto vertexArray = geometry->getVertexArray();
    if (!vertexArray) break;

    auto PointArray = dynamic_cast<osg::MixinVector<osg::Vec3f>*>(vertexArray);
    if (!PointArray) break;

    auto primitive = geometry->getPrimitiveSet(0);
    if (!primitive) break;

    auto pri = dynamic_cast<osg::MixinVector<unsigned int>*>(primitive);
    if (!pri) break;

    auto texCoord = geometry->getTexCoordArray(0);
    auto uv_ = dynamic_cast<osg::MixinVector<osg::Vec2f>*>(texCoord);
    if (!uv_) break;

    data.TrianglePtr = pri->asVector().data();
    data.TriangleCount = pri->size();
    data.uv = uv_->asVector().data();
    data.vertex = PointArray->asVector().data();
    data.vertexCount = PointArray->size();

    mesh.push_back(data);
  }

  return true;
}

void ReadOsgbLodDataBase(osg::ref_ptr<osg::Node> node, FOsgbLoad::FOsgbData &data);
void ReadPagedLodData(osg::PagedLOD* pagedLod, FOsgbLoad::FOsgbData &data);

void ReadOsgbLodData(osg::ref_ptr<osg::Node> node, FOsgbLoad::FOsgbData &data)
{
    if (!node)
    {
        return;
    }
    ReadOsgbLodDataBase(node, data);
}

bool FOsgbLoad::GetOsgbData(FOsgbData &data)
{
  if(!node)
  {
    return false;
  }
  ReadOsgbLodData(node, data);

  return true;
}

void ReadOsgbLodDataBase(osg::ref_ptr<osg::Node> n, FOsgbLoad::FOsgbData &data)
{
  if (!n.valid()) return;

  if (auto geo = n->asGeode())
  {

  }
  if(auto pagLod = dynamic_cast<osg::PagedLOD*>(n.get()))
  {
    ReadPagedLodData(pagLod, data);
  }
  else if(auto group = dynamic_cast<osg::Group*>(n.get()))
  {
    for (unsigned int index = 0; index < group->getNumChildren(); index++)
    {
      auto child = group->getChild(index);
      if (auto nPagLod = dynamic_cast<osg::PagedLOD*>(child))
      {
        ReadPagedLodData(nPagLod, data);
      }
      //因为osgb是二叉树结构，这里必须为Geode填充数据
      else if (auto _geo = child->asGeode())
      {
        data.Center.emplace_back();
        data.Radius.emplace_back();
        data.ShowSize.emplace_back();
        const char* s = "";
        data.NextModePath.push_back(s);
      }
      else if(auto _g = child->asGroup())
      {
        ReadOsgbLodData(_g, data);
      }
    }
  }
}

void ReadPagedLodData(osg::PagedLOD* pagedLod, FOsgbLoad::FOsgbData &data)
{
  auto& center = pagedLod->getCenter();
  data.Center.emplace_back(center.x(), center.y(), center.z());
  data.Radius.push_back(pagedLod->getRadius());
  auto& RangeList = pagedLod->getRangeList();
  if(RangeList.size() > 0)
  {
    auto& range = RangeList[0];
    data.ShowSize.push_back(range.second);
  }

  if (pagedLod->getNumFileNames() > 1)
  {
    data.NextModePath.push_back(pagedLod->getFileName(1).c_str());
  }
  else if(pagedLod->getNumFileNames() > 0)
  {
    auto s = pagedLod->getFileName(0);
    if(!s.empty())
    {
      data.NextModePath.emplace_back(s.c_str());
    }
  }
}